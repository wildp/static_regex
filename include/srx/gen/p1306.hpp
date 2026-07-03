// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <bit>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <limits>
#include <meta>
#include <ranges>
#include <type_traits>

#if __cpp_lib_simd >= 202411L or (defined(__GNUC_MINOR__) and __GLIBCXX__ >= 20260424L)
    #include <simd>
#endif

#include "srx/etc/static_charset.hpp"
#include "srx/etc/string_literal.hpp"
#include "srx/etc/util.hpp"
#include "srx/fsm/flags.hpp"
#include "srx/fsm/tdfa.hpp"
#include "srx/gen/compile.hpp"
#include "srx/gen/result.hpp"


namespace srx::detail {

template<typename CharT>
struct optimised_tr_edge
{
    using value_type = std::make_unsigned_t<CharT>;

    value_type sub;
    value_type rng;
    value_type msk;
};

template<unconstrained_charset_like CharsetType>
consteval auto make_optimised_edges(const CharsetType& sc, bool inverted = false)
{
    using char_type = CharsetType::char_type;
    using ote = optimised_tr_edge<char_type>;
    using uchar_type = typename ote::value_type;
    std::vector<ote> result;

    if (inverted)
    {
        for (const auto& [first, last] : (~sc).get_intervals())
            result.emplace_back(static_cast<uchar_type>(first),
                                static_cast<uchar_type>(static_cast<char_type>(last - first)),
                                static_cast<uchar_type>(~0u));
    }
    else
    {
        for (const auto& [first, last] : sc.get_intervals())
            result.emplace_back(static_cast<uchar_type>(first),
                                static_cast<uchar_type>(static_cast<char_type>(last - first)),
                                static_cast<uchar_type>(~0u));
    }


    std::vector<ote> input;

    while (result.size() != input.size())
    {
        std::swap(result, input);
        result.clear();

        std::vector<bool> visited(input.size(), false);

        for (std::size_t i{ 0 }, i_end{ input.size() }; i < i_end; ++i)
        {
            if (visited.at(i))
                continue;

            visited.at(i) = true;
            const auto& [sub1, rng1, msk1] = input.at(i);

            bool inserted{ false };

            for (std::size_t j{ i + 1 }; j < i_end; ++j)
            {
                if (visited.at(j))
                    continue;

                const auto& [sub2, rng2, msk2] = input.at(j);

                if (rng1 != rng2 or msk1 != msk2)
                    continue;

                const uchar_type masked_bit{ static_cast<uchar_type>(sub1 ^ sub2) };

                if (not std::has_single_bit(masked_bit) or (masked_bit | msk1) != msk1)
                    continue;

                visited.at(j) = true;
                inserted = true;
                result.emplace_back(std::ranges::min(sub1, sub2), rng1, static_cast<uchar_type>(msk1 - masked_bit));
                break;
            }

            if (not inserted)
                result.emplace_back(sub1, rng1, msk1);
        }
    }

    return result;
}

template<typename CharT>
consteval std::size_t score_optimised_edges(const std::vector<optimised_tr_edge<CharT>>& otes, bool inverted = false)
{
    using ote_t = optimised_tr_edge<CharT>;
    using value_type = ote_t::value_type;

    static constexpr std::size_t bit_count{ std::numeric_limits<value_type>::digits };
    static constexpr std::size_t range_penalty{ 2 };
    static constexpr std::size_t scale{ 4 }; /* arbitrarily selected constant */

    if (otes.empty())
        return -1uz;

    /* note: this is just a heuristic; there may be better ones */

    std::size_t count{ 0 };
    std::size_t ops{ 0 };

    for (const auto& [sub, rng, msk] : otes)
    {
        std::size_t mask_bits{ bit_count - std::popcount(msk) };
        std::size_t number{ (1 + rng) * mask_bits };
        count += number;
        ops += 2 + ((rng != 0) * range_penalty) + (mask_bits != 0);
    }

    if (inverted)
        count = (~0uz >> (std::numeric_limits<std::size_t>::digits - bit_count)) - count;

    return (count * scale) + ops;
}

template<typename CharT>
consteval auto get_flattened_states(const tdfa_info<CharT>& dfa, std::size_t state, bool trailing = false)
{
    std::vector<std::size_t> result;

    while (true)
    {
        if (dfa.final_nodes.contains(state))
            break;

        /* assume fallback_nodes is subset of final_nodes */
        // if (dfa.fallback_nodes.contains(state))
        //     break;

        if (dfa.nodes.at(state).size() != 1)
        {
            if (trailing)
                result.emplace_back(state);
            break;
        }

        const auto& tr = dfa.nodes.at(state).front();
        result.emplace_back(state);
        state = tr.next;
    }

    return result;
}

template<typename CharT>
consteval auto get_edge_scores(const tdfa_info<CharT>& dfa, const std::span<const std::size_t> qs)
{
    if (qs.empty())
        throw std::range_error("get_edge_scores: qs is empty");

    const auto edges = qs
                       | std::views::transform([&](std::size_t q){
                             if (dfa.nodes[q].size() == 1)
                             {
                                 const auto& cs = dfa.nodes[q].front().cs;
                                 const bool should_invert{ cs.should_invert() };
                                 return std::pair{ make_optimised_edges(cs, should_invert), should_invert };
                             }
                             else
                             {
                                 charset<CharT> cs;
                                 for (const auto tr : dfa.nodes[q])
                                     cs |= tr.cs;
                                 const bool should_invert{ cs.should_invert() };
                                 return std::pair{ make_optimised_edges(cs, should_invert), should_invert };
                             }
                         })
                       | std::ranges::to<std::vector>();

    return edges
           | std::views::transform([](const auto& e){ return score_optimised_edges(e.first, e.second); })
           | std::ranges::to<std::vector>();
}

template<typename CharT>
consteval auto get_outer_state_position_pair(const tdfa_info<CharT>& dfa, const std::span<const std::size_t> qs)
{
    auto scores = get_edge_scores(dfa, qs);

    const auto left_elem = std::ranges::min_element(scores);
    *left_elem = -1uz;
    const std::size_t left{ static_cast<std::size_t>(std::ranges::distance(scores.begin(), left_elem)) };

    const auto right_elem = std::ranges::min_element(scores.rbegin(), scores.rend());
    const std::size_t right{ static_cast<std::size_t>(std::ranges::distance(right_elem, scores.rend()) - 1) };

    if (left > right)
        return std::pair{ right, left };
    else
        return std::pair{ left, right };
}

template<typename CharT>
consteval auto get_outer_state_position_single(const tdfa_info<CharT>& dfa, const std::span<const std::size_t> qs)
{
    const auto scores = get_edge_scores(dfa, qs);
    const auto left_elem = std::ranges::min_element(scores);
    return static_cast<std::size_t>(std::ranges::distance(scores.begin(), left_elem));
}

template<static_charset Sc>
[[gnu::always_inline]] constexpr bool tr_possible(const typename decltype(Sc)::char_type c)
{
#ifndef __GNUC_MINOR__
    // TODO: change to use structured binding when supported
    template for (constexpr auto pair : Sc.get_intervals())
    {
        if (pair.first <= c and c <= pair.second)
            return true;
    }
    return false;
#else
    using char_type = decltype(Sc)::char_type;
    using uchar_type = std::make_unsigned_t<char_type>;

    if constexpr (std::signed_integral<char_type>)
    {
        static constexpr static_charset usc{ Sc.make_unsigned() };
        return tr_possible<usc>(static_cast<uchar_type>(c));
    }
    else
    {
        static constexpr bool inverted{ false /* Sc.should_invert() */ };

        template for (constexpr auto ote : std::define_static_array(make_optimised_edges(Sc, inverted)))
        {
            uchar_type d{ c };

            if constexpr (ote.msk != ~0)
                d &= ote.msk;

            if constexpr (ote.rng == 0)
            {
                if (d == ote.sub)
                    return not inverted;
            }
            else
            {
                d -= ote.sub;
                if (d <= ote.rng)
                    return not inverted;
            }
        }
        return inverted;
    }
#endif
}

template<static_charset Sc>
[[gnu::always_inline]] constexpr bool tr_possible_exclude_null(const typename decltype(Sc)::char_type c)
{
    using char_type = decltype(Sc)::char_type;

    if constexpr (tr_possible<Sc>(char_type{}))
    {
        if (c == char_type{})
            return false;
    }

    return tr_possible<Sc>(c);
}

#if __cpp_lib_simd >= 202411L or (defined(__GNUC_MINOR__) and __GLIBCXX__ >= 20260424L)
template<static_charset Sc, std::unsigned_integral UCharT, typename Abi>
[[gnu::always_inline]] constexpr auto vector_tr_find(const std::simd::basic_vec<UCharT, Abi>& c_vec)
{
    using char_type = decltype(Sc)::char_type;

    if constexpr (std::signed_integral<char_type>)
    {
        static_assert(std::same_as<UCharT, std::make_unsigned_t<char_type>>);
        static constexpr static_charset usc{ Sc.make_unsigned() };
        return vector_tr_find<usc>(c_vec);
    }
    else
    {
        static_assert(std::same_as<UCharT, char_type>);
        using vec_type = std::simd::basic_vec<UCharT, Abi>;
        using mask_type = vec_type::mask_type;

        static constexpr bool inverted{ Sc.should_invert() };
        mask_type result{ inverted };

        template for (constexpr auto ote : std::define_static_array(make_optimised_edges(Sc, inverted)))
        {
            auto d_vec{ c_vec };

            if constexpr (ote.msk != ~0)
            {
                static constexpr vec_type mask_vec{ ote.msk };
                d_vec &= mask_vec;
            }

            if constexpr (ote.rng == 0)
            {
                static constexpr vec_type cmp_vec{ ote.sub };

                if constexpr (inverted)
                    result &= (d_vec != cmp_vec);
                else
                    result |= (d_vec == cmp_vec);
            }
            else
            {
                static constexpr vec_type sub_vec{ ote.sub };
                static constexpr vec_type lte_vec{ ote.rng };
                d_vec -= sub_vec;

                if constexpr (inverted)
                    result &= (d_vec > lte_vec);
                else
                    result |= (d_vec <= lte_vec);
            }
        }

        return result;
    }
}
#endif // __cpp_lib_simd >= 202411L


template<std::meta::info DFARefl>
struct p1306dfa
{
    static constexpr tdfa_info DFA{ [:DFARefl
        :] }; // line break avoid breaking syntax highlighting
    using char_type = decltype(DFA)::char_type;

