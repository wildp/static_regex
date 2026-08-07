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
    static_assert(template_of(type_of(Info)) == ^^tdfa_info);

    using char_type = typename [: template_arguments_of(type_of(Info))[0] :];
    using uchar_type = std::make_unsigned_t<char>;

    static_assert(sizeof(char_type) == 1);

private:
    static constexpr tdfa_info<char_type> DFA = [: Info :];
    static_assert(not DFA.onepass, "Invalid reflection value: use p1306dfa instead");

    static constexpr bool never_empty{ DFA.additional_continue_nodes.empty() };
    static constexpr bool has_fallback{ DFA.flags.enable_fallback };
    static constexpr bool has_continue{ DFA.flags.is_iterator };
    static constexpr bool has_alternative{ DFA.alt_mode };
    static constexpr bool has_xcontinue{ has_continue and not never_empty };
    static constexpr bool has_backlinks{ not DFA.flags.return_bool and DFA.tag_count > 0 };
    static constexpr auto largest_offset{ DFA.largest_offset() };
    static constexpr auto largest_alternative{ DFA.largest_alt() };

    static constexpr std::size_t actual_length{ 0 /* TODO: calculate maximum length, inclusive of assertions extending beyond the end of patterns */ };
    static constexpr std::size_t backlink_array_count{ DFA.backlink_arrays.size() };

    using state_index_t = [: smallest_integer_type(DFA.nodes.size() + 1) :];
    using backlink_index_t = [: smallest_integer_type(backlink_array_count) :]; // TODO: add maybe_type_t of this to disable

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
    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    using result = static_match_results<I, smr_layout::reg_id<DFA.register_count, DFA.captures>>;

    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    struct iterated_result;

    // TODO: depending on pattern length, replace with a circular buffer?
    using backlink_buffer_t = std::vector<backlink_index_t>;

