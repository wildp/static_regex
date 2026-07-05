// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <print>

#include <srx/fsm/tdfa.hpp>

#include "graph_export.hpp"


namespace srx {
namespace tools {

template<typename T, typename CharT>
    requires requires (T t) { std::println(t); }
void dump_fsm(T target, const detail::tagged_dfa<CharT>& dfa)
{
    using namespace detail::tdfa;

    auto print_regop = [&](const regop& op, std::string_view indent = ""){
        if (auto* set = get_if<regop::set>(&op.op); set != nullptr)
            std::println(target, "{}r{} <- {}", indent, op.dst, (set->val) ? 'p' : 'n');
        else if (auto* copy = get_if<regop::copy>(&op.op); copy != nullptr)
            std::println(target, "{}r{} <- r{}", indent, op.dst, copy->src);
        else
            std::unreachable();
    };

    auto print_offset_cont = [&]( std::uint_least16_t offset, std::optional<continue_at_t> continue_at = {}){
        if (offset != 0)
            std::print(target, " offset={}", -offset);
        if (continue_at.has_value() and *continue_at != no_continue)
            std::print(target, " cont={}", *continue_at);
    };

    auto print_regop_blk = [&](std::size_t op_index, std::string_view indent = ""){
        if (op_index != no_transition_regops)
            std::print(target, " regops=blk{}:", op_index);
        std::println(target);

        for (const auto& op : dfa.get_regops(op_index))
            print_regop(op, indent);
    };

    for (std::size_t i{ 0 }, i_end{ dfa.node_count() }; i < i_end; ++i)
    {
        if (dfa.final_nodes().contains(i))
            std::println(target, "State q{}: (a)", i);
        else
            std::println(target, "State q{}:", i);

        const auto& node = dfa.get_node(i);

        for (const auto& tr : node.tr)
        {
            if constexpr (std::same_as<CharT, char>)
                std::print("\t{} -> q{}:", make_pretty_charset_string(tr.cs), tr.next);
            else
                std::print("\t{} -> q{}:", tr.cs.get_intervals(), tr.next);

            print_regop_blk(tr.op_index, "\t\t");
        }

        if (node.default_tr.has_value())
        {
            if (const auto op_index = node.default_tr->op_index; op_index == detail::tdfa::default_transition_is_not_state)
            {
                std::println(target, "\tFALLTHROUGH to q{}", node.default_tr->next);
            }
            else
            {
                std::print(target, "\t. -> q{}:", i, node.default_tr->next);
                print_regop_blk(node.default_tr->op_index, "\t\t");
            }
        }

        if (dfa.final_nodes().contains(i))
        {
            const auto& fni = dfa.final_nodes().at(i);

            std::print(target, "\tACCEPT:");
            print_offset_cont(fni.offset);
            print_regop_blk(fni.op_index, "\t\t");
        }

        if (dfa.fallback_nodes().contains(i))
        {
            const auto& fni = dfa.final_nodes().at(i);
            const auto& fbni = dfa.fallback_nodes().at(i);

            std::print(target, "\tFALLBACK:");
            print_offset_cont(fni.offset, fbni.continue_at);
            print_regop_blk(fbni.op_index, "\t\t");
        }
    }

    std::println(target, "Final Registers: {}", dfa.final_registers());
    std::println(target, "Continue States: {}", dfa.continue_nodes());
}

template<typename CharT>
void dump_fsm(const detail::tagged_dfa<CharT>& dfa)
{
    return dump_fsm(stdout, dfa);
}

} // namespace tools
} // namespace srx
