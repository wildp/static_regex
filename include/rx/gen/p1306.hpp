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

#include "rx/etc/string_literal.hpp"
#include "rx/fsm/flags.hpp"
#include "rx/fsm/tdfa.hpp"
#include "rx/gen/compile.hpp"
#include "rx/gen/result.hpp"


namespace rx::detail
{
    template<typename CharT, std::pair<CharT, CharT>... Intervals>
    [[gnu::always_inline]] constexpr bool tr_possible_impl(CharT c)
    {
        return ((Intervals.first <= c and c <= Intervals.second) or ...);
    }

    template<typename CharT>
    consteval auto tr_possible_make_refl(static_transition<CharT> tr) -> std::vector<std::meta::info>
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

    template<typename CharT, CharT Lower, CharT Upper>
    [[gnu::always_inline]] constexpr bool tr_possible_impl_exclude_null_helper(CharT c)
    {
        static constexpr CharT null_char{};
        if constexpr (Lower == null_char and Upper == null_char)
            return false;
        else if constexpr (Lower == null_char)
            return (Lower + 1) <= c and c <= Upper;
        else if constexpr (Upper == null_char)
            return Lower <= c and c <= (Upper - 1);
        else if constexpr (Lower < null_char and null_char < Upper)
            return (Lower <= c and c <= (null_char - 1)) or ((null_char + 1) <= c and c <= Upper);
        else
            return Lower <= c and c <= Upper;
    }

    template<typename CharT, std::pair<CharT, CharT>... Intervals>
    [[gnu::always_inline]] constexpr bool tr_possible_impl_exclude_null(CharT c)
    {
        return (tr_possible_impl_exclude_null_helper<CharT, Intervals.first, Intervals.second>(c) or ...);
    }

    template<static_transition Tr, typename CharT>
    [[gnu::always_inline]] constexpr bool tr_possible_exclude_null(CharT c)
    {
        constexpr auto func{ std::meta::substitute(^^tr_possible_impl_exclude_null, tr_possible_make_refl(Tr)) };
        return [: func :](c);
    }

    consteval fsm_flags adapt_searcher_flags_to_matcher(fsm_flags f)
    {
        return (f & default_fsm_flags::disable_search_modifier) | default_fsm_flags::make_iterator_modifier;
    }


    template<string_literal Pattern, fsm_flags Flags>
    struct p1306_matcher
    {
        using dfa_t = compiled_dfa<Pattern, Flags>;
        using char_type = decltype(Pattern)::char_type;

        template<typename I>
        using result = static_regex_match_result<I, dfa_t::value.make_match_result_info(Flags.is_iterator)>;

    private:
        static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };

        template<std::size_t Blk, std::bidirectional_iterator I>
        static constexpr void register_operations(result<I>& res, I it)
        {
            if constexpr (Blk != tdfa::no_transition_regops)
            {
                template for (constexpr register_operation op : dfa_t::value.regops.at(Blk))
                {
                    if constexpr (op.is_copy)
                        res.reg_[op.dst] = res.reg_[op.cpy_src];
                    else if constexpr (op.set_val)
                        res.reg_[op.dst] = it;
                    else if constexpr (std::contiguous_iterator<I>)
                        res.reg_[op.dst] = I{};

                    if constexpr (not std::contiguous_iterator<I>)
                    {
                        if constexpr (op.is_copy)
                            res.enabled_[op.dst] = res.enabled_[op.cpy_src];
                        else
                            res.enabled_[op.dst] = op.set_val;
                    }
                }
            }
        }

        template<std::size_t Blk, std::ptrdiff_t Offset, std::bidirectional_iterator I>
        static constexpr void set_final_info(result<I>& res, const I it)
        {
            register_operations<Blk>(res, it);
            res.match_end_ = std::ranges::prev(it, Offset);

            if constexpr (not std::contiguous_iterator<I>)
                res.match_success_ = true;
        }

