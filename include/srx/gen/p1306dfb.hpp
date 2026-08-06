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

#include "srx/etc/string_literal.hpp"
#include "srx/etc/util.hpp"
#include "srx/fsm/flags.hpp"
#include "srx/fsm/tdfa.hpp"
#include "srx/gen/compile.hpp"
#include "srx/gen/result.hpp"


namespace srx {
namespace detail {

namespace backlink_types {

using small_t  = std::uint_least8_t;
using medium_t = std::uint_least16_t;
using large_t  = std::uint_least32_t;
using xlarge_t = std::uint_least64_t;

} // namespace backlink_types

template<std::meta::info Info>
struct p1306dfb
{
    static_assert(has_template_arguments(type_of(Info)), "Invalid reflection value");
    static_assert(template_of(type_of(Info)) == ^^tdfa_info);

    using char_type = typename [: template_arguments_of(type_of(Info))[0] :];

private:
    static constexpr const tdfa_info<char_type>& DFA =  [: Info :];
    static_assert(not DFA.onepass, "Invalid reflection value: use p1306dfa instead");

    static constexpr bool never_empty{ DFA.additional_continue_nodes.empty() };
    static constexpr bool bounded_size{ DFA.min_max_lengths.first != std::numeric_limits<std::size_t>::max() };
    static constexpr bool fixed_length{ bounded_size and DFA.min_max_lengths.first == DFA.min_max_lengths.second };
    static constexpr bool branch_free{ std::ranges::all_of(DFA.nodes, [](const auto& n){ return n.size() <= 1; }) };

    static constexpr std::size_t actual_length{ 0 /* TODO: calculate maximum length, inclusive of assertions extending beyond the end of patterns */ };
    static constexpr std::size_t backlink_array_count{ DFA.backlink_arrays.size() };

    static consteval bool has_different_alt(tdfa::continue_at_t continue_at = tdfa::no_continue)
    {
        if (never_empty)
            return false;
        else if (continue_at == tdfa::no_continue)
            return DFA.match_start != DFA.additional_continue_nodes.back();
        else
            return DFA.continue_nodes[continue_at] != DFA.additional_continue_nodes[continue_at];
    }

    static consteval std::size_t get_start(tdfa::continue_at_t continue_at = tdfa::no_continue, bool use_alt = false)
    {
        if (continue_at == tdfa::no_continue)
        {
            if (use_alt and not never_empty)
                return DFA.additional_continue_nodes.back();
            else
                return DFA.match_start;
        }
        else
        {
            if (use_alt and not never_empty)
                return DFA.additional_continue_nodes[continue_at];
            else
                return DFA.continue_nodes[continue_at];
        }
    }

    using backlink_index_type = typename [: [] consteval {
        const auto size = DFA.nodes.size(); /* replace with size of backlink array array? */
        template for (constexpr const std::meta::info& type : define_static_array(members_of(^^backlink_types, std::meta::access_context::unprivileged())))
            if (size <= static_cast<std::size_t>(std::numeric_limits<typename [: type :]>::max()))
                return type;
       return std::meta::info{};
    }() :];

public:
    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    using result = static_match_results<I, smr_layout::reg_id<DFA.tag_count, DFA.captures>>;

    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    struct iterated_result;

    struct stateful
    {
        static constexpr bool has_continue{ DFA.has_continue() };
        static constexpr bool is_stateless{ not (has_continue /* and other has_x */) };

        using continue_type = maybe_type_t<has_continue, tdfa::continue_at_t>;

        // TODO: add alternative_number when is_regex is true and there is more than one alt
        [[no_unique_address]] continue_type continue_at{ detail::tdfa::no_continue };
    };

    // TODO: depending on pattern length, replace with a circular buffer?
    using backlinks_type = std::vector<backlink_index_type>;

private:
    static constexpr std::ptrdiff_t fallback_disabled{ 0 };

    template<typename I>
    using result_ref = std::add_lvalue_reference_t<result<I>>;

    template<typename I>
    using iterated_result_ref = std::add_lvalue_reference_t<iterated_result<I>>;

    using backlinks_ref = std::add_lvalue_reference_t<backlinks_type>;