    static constexpr bool never_empty{ DFA.additional_continue_nodes.empty() };
    static constexpr bool fixed_length{ DFA.min_max_lengths.first != std::numeric_limits<std::size_t>::max()
                                        and DFA.min_max_lengths.first == DFA.min_max_lengths.second };
    static constexpr bool branch_free{ std::ranges::all_of(DFA.nodes, [](const auto& n){ return n.size() <= 1; }) };

    template<typename I>
    using result = static_match_results<I, DFA.make_match_result_info(DFA.flags.is_iterator)>;

private:
    static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };
    static constexpr std::size_t avoid_simd_threshold{ (1uz << std::numeric_limits<std::make_unsigned_t<char_type>>::digits) / 2 };

    struct generation
    {
        std::array<std::size_t, DFA.register_count> reg{};
        std::size_t current{ 0 };
    };

    using gen_info = maybe_type_t<(DFA.register_count > 0), generation>;
    using gen_ref = maybe_type_t<(DFA.register_count > 0), std::add_lvalue_reference_t<generation>>;

    template<typename I>
    using result_ref = std::add_lvalue_reference_t<result<I>>;

    using no_result = terminal_object;

    template<typename I, bool HasGen>
    struct result_t
    {
        result_ref<I> res;
        [[no_unique_address]] gen_ref gen;
    };