private:
    using maybe_buf_t = maybe_type_t<has_backlinks, backlink_buffer_t&>;

    template<typename I>
    using result_ref = std::add_lvalue_reference_t<result<I>>;

    template<typename I>
    using iterated_result_ref = std::add_lvalue_reference_t<iterated_result<I>>;


    template<typename I>
    struct fallback_info
    {
        I it;
        state_index_t state{ 0 };

        constexpr explicit(false) fallback_info(I it) : it{ it } {}
    };

    template<typename I>
    using maybe_fallback_t = maybe_type_t<has_fallback, fallback_info<I>>;


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


    /* table contents */

    // TODO: benchmark AoS vs SoA?
    struct table_entry
    {
        state_index_t                                                       state;
        [[no_unique_address]] maybe_type_t<has_backlinks, backlink_index_t> backlink;
    };

    struct table_row
    {
        static constexpr std::size_t row_size{ 0b1 << std::numeric_limits<uchar_type>::digits };

        // NOLINTBEGIN(*-pro-bounds-constant-array-index)
        constexpr table_entry& operator[](uchar_type n) noexcept { return data_[n]; }
        constexpr const table_entry& operator[](uchar_type n) const noexcept { return data_[n]; }
        // NOLINTEND(*-pro-bounds-constant-array-index)

        table_row() = default;

        /* note: we use a C-array because it's a structural type, which std::array is not */
        table_entry data_[row_size]{}; // NOLINT(*-avoid-c-arrays)
    };


    struct accepting_state
    {
        using ofs_t = [: smallest_integer_type(largest_offset) :];
        using alt_t = [: smallest_integer_type(largest_alternative) :];

        // [[no_unique_address]] maybe_type_t<(largest_offset > 0), ofs_t>     offset{ 0 };
        // [[no_unique_address]] maybe_type_t<has_backlinks, backlink_index_t> backlink{ 0 };
        // [[no_unique_address]] maybe_type_t<has_continue, state_index_t>     nstart{ 0 };
        // [[no_unique_address]] maybe_type_t<has_xcontinue, state_index_t>    nenstart{ 0 };
        // [[no_unique_address]] maybe_type_t<has_alternative, alt_t>          resalt{ 0 };
        // bool                                                                accept{ false };

        struct contents_type;

        consteval {
            /* note: using maybe_type_t resulted in structs with unwanted padding */
            std::vector<std::meta::info> mems;
            if (largest_offset > 0)
                mems.emplace_back(data_member_spec(^^ofs_t, { .name = "offset" }));
            if (has_backlinks)
                mems.emplace_back(data_member_spec(^^backlink_index_t, { .name = "backlink" }));
            if (has_continue)
                mems.emplace_back(data_member_spec(^^state_index_t, { .name = "nstart" }));
            if (has_xcontinue)
                mems.emplace_back(data_member_spec(^^state_index_t, { .name = "nenstart" }));
            if (has_alternative)
                mems.emplace_back(data_member_spec(^^state_index_t, { .name = "resalt" }));
            mems.emplace_back(data_member_spec(^^bool, { .name = "accept" }));
            define_aggregate(^^contents_type, mems);
        }

        contents_type data{};
        constexpr explicit operator bool() const noexcept { return data.accept; }
    };

    static constexpr bool accepting_state_empty{ not ((largest_offset > 0) or has_backlinks or has_continue or has_xcontinue or has_alternative) };


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
                        .state    = static_cast<state_index_t>(tr.next + 1),
                        .backlink = static_cast<backlink_index_t>(tr.op_index)
                    };

                    if (c == static_cast<uchar_type>(hi))
                        break;
                }
            }
        }

        return row;
    }

    static consteval accepting_state make_accepting_state(std::size_t q)
    {
        const auto* const fni = DFA.final_nodes.at_if(q);

        accepting_state as{};

        if (fni == nullptr)
            return as;

        const auto* const fbni = DFA.fallback_nodes.at_if(q);

        const auto cont{ (fbni != nullptr and has_continue) ? (1 + DFA.continue_nodes[fbni->continue_at]) : 0uz };
        const auto xcont{ (fbni != nullptr and has_xcontinue) ? (1 + DFA.additional_continue_nodes[fbni->continue_at]) : 0uz };

        // return accepting_state{
        //     .offset   = static_cast<accepting_state::ofs_t>(fni->offset),
        //     .backlink = static_cast<backlink_index_t>(fni->op_index),
        //     .nstart   = static_cast<state_index_t>(cont),
        //     .nenstart = static_cast<state_index_t>(xcont),
        //     .resalt   = static_cast<accepting_state::alt_t>(fni->alternative),
        //     .accept   = true
        // };

        if constexpr (largest_offset > 0)
            as.data.offset = static_cast<accepting_state::ofs_t>(fni->offset);
        if constexpr (has_backlinks)
            as.data.backlink = static_cast<backlink_index_t>(fni->op_index);
        if constexpr (has_continue)
            as.data.nstart = static_cast<state_index_t>(cont);
        if constexpr (has_xcontinue)
            as.data.nenstart = static_cast<state_index_t>(xcont);
        if constexpr (has_alternative)
            as.data.resalt = static_cast<accepting_state::alt_t>(fni->alternative);
        as.data.accept = true;

        return as;
    }


    /* table definitions */

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
            constexpr auto [...Idxs] = std::make_index_sequence<DFA.nodes.size()>();
            return std::array{ accepting_state{},  (make_accepting_state(Idxs))... };
        }
    }();


    /* backwards pass */

    using bitset_type = std::bitset<DFA.tag_count>;

    template<std::size_t IdxA, std::size_t IdxB, std::bidirectional_iterator I, int X>
    static constexpr std::size_t assign_tags(context<I, X> ctx, bitset_type& assigned, I it)
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
    static constexpr std::size_t assign_tags_dispatch(context<I, X> ctx, bitset_type& assigned, I it, backlink_index_t outer, std::size_t index)
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
                        return assign_tags<idxa, idxb>(ctx, assigned, it);

                std::unreachable();
            }
        }

        std::unreachable();
    }

    template<std::bidirectional_iterator I, int X>
    static constexpr bool backwards_pass(context<I, X> ctx, backlink_buffer_t& backlinks, I it, backlink_index_t back)
        requires (X != 0)
    {
        bitset_type assigned{};
        std::size_t index{ 0 };

        while (true)
        {
            index = assign_tags_dispatch(ctx, assigned, it, back, index);

            if (assigned.all())
                break;

            back = backlinks.back();
            backlinks.pop_back();
            --it;
        }

        return true;
    }


    /* matcher implementation */

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, int X>
    static constexpr bool forwards_pass(context<I, X> ctx, maybe_buf_t backlinks, I it, const S end, state_index_t state)
    {
        maybe_fallback_t<I> fallback{ it };

        while (true)
        {
            if (it == end) [[unlikely]]
            {
                if (not state_table[state])
                {
                    if constexpr (not has_fallback)
                        return false;
                    else
                        state = 0; /* fallback */
                }
                break;
            }

            if constexpr (has_fallback)
            {
                if (fallback_bitset[state])
                {
                    fallback.state = state;
                    if constexpr (not DFA.flags.return_bool)
                        fallback.it = it;
                }
            }

            const auto& [next, backlink] = transition_table[state][static_cast<uchar_type>(*it)];

            if (next == 0) [[unlikely]]
            {
                if constexpr (not has_fallback)
                    return false;
                else
                    if (not fallback_bitset[state])
                        state = 0; /* fallback */
                break;
            }

            if constexpr (has_backlinks)
                backlinks.push_back(backlink);

            state = next;
            ++it;
        }

        /* process fallback */

        if constexpr (has_fallback)
        {
            if (state == 0)
            {
                if (not fallback_bitset[state])
                {
                    if (fallback.state == 0)
                        return false;

                    if constexpr (has_backlinks)
                    {
                        std::ptrdiff_t to_erase{ std::ranges::distance(fallback.it, it) };

                        /* this is always true since we only ever call this function
                            when the current state itself is not a fallback state */
                        [[assume(to_erase > 0)]];

                        while (to_erase-- > 0)
                            backlinks.pop_back();
                    }

                    it = fallback.it;
                    state = fallback.state;
                }

                if (not state_table[state])
                    return false;
            }
        }


        if constexpr (accepting_state_empty)
        {
            if constexpr (X != 0)
                update_ctx(ctx, backlinks, it);
        }
        else
        {
            const accepting_state& as{ state_table[state] };

            if (not as)
                return false;

            if constexpr (has_fallback)
                if (not (fallback_bitset[state] or it == end))
                    return false;

            if constexpr (X != 0)
                update_ctx(ctx, backlinks, it, as);
        }

        return true;
    }

    template<std::bidirectional_iterator I, int X>
        requires (X != 0)
    static constexpr void update_ctx(context<I, X> ctx, maybe_buf_t /* backlinks */, I it)
    {
        ctx.get_res().match_end_ = it;

        if constexpr (not std::contiguous_iterator<I>)
            ctx.get_res().match_success_ = true;
    }

    template<std::bidirectional_iterator I, int X>
        requires (X != 0)
    static constexpr void update_ctx(context<I, X> ctx, maybe_buf_t backlinks, I it, const accepting_state& as)
    {
        if constexpr (largest_offset > 0)
            ctx.get_res().match_end_ = std::ranges::prev(it, as.data.offset);
        else
            ctx.get_res().match_end_ = it;

        if constexpr (not std::contiguous_iterator<I>)
            ctx.get_res().match_success_ = true;

        if constexpr (X == 2 and has_continue and not has_xcontinue)
            ctx.get_stf().continue_at = as.data.nstart;

        // if constexpr (X == 2 and has_alternative)
        //     ctx.get_stf().alt = qf.data.alternative;

        if constexpr (has_backlinks)
            backwards_pass(ctx, backlinks, it, as.data.backlink);

        if constexpr (X == 2 and has_continue and has_xcontinue)
            ctx.get_stf().continue_at = (ctx.get_res().template force_get<0>().empty()) ? as.data.nenstart : as.data.nstart;
    }


