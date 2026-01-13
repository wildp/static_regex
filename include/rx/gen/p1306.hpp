// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <limits>
#include <meta>
#include <type_traits>

#include "../etc/string_literal.hpp"
#include "../fsm/tdfa.hpp"
#include "../gen/compile.hpp"
#include "../gen/result.hpp"


namespace rx::detail
{
    /* p1306 matcher implementation */

    template<typename CharT, CharT Lower, CharT Upper>
    [[clang::always_inline]] constexpr bool tr_possible_interval_impl(CharT c)
    {
        if constexpr (Lower == Upper)
            return (c == Lower);
        else
            return (Lower <= c and c <= Upper);
    }

    template<typename CharT, std::pair<CharT, CharT>... Intervals>
    [[clang::always_inline]] constexpr bool tr_possible_impl(CharT c)
    {
        return (tr_possible_interval_impl<CharT, Intervals.first, Intervals.second>(c) or ...);
    }

    template<typename CharT>
    consteval auto tr_possible_make_refl(static_transition<CharT> tr)
    {
        std::vector<std::meta::info> result{ ^^CharT };
        result.reserve(1 + tr.cs.size());
        for (const auto& pair : tr.cs)
            result.emplace_back(std::meta::reflect_constant(pair));
        return result;
    }


    template<static_transition Tr, typename CharT>
    [[clang::always_inline]] constexpr bool tr_possible(CharT c)
    {
        constexpr auto func{ std::meta::substitute(^^tr_possible_impl, tr_possible_make_refl(Tr)) };
        return [: func :](c);
    }


    template<string_literal Pattern, fsm_flags Flags>
    struct p1306_matcher
    {
        using dfa_t = compiled_dfa<Pattern, Flags>;
        using char_type = decltype(Pattern)::char_type;

        template<typename I>
        using result_type = static_regex_match_result<I, Pattern, Flags>;

        template<typename I>
        struct result : result_type<I>
        {
            constexpr result() = delete;
            friend struct p1306_matcher;
        };

    private:
        static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };

        template<std::size_t Blk, typename I>
        static constexpr void register_operations(const I it, result<I>& res)
        {
            if constexpr (Blk != tdfa::no_transition_regops)
            {
                template for (constexpr register_operation op : dfa_t::value.regops.at(Blk))
                {
                    if constexpr (op.is_copy)
                        res.reg()[op.dst] = res.reg()[op.cpy_src];
                    else if constexpr (op.set_val)
                        res.reg()[op.dst] = it;
                    else
                        res.reg()[op.dst] = I{};
                }
            }
        }

        template<typename I>
        static constexpr void fallback(result<I>& res, std::size_t fallback_state, I fallback_it)
        {
            if (fallback_state == fallback_disabled)
                return;

            template for (constexpr std::size_t i : std::views::iota(0uz, dfa_t::value.fallback_nodes.size()))
            {
                static constexpr auto state{ dfa_t::value.fallback_nodes[i] };
                if (fallback_state == state)
                {
                    static constexpr auto key{ std::ranges::lower_bound(dfa_t::value.final_nodes, state) };
                    static_assert(key != dfa_t::value.final_nodes.end() and *key == state);
                    static constexpr auto fni{ dfa_t::value.final_node_regops.at(key - dfa_t::value.final_nodes.begin()) };
                    static constexpr auto fbni{ dfa_t::value.fallback_node_regops.at(i) };

                    register_operations<fbni.op_index>(fallback_it, res);

                    if constexpr (fni.final_offset != 0)
                        res.match_end() = fallback_it - fni.final_offset;
                    else
                        res.match_end() = fallback_it;

                    if constexpr (Flags.is_iterator and fbni.continue_at != tdfa::no_continue)
                        res.continue_at() = fbni.continue_at;

                    return;
                }
            }
        }

