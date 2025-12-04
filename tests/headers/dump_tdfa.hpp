#pragma once

#include <print>

#include <rx/fsm/tdfa.hpp>


namespace rx::testing
{
    template<typename T, typename CharT>
    requires requires (T t) { std::println(t); } 
    void dump_tdfa(T target, const rx::detail::tagged_dfa<CharT>& dfa)
    {
        using namespace rx::detail::tdfa;

        auto print_regop = [](T target, const regop& op, std::string_view indent = "")
        {
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
                if (fni.final_offset == 0)
                    std::print(target, "\t'' -> ACCEPT:");
                else
                    std::print(target, "\t'' -> ACCEPT: [Offset {}]", fni.final_offset);

                if (fni.op_index != no_transition_regops)
                    std::println(target, " [Block {}]", fni.op_index);
                else
                    std::println(target);

                for (const auto& op : dfa.get_regops(fni.op_index))
                    print_regop(target, op, "\t\t");
            }

            if (dfa.fallback_nodes().contains(i))
            {
                const auto& fin{ dfa.final_nodes().at(i) };
                if (fin.final_offset == 0)
                    std::print(target, "\tFALLBACK -> ACCEPT:");
                else
                    std::print(target, "\tFALLBACK -> ACCEPT: [Offset {}]", fin.final_offset);

                if (dfa.fallback_nodes().at(i) != no_transition_regops)
                    std::println(target, " [Block {}]", dfa.fallback_nodes().at(i));
                else
                    std::println(target);


                for (const auto& op : dfa.get_regops(dfa.fallback_nodes().at(i)))
                    print_regop(target, op, "\t\t");
            }
        }

        std::println(target, "Final Registers: {}", dfa.final_registers());
    }

    template<typename CharT>
    void dump_tdfa(const rx::detail::tagged_dfa<CharT>& dfa)
    {
        return dump_tdfa(stdout, dfa);
    }
}