    template<typename I>
    struct result_t<I, false>
    {
        result_ref<I> res;
    };

    template<typename I>
    result_t(result<I>&) -> result_t<I, false>;

    template<typename I>
    result_t(result<I>&, gen_info&) -> result_t<I, true>;

    static constexpr bool has_fallback_it{ not DFA.flags.return_bool };

    template<typename I>
    struct fallback_info
    {
        [[no_unique_address]] maybe_type_t<(not DFA.flags.return_bool), I> it;
        std::size_t state;
    };

    template<typename I>
    using maybe_fallback_t = maybe_type_t<DFA.flags.enable_fallback, fallback_info<I>>;

    template<std::bidirectional_iterator I>
    static constexpr void clean_generations(result<I>& res, const gen_info& gen)
    {
        if constexpr (DFA.register_count > 0)
        {
            if constexpr (std::contiguous_iterator<I>)
            {
                for (std::size_t i{ 0 }; i < DFA.register_count; ++i)
                    res.reg_[i] = (gen.reg[i] == gen.current) ? res.reg_[i] : I{};
            }
            else
            {
                for (std::size_t i{ 0 }; i < DFA.register_count; ++i)
                    res.enabled_[i] = (gen.reg[i] == gen.current);
            }
        }
    }

    template<std::size_t Blk, std::bidirectional_iterator I>
    static constexpr void register_operations(result_t<I, true> result, const I it)
    {
        if constexpr (Blk != tdfa::no_transition_regops)
        {
            static_assert(DFA.register_count > 0);

            template for (constexpr register_operation op : DFA.regops[Blk])
            {
                if constexpr (op.is_copy)
                    result.res.reg_[op.dst] = result.res.reg_[op.cpy_src];
                else if constexpr (op.set_val)
                    result.res.reg_[op.dst] = it;
            }

            template for (constexpr register_operation op : DFA.regops[Blk])
            {
                if constexpr (op.is_copy)
                    result.gen.reg[op.dst] = result.gen.reg[op.cpy_src];
                else if constexpr (op.set_val)
                    result.gen.reg[op.dst] = result.gen.current;
                else
                    result.gen.reg[op.dst] = 0;
            }
        }
    }

    template<std::size_t Blk, std::bidirectional_iterator I>
    static constexpr void register_operations(result_t<I, false> result, const I it)
    {
        if constexpr (Blk != tdfa::no_transition_regops)
        {
            template for (constexpr register_operation op : DFA.regops[Blk])
            {
                if constexpr (op.is_copy)
                    result.res.reg_[op.dst] = result.res.reg_[op.cpy_src];
                else if constexpr (op.set_val)
                    result.res.reg_[op.dst] = it;
                else if constexpr (std::contiguous_iterator<I>)
                    result.res.reg_[op.dst] = I{};

                if constexpr (not std::contiguous_iterator<I>)
                {
                    if constexpr (op.is_copy)
                        result.res.enabled_[op.dst] = result.res.enabled_[op.cpy_src];
                    else
                        result.res.enabled_[op.dst] = op.set_val;
                }
            }
        }
    }

