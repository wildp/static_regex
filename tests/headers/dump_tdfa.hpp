// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <print>

#include <rx/fsm/tdfa.hpp>


namespace rx::testing
{
    template<typename T, typename CharT>
    requires requires (T t) { std::println(t); }
    void dump_tdfa(T target, const detail::tagged_dfa<CharT>& dfa)
    {
        using namespace detail::tdfa;

        auto print_regop = [](T target, const regop& op, std::string_view indent = "") {
            if (auto* set{ std::get_if<regop::set>(&op.op) }; set != nullptr)
                std::println(target, "{}r{} <- {}", indent, op.dst, (set->val) ? 'p' : 'n');
            else if (auto* copy{ std::get_if<regop::copy>(&op.op) }; copy != nullptr)
                std::println(target, "{}r{} <- r{}", indent, op.dst, copy->src);
            else
                std::unreachable();
        };

        for (std::size_t i{ 0 }; i < dfa.node_count(); ++i)
        {
            if (dfa.final_nodes().contains(i))
                std::println(target, "Node {}: (ACCEPTING)", i);
            else
                std::println(target, "Node {}:", i);

            for (const auto& tr : dfa.get_node(i).tr)
            {
                std::print("\t{} -> Node {}:", tr.cs.get_intervals(), tr.next);

                if (tr.op_index != no_transition_regops)
                    std::println(target, " [Block {}]", tr.op_index);
                else
                    std::println(target);

                for (const auto& op : dfa.get_regops(tr.op_index))
                    print_regop(target, op, "\t\t");
            }

            if (dfa.final_nodes().contains(i))
            {
                const auto& fni{ dfa.final_nodes().at(i) };

                std::print(target, "\t'' -> ACCEPT:");
                if (fni.final_offset != 0)
                    std::print(target, " [Offset {}]", fni.final_offset);
                if (fni.op_index != no_transition_regops)
                    std::print(target, " [Block {}]", fni.op_index);
                std::println(target);

                for (const auto& op : dfa.get_regops(fni.op_index))
                    print_regop(target, op, "\t\t");
            }

            if (dfa.fallback_nodes().contains(i))
            {
                const auto& fni{ dfa.final_nodes().at(i) };
                const auto& fbni{ dfa.fallback_nodes().at(i) };

                std::print(target, "\tFALLBACK -> ACCEPT:");
                if (fni.final_offset != 0)
                    std::print(target, " [Offset {}]", fni.final_offset);
                if (fbni.continue_at != no_continue)
                    std::print(target, " [Continue {}]", fbni.continue_at);
                if (fbni.op_index != no_transition_regops)
                    std::print(target, " [Block {}]", fbni.op_index);
                std::println(target);

                for (const auto& op : dfa.get_regops(fbni.op_index))
                    print_regop(target, op, "\t\t");
            }
        }

        std::println(target, "Continue states: {}", dfa.continue_nodes());
        std::println(target, "Final Registers: {}", dfa.final_registers());
    }

    template<typename CharT>
    void dump_tdfa(const detail::tagged_dfa<CharT>& dfa)
    {
        return dump_tdfa(stdout, dfa);
    }
}
