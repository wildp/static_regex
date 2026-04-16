// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <limits>
#include <meta>
#include <type_traits>

#include "rx/etc/string_literal.hpp"
#include "rx/etc/util.hpp"
#include "rx/fsm/flags.hpp"
#include "rx/fsm/tdfa.hpp"
#include "rx/gen/compile.hpp"
#include "rx/gen/result.hpp"


namespace rx::detail
{
    template<static_transition Tr, typename CharT>
    [[gnu::always_inline]] constexpr bool tr_possible(CharT c)
    {
        // TODO: change to use structured binding when supported
        template for (constexpr auto pair : Tr.cs)
        {
            if (pair.first <= c and c <= pair.second)
                return true;
        }

        return false;
    }

    template<static_transition Tr, typename CharT>
    [[gnu::always_inline]] constexpr bool tr_possible_exclude_null(CharT c)
    {
        if constexpr (tr_possible<Tr>(CharT{}))
        {
            if (c == CharT{})
                return false;
        }

        return tr_possible<Tr>(c);
    }


    template<string_literal Pattern, fsm_flags Flags>
    struct p1306dfa
    {
        using char_type = decltype(Pattern)::value_type;

        static constexpr tdfa_info DFA{ compile_pattern(Pattern.view(), Flags) };
        static constexpr bool never_empty{ DFA.additional_continue_nodes.empty() };
        static constexpr bool fixed_length{ DFA.min_max_lengths.first != std::numeric_limits<std::size_t>::max()
                                            and DFA.min_max_lengths.first == DFA.min_max_lengths.second };

        template<typename I>
        using result = static_regex_match_result<I, DFA.make_match_result_info(Flags.is_iterator)>;

    private:
        static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };

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

                template for (constexpr register_operation op : DFA.regops.at(Blk))
                {
                    if constexpr (op.is_copy)
                        result.res.reg_[op.dst] = result.res.reg_[op.cpy_src];
                    else if constexpr (op.set_val)
                        result.res.reg_[op.dst] = it;
                }

                template for (constexpr register_operation op : DFA.regops.at(Blk))
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
                template for (constexpr register_operation op : DFA.regops.at(Blk))
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
                template for (constexpr static_transition<char_type> tr : DFA.nodes.at(DFAState))
                {
                    if (tr_possible<tr>(*it))
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

            template for (constexpr static_transition<char_type> tr : DFA.nodes.at(DFAState))
            {
                if (tr_possible_exclude_null<tr>(*it))
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
                template for (constexpr static_transition<char_type> tr : DFA.nodes.at(DFAState))
                {
                    if (tr_possible<tr>(*it))
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
        static constexpr bool linear_outer_state(Result result, I it, const S last)
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
                if (tr_possible<tr>(*it))
                    [[clang::musttail]] return linear_outer_state<tr.next>(result, ++it, last);
            }

            return false;
        }

        template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sized_sentinel_for<I> S>
        static constexpr bool linear_outer_state(Result result, I it, const S last)
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
                if (tr_possible<tr>(*it))
                    [[clang::musttail]] return linear_outer_state<tr.next>(result, ++it, last);
            }

            return false;
        }

        template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sized_sentinel_for<I> S>
        static constexpr bool linear_outer_state(Result result, I it, const S last)
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
        static constexpr bool linear_outer_state(Result result, I it, const S last)
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

        template<std::size_t DFAState, typename Result, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool outer_state(Result result, I it, const S last)
        {
            return linear_outer_state<DFAState>(result, it, last);
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