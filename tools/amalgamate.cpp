// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string_view>
#include <span>
#include <unordered_set>

#include <sys/mman.h>

#include <rx/regex.hpp>

namespace
{
    using path = std::filesystem::path;

    class mmap_data
    {
    public:
        mmap_data(const path& p)
            : size_{ file_size(p) }
            , data_{ ::mmap(nullptr, size_, PROT_READ, MAP_PRIVATE | MAP_POPULATE, std::ifstream{ p }.native_handle(), 0) }
        {
            if (data_ == MAP_FAILED)
            {
                std::println(std::cerr, "error: could not open file {} for reading", p.string());
                std::quick_exit(1);
            }
        }

        mmap_data(const mmap_data&) = delete;
        mmap_data(mmap_data&&) noexcept = default;
        mmap_data& operator=(const mmap_data&) = delete;
        mmap_data& operator=(mmap_data&&) noexcept = default;
        ~mmap_data() { if (data_ != nullptr) ::munmap(data_, size_); }

        [[nodiscard]] std::string_view view() const { return { static_cast<char*>(data_), size_ }; }

    private:
        std::size_t size_;
        void* data_;
    };

    class amalgamator
    {
    public:
        amalgamator(const path& p)
            : include_dirs_{ absolute(p) }
        {
            for (const auto& dir : include_dirs_)
            {
                if (not is_directory(dir))
                {
                    std::println(std::cerr, "error: {} is not a directory", dir.string());
                    std::quick_exit(1);
                }
            }
        }

        void operator()(const path& out, const path& in, const path& licence)
        {
            path tmpfile = out;
            tmpfile += ".tmp";

            std::ofstream tmp{ tmpfile };

            if (not tmp.is_open())
            {
                std::println(std::cerr, "error: could not open file {} for writing", tmpfile.string());
                std::quick_exit(1);
            }

            visit(tmp, absolute(in));
            tmp.flush();
            tmp.close();

            std::ofstream ofs{ out };

            if (not ofs.is_open())
            {
                std::println(std::cerr, "error: could not open file {} for writing", out.string());
                std::quick_exit(1);
            }

            ofs << "// THIS FILE IS AUTOMATICALLY AMALGAMATED FROM " << std::quoted(in.string()) << '\n';
            insert_copyrights(ofs);
            copy_licence_to(ofs, licence);
            insert_includes(ofs);
            copy_tmpfile_to(ofs, tmpfile);

            remove(tmpfile);
        }

    private:
        static constexpr auto pattern = rx::static_regex<
            R"--((?m:^#include "(.*?)"$\n?|^(#include <.*?>)$\n?|^(#pragma once)$\n?|^(// *?(?i:copyright \(c\)).*?)$|^ *?//.*?$\n?|//.*?$))--"
            , rx::mode::linear>{};

        [[nodiscard]] path find_path(const path& next, const path& current_dir) const
        {
            auto path = current_dir / next;

            if (is_regular_file(path))
                return absolute(path);

            for (const auto& dir : include_dirs_)
            {
                path = dir / next;
                if (is_regular_file(path))
                    return absolute(path);
            }

            std::println(std::cerr, "error: file {} does not exist", next.string());
            std::quick_exit(1);
        }

        void visit(std::ofstream& ofs, const path& current)
        {
            if (visited_.contains(current))
                return; /* already visited */

            mmap_data data{ current };
            const auto input = data.view();

            auto saved{ input.begin() };

            for (const auto& match : pattern.range(input))
            {
                const auto [mfirst, mlast] = get<0>(match);

                ofs << std::string_view{ saved, mfirst };
                saved = mlast;

                if (const auto cap1 = get<1>(match); cap1.matched())
                    visit(ofs, find_path(cap1.view(), current.parent_path()));
                else if (const auto cap2 = get<2>(match); cap2.matched())
                    to_reinsert_.emplace_back(cap2.str());
                else if (get<3>(match).matched())
                    visited_.emplace(current);
                else if (const auto cap4 = get<4>(match); cap4.matched())
                    copyrights_.emplace_back(cap4.str());
            }

            ofs << std::string_view{ saved, input.end() };
        }

        void insert_copyrights(std::ofstream& ofs)
        {
            std::ranges::sort(copyrights_);
            const auto& [first, last]{ std::ranges::unique(copyrights_) };
            copyrights_.erase(first, last);

            for (const auto& include : copyrights_)
                ofs << include << '\n';

            ofs << "\n\n";
        }

        void insert_includes(std::ofstream& ofs)
        {
            std::ranges::sort(to_reinsert_);
            const auto& [first, last]{ std::ranges::unique(to_reinsert_) };
            to_reinsert_.erase(first, last);

            ofs << "#pragma once" << "\n\n";

            for (const auto& include : to_reinsert_)
                ofs << include << '\n';
        }

        void copy_licence_to(std::ofstream& ofs, const path& file)
        {
            mmap_data data{ file };
            const auto input = data.view();

            rx::regex_replace(input.begin(), input.end(), std::ostream_iterator<char>{ ofs },
                              rx::static_regex<R"((?m:^.*?\n))">{}, rx::fmt<"// $0">);

            ofs << "\n\n";
        }

        void copy_tmpfile_to(std::ofstream& ofs, const path& file)
        {
            mmap_data data{ file };
            const auto input = data.view();

            rx::regex_replace(input.begin(), input.end(), std::ostream_iterator<char>{ ofs },
                              rx::static_regex<R"(\n\s+\n)", rx::mode::linear>{}, rx::fmt<"\n\n">);
        }

        std::vector<path> include_dirs_;
        std::unordered_set<path> visited_;
        std::vector<std::string> to_reinsert_;
        std::vector<std::string> copyrights_;
    };
}

int main(int argc, char* argv[])
{
    std::span args(argv, argc);

    if (args.size() != 5)
    {
        std::println(std::cerr, "usage: amalgamate include_dir input output licence");
        return 1;
    }

    amalgamator fn{ args[1] };
    fn(args[3], args[2], args[4]);
}
