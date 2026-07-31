// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <bitset>
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
struct table_dfa
{
    static_assert(has_template_arguments(type_of(Info)), "Invalid reflection value");

    // using char_type = typename [: template_arguments_of(type_of(Info))[0] :];
    using char_type = char;
    using uchar_type = std::make_unsigned_t<char>;

    static_assert(sizeof(char_type) == 1);

    // static constexpr bool is_regex{ template_of(type_of(Info)) == ^^tdfa_info };
    // static constexpr bool is_lexer{ template_of(type_of(Info)) == ^^lexer_info };
    // static_assert(is_regex and not is_lexer);

private:
    static constexpr tdfa_info<char_type> DFA = [: Info :];
    static constexpr bool never_empty{ DFA.additional_continue_nodes.empty() };
    static constexpr bool has_regops{ not DFA.regops.empty() };
    static constexpr bool has_fallback{ DFA.flags.enable_fallback };
    static constexpr bool has_continue{ DFA.flags.is_iterator };
    static constexpr bool has_alternative{ DFA.alt_mode };
    static constexpr bool has_extra_continue{ has_continue and not never_empty };
    static constexpr auto largest_offset{ DFA.largest_offset() };

    using state_offset_t = std::uint_least8_t;
    using regop_offset_t = std::uint_least8_t;

    static_assert(DFA.nodes.size() < std::numeric_limits<state_offset_t>::max() - 1);
    static_assert(DFA.regops.size() < std::numeric_limits<regop_offset_t>::max() - 1);

public:
    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    using result = static_match_results<I, smr_layout::reg_map<DFA.register_count, DFA.captures, DFA.final_registers>>;

    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    struct iterated_result;

private:
    using maybe_regop_offset_t = maybe_type_t<has_regops, regop_offset_t>;
    using maybe_alternative_t = maybe_type_t<has_alternative, std::uint_least32_t>;
    using maybe_next_start_nonempty_t = maybe_type_t<has_extra_continue, state_offset_t>;
    using maybe_next_start_t = maybe_type_t<has_continue, state_offset_t>;

    template<typename I>
    using maybe_saved_first_t = maybe_type_t<has_extra_continue, I>;

    struct overspill
    {
        // TODO: move overspill out of result<I>
        [[no_unique_address]] maybe_alternative_t alt{ 0 };
        [[no_unique_address]] maybe_next_start_t nst{ 0 };

        overspill() = default;
    };

    using overspill_ref = std::add_lvalue_reference_t<overspill>;

    template<typename I>
    using result_ref = std::add_lvalue_reference_t<result<I>>;

