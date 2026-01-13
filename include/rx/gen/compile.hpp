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

#include "../ast/tree.hpp"
#include "../etc/static_span.hpp"
#include "../etc/string_literal.hpp"
#include "../fsm/flags.hpp"
#include "../fsm/tdfa.hpp"
#include "../fsm/tnfa.hpp"


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

            captures = std::define_static_array(captures_tmp);
            overflow = std::define_static_array(overflow_tmp);
        }

        [[nodiscard]] consteval std::size_t capture_count() const noexcept
        {
            return captures.size();
        }

        [[nodiscard]] consteval bool has_match_start() const
        {
            for (const auto [fst, snd] : captures)
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
        static_span<const capture_info::tag_pair_t> captures; /* use invalid tag pairs to point towards overflow being used */
        static_span<const capture_info::tag_pair_t> overflow;
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
        static_span<const std::pair<CharT, CharT>> cs;
    };

    template<typename CharT>
    struct tdfa_info
    {
        using char_type = CharT;

    private:
        static consteval auto make_static_transition(const tdfa::transition<char_type>& tr)
        {
            return static_transition{ tr.next, tr.op_index, static_span{ std::define_static_array(tr.cs.get_intervals()) } };
        }

        static consteval auto make_node_transitions(const tdfa::node<char_type>& n)
        {
            return static_span<const static_transition<char_type>>{ std::define_static_array(n.tr | std::views::transform(make_static_transition)) };
        }

        static consteval auto make_register_operations(const tdfa::regops_t& o) -> static_span<const register_operation>
        {
            return std::define_static_array(o | std::views::transform(
                [](const tdfa::regop& op) consteval -> register_operation {
                    if (const auto* set{ std::get_if<tdfa::regop::set>(&op.op) }; set != nullptr)
                        return { .dst = op.dst, .cpy_src = 0, .set_val = set->val, .is_copy = false };
                    else if (const auto* cpy{ std::get_if<tdfa::regop::copy>(&op.op) }; cpy != nullptr)
                        return { .dst = op.dst, .cpy_src = cpy->src, .set_val = false, .is_copy = true };
                    else
                        std::unreachable();
                }
            ));
        }

    public:
        explicit consteval tdfa_info(const tagged_dfa<char_type>& dfa)
            : nodes{ std::define_static_array(dfa.nodes_ | std::views::transform(make_node_transitions)) },
              regops{ std::define_static_array(dfa.regops_ | std::views::transform(make_register_operations)) },
              continue_nodes{ std::define_static_array(dfa.continue_nodes()) },
              final_nodes{ std::define_static_array(dfa.final_nodes().keys()) },
              final_node_regops{ std::define_static_array(dfa.final_nodes().values()) },
              fallback_nodes{ std::define_static_array(dfa.fallback_nodes().keys()) },
              fallback_node_regops{ std::define_static_array(dfa.fallback_nodes().values()) },
              final_registers{ std::define_static_array(dfa.final_registers()) },
              register_count{ dfa.reg_count() },
              match_start{ dfa.match_start },
              captures{ dfa.get_capture_info() } {}

        /* data members (public so that tdfa_info is structural) */
        static_span<const static_span<const static_transition<char_type>>> nodes;
        static_span<const static_span<const register_operation>> regops;
        static_span<const std::size_t> continue_nodes;
        static_span<const std::size_t> final_nodes;
        static_span<const tdfa::final_node_info> final_node_regops;
        static_span<const std::size_t> fallback_nodes;
        static_span<const tdfa::fallback_node_info> fallback_node_regops;
        static_span<const tdfa::reg_t> final_registers;

        std::size_t register_count{ 0 };
        std::size_t match_start{ 0 };
        final_capture_info captures;
    };


    template<typename CharT, std::size_t N>
    consteval tdfa_info<CharT> compile_pattern(string_literal<CharT, N> pattern, fsm_flags f)
    {
        /* parse pattern string into tree */
        expr_tree ast{ pattern.view() };
        if (f.is_search) ast.insert_search_prefix();
        ast.optimise_tags();

        /* convert to tnfa */
        tagged_nfa nfa{ ast, f };
        nfa.rewrite_assertions();

        /* convert to tdfa */
        tagged_dfa dfa{ nfa };
        dfa.optimise_registers();
        dfa.minimise_states();

        return tdfa_info{ dfa };
    }

    template<string_literal Pattern, fsm_flags Flags>
    struct compiled_dfa
    {
        static constexpr auto value{ compile_pattern(Pattern, Flags) };
    };
}