    template<typename I, int X = 0>
    struct context
    {
        /* empty context */
    };

    template<typename I>
    struct context<I, 1>
    {
        /* store results */
        result_ref<I> res;
        backlinks_ref arr;

        [[gnu::always_inline]] constexpr auto& get_res() noexcept { return res; }
    };

    template<typename I>
    struct context<I, 2>
    {
        /* store results with iterated search info */
        iterated_result_ref<I> res;
        backlinks_ref arr;

        [[gnu::always_inline]] constexpr auto& get_res() noexcept { return res.res; }
        [[gnu::always_inline]] constexpr auto& get_stf() noexcept { return res.stf; }
    };


    template<typename I>
    context(result<I>&, backlinks_type&) -> context<I, 1>;

    template<typename I>
    context(iterated_result<I>&, backlinks_type&) -> context<I, 2>;


    template<typename I>
    struct fallback_info
    {
        [[no_unique_address]] maybe_type_t<(not DFA.flags.return_bool), I> it;
        std::ptrdiff_t idx{ fallback_disabled };

        constexpr explicit(false) fallback_info(I it) : it{ it } {}
    };

    template<typename I>
    using maybe_fallback_t = maybe_type_t<DFA.flags.enable_fallback, fallback_info<I>>;


    /* backwards pass */

    using bitset_type = std::bitset<DFA.tag_count>;

    template<std::size_t IdxA, std::size_t IdxB, std::bidirectional_iterator I>
    static constexpr tdfa::backlink_index_t assign_tags(result<I>& res, bitset_type& assigned, I it)
    {
        template for (constexpr auto tag : DFA.backlink_arrays[IdxA][IdxB].tag_seq | std::views::reverse)
        {
            if constexpr (tag > 0)
            {
                /* static */ constexpr std::size_t tag_position{ tag - 1 };
                if (not std::as_const(assigned)[tag_position])
                {
                    assigned[tag_position] = true;

                    res.reg_[tag_position] = it;
                    if constexpr (not std::contiguous_iterator<I>)
                        res.enabled_[tag_position] = true;
                }
            }
            else if constexpr (tag < 0)
            {
                static constexpr std::size_t tag_position{ (-tag) - 1 };
                assigned[tag_position] = true;
                /* note: assigned to empty within constructor */
            }
        }

        static constexpr auto prev_index{ DFA.backlink_arrays[IdxA][IdxB].prev_index };
        return prev_index;
    }

