// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <concepts>
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
    [[gnu::always_inline]] constexpr bool tr_possible_interval_impl(CharT c)
    {
        if constexpr (Lower == Upper)
            return (c == Lower);
        else
            return (Lower <= c and c <= Upper);
    }

    template<typename CharT, std::pair<CharT, CharT>... Intervals>
    [[gnu::always_inline]] constexpr bool tr_possible_impl(CharT c)
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
    [[gnu::always_inline]] constexpr bool tr_possible(CharT c)
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
        struct result
        {
            template<typename... Args>
            constexpr explicit result(Args... args) noexcept(noexcept(result_type{ args... }))
                : value(std::forward<Args>(args)...) {}
            
            [[no_unique_address]] result_type<I> value;
        };

    private:
        static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };

        template<std::size_t Blk, std::bidirectional_iterator I>
        static constexpr void register_operations(I it, result<I>& res)
        {
            if constexpr (Blk != tdfa::no_transition_regops)
            {
                template for (constexpr register_operation op : dfa_t::value.regops.at(Blk))
                {
                    if constexpr (op.is_copy)
                        res.value.reg_[op.dst] = res.value.reg_[op.cpy_src];
                    else if constexpr (op.set_val)
                        res.value.reg_[op.dst] = it;
                    else if constexpr (std::contiguous_iterator<I>)
                        res.value.reg_[op.dst] = I{};

                    if constexpr (not std::contiguous_iterator<I>)
                    {
                        res.value.enabled_[op.dst] = (op.is_copy or op.set_val);
                    }
                }
            }
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void fallback(I /* it */, result<I>& res, const S /* last */, std::size_t fallback_state, I fallback_it)
        requires (Flags.enable_fallback)
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
                    res.value.match_end_ = std::ranges::prev(fallback_it, fni.final_offset);

                    if constexpr (not std::contiguous_iterator<I>)
                        res.value.match_success_ = true;

                    if constexpr (Flags.is_iterator)
                    {
                        if constexpr (fbni.continue_at != tdfa::no_continue)
                            res.value.continue_at_ = fbni.continue_at;
                    }

                    return;
                }
            }
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void state(I it, result<I>& res, const S last, std::size_t fallback_state, I fallback_it)
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
                    res.value.match_end_ = std::ranges::prev(it, fni.final_offset);

                    if constexpr (not std::contiguous_iterator<I>)
                        res.value.match_success_ = true;

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
                [[clang::musttail]] return fallback(it, res, last, fallback_state, fallback_it);
        }

    public:
        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last)
        {
            result<I> res{ first };
            state<dfa_t::value.match_start>(first, res, last, fallback_disabled, first);
            return res.value;
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at) requires (Flags.is_iterator)
        {
            result<I> res{ first };

            template for (constexpr std::size_t i : std::views::iota(0uz, dfa_t::value.continue_nodes.size()))
            {
                if (i == continue_at)
                    state<dfa_t::value.continue_nodes[i]>(first, res, last, fallback_disabled, first);
            }

            return res.value;
        }
    };

    
    template<string_literal Pattern, fsm_flags Flags>
    requires (Flags.return_bool)
    struct p1306_matcher<Pattern, Flags>
    {
        using dfa_t = compiled_dfa<Pattern, Flags>;
        using char_type = decltype(Pattern)::char_type;

    private:
        static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool fallback(I /* it */, const S /* last */, std::size_t fallback_state)
        requires (Flags.enable_fallback)
        {
            if (fallback_state == fallback_disabled)
                return false;

            template for (constexpr std::size_t i : std::views::iota(0uz, dfa_t::value.fallback_nodes.size()))
            {
                if (fallback_state == dfa_t::value.fallback_nodes[i])
                    return true;
            }

            return false;
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool state(I it, const S last, std::size_t fallback_state)
        {
            if constexpr (Flags.enable_fallback and std::ranges::binary_search(dfa_t::value.fallback_nodes, DFAState))
                fallback_state = DFAState;
   
            if (it == last)
            {
                if constexpr (constexpr auto key{ std::ranges::lower_bound(dfa_t::value.final_nodes, DFAState) };
                              key != dfa_t::value.final_nodes.end() and *key == DFAState)
                    return true;
            }
            else
            {
                template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
                {
                    if (tr_possible<tr>(*it))
                        [[clang::musttail]] return state<tr.next>(++it, last, fallback_state);
                }
            }

            if constexpr (Flags.enable_fallback)
                [[clang::musttail]] return fallback(it, last, fallback_state);

            return false;
        }

    public:
        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr bool operator()(const I first, const S last)
        {
            return state<dfa_t::value.match_start>(first, last, fallback_disabled);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        static constexpr bool operator()(const I first, const S last, const tdfa::continue_at_t continue_at) = delete;
    };
}
