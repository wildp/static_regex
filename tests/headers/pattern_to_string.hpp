// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <concepts>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

#include <rx/ast/tree.hpp>
#include <rx/etc/util.hpp>


// TODO: replace with constexpr std::format specialization
// TODO: add unicode transcoding support

namespace rx::testing
{
    template<typename CharT>
    [[nodiscard]] constexpr std::basic_string<CharT> to_basic_string(const detail::expr_tree<CharT>& ast)
    {
        class cont_val
        {
        public:
            using rep_t = std::int_least16_t;

            constexpr explicit(false) cont_val(std::size_t i) : pos_{ i } {}
            constexpr explicit cont_val(std::size_t i, rep_t r) : pos_{ i }, reps_{ r } {}
#if __cpp_lib_saturation_arithmetic >= 202603L
            constexpr explicit cont_val(std::size_t i, rep_t r, rep_t amt) : pos_{ i }, reps_{ std::saturating_add(r, amt) } {}
#else
            constexpr explicit cont_val(std::size_t i, rep_t r, rep_t amt) : pos_{ i }, reps_{ std::add_sat(r, amt) } {}
#endif

            [[nodiscard]] constexpr std::size_t pos() const noexcept { return pos_; }
            [[nodiscard]] constexpr rep_t reps() const noexcept { return reps_; }

        private:
            std::uint_least64_t pos_ : 48;
            rep_t reps_              : 16 { 0 };
        };


        using ast_t = detail::expr_tree<CharT>;
        using char_type = ast_t::char_type;
        using string_type = std::basic_string<char_type>;

        // Unicode transcoding unsupported -- TODO: change later
        static_assert(std::same_as<char_type, CharT>);
        static_assert(std::same_as<char_type, typename ast_t::char_class::underlying_char_type>);

        static constexpr auto append_int = [](string_type& result, const std::integral auto value) {
            static constexpr decltype(value) base{ 10 };
            std::vector<char> buf;
            if (value == 0)
                buf.emplace_back('0');
            else
                for (auto v = value; v > 0; v /= base)
                    buf.emplace_back('0' + (v % base));
            if (value < 0)
                buf.emplace_back('-');
            std::ranges::move(buf.rbegin(), buf.rend(), std::back_insert_iterator(result));
        };

        static constexpr auto escape = [](string_type& result, char_type c) {
            if (c == '\177' or (c >= '\0' and c < '\40'))
            {
                result += '\\';
                switch (c)
                {
                case '\0': result += '0'; break;
                case '\a': result += 'a'; break;
                case '\b': result += 'b'; break;
                case '\f': result += 'f'; break;
                case '\t': result += 't'; break;
                case '\n': result += 'n'; break;
                case '\r': result += 'r'; break;
                case '\v': result += 'v'; break;
                default:
                {
                    static constexpr char octal_base{ 010 };
                    string_type tmp;
                    for (; c != 0; c /= octal_base)
                        tmp.push_back('0' + (c % octal_base));
                    result.append(tmp.rbegin(), tmp.rend());
                    break;
                }
                }
            }
            else
            {
                if (c == '-' or c == '[' or c == ']' or c == '^') result += '\\';
                result += c;
            }
        };

        string_type result{};

        std::vector<cont_val> cont{};
        cont.emplace_back(ast.root_idx());

        while (not cont.empty())
        {
            const auto raw_data = cont.back();
            cont.pop_back();

            const auto pos = raw_data.pos();
            const auto reps = raw_data.reps();

            const auto& entry = ast.get_expr(pos);

            using detail::index_in_variant;

            switch (entry.index())
            {
            case index_in_variant(^^typename ast_t::assertion, ^^typename ast_t::type):
            {
                const auto& asr = std::get<typename ast_t::assertion>(entry);
                switch (asr.type)
                {
                case detail::assert_type::text_start:
                case detail::assert_type::line_start:
                    result += '^';
                    break;
                case detail::assert_type::text_end:
                case detail::assert_type::line_end:
                    result += '$';
                    break;
                case detail::assert_type::ascii_word_boundary:
                    result += '\\';
                    result += 'b';
                    break;
                case detail::assert_type::not_ascii_word_boundary:
                    result += '\\';
                    result += 'B';
                    break;
                default:
                    throw std::runtime_error("unrecognised assert_type");
                }
                break;
            }
            case index_in_variant(^^typename ast_t::concat, ^^typename ast_t::type):
            {
                const auto& cat = std::get<typename ast_t::concat>(entry);
                cont.append_range(cat.idxs | std::views::reverse);
                break;
            }
            case index_in_variant(^^typename ast_t::alt, ^^typename ast_t::type):
            {
                const auto& alt = std::get<typename ast_t::alt>(entry);
                if (std::cmp_less(reps, alt.idxs.size()))
                {
                    if (reps > 0)
                        result += '|';
                    cont.emplace_back(pos, reps + 1);
                    cont.emplace_back(alt.idxs.at(reps));
                }
                break;
            }
            case index_in_variant(^^typename ast_t::tag, ^^typename ast_t::type):
            {
                const auto& tag = std::get<typename ast_t::tag>(entry);
                auto [is_lhs, is_rhs] = ast.get_capture_info().capture_side(tag.number);
                if (is_lhs)
                    result += '(';
                if (is_rhs)
                    result += ')';
                break;
            }
            case index_in_variant(^^typename ast_t::backref, ^^typename ast_t::type):
            {
                const auto& bref = std::get<typename ast_t::backref>(entry);
                result += '\\';
                append_int(result, bref.number);
                break;
            }
            case index_in_variant(^^typename ast_t::repeat, ^^typename ast_t::type):
            {
                const auto& rep = std::get<typename ast_t::repeat>(entry);
                if (reps == 0)
                {
                    cont.emplace_back(pos, reps + 1);
                    cont.emplace_back(rep.idx);
                }
                else
                {
                    if (rep.min == 0 and rep.max < 0)
                    {
                        result += '*';
                    }
                    else if (rep.min == 1 and rep.max < 1)
                    {
                        result += '+';
                    }
                    else if ((rep.min == 0 and rep.max == 1))
                    {
                        result += '?';
                    }
                    else
                    {
                        result += '{';
                        append_int(result, rep.min);
                        if (rep.max != rep.min)
                            result += ',';
                        if (rep.max > rep.min)
                            append_int(result, rep.max);
                        result += '}';
                    }

                    if (rep.mode == detail::repeater_mode::lazy)
                        result += '?';
                    else if (rep.mode == detail::repeater_mode::possessive)
                        result += '+';
                }
                break;
            }
            case index_in_variant(^^typename ast_t::char_str, ^^typename ast_t::type):
            {
                const auto& lit = std::get<typename ast_t::char_str>(entry);
                result += lit.data;
                break;
            }
            case index_in_variant(^^typename ast_t::char_class, ^^typename ast_t::type):
            {
                const auto& cla = std::get<typename ast_t::char_class>(entry);

                using pair_t = typename ast_t::char_class::impl_type::underlying_type::char_interval;
                static constexpr std::size_t threshold{ (std::numeric_limits<char_type>::max() - std::numeric_limits<char_type>::min() + 1) / 2 };

                auto underlying = cla.data.get();
                const bool negated{ underlying.count() > threshold };

                if (negated)
                    underlying = ~underlying;

                const auto ci = underlying.get_intervals();

                if (negated and ((ci.empty()) or (ci.size() == 1 and ci.front() == pair_t{ '\n', '\n' })))
                {
                    result += '.';
                    break;
                }

                result += '[';
                if (negated)
                    result += '^';

                for (const auto& [lower, upper] : ci)
                {
                    escape(result, lower);
                    if (upper == lower)
                        continue;
                    result += '-';
                    escape(result, upper);
                }

                result += ']';

                break;
            }
            }
        }

        return result;
    }

    [[nodiscard]] constexpr std::string to_string(const detail::expr_tree<char>& ast)
    {
        return to_basic_string(ast);
    }
}