    template<std::bidirectional_iterator I>
    static constexpr tdfa::backlink_index_t assign_tags_dispatch(result<I>& res, bitset_type& assigned, I it, backlink_index_type outer, tdfa::backlink_index_t index)
    {
        [[assume(outer < backlink_array_count)]];

        template for (constexpr std::size_t idxa : std::views::indices(backlink_array_count))
        {
            if (outer == idxa)
            {
                static constexpr std::size_t max{ DFA.backlink_arrays[idxa].size() };

                [[assume(index < max)]];

                template for (constexpr std::size_t idxb : std::views::indices(max))
                    if (index == idxb)
                        return assign_tags<idxa, idxb>(res, assigned, it);

                std::unreachable();
            }
        }

        std::unreachable();
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    [[gnu::noinline]] static constexpr bool backwards_pass(context<I, X> ctx, I it, const S /* last */, maybe_fallback_t<I> /* fallback */)
        requires (X != 0)
    {
        bitset_type assigned{};
        tdfa::backlink_index_t index{ 0 };

        while (true)
        {
            index = assign_tags_dispatch(ctx.get_res(), assigned, it, ctx.arr.back(), index);

            ctx.arr.pop_back();

            if ((assigned.all() or ctx.arr.empty()))
                break;

            --it;
        }

        return true;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    [[gnu::always_inline]] static constexpr bool backwards_pass(context<I, 0> /* ctx */, I /* it */, const S /* last */, maybe_fallback_t<I> /* fallback */)
    {
        return true;
    }


    /* final and fallback */

    template<std::size_t Idx, std::ptrdiff_t Offset, std::bidirectional_iterator I, int X>
    static constexpr void set_final(context<I, X> ctx, const I it)
    {
        if constexpr (X != 0)
        {
            ctx.arr.push_back(static_cast<backlink_index_type>(Idx));
            ctx.get_res().match_end_ = std::ranges::prev(it, Offset);

            if constexpr (not std::contiguous_iterator<I>)
                ctx.get_res().match_success_ = true;
        }
    }

    template<std::size_t Idx, std::ptrdiff_t Offset, tdfa::continue_at_t ContinueAt, std::bidirectional_iterator I, int X>
    static constexpr void set_fallback(context<I, X> ctx, const I it)
    {
        if constexpr (X != 0)
        {
            set_final<Idx, Offset>(ctx, it);

            if constexpr (X != 1 and stateful::has_continue and ContinueAt != tdfa::no_continue)
                ctx.get_stf().continue_at = ContinueAt;
        }
    }


    /* state fallback */

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
        requires (DFA.flags.enable_fallback)
    static constexpr bool fallback_state(context<I, X> ctx, I it, const S last, fallback_info<I> fallback)
    {
        static_assert(fallback_disabled == 0);

        if (fallback.idx == fallback_disabled)
            return false;

        if constexpr (X == 0)
        {
            return true;
        }
        else
        {
            std::ptrdiff_t to_erase{ std::ranges::distance(fallback.it, it) };

            /* this is always true since we only ever call this function
               when the current state itself is not a fallback state */
            [[assume(to_erase > 0)]];

            while (to_erase-- > 0)
                ctx.arr.pop_back();

            it = fallback.it;

            [[assume(fallback.idx <= static_cast<std::ptrdiff_t>(DFA.fallback_nodes.size()))]];
            [[assume(fallback.idx > 0)]];

            template for (constexpr auto i : std::views::indices(DFA.fallback_nodes.size()))
            {
                static constexpr auto& [state, fbni] = DFA.fallback_nodes.begin()[i];
                static constexpr std::ptrdiff_t idx{ 1 + static_cast<std::ptrdiff_t>(i) };

                if (fallback.idx == idx)
                {
                    static constexpr auto& fni = DFA.final_nodes.at(state);
                    set_fallback<fbni.op_index, fni.offset, fbni.continue_at>(ctx, it);
                    [[clang::musttail]] return backwards_pass(ctx, it, last, fallback);
                }
            }

            std::unreachable();
        }
    }


    /* next state functions */

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    static constexpr bool state(context<I, X> ctx, I it, const S last, maybe_fallback_t<I> fallback)
    {
        static constexpr auto fb_it = DFA.fallback_nodes.find(DFAState);
        static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState);
        static constexpr auto* fallback_node = (fb_it != DFA.fallback_nodes.end()) ? &(fb_it->second) : nullptr;

        if constexpr (DFA.flags.enable_fallback and fallback_node != nullptr)
        {
            static constexpr std::ptrdiff_t index{ 1 + std::ranges::distance(DFA.fallback_nodes.begin(), fb_it) };

            fallback.idx = index;
            if constexpr (not DFA.flags.return_bool)
                fallback.it = it;
        }

        if (it != last)
        {
            template for (constexpr auto& tr : DFA.nodes[DFAState])
            {
                if (tr_possible<tr.cs>(*it))
                {
                    if constexpr (X != 0)
                        ctx.arr.push_back(static_cast<backlink_index_type>(tr.op_index));
                    [[clang::musttail]] return state<tr.next>(ctx, ++it, last, fallback);
                }
            }
        }
        else
        {
            if constexpr (final_node != nullptr and not (DFA.flags.enable_fallback and fallback_node != nullptr))
            {
                set_final<final_node->op_index, final_node->offset>(ctx, it);
                [[clang::musttail]] return backwards_pass(ctx, it, last, fallback);
            }
        }

        if constexpr (final_node != nullptr and (DFA.flags.enable_fallback and fallback_node != nullptr))
        {
            set_fallback<final_node->op_index, final_node->offset, fallback_node->continue_at>(ctx, it);
            [[clang::musttail]] return backwards_pass(ctx, it, last, fallback);
        }

        if constexpr (DFA.flags.enable_fallback and fallback_node == nullptr)
            [[clang::musttail]] return fallback_state(ctx, it, last, fallback);
        else
            return false;
    }

    template<std::size_t DFAState, std::bidirectional_iterator I, int X>
    static constexpr bool state(context<I, X> ctx, I it, const cstr_sentinel_t last, maybe_fallback_t<I> fallback)
    {
        static constexpr auto fb_it = DFA.fallback_nodes.find(DFAState);
        static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState);
        static constexpr auto* fallback_node = (fb_it != DFA.fallback_nodes.end()) ? &(fb_it->second) : nullptr;

        if constexpr (DFA.flags.enable_fallback and fallback_node != nullptr)
        {
            static constexpr std::ptrdiff_t index{ 1 + std::ranges::distance(DFA.fallback_nodes.begin(), fb_it) };

            fallback.idx = index;
            if constexpr (not DFA.flags.return_bool)
                fallback.it = it;
        }

        template for (constexpr auto& tr : DFA.nodes[DFAState])
        {
            if (tr_possible_exclude_null<tr.cs>(*it))
            {
                if constexpr (X != 0)
                    ctx.arr.push_back(static_cast<backlink_index_type>(tr.op_index));
                [[clang::musttail]] return state<tr.next>(ctx, ++it, last, fallback);
            }
        }

        if constexpr (final_node != nullptr)
        {
            if constexpr (DFA.flags.enable_fallback and fallback_node != nullptr)
            {
                set_fallback<final_node->op_index, final_node->offset, fallback_node->continue_at>(ctx, it);
                [[clang::musttail]] return backwards_pass(ctx, it, last, fallback);
            }
            else
            {
                if (it == last) [[likely]]
                {
                    set_final<final_node->op_index, final_node->offset>(ctx, it);
                    [[clang::musttail]] return backwards_pass(ctx, it, last, fallback);
                }
            }
        }

        if constexpr (DFA.flags.enable_fallback and fallback_node == nullptr)
            [[clang::musttail]] return fallback_state(ctx, it, last, fallback);
        else
            return false;
    }

