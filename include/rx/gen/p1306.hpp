// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <bit>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <limits>
#include <meta>
#include <type_traits>

#if __cpp_lib_simd >= 202411L or (__GNUC__ >= 16 and defined(__GLIBCXX__))
    #include <simd>
#endif

#include "rx/etc/static_charset.hpp"
#include "rx/etc/string_literal.hpp"
#include "rx/etc/util.hpp"
#include "rx/fsm/flags.hpp"
#include "rx/fsm/tdfa.hpp"
#include "rx/gen/compile.hpp"
#include "rx/gen/result.hpp"


namespace rx::detail
{
    template<typename CharT>
    struct optimised_tr_edge
    {
        using value_type = std::make_unsigned_t<CharT>;

        value_type sub;
        value_type rng;
        value_type msk;
    };

    template<typename CharT>
    consteval auto make_optimised_edges(const static_charset<CharT>& sc, bool inverted = false)
    {
        using ote = optimised_tr_edge<CharT>;
        using uchar_type = typename ote::value_type;
        std::vector<ote> result;

        if (inverted)
        {
            for (const auto& [first, last] : (~sc).get_intervals())
                result.emplace_back(static_cast<uchar_type>(first),
                                    static_cast<uchar_type>(static_cast<CharT>(last - first)),
                                    static_cast<uchar_type>(~0u));
        }
        else
        {
            for (const auto& [first, last] : sc.get_intervals())
                result.emplace_back(static_cast<uchar_type>(first),
                                    static_cast<uchar_type>(static_cast<CharT>(last - first)),
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
    consteval auto get_flattened_states(const tdfa_info<CharT>& dfa, std::size_t state)
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
                break;

            const auto& tr = dfa.nodes.at(state).front();
            result.emplace_back(state);
            state = tr.next;
        }

        return result;
    }

    template<static_charset Sc>
    [[gnu::always_inline]] constexpr bool tr_possible(const typename decltype(Sc)::char_type c)
    {
#ifdef __clang_major__
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

#if __cpp_lib_simd >= 202411L or (__GNUC__ >= 16 and defined(__GLIBCXX__))
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

    template<string_literal Pattern, fsm_flags Flags>
    struct p1306dfa
    {
        using char_type = decltype(Pattern)::value_type;

        static constexpr tdfa_info DFA{ compile_pattern(Pattern.view(), Flags) };
        static constexpr bool never_empty{ DFA.additional_continue_nodes.empty() };
        static constexpr bool fixed_length{ DFA.min_max_lengths.first != std::numeric_limits<std::size_t>::max()
                                            and DFA.min_max_lengths.first == DFA.min_max_lengths.second };
        static constexpr bool branch_free{ std::ranges::all_of(DFA.nodes, [](const auto& n){ return n.size() <= 1; }) };

        template<typename I>
        using result = static_regex_match_result<I, DFA.make_match_result_info(Flags.is_iterator)>;

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

        static constexpr bool has_fallback_it{ not Flags.return_bool };

        template<typename I>
        struct fallback_info
        {
            [[no_unique_address]] maybe_type_t<has_fallback_it, I> it;
            std::size_t state;
        };

        template<typename I>
        using maybe_fallback_t = maybe_type_t<Flags.enable_fallback, fallback_info<I>>;

        static constexpr bool return_bool = (Flags.return_bool);


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
        static constexpr bool fallback_state(Result result, I /* it */, const S /* last */, fallback_info<I> fallback)
            requires (Flags.enable_fallback)
        {
            if (fallback.state == fallback_disabled)
                return false;

            // TODO: change to use structured binding when supported
            template for (constexpr const auto& pair : DFA.fallback_nodes)
            {
                if (fallback.state == pair.first)
                {
                    static constexpr auto fni = DFA.final_nodes.at(pair.first);
                    if constexpr (has_fallback_it)
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

            if constexpr (Flags.enable_fallback and fallback_node != nullptr)
            {
                fallback.state = DFAState;
                if constexpr (has_fallback_it)
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

                if constexpr (final_node != nullptr and Flags.enable_fallback and fallback_node != nullptr)
                {
                    set_fallback_info<final_node->op_index, final_node->final_offset, fallback_node->continue_at>(result, it);
                    return true;
                }
            }
            else
            {
                if constexpr (final_node != nullptr)
                {
                    if constexpr (Flags.enable_fallback and fallback_node != nullptr)
                        set_fallback_info<final_node->op_index, final_node->final_offset, fallback_node->continue_at>(result, it);
                    else
                        set_final_info<final_node->op_index, final_node->final_offset>(result, it);
                    return true;
                }
            }

            if constexpr (Flags.enable_fallback and fallback_node == nullptr)
                [[clang::musttail]] return fallback_state(result, it, last, fallback);
            return false;
        }

        template<std::size_t DFAState, typename Result, std::bidirectional_iterator I>
        static constexpr bool state(Result result, I it, const cstr_sentinel_t last, maybe_fallback_t<I> fallback)
        {
            static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState);
            static constexpr auto* fallback_node = DFA.fallback_nodes.at_if(DFAState);

            if constexpr (Flags.enable_fallback and fallback_node != nullptr)
            {
                fallback.state = DFAState;
                if constexpr (has_fallback_it)
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
                if constexpr (Flags.enable_fallback and fallback_node != nullptr)
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

            if constexpr (Flags.enable_fallback and fallback_node == nullptr)
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

        template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool initial_state(Result result, I it, const S last)
        {
            return state<DFAState>(result, it, last, maybe_fallback_t<I>{ it, fallback_disabled });
        }

        template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sized_sentinel_for<I> S>
        static constexpr bool initial_state(Result result, I it, const S last)
            requires (fixed_length and not Flags.enable_fallback)
        {
            static constexpr auto length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

            if (std::ranges::distance(it, last) == length)
                return unchecked_state<DFAState, length>(result, it, last, maybe_fallback_t<I>{ it, fallback_disabled });
            return false;
        }

        template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sized_sentinel_for<I> S>
        static constexpr bool initial_state(Result result, I it, const S last)
            requires (Flags.enable_fallback or not fixed_length)
        {
            static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

            if (std::ranges::distance(it, last) >= min_length)
                return unchecked_state<DFAState, min_length>(result, it, last, maybe_fallback_t<I>{ it, fallback_disabled });
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
            requires (DFA.continue_nodes.size() > 1 and (Flags.enable_fallback or not fixed_length))
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
        static constexpr bool scalar_outer_state(Result result, I it, const S last)
            requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                      and (Flags.enable_fallback or not fixed_length))
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
        static constexpr bool scalar_outer_state(Result result, I it, const S last)
            requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                      and (/* degenerate case */ fixed_length and not Flags.enable_fallback))
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

#if __cpp_lib_simd >= 202411L or (__GNUC__ >= 16 and defined(__GLIBCXX__))
        template<std::size_t DFAState, std::size_t Count, typename Result, std::contiguous_iterator I, std::sized_sentinel_for<I> S>
        [[gnu::always_inline]] static constexpr bool vector_candidate_check(Result result, I it, const S last, unsigned long long mask)
            requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState)
        {
            while (mask != 0)
            {
                const auto offset = std::countr_zero(mask);

                // TODO: implement simd-based checking
                if (unchecked_state<DFAState, Count>(result, it + offset, last, maybe_fallback_t<I>{ it + offset, fallback_disabled }))
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

                mask &= (mask - 1);
            }

            return false;
        }

        template<std::size_t DFAState, typename Result, std::contiguous_iterator I, std::sized_sentinel_for<I> S>
        static constexpr bool vector_outer_state(Result result, I it, const S last)
            requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                      and fixed_length and branch_free)
        {
            static constexpr auto length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);
            static constexpr auto states = std::define_static_array(get_flattened_states(DFA, DFAState));

            using uchar_type = std::make_unsigned_t<char_type>;
            using vec_type = std::simd::vec<uchar_type>;
            using mask_type = vec_type::mask_type;

            constexpr auto flags = []() consteval {
                if constexpr (not std::same_as<char_type, uchar_type>)
                    return std::simd::flag_convert;
                else
                    return std::simd::flag_default;
            }();

            static constexpr std::size_t position1{ 0 };
            static constexpr std::size_t position2{ states.size() - 1 };

            static constexpr bool avoid_simd = []() consteval {
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

                std::ptrdiff_t i{ 0 };
                for (; i + vec_type::size() < max; i += vec_type::size())
                {
                    const auto block1 = std::simd::unchecked_load<vec_type>(it + position1, last, flags);
                    const auto block2 = std::simd::unchecked_load<vec_type>(it + position2, last, flags);

                    const auto mask1 = vector_tr_find<DFA.nodes[states[position1]].front().cs>(block1);
                    const auto mask2 = vector_tr_find<DFA.nodes[states[position2]].front().cs>(block2);

                    if (vector_candidate_check<DFAState, length>(result, it, last, (mask1 & mask2).to_ullong()))
                        return true;

                    it += vec_type::size();
                }

                if (i < max)
                {
                    const auto epi_size = max - i;
                    const mask_type epi_mask{ (1uz << epi_size) - 1 };

                    const auto block1 = std::simd::partial_load<vec_type>(it + position1, epi_size, flags);
                    const auto block2 = std::simd::partial_load<vec_type>(it + position2, epi_size, flags);

                    const auto mask1 = vector_tr_find<DFA.nodes[states[position1]].front().cs>(block1);
                    const auto mask2 = vector_tr_find<DFA.nodes[states[position2]].front().cs>(block2);

                    if (vector_candidate_check<DFAState, length>(result, it, last, (mask1 & mask2 & epi_mask).to_ullong()))
                        return true;
                }

                return false;
            }
        }

        template<std::size_t DFAState, typename Result, std::contiguous_iterator I, std::sized_sentinel_for<I> S>
        static constexpr bool vector_outer_state(Result result, I it, const S last)
            requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                      and (not fixed_length or not branch_free))
        {
            static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

            using uchar_type = std::make_unsigned_t<char_type>;
            using vec_type = std::simd::vec<uchar_type>;
            using mask_type = vec_type::mask_type;

            constexpr auto flags = []() {
                if constexpr (not std::same_as<char_type, uchar_type>)
                    return std::simd::flag_convert;
                else
                    return std::simd::flag_default;
            }();

            static constexpr bool avoid_simd = []() {
                charset<char_type> acc;
                for (const auto& tr : DFA.nodes[DFAState])
                    acc |= tr.cs;
                return acc.count() > avoid_simd_threshold;
            }();

            if constexpr (avoid_simd)
            {
                /* using simd in this case is likely a pessimisation; resort to non-vectorised implementation instead */
                return scalar_outer_state<DFAState>(result, it, last);
            }
            else
            {
                const std::ptrdiff_t max{ std::ranges::distance(it, last) - (min_length - 1) };

                std::ptrdiff_t i{ 0 };
                for (; i + vec_type::size() < max; i += vec_type::size())
                {
                    const auto block = std::simd::unchecked_load<vec_type>(it, last, flags);

                    mask_type mask{};

#ifdef __GNUC_MINOR__
                    [&](){ /* NB: This is a workaround for a name mangling bug */
#endif
                    template for (constexpr static_transition<char_type> tr : DFA.nodes[DFAState])
                        mask |= vector_tr_find<tr.cs>(block);
#ifdef __GNUC_MINOR__
                    }();
#endif

                    if (vector_candidate_check<DFAState, min_length>(result, it, last, mask.to_ullong()))
                        return true;

                    it += vec_type::size();
                }

                if (i < max)
                {
                    const auto epi_size = max - i;
                    const mask_type epi_mask{ (1uz << epi_size) - 1 };

                    const auto block = std::simd::partial_load<vec_type>(it, last, epi_mask, flags);

                    mask_type mask{};

#ifdef __GNUC_MINOR__
                    [&](){ /* NB: This is a workaround for a name mangling bug */
#endif
                    template for (constexpr static_transition<char_type> tr : DFA.nodes[DFAState])
                        mask |= vector_tr_find<tr.cs>(block);
#ifdef __GNUC_MINOR__
                    }();
#endif

                    if (vector_candidate_check<DFAState, min_length>(result, it, last, (mask & epi_mask).to_ullong()))
                        return true;
                }

                return false;
            }
        }
#endif // __cpp_lib_simd >= 202411L

        template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool outer_state(Result result, I it, const S last)
        {
#if __cpp_lib_simd >= 202411L or (__GNUC__ >= 16 and defined(__GLIBCXX__))
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

    template<string_literal Pattern, fsm_flags Flags>
    struct p1306dfa<Pattern, Flags>::matcher
    {
    public:
        template<std::bidirectional_iterator I>
        using result = p1306dfa::result<I>;

        static constexpr bool never_empty{ p1306dfa::never_empty };

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            requires (Flags.return_bool) and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr bool operator()(const I first, const S last)
        {
            return initial_state<DFA.match_start>(no_result{}, first, last);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            requires (not Flags.return_bool) and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last) -> result<I>
        {
            result<I> res{ first };
            initial_state<DFA.match_start>(result_t{ res }, first, last);
            return res;
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            requires (not Flags.return_bool) and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at) -> result<I>
            requires result<I>::has_continue
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
            requires (not Flags.return_bool) and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, match_non_empty_t) -> result<I>
            requires (Flags.maybe_no_empty)
        {
            result<I> res{ first };
            if constexpr (never_empty)
                initial_state<DFA.match_start>(result_t{ res }, first, last);
            else
                initial_state<DFA.additional_continue_nodes.back()>(result_t{ res }, first, last);
            return res;
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            requires (not Flags.return_bool) and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at, match_non_empty_t) -> result<I>
            requires result<I>::has_continue and (Flags.maybe_no_empty)
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

    template<string_literal Pattern, fsm_flags Flags>
    struct p1306dfa<Pattern, Flags>::searcher
    {
    public:
        template<std::bidirectional_iterator I>
        using result = p1306dfa::result<I>;

        static constexpr bool never_empty{ p1306dfa::never_empty };

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            requires (Flags.return_bool) and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr bool operator()(const I first, const S last)
        {
            return outer_state<DFA.match_start>(no_result{}, first, last);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            requires (not Flags.return_bool) and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last) -> result<I>
        {
            result<I> res{ first };
            gen_info gen{};
            outer_state<DFA.match_start>(result_t{ res, gen }, first, last);
            clean_generations(res, gen);
            return res;
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            requires (not Flags.return_bool) and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at) -> result<I>
            requires result<I>::has_continue
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
            requires (not Flags.return_bool) and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, match_non_empty_t) -> result<I>
            requires (Flags.maybe_no_empty)
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
            requires (not Flags.return_bool) and std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at, match_non_empty_t) -> result<I>
            requires result<I>::has_continue and (Flags.maybe_no_empty)
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
    using p1306_matcher = p1306dfa<Pattern, Flags>::matcher;

    template<string_literal Pattern, fsm_flags Flags>
    using p1306_searcher = p1306dfa<Pattern, adapt_searcher_flags_to_matcher(Flags)>::searcher;
}