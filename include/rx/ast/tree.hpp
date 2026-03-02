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

    enum class special_group_mode : int_least8_t
    {
        atomic_group = 0,
        positive_lookahead,
        negative_lookahead,
        positive_lookbehind,
        negative_lookbehind,

        /* backtracking control */
        backtrack_accept,
        backtrack_fail,
        backtrack_mark,
        backtrack_commit,
        backtrack_prune,
        backtrack_skip,
        backtrack_then
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

    namespace ast_entry
    {
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

        struct repeat
        {
            std::size_t idx;
            std::int_least16_t min;
            std::int_least16_t max; /* use max=min for {min} or max<min for {min,} */
            repeater_mode mode;     /* default = greedy */
        };

        struct special
        {
            std::size_t idx;         /* use idx=-1 to ignore when mode is backtrack_.*  */
            special_group_mode mode; /* for backtrack_.*, treat idx as the name of a mark when not ignored */
        };
    }


    /* ast definition */

    template<typename CharT>
    class expr_tree
    {
    public:
        using char_type = CharT;
        using sv_type = std::basic_string_view<char_type>;

        using assertion     = tok::assertion;
        using char_str      = tok::char_str<char_type>;
        using char_class    = tok::char_class<char_type>;
        using backref       = tok::backref;
        using alt           = ast_entry::alt;
        using concat        = ast_entry::concat;
        using tag           = ast_entry::tag;
        using repeat        = ast_entry::repeat;
        using special       = ast_entry::special;

        using type = std::variant<assertion, char_str, char_class, backref, alt, concat, tag, repeat>;

        constexpr expr_tree(sv_type sv, parser_flags flags = {});

        friend class parser::ll1<char_type>;

        [[nodiscard]] constexpr const type& get_expr(std::size_t i) const { return expressions_.at(i); }
        [[nodiscard]] constexpr std::size_t root_idx() const noexcept { return root_idx_; }
        [[nodiscard]] constexpr std::size_t tag_count() const noexcept { return tag_count_; }
        [[nodiscard]] constexpr const auto& get_all_exprs() const noexcept { return expressions_; }
        [[nodiscard]] constexpr const auto& get_capture_info() const noexcept { return capture_info_; }

        constexpr void make_tag_vec(std::vector<std::vector<int>>& tag_vec) const;
        constexpr void optimise_tags();
        constexpr void insert_search_prefix();
        constexpr void simplify_counted_repeat();
        constexpr void optimise_exact_repeat();
        constexpr auto tag_to_register();

    private:
        template<typename T>
        static constexpr std::size_t ast_index{ index_in_variant(^^T, ^^type) };

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