    template<std::size_t DFAState, std::size_t Count, std::bidirectional_iterator I, std::sized_sentinel_for<I> S, int X>
    static constexpr bool unchecked_state(context<I, X> ctx, I it, const S last, maybe_fallback_t<I> fallback)
    {
        if constexpr (Count == 0)
        {
            [[clang::musttail]] return state<DFAState>(ctx, it, last, fallback);
        }
        else
        {
            template for (constexpr auto& tr : DFA.nodes[DFAState])
            {
                if (tr_possible<tr.cs>(*it))
                {
                    if constexpr (X != 0)
                        ctx.arr.push_back(static_cast<backlink_index_type>(tr.op_index));
                    [[clang::musttail]] return unchecked_state<tr.next, Count - 1>(ctx, ++it, last, fallback);
                }
            }

            return false;
        }
    }

    /* next state function entry point */

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    static constexpr bool initial_state(context<I, X> ctx, I it, const S last, maybe_fallback_t<I> fallback)
    {
        static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

        if constexpr (not std::sized_sentinel_for<S, I> or (DFA.flags.is_search and not DFA.flags.adapted_search))
        {
            /* note: searches are very likely to exceed min_length, so prefer reduced code duplication instead */
            [[clang::musttail]] return state<DFAState>(ctx, it, last, fallback);
        }
        else if constexpr (fixed_length and not DFA.flags.enable_fallback)
        {
            if (std::ranges::distance(it, last) == min_length)
            {
                [[clang::musttail]] return unchecked_state<DFAState, min_length>(ctx, it, last, fallback);
            }
        }
        else
        {
            if (std::ranges::distance(it, last) >= min_length)
            {
                [[clang::musttail]] return unchecked_state<DFAState, min_length>(ctx, it, last, fallback);
            }
        }

        return false;
    }

public:
    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires (DFA.flags.return_bool and stateful::is_stateless)
    [[nodiscard]] static constexpr bool operator()(const I first, const S last)
    {
        static_assert(can_tailcall_with<I> and can_tailcall_with<S>, "Iterator is not useable in tail calls");

        return initial_state<DFA.match_start>(context<I, 0>{}, first, last, first);
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires (not DFA.flags.return_bool and stateful::is_stateless)
    [[nodiscard]] static constexpr result<I> operator()(const I first, const S last)
    {
        static_assert(can_tailcall_with<I> and can_tailcall_with<S>, "Iterator is not useable in tail calls");

        result<I> res{ first };
        backlinks_type backlinks{};

        initial_state<DFA.match_start>(context{ res, backlinks }, first, last, first);
        return res;
    }
};


template<std::meta::info Info>
template<std::bidirectional_iterator I>
    requires std::is_nothrow_convertible_v<std::iter_value_t<I>, typename p1306dfb<Info>::char_type>
struct p1306dfb<Info>::iterated_result
{
    static constexpr bool needs_begin{ not never_empty };
    using state_type = stateful;

