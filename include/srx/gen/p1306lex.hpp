// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <bits/sat_arith.h>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <limits>
#include <meta>
#include <ranges>
#include <type_traits>

#include "srx/etc/static_charset.hpp"
#include "srx/etc/util.hpp"
#include "srx/fsm/tdfa.hpp"
#include "srx/gen/compile.hpp"
#include "srx/gen/result.hpp"
#include "srx/gen/p1306.hpp"


namespace srx {
namespace detail {

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
struct p1306lex
{
    static_assert(has_template_arguments(type_of(Info)), "Invalid reflection value");
    static_assert(template_of(type_of(Info)) == ^^lexer_info);

    using char_type = typename [: template_arguments_of(type_of(Info))[0] :];

private:
    static constexpr const tdfa_info<char_type> DFA = [: Info :].dfa;

    static constexpr bool never_empty{ DFA.additional_continue_nodes.empty() };
    static constexpr bool fixed_length{ DFA.min_max_lengths.first != std::numeric_limits<std::size_t>::max()
                                        and DFA.min_max_lengths.first == DFA.min_max_lengths.second };
    static constexpr bool branch_free{ std::ranges::all_of(DFA.nodes, [](const auto& n){ return n.size() <= 1; }) };

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

public:
    static_assert(DFA.flags.enable_fallback, "Invalid DFA used in lexer");
    static_assert(not DFA.flags.return_bool, "Invalid DFA used in lexer");

    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    using result = static_match_results<I, smr_layout::reg_map<DFA.register_count, DFA.captures, DFA.final_registers>>;

    using token = typename [: [: Info :].return_type :];

    // // TODO:
    // template<std::bidirectional_iterator I>
    //     requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    // struct iterated_token;

    struct stateful
    {
        static constexpr bool has_continue{ DFA.has_continue() };
        static constexpr bool is_stateless{ not (has_continue /* and other has_x */) };

        using continue_type = maybe_type_t<has_continue, tdfa::continue_at_t>;

        [[no_unique_address]] continue_type continue_at{ detail::tdfa::no_continue };
    };

private:
    static constexpr std::ptrdiff_t fallback_disabled{ 0 };

    template<typename I>
    struct overspill
    {
        result<I> match_result;
        // TODO: move overspill out of result<I>
        [[no_unique_address]] stateful state;

        overspill() = default;
        constexpr overspill(const I& first) noexcept(noexcept(result<I>{ first })) : match_result{ first } {}
    };

    using token_ref = std::add_lvalue_reference_t<token>;

    template<typename I>
    using overspill_ref = std::add_lvalue_reference_t<overspill<I>>;

    template<typename I>
    struct context
    {
        /* store token, with result in overspill only */
        token_ref tok;
        overspill_ref<I> osr;

        [[gnu::always_inline]] constexpr auto& get_res() noexcept { return osr.match_result; }
        [[gnu::always_inline]] constexpr auto& get_stf() noexcept { return osr.state; }
    };

    template<typename I>
    context(token&, overspill<I>&) -> context<I>;


    /* register operations */

    template<std::size_t Blk, std::bidirectional_iterator I>
    static constexpr void register_operations(context<I> ctx, const I it)
    {
        if constexpr (Blk != tdfa::no_transition_regops)
        {
            template for (constexpr auto& op : DFA.regops[Blk])
            {
                if constexpr (op.is_copy)
                    ctx.get_res().reg_[op.dst] = ctx.get_res().reg_[op.cpy_src];
                else if constexpr (op.set_val)
                    ctx.get_res().reg_[op.dst] = it;
                else if constexpr (std::contiguous_iterator<I>)
                    ctx.get_res().reg_[op.dst] = I{};

                if constexpr (not std::contiguous_iterator<I>)
                {
                    if constexpr (op.is_copy)
                        ctx.get_res().enabled_[op.dst] = ctx.get_res().enabled_[op.cpy_src];
                    else
                        ctx.get_res().enabled_[op.dst] = op.set_val;
                }
            }
        }
    }


    /* final and fallback */

