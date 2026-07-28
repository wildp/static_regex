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
#include <simd>
#include <type_traits>

#include "srx/etc/static_charset.hpp"
#include "srx/etc/string_literal.hpp"
#include "srx/etc/util.hpp"
#include "srx/fsm/flags.hpp"
#include "srx/fsm/tdfa.hpp"
#include "srx/gen/compile.hpp"
#include "srx/gen/result.hpp"


namespace srx {
namespace detail {

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
#if false
    /* simple code path */
    // TODO: change to use structured binding when supported
    template for (constexpr auto pair : Sc.get_intervals())
        if (pair.first <= c and c <= pair.second)
            return true;
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

        template for (constexpr auto ote : define_static_array(make_optimised_edges(Sc, inverted)))
        {
            uchar_type d{ c };

            if constexpr (ote.msk != ~0u)
                d &= ote.msk;

            if constexpr (ote.rng == 0u)
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

        template for (constexpr auto ote : define_static_array(make_optimised_edges(Sc, inverted)))
        {
            auto d_vec{ c_vec };

            if constexpr (ote.msk != ~0u)
            {
                static constexpr vec_type mask_vec{ ote.msk };
                d_vec &= mask_vec;
            }

            if constexpr (ote.rng == 0u)
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


template<std::meta::info Action, typename ResultType>
static constexpr auto exec_lexer_action(const ResultType& res)
{
    static_assert(Action != std::meta::info{});

    if constexpr (is_enumerator(Action))
    {
        return [: Action :];
    }
    if constexpr (is_value(Action) or is_object(Action) or is_variable(Action))
    {
        // TODO: require non-automatic storage duration for is_variable?

        if constexpr (is_invocable_type(type_of(Action), {}))
            return [: Action :]();
        else if constexpr (is_invocable_type(type_of(Action), { type_of(^^res) }))
            return [: Action :](res);
        else
            return [: Action :];
    }
    else if constexpr (is_type(Action))
    {
        using action_type = [: Action :];

        if constexpr (dealias(Action) == ^^void)
            return; /* skip lexer action */
        else if constexpr (not is_default_constructible_type(Action))
            static_assert(false, "Invalid lexer action: type is not default-constructible");
        else if constexpr (is_invocable_type(Action, {}))
            return action_type{}();
        else if constexpr (is_invocable_type(Action, { type_of(^^res) }))
            return action_type{}(res);
        else
            return action_type{};
    }
    else if constexpr (is_function(Action) or is_function_template(Action))
    {
        static_assert(false, "Unimplemented!");
    }
    else
    {
        static_assert(false, "Invalid lexer action");
    }
}


template<std::meta::info Info>
struct p1306dfa
{
    static_assert(has_template_arguments(type_of(Info)), "Invalid reflection value");
    using char_type = typename [: template_arguments_of(type_of(Info))[0] :];

    static constexpr bool is_regex{ template_of(type_of(Info)) == ^^tdfa_info };
    static constexpr bool is_lexer{ template_of(type_of(Info)) == ^^lexer_info };
    static_assert(is_regex != is_lexer);

private:
    static constexpr tdfa_info<char_type> DFA = [] consteval {
        if constexpr (is_regex)
            return [: Info :];
        else if constexpr (is_lexer)
            return [: Info :].dfa;
        else
            static_assert(false, "Invalid reflection value");
    }();

    static constexpr bool never_empty{ DFA.additional_continue_nodes.empty() };
    static constexpr bool fixed_length{ DFA.min_max_lengths.first != std::numeric_limits<std::size_t>::max()
                                        and DFA.min_max_lengths.first == DFA.min_max_lengths.second };
    static constexpr bool branch_free{ std::ranges::all_of(DFA.nodes, [](const auto& n){ return n.size() <= 1; }) };

public:
    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    using result = static_match_results<I, DFA.make_match_result_info()>;

    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    using token = typename [: [] consteval {
        if constexpr (not is_lexer)
            return ^^terminal_object;
        else
            return [: Info :].return_type;
    }() :];

    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    struct iterated_result;

    struct stateful
    {
        static constexpr bool has_continue{ DFA.has_continue() };
        static constexpr bool is_stateless{ not (has_continue /* and other has_x */) };

        using continue_type = maybe_type_t<has_continue, tdfa::continue_at_t>;

        // TODO: add simd vec mask?
        [[no_unique_address]] continue_type continue_at{ detail::tdfa::no_continue };
    };

private:
    static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };
    static constexpr std::size_t avoid_simd_threshold{ (1uz << std::numeric_limits<std::make_unsigned_t<char_type>>::digits) / 2 };

    struct generation
    {
        static constexpr bool enabled{ DFA.flags.adapted_search and DFA.register_count > 0 };

        std::array<std::size_t, DFA.register_count> reg{};
        std::size_t current{ 0 };
    };

    using gen_info = maybe_type_t<generation::enabled, generation>;


    template<typename I, bool B = false>
    struct overspill
    {
        // TODO: move overspill out of result<I>
        [[no_unique_address]] gen_info gen;

        overspill() = default;
    };

    template<typename I>
    struct overspill<I, true>
    {
        result<I> res;
        // TODO: move overspill out of result<I>
        [[no_unique_address]] gen_info gen;
        [[no_unique_address]] stateful stf;

        overspill() = default;
        constexpr overspill(I first) noexcept(noexcept(result<I>{ first })) : res{ first } {}
    };


    template<typename I>
    using token_ref = maybe_type_t<is_lexer, std::add_lvalue_reference_t<token<I>>>;

    template<typename I>
    using result_ref = std::add_lvalue_reference_t<result<I>>;

    template<typename I>
    using iterated_result_ref = std::add_lvalue_reference_t<iterated_result<I>>;

    template<typename I, bool B = false>
    using overspill_ref = std::add_lvalue_reference_t<overspill<I, B>>;


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
        [[no_unique_address]] overspill_ref<I> osr;

        [[gnu::always_inline]] constexpr auto& get_res() noexcept { return res; }
    };

    template<typename I>
    struct context<I, 2>
    {
        /* store results with iterated search info */
        iterated_result_ref<I> itr;
        [[no_unique_address]] overspill_ref<I> osr;

        [[gnu::always_inline]] constexpr auto& get_res() noexcept { return itr.res; }
        [[gnu::always_inline]] constexpr auto& get_stf() noexcept { return itr.stf; }
    };

    template<typename I>
    struct context<I, 3>
    {
        /* store token, with result in overspill only */
        token_ref<I> tok;
        overspill_ref<I, true> osr;

        [[gnu::always_inline]] constexpr auto& get_res() noexcept { return osr.res; }
        [[gnu::always_inline]] constexpr auto& get_stf() noexcept { return osr.stf; }
    };

    template<typename I>
    context(result<I>&, overspill<I>&) -> context<I, 1>;

    template<typename I>
    context(iterated_result<I>&, overspill<I>&) -> context<I, 2>;

    template<typename I>
    context(token<I>&, overspill<I, true>&) -> context<I, 3>;


    template<typename I>
    struct fallback_info
    {
        [[no_unique_address]] maybe_type_t<(not DFA.flags.return_bool), I> it;
        std::size_t state{ fallback_disabled };

        constexpr explicit(false) fallback_info(I it) : it{ it } {}
    };

    template<typename I>
    using maybe_fallback_t = maybe_type_t<DFA.flags.enable_fallback, fallback_info<I>>;


    /* post-match register cleaning */

    template<std::bidirectional_iterator I>
    static constexpr void clean_generations(result<I>& res, const overspill<I, false>& osp)
        requires generation::enabled
    {
        if constexpr (std::contiguous_iterator<I>)
        {
            for (std::size_t i{ 0 }; i < DFA.register_count; ++i)
                res.reg_[i] = (osp.gen.reg[i] == osp.gen.current) ? res.reg_[i] : I{};
        }
        else
        {
            for (std::size_t i{ 0 }; i < DFA.register_count; ++i)
                res.enabled_[i] = (osp.gen.reg[i] == osp.gen.current);
        }
    }

    template<std::bidirectional_iterator I>
    static constexpr void clean_generations(overspill<I, true>& osp)
        requires generation::enabled
    {
        if constexpr (std::contiguous_iterator<I>)
        {
            for (std::size_t i{ 0 }; i < DFA.register_count; ++i)
                osp.res.reg_[i] = (osp.gen.reg[i] == osp.gen.current) ? osp.res.reg_[i] : I{};
        }
        else
        {
            for (std::size_t i{ 0 }; i < DFA.register_count; ++i)
                osp.res.enabled_[i] = (osp.gen.reg[i] == osp.gen.current);
        }
    }


    /* register operations */

    template<std::size_t Blk, std::bidirectional_iterator I, int X>
    static constexpr void register_operations(context<I, X> ctx, const I it)
    {
        if constexpr (Blk != tdfa::no_transition_regops and X != 0)
        {
            template for (constexpr register_operation op : DFA.regops[Blk])
            {
                if constexpr (op.is_copy)
                    ctx.get_res().reg_[op.dst] = ctx.get_res().reg_[op.cpy_src];
                else if constexpr (op.set_val)
                    ctx.get_res().reg_[op.dst] = it;
                else if constexpr (not generation::enabled and std::contiguous_iterator<I>)
                    ctx.get_res().reg_[op.dst] = I{};

                if constexpr (not generation::enabled and not std::contiguous_iterator<I>)
                {
                    if constexpr (op.is_copy)
                        ctx.get_res().enabled_[op.dst] = ctx.get_res().enabled_[op.cpy_src];
                    else
                        ctx.get_res().enabled_[op.dst] = op.set_val;
                }
            }

            if constexpr (generation::enabled)
            {
                template for (constexpr register_operation op : DFA.regops[Blk])
                {
                    if constexpr (op.is_copy)
                        ctx.osr.gen.reg[op.dst] = ctx.osr.gen.reg[op.cpy_src];
                    else if constexpr (op.set_val)
                        ctx.osr.gen.reg[op.dst] = ctx.osr.gen.current;
                    else
                        ctx.osr.gen.reg[op.dst] = 0;
                }
            }
        }
    }


    /* final and fallback */

    template<std::size_t Blk, std::ptrdiff_t Offset, std::bidirectional_iterator I, int X>
    static constexpr void set_final(context<I, X> ctx, const I it)
    {
        if constexpr (X != 0)
        {
            register_operations<Blk>(ctx, it);
            ctx.get_res().match_end_ = std::ranges::prev(it, Offset);

            if constexpr (not std::contiguous_iterator<I>)
                ctx.get_res().match_success_ = true;
        }
    }

    template<std::size_t Blk, std::ptrdiff_t Offset, tdfa::continue_at_t ContinueAt, std::bidirectional_iterator I, int X>
    static constexpr void set_fallback(context<I, X> ctx, const I it)
    {
        if constexpr (X != 0)
        {
            set_final<Blk, Offset>(ctx, it);

            if constexpr (stateful::has_continue and ContinueAt != tdfa::no_continue)
            {
                if constexpr (X == 1)
                    ctx.res.continue_at_ = ContinueAt; // temporary; TODO: remove
                else
                    ctx.get_stf().continue_at = ContinueAt;
            }
        }
    }


    /* lexer actions */

    template<std::meta::info Action, std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr bool lexer_action(context<I, 3> ctx, I it, const S end, maybe_fallback_t<I> fallback)
    {
        static constexpr auto func{ substitute(^^exec_lexer_action, { reflect_constant(Action), ^^result<I> }) };

        if constexpr (return_type_of(func) == ^^void)
        {
            /* void should be returned to continue */
            [: func :](ctx.osr.res);
            [[clang::musttail]] return skip_current(ctx, it, end, fallback);
        }
        else
        {
            ctx.tok = token<I>([: func
                :](ctx.osr.res));
            return true;
        }
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr void token_end_of_input(context<I, 3> ctx, I it, const S /* last */)
    {
        /* note: similar to lexer_action but always returns */
        static constexpr auto action = [: Info :].eof_action;
        static constexpr auto func{ substitute(^^exec_lexer_action, { reflect_constant(action), ^^result<I> }) };

        set_final<tdfa::no_transition_regops, 0>(ctx, it);

        if constexpr (return_type_of(func) == ^^void)
            [: func :](ctx.osr.res);
        else
            ctx.tok = token<I>([: func
                :](ctx.osr.res));
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr bool skip_current(context<I, 3> ctx, I it, const S last, maybe_fallback_t<I> /* fallback */)
    {
        if (it == last) [[unlikely]]
        {
            token_end_of_input(ctx, it, last);
            return false;
        }

        if constexpr (generation::enabled)
            ++ctx.osr.gen.current;

        if constexpr (not stateful::has_continue)
        {
            if constexpr (DFA.flags.maybe_no_empty and not never_empty
                          and DFA.match_start != DFA.additional_continue_nodes.back())
            {
                const bool empty{ ctx.osr.res.template force_get<0>().empty() };
                ctx.osr.res.reset(it);
                if (empty)
                    [[clang::musttail]] return initial_state<DFA.additional_continue_nodes.back()>(ctx, it, last, it);
            }
            else
            {
                ctx.osr.res.reset(it);
            }

            [[clang::musttail]] return initial_state<DFA.match_start>(ctx, it, last, it);
        }
        else if constexpr (DFA.flags.maybe_no_empty and not never_empty)
        {
            const stateful state_info{ std::exchange(ctx.osr.stf, {}) };
            const bool empty{ ctx.osr.res.template force_get<0>().empty() };
            ctx.osr.res.reset(it);

            template for (constexpr std::size_t i : std::views::iota(0uz, DFA.continue_nodes.size()))
            {
                if (i == state_info.continue_at)
                {
                    if constexpr (DFA.continue_nodes[i] != DFA.additional_continue_nodes[i])
                        if (empty)
                            [[clang::musttail]] return initial_state<DFA.continue_nodes[i]>(ctx, it, last, it);
                    [[clang::musttail]] return initial_state<DFA.additional_continue_nodes[i]>(ctx, it, last, it);
                }
            }
        }
        else
        {
            const stateful state_info{ std::exchange(ctx.osr.stf, {}) };
            ctx.osr.res.reset(it);

            template for (constexpr std::size_t i : std::views::iota(0uz, DFA.continue_nodes.size))
                if (i == state_info.continue_at)
                    [[clang::musttail]] return initial_state<DFA.continue_nodes[i]>(ctx, it, last, it);
        }

        return false;
    }


    /* end of input actions */

    template<std::size_t Alt, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    [[gnu::always_inline]] static constexpr bool success(context<I, X> /* ctx */, I /* it */, const S /* last */, maybe_fallback_t<I> /* fallback */)
        requires is_regex
    {
        return true;
    }

    template<std::size_t Alt, std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr bool success(context<I, 3> ctx, I it, const S last, maybe_fallback_t<I> fallback)
        requires is_lexer
    {
        if constexpr (generation::enabled)
            clean_generations(ctx.osr);
        [[clang::musttail]] return lexer_action<[: Info :].actions[Alt]>(ctx, it, last, fallback);
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    [[gnu::always_inline]] static constexpr bool failure(context<I, X> /* ctx */, I /* it */, const S /* last */, maybe_fallback_t<I> /* fallback */)
        requires is_regex
    {
        return false;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr bool failure(context<I, 3> ctx, I it, const S last, maybe_fallback_t<I> fallback)
        requires is_lexer
    {
        set_final<tdfa::no_transition_regops, 0>(ctx, it);
        [[clang::musttail]] return lexer_action<[: Info :].error_action>(ctx, it, last, fallback);
    }


    /* state fallback */

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
        requires (DFA.flags.enable_fallback)
    static constexpr bool fallback_state(context<I, X> ctx, I it, const S last, fallback_info<I> fallback)
    {
        if (fallback.state == fallback_disabled)
            [[clang::musttail]] return failure(ctx, it, last, fallback);

        // TODO: change to use structured binding when supported
        template for (constexpr auto pair : DFA.fallback_nodes)
        {
            if (fallback.state == pair.first)
            {
                static constexpr auto fni = DFA.final_nodes.at(pair.first);
                if constexpr (not DFA.flags.return_bool)
                    set_fallback<pair.second.op_index, fni.offset, pair.second.continue_at>(ctx, fallback.it);
                [[clang::musttail]] return success<fni.alternative>(ctx, it, last, fallback);
            }
        }

        [[clang::musttail]] return failure(ctx, it, last, fallback);
    }


    /* next state functions */

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    static constexpr bool state(context<I, X> ctx, I it, const S last, maybe_fallback_t<I> fallback)
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
                    register_operations<tr.op_index>(ctx, it);
                    [[clang::musttail]] return state<tr.next>(ctx, ++it, last, fallback);
                }
            }

            if constexpr (final_node != nullptr and DFA.flags.enable_fallback and fallback_node != nullptr)
            {
                set_fallback<final_node->op_index, final_node->offset, fallback_node->continue_at>(ctx, it);
                [[clang::musttail]] return success<final_node->alternative>(ctx, it, last, fallback);
            }
        }
        else
        {
            if constexpr (final_node != nullptr)
            {
                if constexpr (DFA.flags.enable_fallback and fallback_node != nullptr)
                    set_fallback<final_node->op_index, final_node->offset, fallback_node->continue_at>(ctx, it);
                else
                    set_final<final_node->op_index, final_node->offset>(ctx, it);
                [[clang::musttail]] return success<final_node->alternative>(ctx, it, last, fallback);
            }
        }

        if constexpr (DFA.flags.enable_fallback and fallback_node == nullptr)
            [[clang::musttail]] return fallback_state(ctx, it, last, fallback);
        [[clang::musttail]] return failure(ctx, it, last, fallback);
    }

    template<std::size_t DFAState, std::bidirectional_iterator I, int X>
    static constexpr bool state(context<I, X> ctx, I it, const cstr_sentinel_t last, maybe_fallback_t<I> fallback)
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
                register_operations<tr.op_index>(ctx, it);
                [[clang::musttail]] return state<tr.next>(ctx, ++it, last, fallback);
            }
        }

