// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <meta>
#include <ranges>
#include <utility>
#include <vector>

#include "rx/api/regex_error.hpp"
#include "rx/ast/tree.hpp"
#include "rx/etc/static_span.hpp"
#include "rx/etc/string_literal.hpp"
#include "rx/fsm/flags.hpp"
#include "rx/fsm/tdfa.hpp"
#include "rx/fsm/tnfa.hpp"


namespace rx::detail
{
    struct final_capture_info
    {
        explicit consteval final_capture_info(const capture_info& ci)
            : captures{ ci.get_values_branchfree() } {}

        consteval final_capture_info() = default;

        [[nodiscard]] consteval std::size_t capture_count() const noexcept
        {
            return captures.size();
        }

        [[nodiscard]] consteval bool has_match_start() const
        {
            static constexpr auto pred = [](const capture_info::tag_pair_t& pair) {
                const auto& [fst, snd] = pair;
                return (fst.tag_number == start_of_input_tag or snd.tag_number == start_of_input_tag);
            };

            return std::ranges::any_of(captures, pred);
        }

        /* data members (public so that final_capture_info is structural) */
        static_span<capture_info::tag_pair_t> captures;
    };

    struct static_match_result_info
    {
        final_capture_info fci;
        static_span<tdfa::reg_t> final_registers;
        std::size_t register_count{ 0 };
        bool has_continue{ false };
        bool continue_from_it{ false };
    };

    struct register_operation
    {
        tdfa::reg_t dst;
        tdfa::reg_t cpy_src;
        bool        set_val;
        bool        is_copy; /* true if cpy, false if set */
    };

    template<typename CharT>
    struct static_transition
    {
        std::size_t next;
        std::size_t op_index;
        static_span<std::pair<CharT, CharT>> cs;
    };

    template<typename CharT>
    struct tdfa_info
    {
        using char_type = CharT;

    private:
        static consteval auto make_static_transition(const tdfa::transition<char_type>& tr)
        {
            return static_transition{ tr.next, tr.op_index, static_span{ tr.cs.get_intervals() } };
        }

        static consteval auto make_static_outer_transition(const std::pair<std::size_t, tnfa::charset_t<char_type>>& otr)
        {
            return static_transition{ otr.first, tdfa::no_transition_regops, static_span{ otr.second.get_intervals() } };
        }

        static consteval auto make_node_transitions(const tdfa::node<char_type>& n)
        {
            return static_span{ n.tr | std::views::transform(make_static_transition) };
        }

        static consteval auto make_register_operations(const tdfa::regops_t& o)
        {
            return static_span{ o | std::views::transform(
                [](const tdfa::regop& op) consteval -> register_operation {
                    if (const auto* set = get_if<tdfa::regop::set>(&op.op); set != nullptr)
                        return { .dst = op.dst, .cpy_src = 0, .set_val = set->val, .is_copy = false };
                    else if (const auto* cpy = get_if<tdfa::regop::copy>(&op.op); cpy != nullptr)
                        return { .dst = op.dst, .cpy_src = cpy->src, .set_val = false, .is_copy = true };
                    else
                        std::unreachable();
                })
            };
        }

