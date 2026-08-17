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
    static constexpr bool enable_backlinks{ not DFA.flags.return_bool and DFA.tag_count > 0 };
    static constexpr bool finite_length{ DFA.backlink_buf_size != std::numeric_limits<std::size_t>::max() };

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

    using backlink_index_type = [: smallest_integer_type(backlink_array_count) :];

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

    using backlink_buffer_type = std::conditional_t<finite_length, backlink_hist<backlink_index_type, std::bit_width(DFA.backlink_buf_size)>, std::vector<backlink_index_type>>;


private:
    static constexpr backlink_index_type invalid_backlink_index{ std::numeric_limits<backlink_index_type>::max() };

    using blbuf_t = maybe_type_t<enable_backlinks, backlink_buffer_type&>;
    using blidx_t = maybe_type_t<enable_backlinks, backlink_index_type>;

    template<typename I>
    using result_ref = std::add_lvalue_reference_t<result<I>>;

    template<typename I>
    using iterated_result_ref = std::add_lvalue_reference_t<iterated_result<I>>;


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

        [[gnu::always_inline]] constexpr auto& get_res() noexcept { return res; }
    };

    template<typename I>
    struct context<I, 2>
    {
        /* store results with iterated search info */
        iterated_result_ref<I> res;

        [[gnu::always_inline]] constexpr auto& get_res() noexcept { return res.res; }
        [[gnu::always_inline]] constexpr auto& get_stf() noexcept { return res.stf; }
    };


    template<typename I>
    context(result<I>&) -> context<I, 1>;

    template<typename I>
    context(iterated_result<I>&) -> context<I, 2>;

    template<typename I>
    struct fallback_info
    {
        [[no_unique_address]] maybe_type_t<(not DFA.flags.return_bool), I> it;
        std::ptrdiff_t idx{ fallback_disabled };

        constexpr explicit(false) fallback_info(I it) : it{ it } {}
    };

    static constexpr std::ptrdiff_t fallback_disabled{ 0 };

    template<typename I>
    using mfb_t = maybe_type_t<DFA.flags.enable_fallback, fallback_info<I>>;


    /* backwards pass */

    using assigned_t = std::bitset<DFA.tag_count>;

    template<std::size_t IdxA, std::size_t IdxB, std::bidirectional_iterator I, int X>
    static constexpr std::size_t assign_tags(context<I, X> ctx, assigned_t& assigned, I it)
    {
        template for (constexpr auto tag : DFA.backlink_arrays[IdxA][IdxB].tag_seq | std::views::reverse)
        {
            if constexpr (tag > 0)
            {
                /* static */ constexpr std::size_t tag_position{ tag - 1 };
                if (not std::as_const(assigned)[tag_position])
                {
                    assigned[tag_position] = true;

                    ctx.get_res().reg_[tag_position] = it;
                    if constexpr (not std::contiguous_iterator<I>)
                        ctx.get_res().enabled_[tag_position] = true;
                }
            }
            else if constexpr (tag < 0)
            {
                static constexpr std::size_t tag_position{ (-tag) - 1 };
                assigned[tag_position] = true;
                /* note: assigned to empty within constructor */
            }
        }

        static constexpr std::size_t prev_index{ DFA.backlink_arrays[IdxA][IdxB].prev_index };
        return prev_index;
    }

    template<std::bidirectional_iterator I, int X>
    static constexpr std::size_t assign_tags_dispatch(context<I, X> res, assigned_t& assigned, I it, blidx_t outer, std::size_t index)
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
        requires (X != 0 and enable_backlinks)
    [[gnu::noinline]]
    static constexpr bool backwards_pass(blbuf_t arr, blidx_t /* idx */, I it, const S /* last */, mfb_t<I> /* fb */, context<I, X> ctx)
    {
        assigned_t assigned{};
        std::size_t index{ 0 };

        [[assume(arr.size() > 0)]];

        do // NOLINT
        {
            index = assign_tags_dispatch(ctx, assigned, it, arr.back(), index);
            arr.pop_back();
            --it;
        }
        while (not assigned.all());

        return true;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    [[gnu::always_inline]] static constexpr bool backwards_pass(blbuf_t, blidx_t, I, const S, mfb_t<I>, context<I, X>)
    {
        return true;
    }


    /* final and fallback */

    template<std::size_t Idx, std::ptrdiff_t Offset, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    static constexpr void set_final(blbuf_t arr, context<I, X> ctx, const I it)
    {
        if constexpr (X != 0)
        {
            if constexpr (enable_backlinks)
            {
                if constexpr (not finite_length and std::sized_sentinel_for<S, I>)
                {
                    [[assume(arr.size() < arr.capacity())]];
                    arr.push_back(static_cast<backlink_index_type>(Idx));
                }
                else
                {
                    arr.push_back(static_cast<backlink_index_type>(Idx));
                }
            }


            ctx.get_res().match_end_ = std::ranges::prev(it, Offset);

            if constexpr (not std::contiguous_iterator<I>)
                ctx.get_res().match_success_ = true;
        }
    }

    template<std::size_t Idx, std::ptrdiff_t Offset, tdfa::continue_at_t ContinueAt, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    static constexpr void set_fallback(blbuf_t arr, context<I, X> ctx, const I it)
    {
        if constexpr (X != 0)
        {
            set_final<Idx, Offset, I, S>(arr, ctx, it);

            if constexpr (X != 1 and stateful::has_continue and ContinueAt != tdfa::no_continue)
                ctx.get_stf().continue_at = ContinueAt;
        }
    }


    /* state fallback / accept */

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
        requires (DFA.flags.enable_fallback)
    [[gnu::noinline]]
    static constexpr bool fallback_state(blbuf_t arr, blidx_t idx, I it, const S last, mfb_t<I> fb, context<I, X> ctx)
    {
        static_assert(fallback_disabled == 0);

        if (fb.idx == fallback_disabled)
            return false;

        if constexpr (X == 0)
        {
            return true;
        }
        else
        {
            if constexpr (enable_backlinks)
            {
                std::ptrdiff_t to_erase{ std::ranges::distance(fb.it, it) };

                /* this is always true since we only ever call this function
                when the current state itself is not a fallback state */
                [[assume(to_erase > 0)]];

                while (to_erase-- > 0)
                    arr.pop_back();
            }

            it = fb.it;

            [[assume(fb.idx <= static_cast<std::ptrdiff_t>(DFA.fallback_nodes.size()))]];
            [[assume(fb.idx > 0)]];

            template for (constexpr auto i : std::views::indices(DFA.fallback_nodes.size()))
            {
                static constexpr auto& [state, fbni] = DFA.fallback_nodes.begin()[i];
                static constexpr std::ptrdiff_t fallback_idx{ 1 + static_cast<std::ptrdiff_t>(i) };

                if (fb.idx == fallback_idx)
                {
                    static constexpr auto& fni = DFA.final_nodes.at(state);
                    set_fallback<fbni.op_index, fni.offset, fbni.continue_at, I, S>(arr, ctx, it);
                    [[clang::musttail]] return backwards_pass(arr, idx, it, last, fb, ctx);
                }
            }

            std::unreachable();
        }
    }

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    [[gnu::noinline]]
    static constexpr bool accepting_state(blbuf_t arr, blidx_t idx, I it, const S last, mfb_t<I> fb, context<I, X> ctx)
    {
        static constexpr auto& final_node = DFA.final_nodes.at(DFAState);
        static constexpr auto* fallback_node = DFA.fallback_nodes.at_if(DFAState);


        if constexpr (DFA.flags.enable_fallback and fallback_node != nullptr)
        {
            set_fallback<final_node.op_index, final_node.offset, fallback_node->continue_at, I, S>(arr, ctx, it);
            [[clang::musttail]] return backwards_pass(arr, idx, it, last, fb, ctx);
        }
        else
        {
            if (it == last)
            {
                set_final<final_node.op_index, final_node.offset, I, S>(arr, ctx, it);
                [[clang::musttail]] return backwards_pass(arr, idx, it, last, fb, ctx);
            }

            if constexpr (DFA.flags.enable_fallback)
                [[clang::musttail]] return fallback_state(arr, idx, it, last, fb, ctx);
            else
                return false;
        }

    }

    /* next state functions */

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    [[gnu::noinline]]
    static constexpr bool forwards_pass(blbuf_t arr, blidx_t idx, I it, const S last, mfb_t<I> fb, context<I, X> ctx)
    {
        static constexpr auto fb_it = DFA.fallback_nodes.find(DFAState);
        static constexpr bool is_final{ DFA.final_nodes.contains(DFAState) };
        static constexpr bool is_fallback{ fb_it != DFA.fallback_nodes.end() };

        ++it;
        if constexpr (enable_backlinks)
        {
            if constexpr (not finite_length and std::sized_sentinel_for<S, I>)
            {
                [[assume(arr.size() < arr.capacity())]];
                arr.push_back(idx);
            }
            else
            {
                arr.push_back(idx);
            }
        }


        if constexpr (DFA.flags.enable_fallback and is_fallback)
        {
            static constexpr std::ptrdiff_t index{ 1 + std::ranges::distance(DFA.fallback_nodes.begin(), fb_it) };

            fb.idx = index;
            if constexpr (not DFA.flags.return_bool)
                fb.it = it;
        }

        if (it != last) [[likely]]
        {
            template for (constexpr auto& tr : DFA.nodes[DFAState])
            {
                static constexpr blidx_t new_idx{ static_cast<backlink_index_type>(tr.op_index) };
                if (tr_possible<tr.cs>(*it))
                    [[clang::musttail]] return forwards_pass<tr.next>(arr, new_idx, it, last, fb, ctx);
            }
        }

        if constexpr (is_final)
            [[clang::musttail]] return accepting_state<DFAState>(arr, idx, it, last, fb, ctx);
        else if constexpr (DFA.flags.enable_fallback)
            [[clang::musttail]] return fallback_state(arr, idx, it, last, fb, ctx);
        else
            return false;
    }

    /* initial state */

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    [[gnu::noinline]]
    static constexpr bool initial_state(blbuf_t arr, blidx_t idx, I it, const S last, mfb_t<I> fb, context<I, X> ctx)
    {
        static constexpr auto fb_it = DFA.fallback_nodes.find(DFAState);
        static constexpr bool is_final{ DFA.final_nodes.contains(DFAState) };
        static constexpr bool is_fallback{ fb_it != DFA.fallback_nodes.end() };

        if constexpr (DFA.flags.enable_fallback and is_fallback)
        {
            static constexpr std::ptrdiff_t index{ 1 + std::ranges::distance(DFA.fallback_nodes.begin(), fb_it) };

            fb.idx = index;
            if constexpr (not DFA.flags.return_bool)
                fb.it = it;
        }

        if (it != last) [[likely]]
        {
            template for (constexpr auto& tr : DFA.nodes[DFAState])
            {
                static constexpr blidx_t new_idx{ static_cast<backlink_index_type>(tr.op_index) };
                if (tr_possible<tr.cs>(*it))
                    [[clang::musttail]] return forwards_pass<tr.next>(arr, new_idx, it, last, fb, ctx);
            }
        }

        if constexpr (is_final)
            [[clang::musttail]] return accepting_state<DFAState>(arr, idx, it, last, fb, ctx);
        else if constexpr (DFA.flags.enable_fallback)
            [[clang::musttail]] return fallback_state(arr, idx, it, last, fb, ctx);
        else
            return false;
    }

public:
    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires (DFA.flags.return_bool and stateful::is_stateless)
    [[nodiscard]] static constexpr bool operator()(const I first, const S last)
    {
        static_assert(can_tailcall_with<I> and can_tailcall_with<S>, "Iterator is not useable in tail calls");

        return initial_state<DFA.match_start>(blbuf_t{}, invalid_backlink_index, first, last, first, context<I, 0>{} );
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires (not DFA.flags.return_bool and stateful::is_stateless)
    [[nodiscard]] static constexpr result<I> operator()(const I first, const S last)
    {
        static_assert(can_tailcall_with<I> and can_tailcall_with<S>, "Iterator is not useable in tail calls");

        result<I> res{ first };
        backlink_buffer_type backlinks{};

        if constexpr (not finite_length and std::sized_sentinel_for<S, I>)
            backlinks.reserve(1 + std::ranges::distance(first, last));

        initial_state<DFA.match_start>(backlinks, invalid_backlink_index, first, last, first, context{ res });
        return res;
    }
};


template<std::meta::info Info>
template<std::bidirectional_iterator I>
    requires std::is_nothrow_convertible_v<std::iter_value_t<I>, typename p1306dfb<Info>::char_type>
struct p1306dfb<Info>::iterated_result
{
    static constexpr bool needs_begin{ not never_empty };
    using continue_type = stateful::continue_type;

    iterated_result() = default;

    template<std::sentinel_for<I> S>
    constexpr iterated_result(const I first, const S last)
        : res{ first }
    {
        static_assert(can_tailcall_with<I> and can_tailcall_with<S>, "Iterator is not useable in tail calls");

        start<S>(first, last);
    }

    template<std::sentinel_for<I> S>
    constexpr iterated_result(const I first, const S last, match_non_empty_t)
        : res{ first }
    {
        static_assert(can_tailcall_with<I> and can_tailcall_with<S>, "Iterator is not useable in tail calls");

        start<S, true>(first, last);
    }

    template<std::sentinel_for<I> S>
    constexpr iterated_result(const I /* first */, const I it, const S last, continue_type cont, bool prev_empty)
        : res{ it }, stf{ .continue_at = cont }
    {
        static_assert(can_tailcall_with<I> and can_tailcall_with<S>, "Iterator is not useable in tail calls");

        if constexpr (not never_empty)
        {
            if (prev_empty)
            {
                resume<S, true>(it, last);
                return;
            }
        }
        resume<S>(it, last);
    }

    constexpr operator bool() const noexcept
    {
        return res.has_value();
    }

    constexpr const result<I>& operator*() const noexcept
    {
        return res;
    }

    constexpr continue_type get_continue() const noexcept
    {
        return stf.continue_at;
    }

    template<std::sentinel_for<I> S>
    constexpr I advance(const S last)
        requires never_empty
    {
        static_assert(can_tailcall_with<I> and can_tailcall_with<S>, "Iterator is not useable in tail calls");

        const auto& match = res.template force_get<0>();
        const I current = match.end();
        res.reset(current);
        resume<S>(current, last);
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
                    start<S, true>(current, last);
                else
                    resume<S, true>(current, last);
                return current;
            }
        }

        res.reset(current);
        resume<S>(current, last);
        return current;
    }

    friend p1306dfb<Info>::context<I, 2>;