    iterated_result() = default;

    template<std::sentinel_for<I> S>
    constexpr iterated_result(const I first, const S last)
        : res{ first }
    {
        static_assert(can_tailcall_with<I> and can_tailcall_with<S>, "Iterator is not useable in tail calls");

        start(first, last);
    }

    template<std::sentinel_for<I> S>
    constexpr iterated_result(const I first, const S last, match_non_empty_t)
        : res{ first }
    {
        static_assert(can_tailcall_with<I> and can_tailcall_with<S>, "Iterator is not useable in tail calls");

        start<true>(first, last);
    }

    template<std::sentinel_for<I> S>
    constexpr iterated_result(const I /* first */, const I it, const S last, stateful stf, bool prev_empty)
        : res{ it }, stf{ stf }
    {
        static_assert(can_tailcall_with<I> and can_tailcall_with<S>, "Iterator is not useable in tail calls");

        if constexpr (not never_empty)
        {
            if (prev_empty)
            {
                resume<true>(it, last);
                return;
            }
        }
        resume(it, last);
    }

    template<std::sentinel_for<I> S>
    constexpr I advance(const S last)
        requires never_empty
    {
        static_assert(can_tailcall_with<I> and can_tailcall_with<S>, "Iterator is not useable in tail calls");

        const auto& match = res.template force_get<0>();
        const I current = match.end();
        res.reset(current);
        resume(current, last);
        return current;
    }

    template<std::sentinel_for<I> S>
    constexpr I advance(const I first, const S last)
    {
        static_assert(can_tailcall_with<I> and can_tailcall_with<S>, "Iterator is not useable in tail calls");

        const auto& match = res.template force_get<0>();
        const I current = match.end();

        if constexpr (not never_empty)
        {
            if (match.empty())
            {
                if (current == last)
                {
                    res.clear_match();
                    return current;
                }

                res.reset(current);
                if (current == first)
                    start<true>(current, last);
                else
                    resume<true>(current, last);
                return current;
            }
        }

        res.reset(current);
        resume(current, last);
        return current;
    }


private:
    template<bool NonEmptyMatch = false>
    constexpr void start(const I first, const std::sentinel_for<I> auto last)
    {
        backlinks_type backlinks{};

        initial_state<get_start(tdfa::no_continue, NonEmptyMatch)>(context{ *this, backlinks }, first, last, first);
    }

    template<bool NonEmptyMatch = false>
    constexpr void resume(const I first, const std::sentinel_for<I> auto last)
    {
        if constexpr (not stateful::has_continue)
        {
            return start<NonEmptyMatch>(first, last);
        }
        else
        {
            const stateful state_info{ std::exchange(stf, {}) };
            backlinks_type backlinks{};

            template for (constexpr std::size_t i : std::views::indices(DFA.continue_nodes.size()))
            {
                if (state_info.continue_at == i)
                {

                    initial_state<get_start(i, NonEmptyMatch)>(context{ *this, backlinks }, first, last, first);
#ifndef __GNUC_MINOR__
                    break;
#else
                    return; // temporary workaround for GCC bug
#endif
                }
            }

            return;
        }
    }

public:
    result<I> res;
    stateful stf;
};

template<string_literal Pattern, fsm_flags Flags>
using p1306_multipass = p1306dfb<(^^rf<std::meta::reflect_constant_string(Pattern.view()), pack_flags(Flags)>)>;

} // namespace detail
} // namespace srx