        template<std::size_t Blk, std::ptrdiff_t Offset, tdfa::continue_at_t ContinueAt, std::bidirectional_iterator I>
        static constexpr void set_fallback_info(result<I>& res, const I it)
        {
            set_final_info<Blk, Offset>(res, it);

            if constexpr (result<I>::has_continue)
            {
                if constexpr (ContinueAt != tdfa::no_continue)
                    res.continue_at_ = ContinueAt;
            }
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void fallback(result<I>& res, I /* it */, const S /* last */, I fallback_it, std::size_t fallback_state)
        requires (Flags.enable_fallback)
        {
            if (fallback_state == fallback_disabled)
                return;

            // TODO: change to use structured binding when supported
            template for (constexpr const auto& pair : dfa_t::value.fallback_nodes)
            {
                if (fallback_state == pair.first)
                {
                    static constexpr auto fni{ dfa_t::value.final_nodes.at(pair.first) };
                    set_fallback_info<pair.second.op_index, fni.final_offset, pair.second.continue_at>(res, fallback_it);
                    return;
                }
            }
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void state(result<I>& res, I it, const S last, I fallback_it, std::size_t fallback_state)
        requires (not std::same_as<S, cstr_sentinel_t>)
        {
            if constexpr (Flags.enable_fallback and dfa_t::value.fallback_nodes.contains(DFAState))
            {
                fallback_state = DFAState;
                fallback_it = it;
            }

            if (it != last)
            {
                template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
                {
                    if (tr_possible<tr>(*it))
                    {
                        register_operations<tr.op_index>(res, it);
                        [[clang::musttail]] return state<tr.next>(res, ++it, last, fallback_it, fallback_state);
                    }
                }

                if constexpr (static constexpr auto* dt{ dfa_t::value.default_transitions.at_if(DFAState) }; dt != nullptr)
                {
                    static_assert(dt->op_index != tdfa::default_transition_is_not_state);

                    register_operations<dt->op_index>(res, it);
                    [[clang::musttail]] return state<dt->next>(res, ++it, last, fallback_it, fallback_state);
                }

                if constexpr (static constexpr auto* fn{ dfa_t::value.final_nodes.at_if(DFAState) }; fn != nullptr)
                {
                    if constexpr (static constexpr auto* fbn{ dfa_t::value.fallback_nodes.at_if(DFAState) }; Flags.enable_fallback and fbn != nullptr)
                    {
                        set_fallback_info<fn->op_index, fn->final_offset, fbn->continue_at>(res, it);
                        return;
                    }
                }
            }
            else
            {
                if constexpr (static constexpr auto* fn{ dfa_t::value.final_nodes.at_if(DFAState) }; fn != nullptr)
                {
                    set_final_info<fn->op_index, fn->final_offset>(res, it);
                    return;
                }
            }

            if constexpr (Flags.enable_fallback)
                [[clang::musttail]] return fallback(res, it, last, fallback_it, fallback_state);
        }

        template<std::size_t DFAState, std::bidirectional_iterator I>
        static constexpr void state(result<I>& res, I it, const cstr_sentinel_t last, I fallback_it, std::size_t fallback_state)
        {
            if constexpr (Flags.enable_fallback and dfa_t::value.fallback_nodes.contains(DFAState))
            {
                fallback_state = DFAState;
                fallback_it = it;
            }

            template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
            {
                if (tr_possible_exclude_null<tr>(*it))
                {
                    register_operations<tr.op_index>(res, it);
                    [[clang::musttail]] return state<tr.next>(res, ++it, last, fallback_it, fallback_state);
                }
            }

            if constexpr (static constexpr auto* dt{ dfa_t::value.default_transitions.at_if(DFAState) }; dt != nullptr)
            {
                static_assert(dt->op_index != tdfa::default_transition_is_not_state);

                if (it != last) [[likely]]
                {
                    register_operations<dt->op_index>(res, it);
                    [[clang::musttail]] return state<dt->next>(res, ++it, last, fallback_it, fallback_state);
                }
            }

            if constexpr (static constexpr auto* fn{ dfa_t::value.final_nodes.at_if(DFAState) }; fn != nullptr)
            {
                if constexpr (static constexpr auto* fbn{ dfa_t::value.fallback_nodes.at_if(DFAState) }; Flags.enable_fallback and fbn != nullptr)
                {
                    set_fallback_info<fn->op_index, fn->final_offset, fbn->continue_at>(res, it);
                    return;
                }
                else
                {
                    if (it == last) [[likely]]
                    {
                        set_final_info<fn->op_index, fn->final_offset>(res, it);
                        return;
                    }
                }
            }

            if constexpr (Flags.enable_fallback)
                [[clang::musttail]] return fallback(res, it, last, fallback_it, fallback_state);
        }

    public:
        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last) -> result<I>
        {
            result<I> res{ first };
            state<dfa_t::value.match_start>(res, first, last, first, fallback_disabled);
            return res;
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at) -> result<I>
        requires result<I>::has_continue
        {
            result<I> res{ first };

            template for (constexpr std::size_t i : std::views::iota(0uz, dfa_t::value.continue_nodes.size()))
            {
                if (i == continue_at)
                {
                    state<dfa_t::value.continue_nodes[i]>(res, first, last, first, fallback_disabled);
                    break;
                }
            }

            return res;
        }
    };