        if constexpr (final_node != nullptr)
        {
            if constexpr (DFA.flags.enable_fallback and fallback_node != nullptr)
            {
                set_fallback<final_node->op_index, final_node->offset, fallback_node->continue_at>(ctx, it);
                [[clang::musttail]] return success<final_node->alternative>(ctx, it, last, fallback);
            }
            else
            {
                if (it == last) [[likely]]
                {
                    set_final<final_node->op_index, final_node->offset>(ctx, it);
                    [[clang::musttail]] return success<final_node->alternative>(ctx, it, last, fallback);
                }
            }
        }

        if constexpr (DFA.flags.enable_fallback and fallback_node == nullptr)
            [[clang::musttail]] return fallback_state(ctx, it, last, fallback);
        [[clang::musttail]] return failure(ctx, it, last, fallback);
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
            template for (constexpr static_transition<char_type> tr : DFA.nodes[DFAState])
            {
                if (tr_possible<tr.cs>(*it))
                {
                    register_operations<tr.op_index>(ctx, it);
                    [[clang::musttail]] return unchecked_state<tr.next, Count - 1>(ctx, ++it, last, fallback);
                }
            }

            [[clang::musttail]] return failure(ctx, it, last, fallback);
        }
    }

    template<std::size_t DFAState, std::size_t Count, integer_sequence_like Skip, std::bidirectional_iterator I, std::sized_sentinel_for<I> S, int X>
    static constexpr bool unchecked_state_skip(context<I, X> ctx, I it, const S last, maybe_fallback_t<I> fallback)
    {
        if constexpr (sequence_helper<Skip>::empty or Count == 0 or DFA.nodes[DFAState].size() != 1)
        {
            [[clang::musttail]] return unchecked_state<DFAState, Count>(ctx, it, last, fallback);
        }
        else if constexpr (Count == sequence_helper<Skip>::head)
        {
            static constexpr auto tr = DFA.nodes[DFAState].front();
            register_operations<tr.op_index>(ctx, it);
            [[clang::musttail]] return unchecked_state_skip<tr.next, Count - 1, typename sequence_helper<Skip>::tail>(ctx, ++it, last, fallback);
        }
        else
        {
            static constexpr auto tr = DFA.nodes[DFAState].front();
            if (tr_possible<tr.cs>(*it))
            {
                register_operations<tr.op_index>(ctx, it);
                [[clang::musttail]] return unchecked_state_skip<tr.next, Count - 1, Skip>(ctx, ++it, last, fallback);
            }

            [[clang::musttail]] return failure(ctx, it, last, fallback);
        }
    }

    /* next state function entry point */

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    static constexpr bool initial_state(context<I, X> ctx, I it, const S last, maybe_fallback_t<I> fallback)
    {
        static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

        if constexpr (not std::sized_sentinel_for<S, I>)
        {
            [[clang::musttail]] return state<DFAState>(ctx, it, last, fallback);
        }
        else if constexpr (fixed_length and not DFA.flags.enable_fallback)
        {
            if (std::ranges::distance(it, last) == min_length)
                [[clang::musttail]] return unchecked_state<DFAState, min_length>(ctx, it, last, fallback);
        }
        else
        {
            if (std::ranges::distance(it, last) >= min_length)
                [[clang::musttail]] return unchecked_state<DFAState, min_length>(ctx, it, last, fallback);
        }

        [[clang::musttail]] return failure(ctx, it, last, it);
    }


    /* outer dfa next state functions  */

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    static constexpr bool scalar_outer_state(context<I, X> ctx, I it, const S last)
    {
        if constexpr (generation::enabled)
            ++ctx.osr.gen.current;

        if (it != last)
        {
            if (state<DFAState>(ctx, it, last, it))
            {
                if constexpr (X != 0 and result<I>::has_match_start)
                    ctx.get_res().match_start_ = it;
                return true;
            }
        }
        else
        {
            if constexpr (static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState); final_node != nullptr)
            {
                set_final<final_node->op_index, final_node->offset>(ctx, it);
                if constexpr (X != 0 and result<I>::has_match_start)
                     ctx.get_res().match_start_ = it;
                return true;
            }
            return false;
        }

        template for (constexpr static_transition<char_type> tr : DFA.outer_transitions)
        {
            if (tr_possible<tr.cs>(*it))
                [[clang::musttail]] return scalar_outer_state<tr.next>(ctx, ++it, last);
        }

        return false;
    }

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sized_sentinel_for<I> S, int X>
    static constexpr bool scalar_outer_state(context<I, X> ctx, I it, const S last)
    {
        if constexpr (generation::enabled)
            ++ctx.osr.gen.current;

        static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

        if (std::ranges::distance(it, last) >= min_length)
        {
            if (unchecked_state<DFAState, min_length>(ctx, it, last, it))
            {
                if constexpr (X != 0 and result<I>::has_match_start)
                    ctx.get_res().match_start_ = it;
                return true;
            }
        }
        else
        {
            if constexpr (static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState); final_node != nullptr)
            {
                set_final<final_node->op_index, final_node->offset>(ctx, it);
                if constexpr (X != 0 and result<I>::has_match_start)
                    ctx.get_res().match_start_ = it;
                return true;
            }
            return false;
        }

        template for (constexpr static_transition<char_type> tr : DFA.outer_transitions)
        {
            if (tr_possible<tr.cs>(*it))
                [[clang::musttail]] return scalar_outer_state<tr.next>(ctx, ++it, last);
        }

        return false;
    }

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sized_sentinel_for<I> S, int X>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                  and (DFA.flags.enable_fallback or not fixed_length))
    static constexpr bool scalar_outer_state(context<I, X> ctx, I it, const S last)
    {
        static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

        while (std::ranges::distance(it, last) >= min_length)
        {
            if constexpr (generation::enabled)
                ++ctx.osr.gen.current;

            if (unchecked_state<DFAState, min_length>(ctx, it, last, it))
            {
                if constexpr (X != 0 and result<I>::has_match_start)
                    ctx.get_res().match_start_ = it;
                return true;
            }

            ++it;
        }

        /* since the DFA is never empty, we can skip assigning final node information */
        static_assert(DFA.final_nodes.at_if(DFAState) == nullptr);

        return false;
    }

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sized_sentinel_for<I> S, int X>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                  and (/* degenerate case */ fixed_length and not DFA.flags.enable_fallback))
    static constexpr bool scalar_outer_state(context<I, X> ctx, I it, const S last)
    {
        static constexpr auto length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

        if (const auto input_size = std::ranges::distance(it, last); input_size >= length)
        {
            std::ranges::advance(it, input_size - length);

            if (unchecked_state<DFAState, length>(ctx, it, last, it))
            {
                if constexpr (X != 0 and result<I>::has_match_start)
                    ctx.get_res().match_start_ = it;
                return true;
            }
        }

        /* since the DFA is never empty, we can skip assigning final node information */
        static_assert(DFA.final_nodes.at_if(DFAState) == nullptr);

        return false;
    }


    /* simd outer dfa next state functions */

    template<std::size_t DFAState, std::size_t Count, integer_sequence_like Skip,
             std::contiguous_iterator I, std::sized_sentinel_for<I> S, int X>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState)
    [[gnu::always_inline]] static constexpr bool vector_candidate_check(context<I, X> ctx, I it, const S last, unsigned long long mask)
    {
        while (mask != 0) [[unlikely]]
        {
            const auto offset = std::countr_zero(mask);
            mask &= (mask - 1);

            // TODO: implement simd-based checking?
            if (unchecked_state_skip<DFAState, Count, Skip>(ctx, it + offset, last, it + offset))
            {
                if constexpr (X != 0 and result<I>::has_match_start)
                    ctx.get_res().match_start_ = it + offset;
                return true;
            }
            else
            {
                if constexpr (generation::enabled)
                    ++ctx.osr.gen.current;
            }
        }

        return false;
    }

    template<std::size_t DFAState, std::contiguous_iterator I, std::sized_sentinel_for<I> S, int X>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                  and fixed_length and branch_free)
    static constexpr bool vector_outer_state(context<I, X> ctx, I it, const S last)
    {
        static constexpr auto length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);
        static constexpr auto states = define_static_array(get_flattened_states(DFA, DFAState));

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
            return std::saturating_mul(count1, count2)  > std::saturating_mul(avoid_simd_threshold, avoid_simd_threshold);
        }();

        if constexpr (avoid_simd)
        {
            /* using simd in this case is likely a pessimisation; resort to non-vectorised implementation instead */
            return scalar_outer_state<DFAState>(ctx, it, last);
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

                if (vector_candidate_check<DFAState, length, skipped>(ctx, it, last, (mask1 & mask2).to_ullong()))
                    return true;

                it += vec_type::size();
            }

            if (const auto epi_size = max % vec_type::size(); epi_size > 0) [[likely]]
            {
                const mask_type epi_mask{ (1uz << epi_size) - 1 };

#if false
                /* bug involving non-mask overloads of simd::partial_load */
                const auto block1 = std::simd::partial_load<vec_type>(it + position1, epi_size, flags);
                const auto block2 = std::simd::partial_load<vec_type>(it + position2, epi_size, flags);
#else
                const auto block1 = std::simd::partial_load<vec_type>(it + position1, epi_size, epi_mask, flags);
                const auto block2 = std::simd::partial_load<vec_type>(it + position2, epi_size, epi_mask, flags);
#endif

                const auto mask1 = vector_tr_find<DFA.nodes[states[position1]].front().cs>(block1);
                const auto mask2 = vector_tr_find<DFA.nodes[states[position2]].front().cs>(block2);

                return vector_candidate_check<DFAState, length, skipped>(ctx, it, last, (mask1 & mask2 & epi_mask).to_ullong());
            }

            return false;
        }
    }

    template<std::size_t DFAState, std::contiguous_iterator I, std::sized_sentinel_for<I> S, int X>
        requires (never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState
                  and (not fixed_length or not branch_free))
    static constexpr bool vector_outer_state(context<I, X> ctx, I it, const S last)
    {
        static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);
        static constexpr auto states = define_static_array(get_flattened_states(DFA, DFAState, true));

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
            return scalar_outer_state<DFAState>(ctx, it, last);
        }
        else
        {
            const std::ptrdiff_t max{ std::ranges::distance(it, last) - (min_length - 1) };

            for (std::ptrdiff_t i{ max / vec_type::size() }; i > 0; --i) [[likely]]
            {
                const auto block = std::simd::unchecked_load<vec_type>(it + position, last, flags);
                const auto mask = vector_tr_find<combined_cs>(block);

                if (vector_candidate_check<DFAState, min_length, skipped>(ctx, it, last, mask.to_ullong()))
                    return true;

                it += vec_type::size();
            }

            if (const auto epi_size = max % vec_type::size(); epi_size > 0) [[likely]]
            {
                const mask_type epi_mask{ (1uz << epi_size) - 1 };

#if false
                /* bug involving non-mask overloads of simd::partial_load */
                const auto block = std::simd::partial_load<vec_type>(it + position, epi_size, flags);
#else
                const auto block = std::simd::partial_load<vec_type>(it + position, epi_size, epi_mask, flags);
#endif
                const auto mask = vector_tr_find<combined_cs>(block);

                return vector_candidate_check<DFAState, min_length, skipped>(ctx, it, last, (mask & epi_mask).to_ullong());
            }

            return false;
        }
    }


    /* outer state function dispatch */

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    static constexpr bool outer_state(context<I, X> ctx, I it, const S last)
    {
        if constexpr (std::contiguous_iterator<I> and std::sized_sentinel_for<S, I>
                      and never_empty and DFA.continue_nodes.size() == 1 and DFA.continue_nodes[0] == DFAState)
            return vector_outer_state<DFAState>(ctx, it, last);
        else
            return scalar_outer_state<DFAState>(ctx, it, last);
    }

    static constexpr bool regex_return_bool{ is_regex and DFA.flags.return_bool };
    static constexpr bool regex_normal{ is_regex and not DFA.flags.return_bool };
    static constexpr bool regex_nonempty{ regex_normal and DFA.flags.maybe_no_empty };
    static constexpr bool lexer_normal{ is_lexer };
    static constexpr bool lexer_nonempty{ lexer_normal and DFA.flags.maybe_no_empty };

