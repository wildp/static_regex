// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>
#include <variant>
#include <vector>

#include "rx/ast/tree.hpp"
#include "rx/etc/bitcharset.hpp"
#include "rx/etc/charset.hpp"
#include "rx/fsm/flags.hpp"


namespace rx::detail::tnfa
{
    using state_t = std::size_t;
    using tr_index = std::size_t;
    using continue_at_t = std::uint_least16_t;

    template<typename CharT>
    using charset_t = std::conditional_t<sizeof(CharT) == 1, bitcharset<CharT>, charset<CharT>>;

    namespace assert_category
    {
        struct eof_tag_t { explicit eof_tag_t() = default; };
        struct sof_tag_t { explicit sof_tag_t() = default; };
        struct lookahead1_tag_t { explicit lookahead1_tag_t() = default; };
        struct lookbehind1_tag_t { explicit lookbehind1_tag_t() = default; };

        inline constexpr eof_tag_t         eof_tag{};
        inline constexpr sof_tag_t         sof_tag{};
        inline constexpr lookahead1_tag_t  lookahead1_tag{};
        inline constexpr lookbehind1_tag_t lookbehind1_tag{};
    }

    /* tnfa transitions */

    template<typename CharT>
    struct normal_tr
    {
        charset_t<CharT> cs;
    };

    struct epsilon_tr
    {
        int priority;
        int tag; /* use tag = 0 when tag is absent, tag > 0 for regular tags, and tag < 0 for negative tags */
    };

    struct sof_anchor_tr {};
    struct eof_anchor_tr {};

    template<typename CharT>
    struct lookahead_1_tr
    {
        charset_t<CharT> cs;
    };

    template<typename CharT>
    struct lookbehind_1_tr
    {
        charset_t<CharT> cs;
    };

    // struct lookahead_tr 
    // {
    //     state_t p0, pf;
    //     bool negate;
    // };

    // struct lookbehind_tr
    // {
    //     state_t p0, pf;
    //     bool negate;
    // };

    template<typename CharT>
    struct transition
    {
        using transition_type = std::variant<std::monostate
                                             , normal_tr<CharT>, epsilon_tr
                                             , eof_anchor_tr, sof_anchor_tr
                                             , lookahead_1_tr<CharT>, lookbehind_1_tr<CharT>
                                             //  , lookahead_tr, lookbehind_tr
        >;

        state_t         src, dst;
        transition_type type; 

        template<typename... Args>
        constexpr transition(state_t q0, state_t qf, Args&&... args)
            : src{ q0 }, dst{ qf }, type{ (std::forward<Args>(args))... } {}

        constexpr void unset()
        {
            src = std::numeric_limits<state_t>::max();
            dst = std::numeric_limits<state_t>::max();
            type.template emplace<std::monostate>();
        }
    };


    /* tnfa nodes */

    struct node
    {
        std::vector<tr_index> in_tr;
        std::vector<tr_index> out_tr;

        bool is_final{ false };
        bool is_fallback{ false };             /* must equal false if not is_final */
        std::uint_least16_t final_offset{ 0 }; /* only meaningful if is_final */
        continue_at_t continue_at{ 0 };        /* only meaningful if is_final */
    };

    struct continue_info
    {
        using sub_e_closure = std::optional<std::vector<state_t>>;

        state_t value;
        // sub_e_closure sub_ec;
    };


    /* predicates for variations of e-closure */

    struct reachable_predicate
    {
        template<typename CharT>
        static constexpr bool operator()(const transition<CharT>& /* tr */)
        {
            /* count everything */
            return true;
        }
    };

    struct e_closure_predicate
    {
        template<typename CharT>
        static constexpr bool operator()(const transition<CharT>& tr)
        {
            /* only count e-transitions */
            return std::holds_alternative<epsilon_tr>(tr.type);
        }
    };
}


namespace rx::detail
{
    /* tnfa class */

    template<typename CharT>
    class tagged_nfa
    {
    public:
        using char_type = CharT;
        using state_t = tnfa::state_t;
        using tr_index = tnfa::tr_index;
        using charset_type = tnfa::charset_t<char_type>;

        using normal_tr = tnfa::normal_tr<char_type>;
        using epsilon_tr = tnfa::epsilon_tr;
        using sof_anchor_tr = tnfa::sof_anchor_tr;
        using eof_anchor_tr = tnfa::eof_anchor_tr;
        using lookbehind_1_tr = tnfa::lookbehind_1_tr<char_type>;
        using lookahead_1_tr = tnfa::lookahead_1_tr<char_type>;