    template<std::size_t Blk, std::bidirectional_iterator I>
    static constexpr void register_operations(no_result /* result */, const I /* it */) {}

    template<std::size_t Blk, std::ptrdiff_t Offset, typename Result, std::bidirectional_iterator I>
    static constexpr void set_final_info(Result result, const I it)
    {
        register_operations<Blk>(result, it);
        result.res.match_end_ = std::ranges::prev(it, Offset);

        if constexpr (not std::contiguous_iterator<I>)
            result.res.match_success_ = true;
    }

    template<std::size_t Blk, std::ptrdiff_t Offset, tdfa::continue_at_t ContinueAt, typename Result, std::bidirectional_iterator I>
    static constexpr void set_fallback_info(Result rag, const I it)
    {
        set_final_info<Blk, Offset>(rag, it);

        if constexpr (result<I>::has_continue)
        {
            if constexpr (ContinueAt != tdfa::no_continue)
                rag.res.continue_at_ = ContinueAt;
        }
    }

    template<std::size_t Blk, std::ptrdiff_t Offset, std::bidirectional_iterator I>
    static constexpr void set_final_info(no_result /* result */, const I /* it */) {};

    template<std::size_t Blk, std::ptrdiff_t Offset, tdfa::continue_at_t ContinueAt, std::bidirectional_iterator I>
    static constexpr void set_fallback_info(no_result /* result */, const I /* it */) {};

    template<typename Result, std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires (DFA.flags.enable_fallback)
    static constexpr bool fallback_state(Result result, I /* it */, const S /* last */, fallback_info<I> fallback)
    {
        if (fallback.state == fallback_disabled)
            return false;

        // TODO: change to use structured binding when supported
        template for (constexpr auto pair : DFA.fallback_nodes)
        {
            if (fallback.state == pair.first)
            {
                static constexpr auto fni = DFA.final_nodes.at(pair.first);
                if constexpr (not DFA.flags.return_bool)
                    set_fallback_info<pair.second.op_index, fni.final_offset, pair.second.continue_at>(result, fallback.it);
                return true;
            }
        }

        return false;
    }

    template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr bool state(Result result, I it, const S last, maybe_fallback_t<I> fallback)
    {
        static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState);
        static constexpr auto* fallback_node = DFA.fallback_nodes.at_if(DFAState);

        if constexpr (DFA.flags.enable_fallback and fallback_node != nullptr)
        {
            fallback.state = DFAState;
            if constexpr (not DFA.flags.return_bool)
                fallback.it = it;
        }

        if (it != last)
        {
            template for (constexpr static_transition<char_type> tr : DFA.nodes[DFAState])
            {
                if (tr_possible<tr.cs>(*it))
                {
                    register_operations<tr.op_index>(result, it);
                    [[clang::musttail]] return state<tr.next>(result, ++it, last, fallback);
                }
            }

            if constexpr (final_node != nullptr and DFA.flags.enable_fallback and fallback_node != nullptr)
            {
                set_fallback_info<final_node->op_index, final_node->final_offset, fallback_node->continue_at>(result, it);
                return true;
            }
        }
        else
        {
            if constexpr (final_node != nullptr)
            {
                if constexpr (DFA.flags.enable_fallback and fallback_node != nullptr)
                    set_fallback_info<final_node->op_index, final_node->final_offset, fallback_node->continue_at>(result, it);
                else
                    set_final_info<final_node->op_index, final_node->final_offset>(result, it);
                return true;
            }
        }

