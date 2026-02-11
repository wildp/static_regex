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

                    register_operations<pair.second.op_index>(res, fallback_it);
                    res.match_end_ = std::ranges::prev(fallback_it, fni.final_offset);

                    if constexpr (not std::contiguous_iterator<I>)
                        res.match_success_ = true;

                    if constexpr (Flags.is_iterator)
                    {
                        if constexpr (pair.second.continue_at != tdfa::no_continue)
                            res.continue_at_ = pair.second.continue_at;
                    }

                    return;
                }
            }
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void transition(result<I>& res, I it, const S last, I fallback_it, std::size_t fallback_state)
        {
            template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
            {
                if (tr_possible<tr>(*it))
                {
                    register_operations<tr.op_index>(res, it);
                    [[clang::musttail]] return state<tr.next>(res, ++it, last, fallback_it, fallback_state);
                }
            }

            // TODO: reimplement using std::optional<T&> accessor?
            if constexpr (static constexpr auto dt{ dfa_t::value.default_transitions.find(DFAState) }; dt != dfa_t::value.default_transitions.end())
            {
                if constexpr ((*dt).second.op_index == tdfa::default_transition_is_not_state)
                {
                    [[clang::musttail]] return transition<(*dt).second.next>(res, it, last, fallback_it, fallback_state);
                }
                else
                {
                    register_operations<(*dt).second.op_index>(res, it);
                    [[clang::musttail]] return state<(*dt).second.next>(res, ++it, last, fallback_it, fallback_state);
                }
            }

            if constexpr (Flags.enable_fallback)
                [[clang::musttail]] return fallback(res, it, last, fallback_it, fallback_state);
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void state(result<I>& res, I it, const S last, I fallback_it, std::size_t fallback_state)
        {
            if constexpr (Flags.enable_fallback and dfa_t::value.fallback_nodes.contains(DFAState))
            {
                fallback_state = DFAState;
                fallback_it = it;
            }

            if (it != last) [[likely]]
                [[clang::musttail]] return transition<DFAState>(res, it, last, fallback_it, fallback_state);

            // TODO: reimplement using std::optional<T&> accessor?
            if constexpr (static constexpr auto fn{ dfa_t::value.final_nodes.find(DFAState) }; fn != dfa_t::value.final_nodes.end())
            {
                register_operations<(*fn).second.op_index>(res, it);
                res.match_end_ = std::ranges::prev(it, (*fn).second.final_offset);

                if constexpr (not std::contiguous_iterator<I>)
                    res.match_success_ = true;

                return;
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
        requires (Flags.is_iterator)
        {
            result<I> res{ first };

            template for (constexpr std::size_t i : std::views::iota(0uz, dfa_t::value.continue_nodes.size()))
            {
                if (i == continue_at)
                {
                    state<dfa_t::value.continue_nodes[i]>(res, first, last, first, fallback_disabled);
                    return res;
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
        static constexpr void register_operations(result<I>& res, gen_info& gen, I it)
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

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void fallback(result<I>& res, gen_info& gen, I /* it */, const S /* last */, I fallback_it, std::size_t fallback_state)
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

                    register_operations<pair.second.op_index>(res, gen, fallback_it);
                    res.match_end_ = std::ranges::prev(fallback_it, fni.final_offset);

                    if constexpr (not std::contiguous_iterator<I>)
                        res.match_success_ = true;

                    if constexpr (Flags.is_iterator)
                    {
                        if constexpr (pair.second.continue_at != tdfa::no_continue)
                            res.continue_at_ = pair.second.continue_at;
                    }

                    return;
                }
            }
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void transition(result<I>& res, gen_info& gen, I it, const S last, I fallback_it, std::size_t fallback_state)
        {
            template for (constexpr static_transition<char_type> tr : dfa_t::value.nodes.at(DFAState))
            {
                if (tr_possible<tr>(*it))
                {
                    register_operations<tr.op_index>(res, gen, it);
                    [[clang::musttail]] return state<tr.next>(res, gen, ++it, last, fallback_it, fallback_state);
                }
            }

            // TODO: reimplement using std::optional<T&> accessor?
            if constexpr (static constexpr auto dt{ dfa_t::value.default_transitions.find(DFAState) }; dt != dfa_t::value.default_transitions.end())
            {
                if constexpr ((*dt).second.op_index == tdfa::default_transition_is_not_state)
                {
                    [[clang::musttail]] return transition<(*dt).second.next>(res, gen, it, last, fallback_it, fallback_state);
                }
                else
                {
                    register_operations<(*dt).second.op_index>(res, gen, it);
                    [[clang::musttail]] return state<(*dt).second.next>(res, gen, ++it, last, fallback_it, fallback_state);
                }
            }

            if constexpr (Flags.enable_fallback)
                [[clang::musttail]] return fallback(res, gen, it, last, fallback_it, fallback_state);
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void state(result<I>& res, gen_info& gen, I it, const S last, I fallback_it, std::size_t fallback_state)
        {
            if constexpr (Flags.enable_fallback and dfa_t::value.fallback_nodes.contains(DFAState))
            {
                fallback_state = DFAState;
                fallback_it = it;
            }

            if (it != last) [[likely]]
                [[clang::musttail]] return transition<DFAState>(res, gen, it, last, fallback_it, fallback_state);

            // TODO: reimplement using std::optional<T&> accessor?
            if constexpr (static constexpr auto fn{ dfa_t::value.final_nodes.find(DFAState) }; fn != dfa_t::value.final_nodes.end())
            {
                register_operations<(*fn).second.op_index>(res, gen, it);;
                res.match_end_ = std::ranges::prev(it, (*fn).second.final_offset);

                if constexpr (not std::contiguous_iterator<I>)
                    res.match_success_ = true;

                return;
            }

            if constexpr (Flags.enable_fallback)
                [[clang::musttail]] return fallback(res, gen, it, last, fallback_it, fallback_state);
        }

        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr void outer_state(result<I>& res, gen_info& gen, I it, const S last)
        {
            if constexpr (dfa_t::value.outer_transitions.size() == 1 and DFAState == dfa_t::value.outer_transitions[0].next)
            {
                while (true)
                {
                    if constexpr (result<I>::has_match_start)
                        res.match_start_ = it;

                    state<DFAState>(res, gen, it, last, it, fallback_disabled);

                    if (res.has_value() or it == last) [[unlikely]]
                        return;

                    if constexpr (dfa_t::value.register_count > 0)
                        ++gen.current;

                    ++it;
                }
            }
            else
            {
                if constexpr (result<I>::has_match_start)
                    res.match_start_ = it;

                state<DFAState>(res, gen, it, last, it, fallback_disabled);

                if (res.has_value() or it == last) [[unlikely]]
                    return;

                if constexpr (dfa_t::value.register_count > 0)
                    ++gen.current;

                template for (constexpr static_transition<char_type> tr : dfa_t::value.outer_transitions)
                {
                    if (tr_possible<tr>(*it))
                        [[clang::musttail]] return outer_state<tr.next>(res, gen, ++it, last);
                }
            }
        }

    public:
        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last) -> result<I>
        {
            result<I> res{};
            gen_info gen{};
            outer_state<dfa_t::value.match_start>(res, gen, first, last);
            clean_generations(res, gen);
            return res;
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto operator()(const I first, const S last, const tdfa::continue_at_t continue_at) -> result<I>
        requires (Flags.is_iterator)
        {
            result<I> res{};
            gen_info gen{};

            template for (constexpr std::size_t i : std::views::iota(0uz, dfa_t::value.continue_nodes.size()))
            {
                if (i == continue_at)
                {
                    outer_state<dfa_t::value.continue_nodes[i]>(res, gen, first, last);
                    clean_generations(res, gen);
                    return res;
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

    protected:
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

            // TODO: reimplement using std::optional<T&> accessor?
            if constexpr (static constexpr auto dt{ dfa_t::value.default_transitions.find(DFAState) }; dt != dfa_t::value.default_transitions.end())
            {
                if constexpr ((*dt).second.op_index == tdfa::default_transition_is_not_state)
                    [[clang::musttail]] return transition<(*dt).second.next>(it, last, fallback_state);
                else
                    [[clang::musttail]] return state<(*dt).second.next>(++it, last, fallback_state);
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
    struct p1306_searcher<Pattern, Flags> : public p1306_matcher<Pattern, adapt_searcher_flags_to_matcher(Flags)>
    {
        // static_assert(Flags.is_search); // TODO: fix this

        using base = p1306_searcher<Pattern, adapt_searcher_flags_to_matcher(Flags)>;
        using dfa_t = base::dfa_t;
        using char_type = base::char_type;

    private:
        template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
        static constexpr bool outer_state(I it, const S last)
        {
            if constexpr (dfa_t::value.outer_transitions.size() == 1 and DFAState == dfa_t::value.outer_transitions[0].next)
            {
                while (true)
                {
                    const auto result{ base::template state<DFAState>(it, last, base::fallback_disabled) };

                    if (result) [[unlikely]]
                        return true;

                    if (it == last) [[unlikely]]
                        return false;

                    ++it;
                }
            }
            else
            {
                const auto result{ base::template state<DFAState>(it, last, base::fallback_disabled) };

                if (result) [[unlikely]]
                    return true;

                if (it == last) [[unlikely]]
                    return false;

                template for (constexpr static_transition<char_type> tr : dfa_t::value.outer_transitions)
                {
                    if (tr_possible<tr>(*it))
                        [[clang::musttail]] return outer_state<tr.next>(++it, last);
                }

                std::unreachable();
            }
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