    static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };

    template<typename I>
    struct fallback_info
    {
        I it;
        std::size_t state{ fallback_disabled };

        constexpr explicit(false) fallback_info(I it) : it{ it } {}
    };

    template<typename I>
    using maybe_fallback_t = maybe_type_t<has_fallback, fallback_info<I>>;

    template<typename I>
    struct context
    {
        result_ref<I> res;
        overspill_ref osr;
    };


    /* register operations */

    template<std::size_t Blk, std::bidirectional_iterator I>
    static constexpr void exec_regop(context<I> ctx, I it)
    {
        if constexpr (Blk != tdfa::no_transition_regops)
        {
            template for (constexpr register_operation op : DFA.regops[Blk])
            {
                if constexpr (op.is_copy)
                    ctx.res.reg_[op.dst] = ctx.res.reg_[op.cpy_src];
                else if constexpr (op.set_val)
                    ctx.res.reg_[op.dst] = it;
                else if constexpr (std::contiguous_iterator<I>)
                    ctx.res.reg_[op.dst] = I{};

                if constexpr (not std::contiguous_iterator<I>)
                {
                    if constexpr (op.is_copy)
                        ctx.res.enabled_[op.dst] = ctx.res.enabled_[op.cpy_src];
                    else
                        ctx.res.enabled_[op.dst] = op.set_val;
                }
            }
        }
    }


    /* table contents */

    struct table_entry
    {
        state_offset_t state;
        [[no_unique_address]] maybe_regop_offset_t regop;
    };

    struct table_row
    {
        static constexpr std::size_t row_size{ 0b1 << std::numeric_limits<uchar_type>::digits };

        constexpr table_entry& operator[](std::size_t n) noexcept { return data_[n]; }
        constexpr const table_entry& operator[](std::size_t n) const noexcept { return data_[n]; }

        table_row() = default;

        table_entry data_[row_size]{};
    };

    template<typename I>
    struct accepting_state
    {
        struct contents_type;

        using function_pointer_t = decltype(&exec_regop<tdfa::no_transition_regops, I>);
        using alt_type = std::uint_least32_t;
        using ofs_type = std::uint_least16_t;
        using small_ofs_type = std::uint_least8_t;

        consteval {
            /* note: using maybe_type_t resulted in structs with unwanted padding */
            std::vector<std::meta::info> mems;
            if (has_regops)
                mems.push_back(data_member_spec(^^function_pointer_t, { .name = "final_regop" }));
            if (has_fallback and has_regops)
                mems.push_back(data_member_spec(^^function_pointer_t, { .name = "fallback_regop" }));
            if (has_alternative)
                mems.push_back(data_member_spec(^^alt_type, { .name = "alternative" }));
            if (largest_offset > std::numeric_limits<small_ofs_type>::max())
                mems.push_back(std::meta::data_member_spec(^^ofs_type, { .name = "offset" }));
            else if (largest_offset > 0)
                mems.push_back(std::meta::data_member_spec(^^small_ofs_type, { .name = "offset" }));
            if (has_continue)
                mems.push_back(data_member_spec(^^state_offset_t, { .name = "next_start" }));
            if (has_extra_continue)
                mems.push_back(data_member_spec(^^state_offset_t, { .name = "nonempty_next_start" }));
            define_aggregate(^^contents_type, mems);
        }

        contents_type data{};
    };

    static constexpr bool accepting_state_empty{ not (has_regops or (has_fallback and has_regops) or has_alternative
                                                      or (largest_offset > 0) or has_continue or has_extra_continue) };


    /* helpers for table construction */

    static consteval table_row make_transition_row(std::size_t q)
    {
        table_row row{};

        for (const static_transition<char_type>& tr : DFA.nodes[q] | std::views::reverse)
        {
            for (const auto& [lo, hi] : tr.cs.get_intervals())
            {
                for (uchar_type c{ static_cast<uchar_type>(lo) }; true; ++c)
                {
                    row[c] = {
                        .state = static_cast<state_offset_t>(tr.next + 1),
                        .regop = static_cast<regop_offset_t>(tr.op_index + 1)
                    };

                    if (c == static_cast<uchar_type>(hi))
                        break;
                }
            }
        }

        return row;
    }

    template<std::size_t Index, typename I>
    static consteval accepting_state<I> make_accepting_state()
    {
        static constexpr auto [state, fni] = *(DFA.final_nodes.cbegin() + Index);
        static constexpr auto* fbni = DFA.fallback_nodes.at_if(state);

        accepting_state<I> qf{};

        if constexpr (has_regops)
            qf.data.final_regop = &exec_regop<fni.op_index, I>;

        if constexpr (has_alternative)
            qf.data.alternative = fni.alternative;

        if constexpr (largest_offset > std::numeric_limits<typename accepting_state<I>::small_ofs_type>::max())
            qf.data.offset = fni.offset;
        else if constexpr (largest_offset > 0)
            qf.data.offset = static_cast<typename accepting_state<I>::small_ofs_type>(fni.offset);

        if constexpr (fbni != nullptr)
        {
            if constexpr (has_regops)
                qf.data.fallback_regop = &exec_regop<fbni->op_index, I>;
            if constexpr (has_continue)
                qf.data.next_start = static_cast<state_offset_t>(1 + DFA.continue_nodes[fbni->continue_at]);
            if constexpr (has_extra_continue)
                qf.data.nonempty_next_start = static_cast<state_offset_t>(1 + DFA.additional_continue_nodes[fbni->continue_at]);
        }

        return qf;
    }


    /* table definitions */

    template<typename I>
    static constexpr auto regop_table = [] consteval {
        if constexpr (DFA.regops.empty())
        {
            return terminal_object{};
        }
        else
        {
            constexpr auto [...Blks] = std::make_index_sequence<DFA.regops.size()>();
            return std::array{ &exec_regop<tdfa::no_transition_regops, I>,  (&exec_regop<Blks, I>)... };
        }
    }();

    static constexpr auto transition_table = [] consteval {
        constexpr auto [...Idxs] = std::make_index_sequence<DFA.nodes.size()>();
        return std::array{ table_row{},  (make_transition_row(Idxs))... };
    }();

    static constexpr auto fallback_bitset = [] consteval {
        std::bitset<DFA.nodes.size() + 1> bitset;
        for (const auto& [q, _] : DFA.fallback_nodes)
            bitset[q + 1] = true;
        return bitset;
    }();

    template<typename I>
    static constexpr auto accepting_state_table = [] consteval {
        if constexpr (accepting_state_empty)
        {
            return terminal_object{};
        }
        else
        {
            constexpr auto [...Idxs] = std::make_index_sequence<DFA.final_nodes.size()>();
            return std::array{ accepting_state<I>{}, (make_accepting_state<Idxs, I>())... };
        }
    }();

    static constexpr auto state_table = [] consteval {
        if constexpr (accepting_state_empty)
        {
            std::bitset<DFA.nodes.size() + 1> bitset;
            for (const auto& [q, _] : DFA.final_nodes)
                bitset[q + 1] = true;
            return bitset;
        }
        else
        {
            std::array<state_offset_t, DFA.nodes.size() + 1> array{};
            std::size_t i{ 0 };
            for (const auto& [q, _] : DFA.final_nodes)
                array[q + 1] = static_cast<state_offset_t>(++i);
            return array;
        }
    }();


    /* matcher implementation */

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr bool regex_state(context<I> ctx, I it, const S end, state_offset_t state)
    {
        const maybe_saved_first_t<I> first{ it };
        maybe_fallback_t<I> fallback{ it };

        for (; it != end; ++it)
        {
            if constexpr (has_fallback)
            {
                if (fallback_bitset[state])
                {
                    fallback.state = state;
                    fallback.it = it;
                }
            }

            const auto& [next, regop] = transition_table[state][static_cast<uchar_type>(*it)];

            if (next == 0) [[unlikely]]
                break;

            /* N.B: this probably benefits from a two pass implementation
               I expect this will be slower than the other "linear" dfa implementation
               because we need to chase pointers here */
            if constexpr (has_regops)
                if (regop != 0)
                    (regop_table<I>[regop])(ctx, it);

            state = next;
        }

        if (it == end or (has_fallback and fallback_bitset[state]))
        {
            if constexpr (accepting_state_empty)
            {
                if (state_table[state])
                {
                    ctx.res.match_end_ = it;
                    return true;
                }
            }
            else
            {
                if (const auto index{ state_table[state] }; index != 0)
                {
                    const accepting_state<I>& qf{ accepting_state_table<I>[index] };

                    /* assume qf.data.final_regop != nullptr */
                    if constexpr (has_regops)
                        (qf.data.final_regop)(ctx, it);

                    update_ctx(ctx, it, qf, first);
                    return true;
                }
            }
        }

        /* compute and exec fallback state */
        if constexpr (has_fallback)
        {
            if (fallback.state != fallback_disabled)
            {
                if constexpr (accepting_state_empty)
                {
                    ctx.res.match_end_ = fallback.it;
                    return true;
                }
                else
                {
                    /* since this is a fallback, assume index != 0 */
                    const auto index{ state_table[fallback.state] };
                    const accepting_state<I>& qf{ accepting_state_table<I>[index] };

                    /* assume qf.data.fallback_regop != nullptr */
                    if constexpr (has_regops)
                        (qf.data.fallback_regop)(ctx, fallback.it);

                    update_ctx(ctx, fallback.it, qf, first);
                    return true;
                }
            }
        }

        return false;
    }

    template<std::bidirectional_iterator I>
    [[gnu::always_inline]] static constexpr void update_ctx(context<I> ctx, const I it, const accepting_state<I>& qf, const maybe_saved_first_t<I> first)
    {
        if constexpr (largest_offset > 0)
            ctx.res.match_end_ = std::ranges::prev(it, qf.data.offset);
        else
            ctx.res.match_end_ = it;

        if constexpr (not std::contiguous_iterator<I>)
            ctx.res.match_success_ = true;

        if constexpr (has_continue)
        {
            if constexpr (has_extra_continue)
            {
                if (first == it)
                    ctx.osr.nst = qf.data.nonempty_next_start;
                else
                    ctx.osr.nst = qf.data.next_start;
            }
            else
            {
                ctx.osr.nst = qf.data.next_start;
            }
        }

        if constexpr (has_alternative)
            ctx.osr.alt = qf.data.alternative;
    }


