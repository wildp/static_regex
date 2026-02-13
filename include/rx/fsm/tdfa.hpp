// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <cstddef>
#include <cstdlib>
#include <flat_map>
#include <limits>
#include <variant>
#include <vector>

#include "rx/etc/captures.hpp"
#include "rx/fsm/flags.hpp"
#include "rx/fsm/tnfa.hpp"


namespace rx::detail::tdfa
{
    template<typename CharT>
    using charset_t = tnfa::charset_t<CharT>;

    template<typename CharT> class factory;
    template<typename CharT> class opt;
    template<typename CharT> class min;

    using reg_t = std::uint_least32_t;
    using continue_at_t = tnfa::continue_at_t;

    struct regop
    {
        struct set
        {
            bool val; /* use false for nil and true for current */

            friend constexpr bool operator==(set, set) noexcept = default;
            friend constexpr std::strong_ordering operator<=>(set, set) noexcept = default;
        };

        struct copy
        {
            reg_t src;

            friend constexpr bool operator==(copy, copy) noexcept = default;
            friend constexpr std::strong_ordering operator<=>(copy, copy) noexcept = default;
        };

        using op_t = std::variant<set, copy>;

        // TODO: optimise layout as necessary to be as compact as possible

        op_t op;
        reg_t dst;

        constexpr regop(reg_t destination, op_t operation)
            : op{ operation }, dst{ destination } {}

        friend constexpr bool operator==(regop, regop) = default;
        friend constexpr std::strong_ordering operator<=>(regop, regop) noexcept = default;
    };

    using regops_t = std::vector<regop>;
    inline constexpr regops_t empty_regops{};

    struct final_node_info
    {
        std::size_t         op_index;
        std::uint_least16_t final_offset;

        friend constexpr bool operator==(const final_node_info&, const final_node_info&) noexcept = default;
        friend constexpr auto operator<=>(const final_node_info&, const final_node_info&) noexcept = default;
    };

    struct fallback_node_info
    {
        std::size_t   op_index;
        continue_at_t continue_at;

        friend constexpr bool operator==(const fallback_node_info&, const fallback_node_info&) noexcept = default;
        friend constexpr auto operator<=>(const fallback_node_info&, const fallback_node_info&) noexcept = default;
    };

    using continue_nodes_t = std::vector<std::size_t>;
    using final_nodes_t    = std::flat_map<std::size_t, final_node_info>;
    using fallback_nodes_t = std::flat_map<std::size_t, fallback_node_info>;
    using final_regs_t     = std::vector<reg_t>;

    inline constexpr continue_at_t no_continue{ std::numeric_limits<continue_at_t>::max() };
    inline constexpr std::size_t   no_transition_regops{ std::numeric_limits<std::size_t>::max() };
    inline constexpr std::size_t   default_transition_is_not_state{ std::numeric_limits<std::size_t>::max() - 1 };

    constexpr bool toposort_regops(regops_t::iterator beg, regops_t::iterator end, reg_t regcount);

    template<typename CharT>
    struct transition
    {
        std::size_t next;
        std::size_t op_index; /* use no_transition_regops for no ops */
        charset_t<CharT> cs;

        friend constexpr bool operator==(const transition&, const transition&) = default;
    };

    struct default_transition
    {
        std::size_t next;
        std::size_t op_index; /* use no_transition_regops for no ops, and default_transition_is_not_state for jumps */

        friend constexpr bool operator==(const default_transition&, const default_transition&) = default;
    };

    template<typename CharT>
    struct node
    {
        std::vector<transition<CharT>> tr;
        std::optional<default_transition> default_tr;

        friend constexpr bool operator==(const node&, const node&) = default;
    };
}

namespace rx::detail
{
    template<typename CharT>
    class tagged_dfa
    {
    public:
        using char_type = CharT;

        explicit constexpr tagged_dfa(const tagged_nfa<char_type>& tnfa);
        constexpr void optimise_registers();
        constexpr void compact_regop_blocks();
        constexpr void minimise_states();
        constexpr void minimise_transition_edges();
        constexpr void make_default_transitions();
        constexpr void make_shared_transitions();
        constexpr void de_default_edges();

        friend class tdfa::factory<char_type>;
        friend class tdfa::opt<char_type>;
        friend class tdfa::min<char_type>;

        /* observers */

        static constexpr std::size_t match_start{ 0 };

        [[nodiscard]] constexpr const tdfa::node<CharT>& get_node(std::size_t i) const { return nodes_.at(i); }
        [[nodiscard]] constexpr const tdfa::regops_t& get_regops(std::size_t i) const { return (i == tdfa::no_transition_regops) ? tdfa::empty_regops : regops_.at(i); }
        [[nodiscard]] constexpr const tdfa::continue_nodes_t& continue_nodes() const { return continue_nodes_; }
        [[nodiscard]] constexpr const tdfa::final_nodes_t& final_nodes() const { return final_nodes_; }
        [[nodiscard]] constexpr const tdfa::fallback_nodes_t& fallback_nodes() const { return fallback_nodes_; }
        [[nodiscard]] constexpr const tdfa::final_regs_t& final_registers() const { return final_registers_; }
        [[nodiscard]] constexpr std::size_t node_count() const { return nodes_.size(); }
        [[nodiscard]] constexpr std::size_t reg_count() const { return register_count_; }
        [[nodiscard]] constexpr std::size_t tag_count() const { return tag_count_; }
        [[nodiscard]] constexpr const capture_info& get_capture_info() const { return capture_info_; }

        template<typename>
        friend struct tdfa_info;

    private:
        using data_t = std::vector<tdfa::node<char_type>>;
        using regop_data_t = std::vector<tdfa::regops_t>;

        data_t                  nodes_;
        tdfa::continue_nodes_t  continue_nodes_;
        tdfa::final_nodes_t     final_nodes_;
        tdfa::fallback_nodes_t  fallback_nodes_;
        tdfa::final_regs_t      final_registers_;
        regop_data_t            regops_;
        capture_info            capture_info_;
        std::size_t             tag_count_;
        tdfa::reg_t             register_count_{ 0 };
        fsm_flags               flags_;
    };
}

#include "tdfa.tpp"
#include "regoptimise.tpp"
#include "minimise.tpp"