        static consteval auto make_continue_info(const tagged_dfa<char_type>& dfa, const tagged_nfa<char_type>& nfa)
        {
            /* adapted from tagged_dfa::minimise_transition_edges */
            using tr_type = std::pair<std::size_t, tdfa::charset_t<char_type>>;

            std::vector<tr_type> vec;

            for (const auto& [nfa_cont, dfa_cont] : std::views::zip(nfa.get_cont_info(), dfa.continue_nodes()))
                vec.emplace_back(dfa_cont, std::cref(nfa_cont.cs));

            if (not vec.empty())
            {
                // TODO: switch to using views::enumerate when supported by clang
                auto scored_pairs = std::views::zip(std::views::iota(0uz),
                                                    vec | std::views::transform([](const auto& t) { return t.second.score_intervals(); }))
                                    | std::ranges::to<std::vector>();

                std::ranges::sort(scored_pairs, {}, [](const auto& x){ return get<1>(x); });

                std::vector<tr_type> new_tr;
                std::vector<tdfa::charset_t<char_type>> dont_cares;
                tdfa::charset_t<char_type> acc;

                for (const auto& [i, _] : scored_pairs)
                {
                    auto& tr = vec.at(i);
                    dont_cares.emplace_back(acc);
                    acc |= tr.second;
                    new_tr.emplace_back(std::move(tr));
                }

                if (acc.full())
                {
                    new_tr.back().second = std::move(acc);
                    dont_cares.back() = tdfa::charset_t<char_type>{};
                }

                /* fill gaps where possible */

                for (const auto& [tr_ref, dont_cares] : std::views::zip(std::ranges::ref_view(new_tr), dont_cares))
                {
                    tr_type& tr{ tr_ref };

                    if (dont_cares.empty())
                        continue;

                    using interval_t = tdfa::charset_t<char_type>::char_interval;
                    std::vector<interval_t> to_insert;

                    std::ranges::set_intersection((~tr.second).get_intervals(), dont_cares.get_intervals(), std::back_inserter(to_insert));

                    for (const auto [beg, end] : to_insert)
                    {
                        if (beg == end)
                            tr.second.insert(beg);
                        else
                            tr.second.insert(beg, end);
                    }
                }

                vec = std::move(new_tr);
            }

            return static_span{ vec | std::views::transform(make_static_outer_transition) };
        }

    public:
        explicit consteval tdfa_info(const tagged_dfa<char_type>& dfa, const tagged_nfa<char_type>& nfa)
            : nodes{ dfa.nodes_ | std::views::transform(make_node_transitions) }
            , regops{ dfa.regops_ | std::views::transform(make_register_operations) }
            , continue_nodes{ dfa.continue_nodes() }
            , additional_continue_nodes{ dfa.additional_continue_nodes() }
            , final_nodes{ dfa.final_nodes() }
            , fallback_nodes{ dfa.fallback_nodes() }
            , final_registers{ dfa.final_registers() }
            , register_count{ dfa.reg_count() }
            , match_start{ dfa.match_start }
            , captures{ dfa.get_capture_info() }
            , outer_transitions{ make_continue_info(dfa, nfa) } {}

        [[nodiscard]] consteval static_match_result_info make_match_result_info(bool has_continue) const
        {
            if (has_continue and continue_nodes.size() == 1 and continue_nodes[0] == match_start)
                has_continue = false;

            return { .fci = captures, .final_registers = final_registers, .register_count = register_count, .has_continue = has_continue };
        }

        /* data members (public so that tdfa_info is structural) */
        static_span<static_span<static_transition<char_type>>> nodes;
        static_span<static_span<register_operation>> regops;
        static_span<std::size_t> continue_nodes;
        static_span<std::size_t> additional_continue_nodes;
        static_map<std::size_t, tdfa::final_node_info> final_nodes;
        static_map<std::size_t, tdfa::fallback_node_info> fallback_nodes;
        static_span<tdfa::reg_t> final_registers;

        std::size_t register_count{ 0 };
        std::size_t match_start{ 0 };
        final_capture_info captures;

        static_span<static_transition<char_type>> outer_transitions;
    };


    template<typename CharT>
    consteval tdfa_info<CharT> compile_pattern(std::basic_string_view<CharT> pattern, fsm_flags f)
    {
        /* set parser flags as appropriate */
        parser_flags p{};
        if (f.no_captures)
            p.enable_captures = false;
        if (f.return_bool)
            p.enable_start_tag = false;

        /* parse pattern string into tree */
        expr_tree ast{ pattern, p };
        if (f.is_search)
            ast.insert_search_prefix();
        ast.optimise_tags();

        /* convert to tnfa */
        tagged_nfa nfa{ ast, f };
        nfa.rewrite_assertions();
        if (f.maybe_no_empty and ast.empty_match_possible())
            nfa.add_non_empty_match_pathway();

        /* convert to tdfa */
        tagged_dfa dfa{ nfa };
        dfa.optimise_registers();
        // dfa.minimise_states();

        /* optimise transition edges and their order to produce fewest comparisons */
        /* (if using tables, do `dfa.make_default_tr_if_possible()` instead) */
        dfa.minimise_transition_edges();
        dfa.de_default_edges();

        return tdfa_info{ dfa, nfa };
    }


    struct match_non_empty_t {};
    inline constexpr match_non_empty_t match_non_empty;
}