public:
    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr result<I> operator()(const I first, const S last)
    {
        result<I> res{ first };
        overspill osp{};
        regex_state(context<I>{ res, osp }, first, last, 1 + DFA.match_start);
        return res;
    }
};


template<std::meta::info Info>
template<std::bidirectional_iterator I>
    requires std::is_nothrow_convertible_v<std::iter_value_t<I>, typename table_dfa<Info>::char_type>
struct table_dfa<Info>::iterated_result
{
    static constexpr bool needs_begin{ not never_empty };

    struct state_type
    {
        static constexpr bool has_continue{ true };
        static constexpr bool is_stateless{ false };

        using continue_type = state_offset_t;

        [[no_unique_address]] continue_type continue_at{ 0 };
    };

    iterated_result() = default;

    constexpr iterated_result(const I first, const std::sentinel_for<I> auto last)
        : res{ first }, stf{ .continue_at = 1 + DFA.match_start }
    {
        resume(first, last);
    }

    constexpr iterated_result(const I first, const std::sentinel_for<I> auto last, match_non_empty_t)
        : res{ first }, stf{ .continue_at = 1 + (never_empty ? DFA.match_start : DFA.additional_continue_nodes.back()) }
    {
        resume(first, last);
    }

    constexpr iterated_result(const I /* first */, const I it, const std::sentinel_for<I> auto last, state_type stf, bool /* prev_empty */)
        : res{ it }, stf{ stf }
    {
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
            if (match.empty() and current == last)
            {
                res.clear_match();
                return current;
            }

            if (current == first)
                stf.continue_at = 1 + (never_empty ? DFA.match_start : DFA.additional_continue_nodes.back());
        }

        res.reset(current);
        resume(current, last);
        return current;
    }

private:
    constexpr void resume(const I first, const std::sentinel_for<I> auto last)
    {
        overspill osp{};
        regex_state(context<I>{ res, osp }, first, last, stf.continue_at);
        stf.continue_at = osp.nst;
        return;
    }

public:
    result<I> res;
    state_type stf;
};


template<string_literal Pattern, fsm_flags Flags>
using table_dfa_matcher = table_dfa<(^^re<std::meta::reflect_constant_string(Pattern.view()), pack_flags(Flags)>)>;

} // namepace detail
} // namepace srx