public:
    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires (DFA.flags.return_bool)
    static constexpr bool operator()(const I first, const S last)
    {
        return forwards_pass(context<I>{}, terminal_object{}, first, last, 1 + DFA.match_start);
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires (not DFA.flags.return_bool)
    static constexpr result<I> operator()(const I first, const S last)
    {
        result<I> res{ first };
        backlink_buffer_t buf{};
        forwards_pass(context{ res }, buf, first, last, 1 + DFA.match_start);
        return res;
    }
};


template<std::meta::info Info>
template<std::bidirectional_iterator I>
    requires std::is_nothrow_convertible_v<std::iter_value_t<I>, typename table_dfa<Info>::char_type>
struct table_dfa<Info>::iterated_result
{
    static constexpr bool needs_begin{ false };

    struct state_type
    {
        static constexpr bool has_continue{ true };
        static constexpr bool is_stateless{ false };

        using continue_type = state_index_t;

        // TODO: add alternative_number when is_regex is true and there is more than one alt
        continue_type continue_at{ 0 };
    };

    iterated_result() = default;

    constexpr iterated_result(const I first, const std::sentinel_for<I> auto last)
        : res{ first }, stf{ .continue_at = 1 + DFA.match_start }
    {
        if constexpr (never_empty)
        {
            resume(first, last);
        }
        else
        {
            if (resume(first, last))
                if (res.template force_get<0>().empty())
                    stf.continue_at = 1 + DFA.additional_continue_nodes.back();
        }
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
    {
        const auto& match = res.template force_get<0>();
        const I current = match.end();
        res.reset(current);
        resume(current, last);
        return current;
    }

    constexpr I advance(const I first, const std::sentinel_for<I> auto last) = delete;

private:
    constexpr bool resume(const I first, const std::sentinel_for<I> auto last)
    {
        backlink_buffer_t buf{};
        return forwards_pass(context{ *this }, buf, first, last, stf.continue_at);
    }

public:
    result<I> res;
    state_type stf;
};


template<string_literal Pattern, fsm_flags Flags>
using table_dfa_matcher = table_dfa<(^^rf<std::meta::reflect_constant_string(Pattern.view()), pack_flags(Flags)>)>;

} // namepace detail
} // namepace srx