        template<std::size_t DFAState, typename I>
        static constexpr void state(I it, result<I>& res, const I last, std::size_t fallback_state, I fallback_it)
        {
            if constexpr (Flags.enable_fallback and std::ranges::binary_search(dfa_t::value.fallback_nodes, DFAState))
            {
                fallback_state = DFAState;
                fallback_it = it;
            }

            if (it == last)
            {
                if constexpr (constexpr auto key{ std::ranges::lower_bound(dfa_t::value.final_nodes, DFAState) };
                              key != dfa_t::value.final_nodes.end() and *key == DFAState)
                {
                    static constexpr auto fni{ dfa_t::value.final_node_regops.at(key - dfa_t::value.final_nodes.begin()) };

                    register_operations<fni.op_index>(it, res);

                    if constexpr (fni.final_offset != 0)
                        res.match_end() = it - fni.final_offset;
                    else
                        res.match_end() = it;

                    return;
                }
            }
            else
            {
                template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
                {
                    if (tr_possible<tr>(*it))
                    {
                        register_operations<tr.op_index>(it, res);
                        [[clang::musttail]] return state<tr.next>(++it, res, last, fallback_state, fallback_it);
                    }
                }
            }

            if constexpr (Flags.enable_fallback)
                return fallback(res, fallback_state, fallback_it);
        }

        template<std::size_t DFAState, typename CharT>
        static constexpr void state(const CharT* ptr, result<const CharT*>& res, std::size_t fallback_state, const CharT* fallback_ptr)
        {
            if constexpr (Flags.enable_fallback and std::ranges::binary_search(dfa_t::value.fallback_nodes, DFAState))
            {
                fallback_state = DFAState;
                fallback_ptr = ptr;
            }

            if (*ptr == '\0')
            {
                if constexpr (constexpr auto key{ std::ranges::lower_bound(dfa_t::value.final_nodes, DFAState) };
                              key != dfa_t::value.final_nodes.end() and *key == DFAState)
                {
                    static constexpr auto fni{ dfa_t::value.final_node_regops.at(key - dfa_t::value.final_nodes.begin()) };

                    register_operations<fni.op_index>(ptr, res);

                    if constexpr (fni.final_offset != 0)
                        res.match_end() = ptr - fni.final_offset;
                    else
                        res.match_end() = ptr;

                    return;
                }
            }
            else
            {
                template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
                {
                    if (tr_possible<tr>(*ptr))
                    {
                        register_operations<tr.op_index>(ptr, res);
                        [[clang::musttail]] return state<tr.next>(ptr + 1, res, fallback_state, fallback_ptr);
                    }
                }
            }

            if constexpr (Flags.enable_fallback)
                return fallback(res, fallback_state, fallback_ptr);
        }

    public:
        template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const I last)
        {
            result_type<I> res{ first };
            state<dfa_t::value.match_start>(first, static_cast<result<I>&>(res), last, fallback_disabled, first);
            return res;
        }

        template<typename CharT>
        requires std::is_nothrow_convertible_v<CharT, char_type>
        [[nodiscard]] static constexpr auto operator()(const CharT* cstr)
        {
            result_type<const CharT*> res{ cstr };
            state<dfa_t::value.match_start>(cstr, static_cast<result<const CharT*>&>(res), fallback_disabled, cstr);
            return res;
        }

        template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const I last, const tdfa::continue_at_t continue_at) requires (Flags.is_iterator)
        {
            result_type<I> res{ first };

            template for (constexpr std::size_t i : std::views::iota(0uz, dfa_t::value.continue_nodes.size()))
            {
                if (i == continue_at)
                    state<dfa_t::value.continue_nodes[i]>(first, static_cast<result<I>&>(res), last, fallback_disabled, first);
            }

            return res;
        }

        template<typename CharT>
        requires std::is_nothrow_convertible_v<CharT, char_type>
        [[nodiscard]] static constexpr auto operator()(const CharT* cstr, const tdfa::continue_at_t continue_at) requires (Flags.is_iterator)
        {
            result_type<const CharT*> res{ cstr };

            template for (constexpr std::size_t i : std::views::iota(0uz, dfa_t::value.continue_nodes.size()))
            {
                if (i == continue_at)
                    state<dfa_t::value.continue_nodes[i]>(cstr, static_cast<result<const CharT*>&>(res), fallback_disabled, cstr);
            }

            return res;
        }
    };
}
