#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <limits>
#include <meta>
#include <type_traits>

#include <rx/api/submatch.hpp>
#include <rx/etc/string_literal.hpp>
#include <rx/gen/compile.hpp>
#include <rx/gen/result.hpp>

namespace rx::detail
{
    /* p1306 matcher implementation */

    template<string_literal Pattern, fsm_flags Flags>
    struct p1306_matcher
    {
        static constexpr auto DFA{ compile_pattern(Pattern, Flags) };

        using char_type = decltype(DFA)::char_type;

        template<typename I>
        using result = compile_time_match_result<I, DFA.captures, DFA.final_registers, DFA.register_count>;

    private:
        static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };

        template<std::size_t Blk, typename I>
        static constexpr void register_operations(const I it, result<I>& res)
        {
            if constexpr (Blk != tdfa::no_transition_regops)
            {
                template for (constexpr register_operation op : DFA.regops.at(Blk))
                {
                    if constexpr (op.is_copy)
                        res.reg_[op.dst] = res.reg_[op.cpy_src];
                    else if constexpr (op.set_val)
                        res.reg_[op.dst] = it;
                    else if constexpr (std::is_pointer_v<I>)
                        res.reg_[op.dst] = nullptr;
                }

                if constexpr (not result<I>::iterator_is_pointer)
                {
                    template for (constexpr register_operation op : DFA.regops.at(Blk))
                    {
                        if constexpr (op.is_copy)
                            res.enabled_[op.dst] = res.enabled_[op.cpy_src];
                        else if constexpr (op.set_val)
                            res.enabled_[op.dst] = true;
                        else
                            res.enabled_[op.dst] = false;
                    }
                }
            }
        }

        template<typename I>
        static constexpr void fallback(result<I>& res, std::size_t fallback_state, I fallback_it)
        {
            if (fallback_state == fallback_disabled)
                return;

            template for (constexpr std::size_t i : std::define_static_array(make_iota(DFA.fallback_nodes.size())))
            {
                if (fallback_state == DFA.fallback_nodes[i])
                {
                    register_operations<DFA.fallback_node_regops[i]>(fallback_it, res);
                    res.match_end_ = fallback_it;
                    return;
                }
            }
        }

        template<std::size_t DFAState, typename I>
        static constexpr void state(I it, result<I>& res, const I last, std::size_t fallback_state, I fallback_it)
        { 
            if constexpr (Flags.enable_fallback and std::ranges::binary_search(DFA.fallback_nodes, DFAState))
            {
                fallback_state = DFAState;
                fallback_it = it;
            }

            if (it == last)
            {
                if constexpr (constexpr auto key{ std::ranges::lower_bound(DFA.final_nodes, DFAState) };
                              key != DFA.final_nodes.end() and *key == DFAState)
                {
                    register_operations<DFA.final_node_regops[key - DFA.final_nodes.begin()]>(it, res);
                    res.match_end_ = it;
                    return;
                }
            }
            else
            {
                template for (constexpr tdfa::transition<char_type> tr : DFA.nodes.at(DFAState))
                {
                    if constexpr (tr.upper == tr.lower)
                    {
                        if (tr.lower == *it)
                        {
                            register_operations<tr.op_index>(it, res);
                            [[clang::musttail]] return state<tr.next>(it + 1, res, last, fallback_state, fallback_it);
                        }   
                    }
                    else
                    {
                        if (tr.lower <= *it and *it <= tr.upper)
                        {
                            register_operations<tr.op_index>(it, res);
                            [[clang::musttail]] return state<tr.next>(it + 1, res, last, fallback_state, fallback_it);
                        }
                    }
                }
            }

            if constexpr (Flags.enable_fallback)
                return fallback(res, fallback_state, fallback_it);
        }

        template<typename I>
        static constexpr bool next_state(I it, result<I>& res, std::size_t state, const I last, std::size_t fallback_state, I fallback_it)
        {
            template for (constexpr std::size_t dfa_state : std::define_static_array(make_iota(DFA.nodes.size())))
            {
                if (state == dfa_state)
                {
                    if constexpr (Flags.enable_fallback and std::ranges::binary_search(DFA.fallback_nodes, dfa_state))
                    {
                        fallback_state = dfa_state;
                        fallback_it = it;
                    }

                    if (it == last)
                    {
                        if constexpr (constexpr auto key{ std::ranges::lower_bound(DFA.final_nodes, dfa_state) };
                                      key != DFA.final_nodes.end() and *key == dfa_state)
                        {
                            register_operations<DFA.final_node_regops[key - DFA.final_nodes.begin()]>(it, res);
                            res.match_end_ = it;
                            return false;
                        }
                    }
                    else
                    {
                        template for (constexpr tdfa::transition<char_type> tr : DFA.nodes.at(dfa_state))
                        {
                            if constexpr (tr.upper == tr.lower)
                            {
                                if (tr.lower == *it)
                                {
                                    register_operations<tr.op_index>(it, res);
                                    ++it;
                                    state = tr.next;
                                    return true;
                                }
                            }
                            else
                            {
                                if (tr.lower <= *it and *it <= tr.upper)
                                {
                                    register_operations<tr.op_index>(it, res);
                                    ++it;
                                    state = tr.next;
                                    return true;
                                }
                            }
                        }
                    }

                    if constexpr (Flags.enable_fallback)
                        fallback(res, fallback_state, fallback_it);

                    return false;
                }
            }
            return false;
        }

    public:
        template<std::bidirectional_iterator I>
        requires (std::convertible_to<std::iter_value_t<I>, char_type>)
        [[nodiscard]] static constexpr auto operator()(const I first, const I last)
        {
            result<I> res{ first };

            // if not consteval
            {
                state<DFA.match_start>(first, res, last, fallback_disabled, first);
            }
            // else
            // {
            //     /* TODO: use this implementation if necessary */
            //     I it{ first };
            //     std::size_t s{ DFA.match_start };
            //     while (next_state(it, res, s, last, fallback_disabled, first));
            // }

            return res;
        }
    };
}