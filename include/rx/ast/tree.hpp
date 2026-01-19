// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <cstdint>
#include <variant>
#include <vector>

#include "rx/ast/tok.hpp"
#include "rx/etc/captures.hpp"
#include "rx/etc/util.hpp"


namespace rx::detail
{
    namespace parser
    {
        template<typename CharT>
        class ll1;
    }

    /* types */

    enum class repeater_mode : int_least8_t
    {
        greedy = 0,
        lazy,
        possessive
    };

    struct parser_flags
    {
        bool enable_captures    : 1 { true };
        bool enable_start_tag   : 1 { true };
        bool enable_possessive  : 1 { false };
        bool enable_backrefs    : 1 { false };
        bool enable_branchreset : 1 { false };
        bool enable_alttocc     : 1 { true };
    };


    /* ast definition */

    template<typename CharT>
    class expr_tree
    {
    public:
        using char_type = CharT;
        using sv_type = std::basic_string_view<char_type>;

        struct alt
        {
            std::vector<std::size_t> idxs;
        };

        struct concat
        {
            std::vector<std::size_t> idxs;
        };

        struct tag
        {
            tag_number_t number;
        };

        using backref = tok::backref;

        struct repeat
        {
            std::size_t idx;
            std::int_least16_t min;
            std::int_least16_t max; /* use max=min for {min} or max<min for {min,} */
            repeater_mode mode;     /* default = greedy */
        };

        using assertion = tok::assertion;
        using char_str = tok::char_str<char_type>;
        using char_class = tok::char_class<char_type>;

        using type = std::variant<assertion, char_str, char_class, backref, alt, concat, tag, repeat>;

        constexpr expr_tree(sv_type sv, parser_flags flags = {});

        friend class parser::ll1<char_type>;

        [[nodiscard]] constexpr const type& get_expr(std::size_t i) const { return expressions_.at(i); }
        [[nodiscard]] constexpr std::size_t root_idx() const { return root_idx_; }
        [[nodiscard]] constexpr std::size_t tag_count() const { return tag_count_; }
        [[nodiscard]] constexpr const capture_info& get_capture_info() const { return capture_info_; }

        constexpr void make_tag_vec(std::vector<std::vector<int>>& tag_vec) const;
        constexpr void optimise_tags();
        constexpr void insert_search_prefix();

    private:
        template<in_variant<type> T>
        static constexpr std::size_t ast_index{ index_of_impl<type, T>::value };

        [[nodiscard]] constexpr std::vector<std::optional<std::size_t>> make_const_len_vec();

        std::size_t root_idx_{ 0 };
        std::vector<type> expressions_;
        capture_info capture_info_;
        tag_number_t tag_count_{ 0 };
        parser_flags flags_;
    };

    template<typename CharT>
    expr_tree(const CharT*) -> expr_tree<CharT>;

    template<typename CharT>
    expr_tree(const CharT*, parser_flags) -> expr_tree<CharT>;
}


#include "tree.tpp"
#include "parser.tpp"