        explicit constexpr tagged_nfa(const expr_tree<char_type>& ast, fsm_flags flags);
        constexpr void rewrite_assertions();

        /* observers */

        [[nodiscard]] constexpr auto get_flags() const noexcept { return flags_; }
        [[nodiscard]] constexpr const tnfa::node& get_node(state_t q) const { return nodes_.at(q); }
        [[nodiscard]] constexpr const tnfa::transition<char_type>& get_tr(tr_index idx) const { return transitions_.at(idx); }
        [[nodiscard]] constexpr std::size_t node_count() const noexcept { return nodes_.size(); }
        [[nodiscard]] constexpr std::size_t transition_count() const noexcept { return transitions_.size(); }
        [[nodiscard]] constexpr std::size_t tag_count() const noexcept { return tag_count_; }
        [[nodiscard]] constexpr const capture_info& get_capture_info() const noexcept { return capture_info_; }
        [[nodiscard]] constexpr state_t start_node() const noexcept { return start_node_; }
        [[nodiscard]] constexpr const auto& get_cont_info() const { return cont_info_; }

    private:
        using ast_t = expr_tree<char_type>;
        using transition_info = tnfa::transition<char_type>::transition_type;

        template<in_variant<typename ast_t::type> T>
        static constexpr state_t ast_index{ index_of_impl<typename ast_t::type, T>::value };

        /* fundamental helpers */

        constexpr state_t node_create();
        constexpr state_t node_copy(state_t q);

        template<typename... Args>
        constexpr void transition_create(state_t q0, state_t qf, Args&&... args);

        /* transition creation */

        constexpr void make_epsilon(state_t q0, state_t qf, int priority = 0, int tag = 0);
        
        constexpr void make_transition(state_t q0, state_t qf, char_type c);

        template<typename CharSet>
        requires std::convertible_to<std::remove_cvref_t<CharSet>, charset_type>
        constexpr void make_transition(state_t q0, state_t qf, CharSet&& cs);

        template<typename T>
        requires one_of<T, tnfa::assert_category::eof_tag_t, tnfa::assert_category::sof_tag_t>
        constexpr void make_assert(state_t q0, state_t qf, T category);

        template<typename T, typename CharSet>
        requires one_of<T, tnfa::assert_category::lookahead1_tag_t, tnfa::assert_category::lookbehind1_tag_t>
                 and std::convertible_to<std::remove_cvref_t<CharSet>, charset_type>
        constexpr void make_assert(state_t q0, state_t qf, T category, CharSet&& cs);

        constexpr void make_copy(state_t q0, state_t qf, const transition_info& type);

        constexpr void make_ntags(state_t q0, state_t qf, const std::vector<int>& ntags);

        constexpr void thompson(const expr_tree<char_type>& ast);

        template<bool B, typename Vec, typename Pred, typename NodeProj, typename TrProj>
        [[nodiscard]] constexpr auto closure_impl(Vec&& qs, Pred pred, NodeProj node_proj, TrProj tr_proj) const;

        template<bool B = true, typename Vec = std::vector<state_t>, typename Pred = tnfa::e_closure_predicate>
        [[nodiscard]] constexpr auto epsilon_closure(Vec&& qs, Pred pred = {}) const;

        template<bool B = true, typename Vec = std::vector<state_t>, typename Pred = tnfa::e_closure_predicate>
        [[nodiscard]] constexpr auto backwards_epsilon_closure(Vec&& qs, Pred pred = {}) const;

        constexpr void remove_dead_and_unreachable_states();
        constexpr void rewrite_sof_anchors();
        constexpr void rewrite_eof_anchors();
        constexpr void rewrite_sc_lookahead();
        constexpr void rewrite_sc_lookbehind();

        /* constants */

        static constexpr state_t default_start_node{ 0 };
        static constexpr state_t default_final_node{ 1 };

        /* data members */

        std::vector<tnfa::node>                  nodes_{ 2 };
        std::vector<tnfa::transition<char_type>> transitions_;
        capture_info                             capture_info_;
        std::size_t                              tag_count_;
        state_t                                  start_node_{ default_start_node };
        std::vector<tnfa::continue_info>         cont_info_;

        fsm_flags flags_;

        bool has_eof_anchor_   : 1 { false };
        bool has_sof_anchor_   : 1 { false };
        bool has_lookahead_1_  : 1 { false };
        bool has_lookbehind_1_ : 1 { false };
    };
}

#include "tnfa.tpp"