public:
    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires regex_return_bool and stateful::is_stateless
    [[nodiscard]] static constexpr bool operator()(const I first, const S last)
    {
        if constexpr (DFA.flags.adapted_search)
            return outer_state<DFA.match_start>(context<I, 0>{}, first, last);
        else
            return initial_state<DFA.match_start>(context<I, 0>{}, first, last, first);
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires regex_normal and stateful::is_stateless
    [[nodiscard]] static constexpr result<I> operator()(const I first, const S last)
    {
        result<I> res{ first };
        overspill<I> osp{};

        if constexpr (DFA.flags.adapted_search)
            outer_state<DFA.match_start>(context{ res, osp }, first, last);
        else
            initial_state<DFA.match_start>(context{ res, osp }, first, last, first);

        if constexpr (generation::enabled)
            clean_generations(res, osp);
        return res;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires regex_nonempty and stateful::is_stateless
    [[nodiscard]] static constexpr result<I> operator()(const I first, const S last, match_non_empty_t)
    {
        result<I> res{ first };
        overspill<I> osp{};

        static constexpr auto start_state{ never_empty ? DFA.match_start : DFA.additional_continue_nodes.back() };

        if constexpr (DFA.flags.adapted_search)
            outer_state<start_state>(context{ res, osp }, first, last);
        else
            initial_state<start_state>(context{ res, osp }, first, last, first);

        if constexpr (generation::enabled)
            clean_generations(res, osp);
        return res;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires lexer_normal and stateful::is_stateless
    [[nodiscard]] static constexpr token<I> operator()(I& first, const S last)
    {
        token<I> tok;
        overspill<I, true> osp{ first };

        if (first == last) [[unlikely]]
        {
            token_end_of_input(context{ tok, osp }, first, last);
            return tok;
        }

        initial_state<DFA.match_start>(context{ tok, osp }, first, last, first);
        first = osp.res.match_end_;
        return tok;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires lexer_normal and result<I>::has_continue
    [[nodiscard]] static constexpr token<I> operator()(I& first, const S last, const tdfa::continue_at_t continue_at)
    {
        // TODO: reimplement in a similar manner to iterated_result
        token<I> tok;
        overspill<I, true> osp{ first };

        if (first == last) [[unlikely]]
        {
            token_end_of_input(context{ tok, osp }, first, last);
            return tok;
        }

        template for (constexpr std::size_t i : std::views::iota(0uz, DFA.continue_nodes.size()))
        {
            if (i == continue_at)
            {
                initial_state<DFA.continue_nodes[i]>(context{ tok, osp }, first, last, first);
#ifndef __GNUC_MINOR__
                break;
#else
                first = osp.res.match_end_;
                return tok;
#endif
            }
        }

        first = osp.res.match_end_;
        return tok;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires lexer_nonempty and stateful::is_stateless
    [[nodiscard]] static constexpr token<I> operator()(I& first, const S last, match_non_empty_t)
    {
        static constexpr auto start_state{ never_empty ? DFA.match_start : DFA.additional_continue_nodes.back() };

        token<I> tok;
        overspill<I, true> osp{ first };

        if (first == last) [[unlikely]]
        {
            token_end_of_input(context{ tok, osp }, first, last);
            return tok;
        }

        initial_state<start_state>(context{ tok, osp }, first, last, first);
        first = osp.res.match_end_;
        return tok;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires lexer_nonempty and result<I>::has_continue
    [[nodiscard]] static constexpr token<I> operator()(I& first, const S last, const tdfa::continue_at_t continue_at, match_non_empty_t)
    {
        // TODO: reimplement in a similar manner to iterated_result
        token<I> tok;
        overspill<I, true> osp{ first };

        if (first == last) [[unlikely]]
        {
            token_end_of_input(context{ tok, osp }, first, last);
            return tok;
        }

        template for (constexpr std::size_t i : std::views::iota(0uz, DFA.continue_nodes.size()))
        {
            if (i == continue_at)
            {
                static constexpr auto start_state{ never_empty ? DFA.continue_nodes[i] : DFA.additional_continue_nodes[i] };
                initial_state<start_state>(context{ tok, osp }, first, last, first);
#ifndef __GNUC_MINOR__
                break;
#else
                first = osp.res.match_end_;
                return tok;
#endif
            }
        }

        first = osp.res.match_end_;
        return tok;
    }
};


template<std::meta::info Info>
template<std::bidirectional_iterator I>
    requires std::is_nothrow_convertible_v<std::iter_value_t<I>, typename p1306dfa<Info>::char_type>
struct p1306dfa<Info>::iterated_result
{
    static constexpr bool needs_begin{ not never_empty };
    using state_type = stateful;

    iterated_result() = default;

    constexpr iterated_result(const I first, const std::sentinel_for<I> auto last)
        : res{ first }
    {
        start(first, last);
    }

    constexpr iterated_result(const I first, const std::sentinel_for<I> auto last, match_non_empty_t)
        : res{ first }
    {
        start<true>(first, last);
    }

    constexpr iterated_result(const I /* first */, const I it, const std::sentinel_for<I> auto last, stateful stf, bool prev_empty)
        : res{ it }, stf{ stf }
    {
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

    constexpr I advance(const std::sentinel_for<I> auto last)
        requires never_empty
    {
        const auto& match = res.template force_get<0>();
        const I current = match.end();
        res.reset(current);
        resume(current, last);
        return current;
    }

    constexpr I advance(const I first, const std::sentinel_for<I> auto last)
    {
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
        static constexpr bool use_alt{ NonEmptyMatch and not never_empty };
        static constexpr auto start_state{ use_alt ? DFA.additional_continue_nodes.back() : DFA.match_start };

        overspill<I> osp{};

        if constexpr (DFA.flags.adapted_search)
            outer_state<start_state>(context{ *this, osp }, first, last);
        else
            initial_state<start_state>(context{ *this, osp }, first, last, first);

        if constexpr (generation::enabled)
            clean_generations(res, osp);
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
            static constexpr bool use_alt{ NonEmptyMatch and not never_empty };
            const stateful state_info{ std::exchange(stf, {}) };
            overspill<I> osp{};

            template for (constexpr std::size_t i : std::views::iota(0uz, DFA.continue_nodes.size()))
            {
                static constexpr auto start_state{ use_alt ? DFA.additional_continue_nodes[i] : DFA.continue_nodes[i] };

                if (state_info.continue_at == i)
                {
                    if constexpr (DFA.flags.adapted_search)
                        outer_state<start_state>(context{ *this, osp }, first, last);
                    else
                        initial_state<start_state>(context{ *this, osp }, first, last, first);
#ifndef __GNUC_MINOR__
                    break;
#else
                    if constexpr (generation::enabled)
                        clean_generations(res, osp);
                    return; // temporary workaround for GCC bug
#endif
                }
            }

            if constexpr (generation::enabled)
                clean_generations(res, osp);
            return;
        }
    }

public:
    result<I> res;
    stateful stf;
};

template<string_literal Pattern, fsm_flags Flags>
using p1306_matcher = p1306dfa<(^^re<std::meta::reflect_constant_string(Pattern.view()), pack_flags(Flags)>)>;

template<string_literal Pattern, fsm_flags Flags>
using p1306_searcher = p1306dfa<(^^re<std::meta::reflect_constant_string(Pattern.view()), pack_flags(adapt_searcher_flags_to_matcher(Flags))>)>;

} // namespace detail
} // namespace srx