        if constexpr (DFA.flags.enable_fallback and fallback_node == nullptr)
            [[clang::musttail]] return fallback_state(result, it, last, fallback);
        return false;
    }

    template<std::size_t DFAState, typename Result, std::bidirectional_iterator I>
    static constexpr bool state(Result result, I it, const cstr_sentinel_t last, maybe_fallback_t<I> fallback)
    {
        static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState);
        static constexpr auto* fallback_node = DFA.fallback_nodes.at_if(DFAState);

        if constexpr (DFA.flags.enable_fallback and fallback_node != nullptr)
        {
            fallback.state = DFAState;
            if constexpr (not DFA.flags.return_bool)
                fallback.it = it;
        }

        template for (constexpr static_transition<char_type> tr : DFA.nodes[DFAState])
        {
            if (tr_possible_exclude_null<tr.cs>(*it))
            {
                register_operations<tr.op_index>(result, it);
                [[clang::musttail]] return state<tr.next>(result, ++it, last, fallback);
            }
        }

        if constexpr (final_node != nullptr)
        {
            if constexpr (DFA.flags.enable_fallback and fallback_node != nullptr)
            {
                set_fallback_info<final_node->op_index, final_node->final_offset, fallback_node->continue_at>(result, it);
                return true;
            }
            else
            {
                if (it == last) [[likely]]
                {
                    set_final_info<final_node->op_index, final_node->final_offset>(result, it);
                    return true;
                }
            }
        }

        if constexpr (DFA.flags.enable_fallback and fallback_node == nullptr)
            [[clang::musttail]] return fallback_state(result, it, last, fallback);
        return false;
    }

    template<std::size_t DFAState, std::size_t Count, typename Result, std::bidirectional_iterator I, std::sized_sentinel_for<I> S>
    static constexpr bool unchecked_state(Result result, I it, const S last, maybe_fallback_t<I> fallback)
    {
        if constexpr (Count == 0)
        {
            [[clang::musttail]] return state<DFAState>(result, it, last, fallback);
        }
        else
        {
            template for (constexpr static_transition<char_type> tr : DFA.nodes[DFAState])
            {
                if (tr_possible<tr.cs>(*it))
                {
                    register_operations<tr.op_index>(result, it);
                    [[clang::musttail]] return unchecked_state<tr.next, Count - 1>(result, ++it, last, fallback);
                }
            }

            return false;
        }
    }

    template<std::size_t DFAState, std::size_t Count, integer_sequence_like Skip, typename Result, std::bidirectional_iterator I, std::sized_sentinel_for<I> S>
    static constexpr bool unchecked_state_skip(Result result, I it, const S last, maybe_fallback_t<I> fallback)
    {
        if constexpr (sequence_helper<Skip>::empty or Count == 0 or DFA.nodes[DFAState].size() != 1)
        {
            [[clang::musttail]] return unchecked_state<DFAState, Count>(result, it, last, fallback);
        }
        else if constexpr (Count == sequence_helper<Skip>::head)
        {
            static constexpr auto tr = DFA.nodes[DFAState].front();
            register_operations<tr.op_index>(result, it);
            [[clang::musttail]] return unchecked_state_skip<tr.next, Count - 1, typename sequence_helper<Skip>::tail>(result, ++it, last, fallback);
        }
        else
        {
            static constexpr auto tr = DFA.nodes[DFAState].front();
            if (tr_possible<tr.cs>(*it))
            {
                register_operations<tr.op_index>(result, it);
                [[clang::musttail]] return unchecked_state_skip<tr.next, Count - 1, Skip>(result, ++it, last, fallback);
            }

            return false;
        }
    }

    template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr bool initial_state(Result result, I it, const S last)
    {
        return state<DFAState>(result, it, last, maybe_fallback_t<I>{ it, fallback_disabled });
    }

    template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sized_sentinel_for<I> S>
    static constexpr bool initial_state(Result result, I it, const S last)
    {
        static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

        if constexpr (fixed_length and not DFA.flags.enable_fallback)
        {
            if (std::ranges::distance(it, last) == min_length)
                return unchecked_state<DFAState, min_length>(result, it, last, maybe_fallback_t<I>{ it, fallback_disabled });
        }
        else
        {
            if (std::ranges::distance(it, last) >= min_length)
                return unchecked_state<DFAState, min_length>(result, it, last, maybe_fallback_t<I>{ it, fallback_disabled });
        }

        return false;
    }

    template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr bool scalar_outer_state(Result result, I it, const S last)
    {
        if constexpr (DFA.register_count > 0)
            ++result.gen.current;

        if (it != last)
        {
            if (state<DFAState>(result, it, last, maybe_fallback_t<I>{ it, fallback_disabled }))
            {
                if constexpr (not std::same_as<Result, no_result> and p1306dfa::result<I>::has_match_start)
                    result.res.match_start_ = it;
                return true;
            }
        }
        else
        {
            if constexpr (static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState); final_node != nullptr)
            {
                set_final_info<final_node->op_index, final_node->final_offset>(result, it);
                if constexpr (not std::same_as<Result, no_result> and p1306dfa::result<I>::has_match_start)
                    result.res.match_start_ = it;
                return true;
            }
            return false;
        }

        template for (constexpr static_transition<char_type> tr : DFA.outer_transitions)
        {
            if (tr_possible<tr.cs>(*it))
                [[clang::musttail]] return scalar_outer_state<tr.next>(result, ++it, last);
        }

        return false;
    }

    template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sized_sentinel_for<I> S>
    static constexpr bool scalar_outer_state(Result result, I it, const S last)
    {
        if constexpr (DFA.register_count > 0)
            ++result.gen.current;

        static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

        if (std::ranges::distance(it, last) >= min_length)
        {
            if (unchecked_state<DFAState, min_length>(result, it, last, maybe_fallback_t<I>{ it, fallback_disabled }))
            {
                if constexpr (not std::same_as<Result, no_result> and p1306dfa::result<I>::has_match_start)
                    result.res.match_start_ = it;
                return true;
            }
        }
        else
        {
            if constexpr (static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState); final_node != nullptr)
            {
                set_final_info<final_node->op_index, final_node->final_offset>(result, it);
                if constexpr (not std::same_as<Result, no_result> and p1306dfa::result<I>::has_match_start)
                    result.res.match_start_ = it;
                return true;
            }
            return false;
        }

        template for (constexpr static_transition<char_type> tr : DFA.outer_transitions)
        {
            if (tr_possible<tr.cs>(*it))
                [[clang::musttail]] return scalar_outer_state<tr.next>(result, ++it, last);
        }

        return false;
    }

    template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sized_sentinel_for<I> S>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                  and (DFA.flags.enable_fallback or not fixed_length))
    static constexpr bool scalar_outer_state(Result result, I it, const S last)
    {
        static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

        while (std::ranges::distance(it, last) >= min_length)
        {
            if constexpr (DFA.register_count > 0)
                ++result.gen.current;

            if (unchecked_state<DFAState, min_length>(result, it, last, maybe_fallback_t<I>{ it, fallback_disabled }))
            {
                if constexpr (not std::same_as<Result, no_result> and p1306dfa::result<I>::has_match_start)
                    result.res.match_start_ = it;
                return true;
            }

            ++it;
        }

        /* since the DFA is never empty, we can skip assigning final node information */
        static_assert(DFA.final_nodes.at_if(DFAState) == nullptr);

        return false;
    }

    template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sized_sentinel_for<I> S>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                  and (/* degenerate case */ fixed_length and not DFA.flags.enable_fallback))
    static constexpr bool scalar_outer_state(Result result, I it, const S last)
    {
        static constexpr auto length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

        if (const auto input_size = std::ranges::distance(it, last); input_size >= length)
        {
            std::ranges::advance(it, input_size - length);

            if (unchecked_state<DFAState, length>(result, it, last, maybe_fallback_t<I>{ it, fallback_disabled }))
            {
                if constexpr (not std::same_as<Result, no_result> and p1306dfa::result<I>::has_match_start)
                    result.res.match_start_ = it;
                return true;
            }
        }

        /* since the DFA is never empty, we can skip assigning final node information */
        static_assert(DFA.final_nodes.at_if(DFAState) == nullptr);

        return false;
    }

