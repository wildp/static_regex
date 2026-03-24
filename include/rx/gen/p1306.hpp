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
    struct p1306_matcher
    {
        using dfa_t = compiled_dfa<Pattern, Flags>;
        using char_type = decltype(Pattern)::value_type;
        static constexpr bool never_empty{ dfa_t::value.additional_continue_nodes.empty() };

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
        {
            static_assert(Flags.enable_fallback);

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
                    if constexpr (static constexpr auto* fbn{ dfa_t::value.fallback_nodes.at_if(DFAState) }; Flags.enable_fallback and fbn != nullptr)
                        set_fallback_info<fn->op_index, fn->final_offset, fbn->continue_at>(res, it);
                    else
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

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, match_non_empty_t) -> result<I>
        requires (Flags.maybe_no_empty)
        {
            result<I> res{ first };
            if constexpr (never_empty)
                state<dfa_t::value.match_start>(res, first, last, first, fallback_disabled);
            else
                state<dfa_t::value.additional_continue_nodes.back()>(res, first, last, first, fallback_disabled);
            return res;
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at, match_non_empty_t) -> result<I>
        requires result<I>::has_continue and (Flags.maybe_no_empty)
        {
            result<I> res{ first };

            template for (constexpr std::size_t i : std::views::iota(0uz, dfa_t::value.continue_nodes.size()))
            {
                if (i == continue_at)
                {
                    if constexpr (never_empty)
                        state<dfa_t::value.continue_nodes[i]>(res, first, last, first, fallback_disabled);
                    else
                        state<dfa_t::value.additional_continue_nodes[i]>(res, first, last, first, fallback_disabled);
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
        using char_type = decltype(Pattern)::value_type;
        static constexpr bool never_empty{ dfa_t::value.additional_continue_nodes.empty() };

        template<typename I>
        using result = static_regex_match_result<I, dfa_t::value.make_match_result_info(Flags.is_iterator)>;

    private:
        struct generation
        {
            std::array<std::size_t, dfa_t::value.register_count> reg{};
            std::size_t current{ 0 };
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
                if constexpr (ContinueAt != tdfa::no_continue)
                    res.continue_at_ = ContinueAt;
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool fallback(result<I>& res, gen_info& gen, I /* it */, const S /* last */, I fallback_it, std::size_t fallback_state)
        {
            static_assert(Flags.enable_fallback);

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
                    if constexpr (static constexpr auto* fbn{ dfa_t::value.fallback_nodes.at_if(DFAState) }; Flags.enable_fallback and fbn != nullptr)
                        set_fallback_info<fn->op_index, fn->final_offset, fbn->continue_at>(res, gen, it);
                    else
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

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void outer_state(result<I>& res, gen_info& gen, I it, const S last)
        {
            if constexpr (dfa_t::value.register_count > 0)
                ++gen.current;

            if (it != last)
            {
                if (state<DFAState>(res, gen, it, last, it, fallback_disabled))
                {
                    if constexpr (result<I>::has_match_start)
                        res.match_start_ = it;
                    return;
                }
            }
            else
            {
                if constexpr (static constexpr auto* fn{ dfa_t::value.final_nodes.at_if(DFAState) }; fn != nullptr)
                {
                    set_final_info<fn->op_index, fn->final_offset>(res, gen, it);
                    if constexpr (result<I>::has_match_start)
                        res.match_start_ = it;
                }
                return;
            }

            template for (constexpr static_transition<char_type> tr : dfa_t::value.outer_transitions)
            {
                if (tr_possible<tr>(*it))
                    [[clang::musttail]] return outer_state<tr.next>(res, gen, ++it, last);
            }
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
        requires result<I>::has_continue
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

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, match_non_empty_t) -> result<I>
        requires (Flags.maybe_no_empty)
        {
            result<I> res{ first };
            gen_info gen{};

            if constexpr (never_empty)
                outer_state<dfa_t::value.match_start>(res, gen, first, last);
            else
                outer_state<dfa_t::value.additional_continue_nodes.back()>(res, gen, first, last);

            clean_generations(res, gen);
            return res;
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at, match_non_empty_t) -> result<I>
        requires result<I>::has_continue and (Flags.maybe_no_empty)
        {
            result<I> res{ first };
            gen_info gen{};

            template for (constexpr std::size_t i : std::views::iota(0uz, dfa_t::value.continue_nodes.size()))
            {
                if (i == continue_at)
                {
                    if constexpr (never_empty)
                        outer_state<dfa_t::value.continue_nodes[i]>(res, gen, first, last);
                    else
                        outer_state<dfa_t::value.additional_continue_nodes[i]>(res, gen, first, last);
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
        using char_type = decltype(Pattern)::value_type;

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

            if constexpr (Flags.enable_fallback)
                [[clang::musttail]] return fallback(it, last, fallback_state);

            return false;
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool state(I it, const S last, std::size_t fallback_state)
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

            if constexpr (dfa_t::value.final_nodes.contains(DFAState))
            {
                if constexpr (Flags.enable_fallback and dfa_t::value.fallback_nodes.contains(DFAState))
                {
                    return true;
                }
                else
                {
                    if (it == last) [[likely]]
                        return true;
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

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        static constexpr bool operator()(const I first, const S last, match_non_empty_t) = delete;

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        static constexpr bool operator()(const I first, const S last, const tdfa::continue_at_t continue_at, match_non_empty_t) = delete;
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
        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool outer_state(I it, const S last)
        {
            if (it != last)
            {
                if (underlying::template state<DFAState>(it, last, underlying::fallback_disabled))
                    return true;
            }
            else
            {
                return dfa_t::value.final_nodes.contains(DFAState);
            }

            template for (constexpr static_transition<char_type> tr : dfa_t::value.outer_transitions)
            {
                if (tr_possible<tr>(*it))
                    [[clang::musttail]] return outer_state<tr.next>(++it, last);
            }

            return false;
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

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        static constexpr bool operator()(const I first, const S last, match_non_empty_t) = delete;

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        static constexpr bool operator()(const I first, const S last, const tdfa::continue_at_t continue_at, match_non_empty_t) = delete;
    };
}
