// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <meta>
#include <numeric>
#include <ranges>
#include <utility>
#include <vector>

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
        {
            using namespace std::placeholders;

            std::vector<capture_info::tag_pair_t> captures_tmp;
            std::vector<capture_info::tag_pair_t> overflow_tmp;

            for (std::size_t i{ 0 }; i < ci.capture_count(); ++i)
            {
                const auto range{ ci.lookup(i) };

                if (std::ranges::size(range) == 1)
                {
                    captures_tmp.emplace_back(*std::ranges::begin(range));
                }
                else
                {
                    const auto begin{ std::saturate_cast<int>(overflow_tmp.size()) };
                    overflow_tmp.append_range(range);
                    const auto end{ std::saturate_cast<int>(overflow_tmp.size()) };
                    captures_tmp.emplace_back(capture_info::pair_entry{ .tag_number = end_of_input_tag, .offset = begin },
                                              capture_info::pair_entry{ .tag_number = start_of_input_tag, .offset = end });
                }
            }

            captures = static_span{ captures_tmp };
            overflow = static_span{ overflow_tmp };
        }

        [[nodiscard]] consteval std::size_t capture_count() const noexcept
        {
            return captures.size();
        }

        [[nodiscard]] consteval bool has_match_start() const
        {
            for (const auto& [fst, snd] : captures)
            {
                if (fst.tag_number == end_of_input_tag and snd.tag_number == start_of_input_tag)
                    continue;
                if (fst.tag_number == start_of_input_tag or snd.tag_number == start_of_input_tag)
                    return true;
            }

            return std::ranges::any_of(overflow, [](const capture_info::tag_pair_t& p) {
                return p.first.tag_number == start_of_input_tag or p.second.tag_number == start_of_input_tag;
            });
        }

        /* data members (public so that final_capture_info is structural) */
        static_span<capture_info::tag_pair_t> captures; /* use invalid tag pairs to point towards overflow being used */
        static_span<capture_info::tag_pair_t> overflow;
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

        static consteval auto make_node_transitions(const tdfa::node<char_type>& n)
        {
            return static_span{ n.tr | std::views::transform(make_static_transition) };
        }

        static consteval auto make_register_operations(const tdfa::regops_t& o)
        {
            return static_span{ o | std::views::transform(
                [](const tdfa::regop& op) consteval -> register_operation {
                    if (const auto* set{ std::get_if<tdfa::regop::set>(&op.op) }; set != nullptr)
                        return { .dst = op.dst, .cpy_src = 0, .set_val = set->val, .is_copy = false };
                    else if (const auto* cpy{ std::get_if<tdfa::regop::copy>(&op.op) }; cpy != nullptr)
                        return { .dst = op.dst, .cpy_src = cpy->src, .set_val = false, .is_copy = true };
                    else
                        std::unreachable();
                })
            };
        }

        static consteval auto make_default_transitions(const std::vector<tdfa::node<char_type>>& ns)
        {
            std::flat_map<std::size_t, tdfa::default_transition> result;
            for (std::size_t i{ 0 }; i < ns.size(); ++i)
                if (const auto& n{ ns[i] }; n.default_tr.has_value())
                    result.emplace_hint(result.end(), i, *n.default_tr);
            return result;
        }

    public:
        explicit consteval tdfa_info(const tagged_dfa<char_type>& dfa)
            : nodes{ dfa.nodes_ | std::views::transform(make_node_transitions) },
              regops{ dfa.regops_ | std::views::transform(make_register_operations) },
              continue_nodes{ dfa.continue_nodes() },
              final_nodes{ dfa.final_nodes() },
              fallback_nodes{ dfa.fallback_nodes() },
              default_transitions{ make_default_transitions(dfa.nodes_) },
              final_registers{ dfa.final_registers() },
              register_count{ dfa.reg_count() },
              match_start{ dfa.match_start },
              captures{ dfa.get_capture_info() } {}

        /* data members (public so that tdfa_info is structural) */
        static_span<static_span<static_transition<char_type>>> nodes;
        static_span<static_span<register_operation>> regops;
        static_span<std::size_t> continue_nodes;
        static_map<std::size_t, tdfa::final_node_info> final_nodes;
        static_map<std::size_t, tdfa::fallback_node_info> fallback_nodes;
        static_map<std::size_t, tdfa::default_transition> default_transitions;
        static_span<tdfa::reg_t> final_registers;

        std::size_t register_count{ 0 };
        std::size_t match_start{ 0 };
        final_capture_info captures;
    };


    template<typename CharT, std::size_t N>
    consteval tdfa_info<CharT> compile_pattern(string_literal<CharT, N> pattern, fsm_flags f)
    {
        /* set parser flags as appropriate */
        parser_flags p{};
        if (f.no_captures) p.enable_captures = false;
        if (f.return_bool) p.enable_start_tag = false;

        /* parse pattern string into tree */
        expr_tree ast{ pattern.view(), p };
        if (f.is_search) ast.insert_search_prefix();
        ast.optimise_tags();

        /* convert to tnfa */
        tagged_nfa nfa{ ast, f };
        nfa.rewrite_assertions();

        /* convert to tdfa */
        tagged_dfa dfa{ nfa };
        dfa.optimise_registers();
        dfa.minimise_states();

        /* optimise transition edges and their order to produce fewest comparisons */
        /* (if using tables, do `dfa.make_default_tr_if_possible()` instead) */
        dfa.minimise_transition_edges();

        return tdfa_info{ dfa };
    }

    template<string_literal Pattern, fsm_flags Flags>
    struct compiled_dfa
    {
        static constexpr auto value{ compile_pattern(Pattern, Flags) };
    };
}