#if __cpp_lib_simd >= 202411L or (defined(__GNUC_MINOR__) and __GLIBCXX__ >= 20260424L)
    template<std::size_t DFAState, std::size_t Count, integer_sequence_like Skip,
             typename Result, std::contiguous_iterator I, std::sized_sentinel_for<I> S>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState)
    [[gnu::always_inline]] static constexpr bool vector_candidate_check(Result result, I it, const S last, unsigned long long mask)

    {
        while (mask != 0) [[unlikely]]
        {
            const auto offset = std::countr_zero(mask);
            mask &= (mask - 1);

            // TODO: implement simd-based checking
            if (unchecked_state_skip<DFAState, Count, Skip>(result, it + offset, last, maybe_fallback_t<I>{ it + offset, fallback_disabled }))
            {
                if constexpr (not std::same_as<Result, no_result> and p1306dfa::result<I>::has_match_start)
                    result.res.match_start_ = it + offset;
                return true;
            }
            else
            {
                if constexpr (DFA.register_count > 0)
                    ++result.gen.current;
            }
        }

        return false;
    }

    template<std::size_t DFAState, typename Result, std::contiguous_iterator I, std::sized_sentinel_for<I> S>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                  and fixed_length and branch_free)
    static constexpr bool vector_outer_state(Result result, I it, const S last)
    {
        static constexpr auto length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);
        static constexpr auto states = std::define_static_array(get_flattened_states(DFA, DFAState));

        using uchar_type = std::make_unsigned_t<char_type>;
        using vec_type = std::simd::vec<uchar_type>;
        using mask_type = vec_type::mask_type;

        static constexpr auto flags = [] consteval {
            if constexpr (not std::same_as<char_type, uchar_type>)
                return std::simd::flag_convert;
            else
                return std::simd::flag_default;
        }();

        static constexpr auto [position1, position2] = get_outer_state_position_pair(DFA, states);
        using skipped = std::index_sequence<length - position1, length - position2>;

        static constexpr bool avoid_simd = [] consteval {
            std::size_t count1{ DFA.nodes[states[position1]].front().cs.count() };
            std::size_t count2{ DFA.nodes[states[position2]].front().cs.count() };
#if __cpp_lib_saturation_arithmetic >= 202603L
            return std::saturating_mul(count1, count2)  > std::saturating_mul(avoid_simd_threshold, avoid_simd_threshold);
#else
            return std::mul_sat(count1, count2)  > std::mul_sat(avoid_simd_threshold, avoid_simd_threshold);
#endif
        }();

        if constexpr (avoid_simd)
        {
            /* using simd in this case is likely a pessimisation; resort to non-vectorised implementation instead */
            return scalar_outer_state<DFAState>(result, it, last);
        }
        else
        {
            const std::ptrdiff_t max{ std::ranges::distance(it, last) - (length - 1) };

            for (std::ptrdiff_t i{ max / vec_type::size() }; i > 0; --i) [[likely]]
            {
                const auto block1 = std::simd::unchecked_load<vec_type>(it + position1, last, flags);
                const auto block2 = std::simd::unchecked_load<vec_type>(it + position2, last, flags);

                const auto mask1 = vector_tr_find<DFA.nodes[states[position1]].front().cs>(block1);
                const auto mask2 = vector_tr_find<DFA.nodes[states[position2]].front().cs>(block2);

                if (vector_candidate_check<DFAState, length, skipped>(result, it, last, (mask1 & mask2).to_ullong()))
                    return true;

                it += vec_type::size();
            }

            if (const auto epi_size = max % vec_type::size(); epi_size > 0) [[likely]]
            {
                const mask_type epi_mask{ (1uz << epi_size) - 1 };

#if false
                /* potential bug involving non-mask overloads of simd::partial_load ??? */
                const auto block1 = std::simd::partial_load<vec_type>(it + position1, epi_size, flags);
                const auto block2 = std::simd::partial_load<vec_type>(it + position2, epi_size, flags);
#else
                const auto block1 = std::simd::partial_load<vec_type>(it + position1, epi_size, epi_mask, flags);
                const auto block2 = std::simd::partial_load<vec_type>(it + position2, epi_size, epi_mask, flags);
#endif

                const auto mask1 = vector_tr_find<DFA.nodes[states[position1]].front().cs>(block1);
                const auto mask2 = vector_tr_find<DFA.nodes[states[position2]].front().cs>(block2);

                return vector_candidate_check<DFAState, length, skipped>(result, it, last, (mask1 & mask2 & epi_mask).to_ullong());
            }

            return false;
        }
    }

    template<std::size_t DFAState, typename Result, std::contiguous_iterator I, std::sized_sentinel_for<I> S>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                  and (not fixed_length or not branch_free))
    static constexpr bool vector_outer_state(Result result, I it, const S last)
    {
        static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);
        static constexpr auto states = std::define_static_array(get_flattened_states(DFA, DFAState, true));

        using uchar_type = std::make_unsigned_t<char_type>;
        using vec_type = std::simd::vec<uchar_type>;
        using mask_type = vec_type::mask_type;

        static constexpr auto flags = []{
            if constexpr (not std::same_as<char_type, uchar_type>)
                return std::simd::flag_convert;
            else
                return std::simd::flag_default;
        }();

        static constexpr auto position = get_outer_state_position_single(DFA, states);
        using skipped = std::index_sequence<min_length - position>;

        // TODO: possibly select a second position to check?
        static constexpr static_charset combined_cs = []{
            charset<char_type> result;
            template for (constexpr auto tr : DFA.nodes[position])
                result |= tr.cs;
            return static_charset{ result };
        }();

        static constexpr bool avoid_simd{ combined_cs.count() > avoid_simd_threshold };

        if constexpr (avoid_simd)
        {
            /* using simd in this case is likely a pessimisation; resort to non-vectorised implementation instead */
            return scalar_outer_state<DFAState>(result, it, last);
        }
        else
        {
            const std::ptrdiff_t max{ std::ranges::distance(it, last) - (min_length - 1) };

            for (std::ptrdiff_t i{ max / vec_type::size() }; i > 0; --i) [[likely]]
            {
                const auto block = std::simd::unchecked_load<vec_type>(it + position, last, flags);
                const auto mask = vector_tr_find<combined_cs>(block);

                if (vector_candidate_check<DFAState, min_length, skipped>(result, it, last, mask.to_ullong()))
                    return true;

                it += vec_type::size();
            }

            if (const auto epi_size = max % vec_type::size(); epi_size > 0) [[likely]]
            {
                const mask_type epi_mask{ (1uz << epi_size) - 1 };

#if false
                /* potential bug involving non-mask overloads of simd::partial_load ??? */
                const auto block = std::simd::partial_load<vec_type>(it + position, epi_size, flags);
#else
                const auto block = std::simd::partial_load<vec_type>(it + position, epi_size, epi_mask, flags);
#endif
                const auto mask = vector_tr_find<combined_cs>(block);

                return vector_candidate_check<DFAState, min_length, skipped>(result, it, last, (mask & epi_mask).to_ullong());
            }

            return false;
        }
    }