    template<std::size_t Blk, std::ptrdiff_t Offset, std::bidirectional_iterator I>
    static constexpr void set_final(context<I> ctx, const I it)
    {
        register_operations<Blk>(ctx, it);
        ctx.get_res().match_end_ = std::ranges::prev(it, Offset);

        if constexpr (not std::contiguous_iterator<I>)
            ctx.get_res().match_success_ = true;
    }

    template<std::size_t Blk, std::ptrdiff_t Offset, tdfa::continue_at_t ContinueAt, std::bidirectional_iterator I>
    static constexpr void set_fallback(context<I> ctx, const I it)
    {
        set_final<Blk, Offset>(ctx, it);

        if constexpr (stateful::has_continue and ContinueAt != tdfa::no_continue)
            ctx.get_stf().continue_at = ContinueAt;
    }


    /* lexer actions */

    template<std::meta::info Action, tdfa::continue_at_t Cont, bool Success, std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr void lexer_action(context<I> ctx, I it, const S end, I fallback_it, std::ptrdiff_t fallback_idx)
    {
        static constexpr auto func = substitute(^^exec_lexer_action, { reflect_constant(Action), ^^result<I> });
        static constexpr auto ret = return_type_of(func);

        if constexpr (not is_same_type(ret, ^^void))
        {
            if constexpr (is_convertible_type(ret, ^^token))
                ctx.tok = [: func :](ctx.osr.match_result);
            else if constexpr (is_constructible_type(^^token, { ret }))
                ctx.tok = token([: func
                    :](ctx.osr.match_result));
            else
                static_assert("Invalid lexer action: cannot assign result of lexer action to token");
        }
        else
        {
            [: func :](ctx.osr.match_result);

            if constexpr (Cont != tdfa::no_continue or not Success)
            {
                [[clang::musttail]] return lexer_continue<Cont>(ctx, it, end, fallback_it, fallback_idx);
            }
            else
            {
                /* succeeded with no continue: eof reached */
                ctx.osr.state = stateful{};
                ctx.osr.match_result.reset(it);
                lexer_eof(ctx, it, end);
            }
        }
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr void lexer_eof(context<I> ctx, I it, const S /* last */)
    {
        /* note: similar to lexer_action but never restarts matching */
        static constexpr auto action = [: Info :].eof_action;
        static constexpr auto func{ substitute(^^exec_lexer_action, { reflect_constant(action), ^^result<I> }) };
        static constexpr auto ret = return_type_of(func);

        set_final<tdfa::no_transition_regops, 0>(ctx, it);

        if constexpr (is_same_type(ret, ^^void))
            [: func :](ctx.osr.match_result);
        else if constexpr (is_convertible_type(ret, ^^token))
            ctx.tok = [: func :](ctx.osr.match_result);
        else if constexpr (is_constructible_type(^^token, ret))
            ctx.tok = token([: func
                :](ctx.osr.match_result));
        else
            static_assert("Invalid lexer action: cannot assign result of lexer action to token");
    }

    template<tdfa::continue_at_t Cont, std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr void lexer_continue(context<I> ctx, I it, const S last, I /* fallback_it */, std::ptrdiff_t /* fallback_idx */)
    {
        static constexpr bool track_empty{ has_different_alt(Cont) };
        maybe_type_t<track_empty, bool> empty{};

        if constexpr (track_empty)
            empty = ctx.osr.match_result.template force_get<0>().empty();

        ctx.osr.state = stateful{};
        ctx.osr.match_result.reset(it);

        if (it == last) [[unlikely]]
            return lexer_eof(ctx, it, last);

        if constexpr (track_empty)
            if (empty)
                [[clang::musttail]] return initial_state<get_start(Cont, true)>(ctx, it, last, it);
        [[clang::musttail]] return initial_state<get_start(Cont)>(ctx, it, last, it, fallback_disabled);
    }


    /* success / failure actions */

    template<std::size_t Alt, tdfa::continue_at_t Cont, std::bidirectional_iterator I, std::sentinel_for<I> S>
    [[gnu::noinline]]  static constexpr void success(context<I> ctx, I it, const S last, I fallback_it, std::ptrdiff_t fallback_idx)
    {
        [[clang::musttail]] return lexer_action<[: Info :].actions[Alt], Cont, true>(ctx, it, last, fallback_it, fallback_idx);
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    [[gnu::noinline]] static constexpr void failure(context<I> ctx, I it, const S last, I fallback_it, std::ptrdiff_t fallback_idx)
    {
        set_final<tdfa::no_transition_regops, 0>(ctx, it);
        [[clang::musttail]] return lexer_action<[: Info :].error_action, tdfa::no_continue, false>(ctx, it, last, fallback_it, fallback_idx);
    }


    /* state fallback */

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    [[gnu::noinline]] static constexpr void fallback_state(context<I> ctx, I it, const S last, I fallback_it, std::ptrdiff_t fallback_idx)
    {
        static_assert(fallback_disabled == 0);
        [[assume(fallback_idx <= static_cast<std::ptrdiff_t>(DFA.fallback_nodes.size()))]];
        [[assume(fallback_idx >= 0)]];

        if (fallback_idx == fallback_disabled)
            [[clang::musttail]] return failure(ctx, it, last, fallback_it, fallback_idx);

        template for (constexpr auto i : std::views::indices(DFA.fallback_nodes.size()))
        {
            static constexpr auto& [state, fbni] = DFA.fallback_nodes.begin()[i];
            static constexpr std::ptrdiff_t idx{ 1 + static_cast<std::ptrdiff_t>(i) };

            if (fallback_idx == idx)
            {
                static constexpr auto& fni = DFA.final_nodes.at(state);
                set_fallback<fbni.op_index, fni.offset, fbni.continue_at>(ctx, fallback_it);
                [[clang::musttail]] return success<fni.alternative, fbni.continue_at>(ctx, fallback_it, last, fallback_it, fallback_idx);
            }
        }

        std::unreachable();
    }


    /* next state functions */

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr void state(context<I> ctx, I it, const S last, I fallback_it, std::ptrdiff_t fallback_idx)
    {
        static constexpr auto fb_it = DFA.fallback_nodes.find(DFAState);
        static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState);
        static constexpr auto* fallback_node = (fb_it != DFA.fallback_nodes.end()) ? &(fb_it->second) : nullptr;

        if constexpr (fallback_node != nullptr)
        {
            static constexpr std::ptrdiff_t index{ 1 + std::ranges::distance(DFA.fallback_nodes.begin(), fb_it) };

            fallback_it = it;
            fallback_idx = index;
        }

        if (it != last)
        {
            template for (constexpr auto& tr : DFA.nodes[DFAState])
            {
                if (tr_possible<tr.cs>(*it))
                {
                    register_operations<tr.op_index>(ctx, it);
                    [[clang::musttail]] return state<tr.next>(ctx, ++it, last, fallback_it, fallback_idx);
                }
            }
        }
        else
        {
            if constexpr (final_node != nullptr and fallback_node == nullptr)
            {
                set_final<final_node->op_index, final_node->offset>(ctx, it);
                [[clang::musttail]] return success<final_node->alternative, tdfa::no_continue>(ctx, it, last, fallback_it, fallback_idx);
            }
        }

        if constexpr (final_node != nullptr and fallback_node != nullptr)
        {
            set_fallback<final_node->op_index, final_node->offset, fallback_node->continue_at>(ctx, it);
            [[clang::musttail]] return success<final_node->alternative, fallback_node->continue_at>(ctx, it, last, fallback_it, fallback_idx);
        }

        [[clang::musttail]] return fallback_state(ctx, it, last, fallback_it, fallback_idx);
    }

    template<std::size_t DFAState, std::bidirectional_iterator I>
    static constexpr void state(context<I> ctx, I it, const cstr_sentinel_t last, I fallback_it, std::ptrdiff_t fallback_idx)
    {
        static constexpr auto fb_it = DFA.fallback_nodes.find(DFAState);
        static constexpr auto* final_node = DFA.final_nodes.at_if(DFAState);
        static constexpr auto* fallback_node = (fb_it != DFA.fallback_nodes.end()) ? &(fb_it->second) : nullptr;

        if constexpr (fallback_node != nullptr)
        {
            static constexpr std::ptrdiff_t index{ 1 + std::ranges::distance(DFA.fallback_nodes.begin(), fb_it) };

            fallback_it = it;
            fallback_idx = index;
        }

        template for (constexpr auto& tr : DFA.nodes[DFAState])
        {
            if (tr_possible_exclude_null<tr.cs>(*it))
            {
                register_operations<tr.op_index>(ctx, it);
                [[clang::musttail]] return state<tr.next>(ctx, ++it, last, fallback_it, fallback_idx);
            }
        }

        if constexpr (final_node != nullptr)
        {
            if constexpr (fallback_node != nullptr)
            {
                set_fallback<final_node->op_index, final_node->offset, fallback_node->continue_at>(ctx, it);
                [[clang::musttail]] return success<final_node->alternative, fallback_node->continue_at>(ctx, it, last, fallback_it, fallback_idx);
            }
            else
            {
                if (it == last) [[likely]]
                {
                    set_final<final_node->op_index, final_node->offset>(ctx, it);
                    [[clang::musttail]] return success<final_node->alternative, tdfa::no_continue>(ctx, it, last, fallback_it, fallback_idx);
                }
            }
        }

        [[clang::musttail]] return fallback_state(ctx, it, last, fallback_it, fallback_idx);
    }

    template<std::size_t DFAState, std::size_t Count, std::bidirectional_iterator I, std::sized_sentinel_for<I> S>
    static constexpr void unchecked_state(context<I> ctx, I it, const S last, I fallback_it, std::ptrdiff_t fallback_idx)
    {
        if constexpr (Count == 0)
        {
            [[clang::musttail]] return state<DFAState>(ctx, it, last, fallback_it, fallback_idx);
        }
        else
        {
            template for (constexpr auto& tr : DFA.nodes[DFAState])
            {
                if (tr_possible<tr.cs>(*it))
                {
                    register_operations<tr.op_index>(ctx, it);
                    [[clang::musttail]] return unchecked_state<tr.next, Count - 1>(ctx, ++it, last, fallback_it, fallback_idx);
                }
            }

            [[clang::musttail]] return failure(ctx, it, last, fallback_it, fallback_idx);
        }
    }

    /* next state function entry point */

    template<std::size_t DFAState, std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr void initial_state(context<I> ctx, I it, const S last, I fallback_it, std::ptrdiff_t fallback_idx)
    {
        static constexpr auto min_length = static_cast<std::ptrdiff_t>(DFA.min_max_lengths.first);

        if constexpr (not std::sized_sentinel_for<S, I> or (DFA.flags.is_search and not DFA.flags.adapted_search))
        {
            /* note: searches are very likely to exceed min_length, so prefer reduced code duplication instead */
            [[clang::musttail]] return state<DFAState>(ctx, it, last, fallback_it, fallback_idx);
        }
        else if constexpr (fixed_length)
        {
            if (std::ranges::distance(it, last) == min_length)
                [[clang::musttail]] return unchecked_state<DFAState, min_length>(ctx, it, last, fallback_it, fallback_idx);
        }
        else
        {
            if (std::ranges::distance(it, last) >= min_length)
                [[clang::musttail]] return unchecked_state<DFAState, min_length>(ctx, it, last, fallback_it, fallback_idx);
        }

        [[clang::musttail]] return failure(ctx, it, last, fallback_it, fallback_idx);
    }


public:
    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires stateful::is_stateless
    [[nodiscard]] static constexpr token operator()(I& first, const S last)
    {
        static_assert(can_tailcall_with<I> and can_tailcall_with<S>, "Iterator is not useable in tail calls");

        token tok;
        overspill<I> osp{ first };

        if (first == last) [[unlikely]]
        {
            lexer_eof(context{ tok, osp }, first, last);
            return tok;
        }

        initial_state<DFA.match_start>(context{ tok, osp }, first, last, first, fallback_disabled);
        first = osp.match_result.match_end_;
        return tok;
    }

    // TODO: implement iterated_token
};

} // namespace detail
} // namespace srx