    template<string_literal Pattern, fsm_flags Flags>
    struct p1306_searcher
    {
        static_assert(Flags.is_search);

        using dfa_t = compiled_dfa<Pattern, adapt_searcher_flags_to_matcher(Flags)>;
        using char_type = decltype(Pattern)::char_type;

        template<typename I>
        using result = static_regex_match_result<I, dfa_t::value.make_match_result_info(Flags.is_iterator)>;

    private:
        struct generation
        {
            std::array<std::size_t, dfa_t::value.register_count> reg{};
            std::size_t current{ 1 };
        };

        using gen_info = maybe_type_t<(dfa_t::value.register_count > 0), generation>;

        static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };

        template<std::bidirectional_iterator I>
        static constexpr void clean_generations(result<I>& res, const gen_info& gen)
        {
            if constexpr (dfa_t::value.register_count > 0)
            {
                if constexpr (std::contiguous_iterator<I>)
                {
                    for (std::size_t i{ 0 }; i < dfa_t::value.register_count; ++i)
                        res.reg_[i] = (gen.reg[i] == gen.current) ? res.reg_[i] : I{};
                }
                else
                {
                   for (std::size_t i{ 0 }; i < dfa_t::value.register_count; ++i)
                        res.enabled_[i] = (gen.reg[i] == gen.current);
                }
            }
        }

        template<std::size_t Blk, std::bidirectional_iterator I>
        static constexpr void register_operations(result<I>& res, gen_info& gen, const I it)
        {
            if constexpr (Blk != tdfa::no_transition_regops)
            {
                static_assert(dfa_t::value.register_count > 0);

                template for (constexpr register_operation op : dfa_t::value.regops.at(Blk))
                {
                    if constexpr (op.is_copy)
                        res.reg_[op.dst] = res.reg_[op.cpy_src];
                    else if constexpr (op.set_val)
                        res.reg_[op.dst] = it;
                }

                template for (constexpr register_operation op : dfa_t::value.regops.at(Blk))
                {
                    if constexpr (op.is_copy)
                        gen.reg[op.dst] = gen.reg[op.cpy_src];
                    else if constexpr (op.set_val)
                        gen.reg[op.dst] = gen.current;
                    else
                        gen.reg[op.dst] = 0;
                }
            }
        }

        template<std::size_t Blk, std::ptrdiff_t Offset, std::bidirectional_iterator I>
        static constexpr void set_final_info(result<I>& res, gen_info& gen, const I it)
        {
            register_operations<Blk>(res, gen, it);
            res.match_end_ = std::ranges::prev(it, Offset);

            if constexpr (not std::contiguous_iterator<I>)
                res.match_success_ = true;
        }