#endif // __cpp_lib_simd >= 202411L

    template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr bool outer_state(Result result, I it, const S last)
    {
#if __cpp_lib_simd >= 202411L or (defined(__GNUC_MINOR__) and __GLIBCXX__ >= 20260424L)
        if constexpr (std::contiguous_iterator<I> and std::sized_sentinel_for<S, I>
                      and never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState)
            return vector_outer_state<DFAState>(result, it, last);
        else
#endif // __cpp_lib_simd >= 202411L
            return scalar_outer_state<DFAState>(result, it, last);
    }

public:
    struct matcher;
    struct searcher;
};


template<std::meta::info DFARefl>
struct p1306dfa<DFARefl>::matcher
{
    static constexpr bool enable_normal{ not DFA.flags.return_bool };
    static constexpr bool enable_noempty{ enable_normal and DFA.flags.maybe_no_empty };

public:
    template<std::bidirectional_iterator I>
    using result = p1306dfa::result<I>;

    static constexpr bool never_empty{ p1306dfa::never_empty };

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires (not enable_normal) and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    [[nodiscard]] static constexpr bool operator()(const I first, const S last)
    {
        return initial_state<DFA.match_start>(no_result{}, first, last);
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires enable_normal and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    [[nodiscard]] static constexpr auto operator()(const I first, const S last) -> result<I>
    {
        result<I> res{ first };
        initial_state<DFA.match_start>(result_t{ res }, first, last);
        return res;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires enable_normal and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type> and result<I>::has_continue
    [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at) -> result<I>
    {
        result<I> res{ first };

        template for (constexpr std::size_t i : std::views::iota(0uz, DFA.continue_nodes.size()))
        {
            if (i == continue_at)
            {
                initial_state<DFA.continue_nodes[i]>(result_t{ res }, first, last);
#ifndef __GNUC_MINOR__
                break;
#else
                return res; // temporary workaround for possible GCC bug
#endif
            }
        }

        return res;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires enable_noempty and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    [[nodiscard]] static constexpr auto operator()(const I first, const S last, match_non_empty_t) -> result<I>
    {
        result<I> res{ first };
        if constexpr (never_empty)
            initial_state<DFA.match_start>(result_t{ res }, first, last);
        else
            initial_state<DFA.additional_continue_nodes.back()>(result_t{ res }, first, last);
        return res;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires enable_noempty and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type> and result<I>::has_continue
    [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at, match_non_empty_t) -> result<I>
    {
        result<I> res{ first };

        template for (constexpr std::size_t i : std::views::iota(0uz, DFA.continue_nodes.size()))
        {
            if (i == continue_at)
            {
                if constexpr (never_empty)
                    initial_state<DFA.continue_nodes[i]>(result_t{ res }, first, last);
                else
                    initial_state<DFA.additional_continue_nodes[i]>(result_t{ res }, first, last);
#ifndef __GNUC_MINOR__
                break;
#else
                return res; // temporary workaround for possible GCC bug
#endif
            }
        }

        return res;
    }
};


template<std::meta::info DFARefl>
struct p1306dfa<DFARefl>::searcher
{
    static constexpr bool enable_normal{ not DFA.flags.return_bool };
    static constexpr bool enable_noempty{ enable_normal and DFA.flags.maybe_no_empty };

public:
    template<std::bidirectional_iterator I>
    using result = p1306dfa::result<I>;

    static constexpr bool never_empty{ p1306dfa::never_empty };

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires (not enable_normal) and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    [[nodiscard]] static constexpr bool operator()(const I first, const S last)
    {
        return outer_state<DFA.match_start>(no_result{}, first, last);
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires enable_normal and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    [[nodiscard]] static constexpr auto operator()(const I first, const S last) -> result<I>
    {
        result<I> res{ first };
        gen_info gen{};
        outer_state<DFA.match_start>(result_t{ res, gen }, first, last);
        clean_generations(res, gen);
        return res;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires enable_normal and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type> and result<I>::has_continue
    [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at) -> result<I>
    {
        result<I> res{ first };
        gen_info gen{};

        template for (constexpr std::size_t i : std::views::iota(0uz, DFA.continue_nodes.size()))
        {
            if (i == continue_at)
            {
                outer_state<DFA.continue_nodes[i]>(result_t{ res, gen }, first, last);
#ifndef __GNUC_MINOR__
                break;
#else
                clean_generations(res, gen);
                return res; // temporary workaround for possible GCC bug
#endif
            }
        }

        clean_generations(res, gen);
        return res;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires enable_noempty and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    [[nodiscard]] static constexpr auto operator()(const I first, const S last, match_non_empty_t) -> result<I>
    {
        result<I> res{ first };
        gen_info gen{};

        if constexpr (never_empty)
            outer_state<DFA.match_start>(result_t{ res, gen }, first, last);
        else
            outer_state<DFA.additional_continue_nodes.back()>(result_t{ res, gen }, first, last);

        clean_generations(res, gen);
        return res;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires enable_noempty and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type> and result<I>::has_continue
    [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at, match_non_empty_t) -> result<I>
    {
        result<I> res{ first };
        gen_info gen{};

        template for (constexpr std::size_t i : std::views::iota(0uz, DFA.continue_nodes.size()))
        {
            if (i == continue_at)
            {
                if constexpr (never_empty)
                    outer_state<DFA.continue_nodes[i]>(result_t{ res, gen }, first, last);
                else
                    outer_state<DFA.additional_continue_nodes[i]>(result_t{ res, gen }, first, last);
#ifndef __GNUC_MINOR__
                break;
#else
                clean_generations(res, gen);
                return res; // temporary workaround for possible GCC bug
#endif
            }
        }

        clean_generations(res, gen);
        return res;
    }
};

template<string_literal Pattern, fsm_flags Flags>
using p1306_matcher = p1306dfa<(^^re<Pattern, pack_flags(Flags)>)>::matcher;

template<string_literal Pattern, fsm_flags Flags>
using p1306_searcher = p1306dfa<(^^re<Pattern, pack_flags(adapt_searcher_flags_to_matcher(Flags))>)>::searcher;

} // namespace srx::detail