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
                if (tr.lower == tr.upper)
                    std::print("\t{:?} -> Node {}:", tr.lower, tr.next);
                else
                    std::print("\t[{:?},{:?}] -> Node {}:", tr.lower, tr.upper, tr.next);

                if (tr.op_index != no_transition_regops)
                    std::println(" [Block {}]", tr.op_index);
                else
                    std::println();

                for (const auto& op : this->get_regops(tr.op_index))
                    print_regop(op, "\t\t");
            }

            if (this->final_nodes().contains(i))
            {
                std::print("\t'' -> ACCEPT:");

                if (this->final_nodes().at(i) != no_transition_regops)
                    std::println(" [Block {}]", this->final_nodes().at(i));
                else
                    std::println();

                for (const auto& op : this->get_regops(this->final_nodes().at(i)))
                    print_regop(op, "\t\t");
            }

            if (this->fallback_nodes().contains(i))
            {
                std::print("\tFALLBACK -> ACCEPT:");

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