        template<std::size_t Blk, std::ptrdiff_t Offset, tdfa::continue_at_t ContinueAt, std::bidirectional_iterator I>
        static constexpr void set_fallback_info(result<I>& res, gen_info& gen, const I it)
        {
            set_final_info<Blk, Offset>(res, gen, it);

            if constexpr (result<I>::has_continue)
            {
                if constexpr (ContinueAt != tdfa::no_continue)
                    res.continue_at_ = ContinueAt;
            }
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool fallback(result<I>& res, gen_info& gen, I /* it */, const S /* last */, I fallback_it, std::size_t fallback_state)
        requires (Flags.enable_fallback)
        {
            if (fallback_state == fallback_disabled)
                return false;

            // TODO: change to use structured binding when supported
            template for (constexpr const auto& pair : dfa_t::value.fallback_nodes)
            {
                if (fallback_state == pair.first)
                {
                    static constexpr auto fni{ dfa_t::value.final_nodes.at(pair.first) };
                    set_fallback_info<pair.second.op_index, fni.final_offset, pair.second.continue_at>(res, gen, fallback_it);
                    return true;
                }
            }

            return false;
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool state(result<I>& res, gen_info& gen, I it, const S last, I fallback_it, std::size_t fallback_state)
        requires (not std::same_as<S, cstr_sentinel_t>)
        {
            if constexpr (Flags.enable_fallback and dfa_t::value.fallback_nodes.contains(DFAState))
            {
                fallback_state = DFAState;
                fallback_it = it;
            }

            if (it != last)
            {
                template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
                {
                    if (tr_possible<tr>(*it))
                    {
                        register_operations<tr.op_index>(res, gen, it);
                        [[clang::musttail]] return state<tr.next>(res, gen, ++it, last, fallback_it, fallback_state);
                    }
                }

                if constexpr (static constexpr auto* dt{ dfa_t::value.default_transitions.at_if(DFAState) }; dt != nullptr)
                {
                    static_assert(dt->op_index != tdfa::default_transition_is_not_state);
                    register_operations<dt->op_index>(res, gen, it);
                    [[clang::musttail]] return state<dt->next>(res, gen, ++it, last, fallback_it, fallback_state);
                }

                if constexpr (static constexpr auto* fn{ dfa_t::value.final_nodes.at_if(DFAState) }; fn != nullptr)
                {
                    if constexpr (static constexpr auto* fbn{ dfa_t::value.fallback_nodes.at_if(DFAState) }; Flags.enable_fallback and fbn != nullptr)
                    {
                        set_fallback_info<fn->op_index, fn->final_offset, fbn->continue_at>(res, gen, it);
                        return true;
                    }
                }
            }
            else
            {
                if constexpr (static constexpr auto* fn{ dfa_t::value.final_nodes.at_if(DFAState) }; fn != nullptr)
                {
                    set_final_info<fn->op_index, fn->final_offset>(res, gen, it);
                    return true;
                }
            }

            if constexpr (Flags.enable_fallback)
                [[clang::musttail]] return fallback(res, gen, it, last, fallback_it, fallback_state);

            return false;
        }

        template<std::size_t DFAState, std::bidirectional_iterator I>
        static constexpr bool state(result<I>& res, gen_info& gen, I it, const cstr_sentinel_t last, I fallback_it, std::size_t fallback_state)
        {
            if constexpr (Flags.enable_fallback and dfa_t::value.fallback_nodes.contains(DFAState))
            {
                fallback_state = DFAState;
                fallback_it = it;
            }

            template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
            {
                if (tr_possible_exclude_null<tr>(*it))
                {
                    register_operations<tr.op_index>(res, gen, it);
                    [[clang::musttail]] return state<tr.next>(res, gen, ++it, last, fallback_it, fallback_state);
                }
            }

            if constexpr (static constexpr auto* dt{ dfa_t::value.default_transitions.at_if(DFAState) }; dt != nullptr)
            {
                static_assert(dt->op_index != tdfa::default_transition_is_not_state);

                if (it != last)
                {
                    register_operations<dt->op_index>(res, gen, it);
                    [[clang::musttail]] return state<dt->next>(res, gen, ++it, last, fallback_it, fallback_state);
                }
            }

            if constexpr (static constexpr auto* fn{ dfa_t::value.final_nodes.at_if(DFAState) }; fn != nullptr)
            {
                if constexpr (static constexpr auto* fbn{ dfa_t::value.fallback_nodes.at_if(DFAState) }; Flags.enable_fallback and fbn != nullptr)
                {
                    set_fallback_info<fn->op_index, fn->final_offset, fbn->continue_at>(res, gen, it);
                    return true;
                }
                else
                {
                    if (it == last) [[likely]]
                    {
                        set_final_info<fn->op_index, fn->final_offset>(res, gen, it);
                        return true;
                    }
                }
            }

            if constexpr (Flags.enable_fallback)
                [[clang::musttail]] return fallback(res, gen, it, last, fallback_it, fallback_state);

            return false;
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void outer_transition(result<I>& res, gen_info& gen, I it, const S last)
        {
            if constexpr (result<I>::has_match_start)
                ++res.match_start_;

            if constexpr (dfa_t::value.register_count > 0)
                ++gen.current;

            template for (constexpr static_transition<char_type> tr : dfa_t::value.outer_transitions)
            {
                if (tr_possible<tr>(*it))
                    [[clang::musttail]] return outer_state<tr.next>(res, gen, ++it, last);
            }
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void outer_state(result<I>& res, gen_info& gen, I it, const S last)
        {
            static constexpr std::size_t fallback_state_init{
                (Flags.enable_fallback and dfa_t::value.fallback_nodes.contains(DFAState))
                ? DFAState
                : fallback_disabled
            };

            if (it == last)
            {
                if constexpr (static constexpr auto* fn{ dfa_t::value.final_nodes.at_if(DFAState) }; fn != nullptr)
                    set_final_info<fn->op_index, fn->final_offset>(res, gen, it);
                return;
            }

            template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
            {
                if (tr_possible<tr>(*it))
                {
                    register_operations<tr.op_index>(res, gen, it);
                    if (state<tr.next>(res, gen, std::ranges::next(it), last, it, fallback_state_init))
                        return;
                    [[clang::musttail]] return outer_transition(res, gen, it, last);
                }
            }

            if constexpr (static constexpr auto* dt{ dfa_t::value.default_transitions.at_if(DFAState) }; dt != nullptr)
            {
                static_assert(dt->op_index != tdfa::default_transition_is_not_state);
                register_operations<dt->op_index>(res, gen, it);
                if (state<dt->next>(res, gen, std::ranges::next(it), last, it, fallback_state_init))
                    return;
                [[clang::musttail]] return outer_transition(res, gen, it, last);
            }

            if constexpr (static constexpr auto* fn{ dfa_t::value.final_nodes.at_if(DFAState) }; fn != nullptr and DFAState == fallback_state_init)
            {
                set_fallback_info<fn->op_index, fn->final_offset, dfa_t::value.fallback_nodes.at(DFAState).continue_at>(res, gen, it);
                return;
            }

            [[clang::musttail]] return outer_transition(res, gen, it, last);
        }

    public:
        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last) -> result<I>
        {
            result<I> res{ first };
            gen_info gen{};
            outer_state<dfa_t::value.match_start>(res, gen, first, last);
            clean_generations(res, gen);
            return res;
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at) -> result<I>
        requires (result<I>::has_continue)
        {
            result<I> res{ first };
            gen_info gen{};

            template for (constexpr std::size_t i : std::views::iota(0uz, dfa_t::value.continue_nodes.size()))
            {
                if (i == continue_at)
                {
                    outer_state<dfa_t::value.continue_nodes[i]>(res, gen, first, last);
                    break;
                }
            }

            clean_generations(res, gen);
            return res;
        }
    };


    template<string_literal Pattern, fsm_flags Flags>
    requires (Flags.return_bool)
    struct p1306_matcher<Pattern, Flags>
    {
        using dfa_t = compiled_dfa<Pattern, Flags>;
        using char_type = decltype(Pattern)::char_type;

    private:
        template<string_literal OtherPattern, fsm_flags OtherFlags>
        friend struct p1306_searcher;

        static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool fallback(I /* it */, const S /* last */, std::size_t fallback_state)
        requires (Flags.enable_fallback)
        {
            if (fallback_state == fallback_disabled)
                return false;

            // TODO: change to use structured binding when supported
            template for (constexpr const auto& pair : dfa_t::value.fallback_nodes)
            {
                if (fallback_state == pair.first)
                    return true;
            }

            return false;
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool transition(I it, const S last, std::size_t fallback_state)
        {
            template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
            {
                if (tr_possible<tr>(*it))
                    [[clang::musttail]] return state<tr.next>(++it, last, fallback_state);
            }

            if constexpr (static constexpr auto* dt{ dfa_t::value.default_transitions.at_if(DFAState) }; dt != nullptr)
            {
                static_assert(dt->op_index != tdfa::default_transition_is_not_state);

                [[clang::musttail]] return state<dt->next>(++it, last, fallback_state);
            }

            if constexpr (Flags.enable_fallback)
                [[clang::musttail]] return fallback(it, last, fallback_state);

            return false;
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool state(I it, const S last, std::size_t fallback_state)
        requires (not std::same_as<S, cstr_sentinel_t>)
        {
            if constexpr (Flags.enable_fallback and dfa_t::value.fallback_nodes.contains(DFAState))
                fallback_state = DFAState;

            if (it != last) [[likely]]
                [[clang::musttail]] return transition<DFAState>(it, last, fallback_state);

            if constexpr (dfa_t::value.final_nodes.contains(DFAState))
                return true;

            if constexpr (Flags.enable_fallback)
                [[clang::musttail]] return fallback(it, last, fallback_state);

            return false;
        }

        template<std::size_t DFAState, std::bidirectional_iterator I>
        static constexpr bool state(I it, const cstr_sentinel_t last, std::size_t fallback_state)
        {
            if constexpr (Flags.enable_fallback and dfa_t::value.fallback_nodes.contains(DFAState))
                fallback_state = DFAState;

            template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
            {
                if (tr_possible_exclude_null<tr>(*it))
                    [[clang::musttail]] return state<tr.next>(++it, last, fallback_state);
            }

            if constexpr (static constexpr auto* dt{ dfa_t::value.default_transitions.at_if(DFAState) }; dt != nullptr)
            {
                static_assert(dt->op_index != tdfa::default_transition_is_not_state);

                if (it != last) [[likely]]
                    [[clang::musttail]] return state<dt->next>(++it, last, fallback_state);
                else if constexpr (dfa_t::value.final_nodes.contains(DFAState))
                    return true;
            }
            else if constexpr (dfa_t::value.final_nodes.contains(DFAState))
            {
                if (it == last) [[likely]]
                    return true;
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

    template<string_literal Pattern, fsm_flags Flags>
    requires (Flags.return_bool)
    struct p1306_searcher<Pattern, Flags>
    {
        static_assert(Flags.is_search);

        using underlying = p1306_matcher<Pattern, adapt_searcher_flags_to_matcher(Flags)>;
        using dfa_t = underlying::dfa_t;
        using char_type = underlying::char_type;

    private:
        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool outer_transition(I it, const S last)
        {
            template for (constexpr static_transition<char_type> tr : dfa_t::value.outer_transitions)
            {
                if (tr_possible<tr>(*it))
                    [[clang::musttail]] return outer_state<tr.next>(++it, last);
            }

            return false;
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool outer_state(I it, const S last)
        {
            static constexpr std::size_t fallback_state_init{
                (Flags.enable_fallback and dfa_t::value.fallback_nodes.contains(DFAState))
                ? DFAState
                : underlying::fallback_disabled
            };

            if (it == last)
                return dfa_t::value.final_nodes.contains(DFAState);

            template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
            {
                if (tr_possible<tr>(*it))
                {
                    if (underlying::template state<tr.next>(std::ranges::next(it), last, fallback_state_init))
                        return true;
                    [[clang::musttail]] return outer_transition(it, last);
                }
            }

            if constexpr (static constexpr auto* dt{ dfa_t::value.default_transitions.at_if(DFAState) }; dt != nullptr)
            {
                static_assert(dt->op_index != tdfa::default_transition_is_not_state);
                if (underlying::template state<dt->next>(std::ranges::next(it), last, fallback_state_init))
                    return true;
                [[clang::musttail]] return outer_transition(it, last);
            }

            if constexpr (dfa_t::value.final_nodes.contains(DFAState) and DFAState == fallback_state_init)
                return true;

            [[clang::musttail]] return outer_transition(it, last);
        }

    public:
        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr bool operator()(const I first, const S last)
        {
            return outer_state<dfa_t::value.match_start>(first, last);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        static constexpr bool operator()(const I first, const S last, const tdfa::continue_at_t continue_at) = delete;
    };
}