private:
    template<std::sentinel_for<I> S, bool NonEmptyMatch = false>
    constexpr void start(const I first, const S last)
    {
        backlink_buffer_type backlinks{};

        if constexpr (not finite_length and std::sized_sentinel_for<S, I>)
            backlinks.reserve(1 + std::ranges::distance(first, last));

        initial_state<get_start(tdfa::no_continue, NonEmptyMatch)>(backlinks, invalid_backlink_index, first, last, first, context{ *this });
    }

    template<std::sentinel_for<I> S, bool NonEmptyMatch = false>
    constexpr void resume(const I first, const S last)
    {
        if constexpr (not stateful::has_continue)
        {
            start<S, NonEmptyMatch>(first, last);
        }
        else
        {
            const stateful state_info{ std::exchange(stf, {}) };
            backlink_buffer_type backlinks{};

            if constexpr (not finite_length and std::sized_sentinel_for<S, I>)
                backlinks.reserve(1 + std::ranges::distance(first, last));

            template for (constexpr std::size_t i : std::views::indices(DFA.continue_nodes.size()))
            {
                if (state_info.continue_at == i)
                {
                    initial_state<get_start(i, NonEmptyMatch)>(backlinks, invalid_backlink_index, first, last, first, context{ *this });
                    return;
                }
            }
        }
    }

    result<I> res;
    stateful stf;
};

template<string_literal Pattern, fsm_flags Flags>
using p1306_multipass = p1306dfb<(^^rf<std::meta::reflect_constant_string(Pattern.view()), pack_flags(Flags)>)>;

} // namespace detail
} // namespace srx
