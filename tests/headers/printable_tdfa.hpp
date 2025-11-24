#pragma once

#include <format>
#include <print>

#include <rx/fsm/tdfa.hpp>


namespace rx::testing
{
    template<typename>
    class printable;

    template<rx::detail::derived_from_class_template<^^rx::detail::tagged_dfa> Base>
    class printable<Base> : public Base
    {
    public:
        using base_type = Base;
        using this_type = printable<base_type>;
        using char_type = this_type::char_type;
        using string_type = std::basic_string<char_type>;

        using Base::Base;

        void print() const;
    };


    template<rx::detail::derived_from_class_template<^^rx::detail::tagged_dfa> Base>
    void printable<Base>::print() const    
    {
        using namespace rx::detail::tdfa;

        auto print_regop = [](const regop& op, std::string_view indent = "") {
            if (auto* set{ std::get_if<regop::set>(&op.op) }; set != nullptr)
                std::println("{}r{} <- {}", indent, op.dst, (set->val) ? 'p' : 'n');
            else if (auto* copy{ std::get_if<regop::copy>(&op.op) }; copy != nullptr)
                std::println("{}r{} <- r{}", indent, op.dst, copy->src);
            else
                std::unreachable();
        };

        for (std::size_t i{ 0 }; i < this->node_count(); ++i)
        {
            if (this->final_nodes().contains(i))
                std::println("Node {}: (ACCEPTING)", i);
            else
                std::println("Node {}:", i);

            for (const auto& tr : this->get_node(i).tr)
            {
                std::print("\t{} -> Node {}:", tr.cs.get_intervals(), tr.next);

                if (tr.op_index != no_transition_regops)
                    std::println(" [Block {}]", tr.op_index);
                else
                    std::println();

                for (const auto& op : this->get_regops(tr.op_index))
                    print_regop(op, "\t\t");
            }

            if (this->final_nodes().contains(i))
            {
                const auto& fni{ this->final_nodes().at(i) };
                if (fni.final_offset == 0)
                    std::print("\t'' -> ACCEPT:");
                else
                    std::print("\t'' -> ACCEPT: [Offset {}]", fni.final_offset);

                if (fni.op_index != no_transition_regops)
                    std::println(" [Block {}]", fni.op_index);
                else
                    std::println();

                for (const auto& op : this->get_regops(fni.op_index))
                    print_regop(op, "\t\t");
            }

            if (this->fallback_nodes().contains(i))
            {
                const auto& fin{ this->final_nodes().at(i) };
                if (fin.final_offset == 0)
                    std::print("\tFALLBACK -> ACCEPT:");
                else
                    std::print("\tFALLBACK -> ACCEPT: [Offset {}]", fin.final_offset);

                if (this->fallback_nodes().at(i) != no_transition_regops)
                    std::println(" [Block {}]", this->fallback_nodes().at(i));
                else
                    std::println();


                for (const auto& op : this->get_regops(this->fallback_nodes().at(i)))
                    print_regop(op, "\t\t");
            }
        }

        std::println("Final Registers: {}", this->final_registers());
    }
}