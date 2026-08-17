// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <bit>
#include <bitset>
#include <cstddef>
#include <iterator>
#include <limits>
#include <meta>
#include <ranges>
#include <type_traits>

#include "srx/etc/static_charset.hpp"
#include "srx/etc/string_literal.hpp"
#include "srx/etc/util.hpp"
#include "srx/fsm/flags.hpp"
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
    static constexpr bool has_backlinks{ not DFA.flags.return_bool and DFA.tag_count > 0 };
    static constexpr bool has_finite_length{ DFA.backlink_buf_size != std::numeric_limits<std::size_t>::max() };
    static constexpr auto largest_offset{ DFA.largest_offset() };
    static constexpr auto largest_alternative{ DFA.largest_alt() };
    static constexpr auto char_size{ 0b1uz << std::numeric_limits<uchar_type>::digits };

    static constexpr std::size_t backlink_array_count{ DFA.backlink_arrays.size() };

    using blidx_t = [: smallest_integer_type(backlink_array_count) :];

public:
    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    using result = static_match_results<I, smr_layout::reg_id<DFA.register_count, DFA.captures>>;

    template<std::bidirectional_iterator I>
        requires std::is_nothrow_convertible_v<std::iter_value_t<I>, char_type>
    struct iterated_result;

    using backlink_buffer_t = std::conditional_t<has_finite_length, backlink_hist<blidx_t, std::bit_width(DFA.backlink_buf_size)>, std::vector<blidx_t>>;

private:
    using maybe_buf_t = maybe_type_t<has_backlinks, backlink_buffer_t&>;

    /* table contents */

    static constexpr auto make_partitions()
    {
        // TODO: it might be better to retain the original bitcharsets?
        using charset_type = charset<char_type>;
        const auto entire = ~charset_type{};

        std::vector<charset_type> css;
        for (const auto& nodes : DFA.nodes)
            for (const auto& tr : nodes)
                css.emplace_back(tr.cs);

        auto refs = css | std::views::transform([](const auto& x){ return std::cref(x); }) | std::ranges::to<std::vector>();
        refs.emplace_back(std::cref(entire));
        auto result = charset_type::partition(refs);
        return std::define_static_array(result | std::views::transform([](const auto& x){ return static_charset{ x }; }));
    }

    static constexpr std::span partitions = make_partitions();

    struct table_row
    {
        static constexpr std::size_t size{ partitions.size() };
        static_assert(size > 0);

        // std::array<table_row*, size> next{};

        table_row() = default;

        /* note: we use a C-array because it's a structural type, which std::array is not */
        table_row* next[size]{}; // NOLINT(*-avoid-c-arrays)
    };

    struct table
    {
        static constexpr std::size_t table_size{ DFA.nodes.size() };

        constexpr table() noexcept
        {
            for (std::size_t i{ 0 }; i < table_size; ++i)
            {
                for (const auto& [j, part] : partitions | std::views::enumerate)
                {
                    for (const auto& tr : DFA.nodes[i] | std::views::reverse)
                    {
                        if ((part & tr.cs).empty())
                            continue;

                        transitions[i].next[j] = &transitions[tr.next];

                        if constexpr (has_backlinks)
                            backlinks[i][j] = static_cast<blidx_t>(tr.op_index);
                    }
                }
            }
        }


        using transition_table_t = std::array<table_row, table_size>;
        using backlink_table_t   = std::array<std::array<blidx_t, table_row::size>, table_size>;
        using maybe_backlinks_t  = maybe_type_t<has_backlinks, backlink_table_t>;

        transition_table_t transitions{};
        [[no_unique_address]] maybe_backlinks_t backlinks{};
    };


    struct accepting_state
    {
        using ofs_t = [: smallest_integer_type(largest_offset) :];
        using con_t = [: smallest_integer_type(DFA.continue_nodes.size()) :];
        using alt_t = [: smallest_integer_type(largest_alternative) :];

        struct contents_type;

        consteval {
            /* note: using maybe_type_t resulted in structs with unwanted padding */
            std::vector<std::meta::info> mems;
            if (largest_offset > 0)
                mems.emplace_back(data_member_spec(^^ofs_t, { .name = "offset" }));
            if (has_backlinks)
                mems.emplace_back(data_member_spec(^^blidx_t, { .name = "backlink" }));
            if (has_continue)
                mems.emplace_back(data_member_spec(^^con_t, { .name = "contat" }));
            if (has_alternative)
                mems.emplace_back(data_member_spec(^^alt_t, { .name = "resalt" }));
            define_aggregate(^^contents_type, mems);
        }

        contents_type data{};
        constexpr explicit operator bool() const noexcept { return data.accept; }
    };

    static constexpr bool accepting_state_empty{ not ((largest_offset > 0) or has_backlinks or has_continue or has_alternative) };


    /* helpers for table construction */

    static consteval auto make_partition_table()
    {
        using result_type = [: smallest_integer_type(partitions.size()) :];
        static_assert(std::same_as<result_type, std::uint_least8_t>);
        std::array<std::uint_least8_t, char_size> result{};

        for (const auto& [i, cs] : partitions | std::views::enumerate)
        {
            if (i == 0)
                continue;

            for (const auto& [lo, hi] : cs.get_intervals())
            {
                for (uchar_type c{ static_cast<uchar_type>(lo) }; true; ++c)
                {
                    result[c] = static_cast<std::uint_least8_t>(i);

                    if (c == static_cast<uchar_type>(hi))
                        break;
                }
            }
        }

        return result;
    }

    static consteval accepting_state make_accepting_state(std::size_t q)
    {
        const auto* const fni = DFA.final_nodes.at_if(q);

        accepting_state as{};

        if (fni == nullptr)
            return as;

        const auto* const fbni = DFA.fallback_nodes.at_if(q);

        const auto cont{ (fbni != nullptr and has_continue) ? fbni->continue_at : 0uz };

        if constexpr (largest_offset > 0)
            as.data.offset = static_cast<accepting_state::ofs_t>(fni->offset);
        if constexpr (has_backlinks)
            as.data.backlink = static_cast<blidx_t>(fni->op_index);
        if constexpr (has_continue)
            as.data.contat = static_cast<accepting_state::con_t>(cont);
        if constexpr (has_alternative)
            as.data.resalt = static_cast<accepting_state::alt_t>(fni->alternative);

        return as;
    }


    /* table definitions */

    static constexpr auto partition_table = make_partition_table();

    static constexpr table transition_table{};

    static constexpr auto final_bitset = [] consteval {
        std::bitset<DFA.nodes.size()> bitset;
        for (const auto& [q, _] : DFA.final_nodes)
            bitset[q] = true;
        return bitset;
    }();

    static constexpr auto fallback_bitset = [] consteval {
        std::bitset<DFA.nodes.size()> bitset;
        for (const auto& [q, _] : DFA.fallback_nodes)
            bitset[q] = true;
        return bitset;
    }();

    static constexpr auto state_table = [] consteval {
        constexpr auto [...Idxs] = std::make_index_sequence<DFA.nodes.size()>();
        return std::array{ (make_accepting_state(Idxs))... };
    }();


    /* types for forward/backward passes */

    template<typename I>
    struct fwd_result
    {
        using success_type     = maybe_type_t<std::contiguous_iterator<I>, bool>;
        using backlink_type    = maybe_type_t<has_backlinks, blidx_t>;
        using offset_type      = maybe_type_t<(largest_offset > 0), typename accepting_state::ofs_t>;
        using continue_type    = maybe_type_t<has_continue, typename accepting_state::con_t>;
        using alternative_type = maybe_type_t<has_alternative, typename accepting_state::alt_t>;

        I it;
        [[no_unique_address]] success_type     success;
        [[no_unique_address]] offset_type      offset;
        [[no_unique_address]] backlink_type    backlink;
        [[no_unique_address]] continue_type    continue_at;
        [[no_unique_address]] alternative_type alternative;

        constexpr fwd_result() noexcept
            : success{ false } {}

        constexpr fwd_result() noexcept
            requires std::contiguous_iterator<I>
            : it{} {}

        constexpr fwd_result(I iterator) noexcept(std::is_nothrow_move_constructible_v<I>)
            requires (not (has_backlinks or has_continue or has_alternative))
            : it{ std::move(iterator) }, success{ true } {}

        constexpr fwd_result(I iterator, const accepting_state& as)
            noexcept(std::is_nothrow_move_constructible_v<I> and largest_offset == 0)
            : it{ std::move(iterator) }, success{ true }
        {
            if constexpr (largest_offset > 0)
                offset = as.data.offset;
            if constexpr (has_backlinks)
                backlink = as.data.backlink;
            if constexpr (has_continue)
                continue_at = as.data.contat;
            if constexpr (has_alternative)
                alternative = as.data.resalt;
        }

        [[nodiscard]] constexpr bool has_value() const noexcept
        {
            if constexpr (std::contiguous_iterator<I>)
                return std::to_address(it) != std::to_address(I{});
            else
                return success;
        }

        constexpr operator bool() const noexcept
        {
            return has_value();
        }

        constexpr void reset()
        {
            if constexpr (std::contiguous_iterator<I>)
                it = std::to_address(I{});
            else
                success = false;
        }

    };

    template<typename I>
    struct bwd_result
    {
        /* store results */
        std::add_lvalue_reference_t<result<I>> res;
    };

    template<typename I>
    bwd_result(result<I>&) -> bwd_result<I>;


    /* backward pass */

    using bitset_type = std::bitset<DFA.tag_count>;

    template<std::size_t IdxA, std::size_t IdxB, std::bidirectional_iterator I>
    static constexpr std::size_t assign_tags(bwd_result<I> ctx, bitset_type& assigned, I it)
    {
        template for (constexpr auto tag : DFA.backlink_arrays[IdxA][IdxB].tag_seq | std::views::reverse)
        {
            if constexpr (tag > 0)
            {
                static constexpr std::size_t tag_position{ tag - 1 };
                if (not std::as_const(assigned)[tag_position])
                {
                    assigned[tag_position] = true;

                    ctx.res.reg_[tag_position] = it;
                    if constexpr (not std::contiguous_iterator<I>)
                        ctx.res.enabled_[tag_position] = true;
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

    template<std::bidirectional_iterator I>
    static constexpr std::size_t assign_tags_dispatch(bwd_result<I> ctx, bitset_type& assigned, I it, blidx_t outer, std::size_t index)
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

    template<std::bidirectional_iterator I>
    static constexpr void backward_pass(bwd_result<I> ctx, maybe_buf_t backlinks, const fwd_result<I>& fwd)
    {
        if constexpr (largest_offset > 0)
            ctx.res.match_end_ = std::ranges::prev(fwd.it, fwd.offset);
        else
            ctx.res.match_end_ = fwd.it;

        if constexpr (not std::contiguous_iterator<I>)
            ctx.res.match_success_ = true;

        if constexpr (has_backlinks)
        {
            blidx_t back{ fwd.backlink };
            I it{ fwd.it };
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
        }
    }


    /* forward pass */

    template<typename I>
    struct fallback_info
    {
        I it;
        const table_row* state_ptr{ nullptr };

        constexpr explicit(false) fallback_info(I it) : it{ it } {}
    };

    template<typename I>
    using maybe_fallback_t = maybe_type_t<has_fallback, fallback_info<I>>;


    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    static constexpr fwd_result<I> forward_pass(maybe_buf_t backlinks, I it, const S end, const std::size_t start_state)
    {
        maybe_fallback_t<I> fallback{ it };

        const table_row* state_ptr{ &transition_table.transitions[start_state] };
        blidx_t backlink{};

        while (true)
        {
            const auto state{ state_ptr - &transition_table.transitions[0] };

            if (it == end) [[unlikely]]
            {
                if (not final_bitset[state])
                {
                    if constexpr (not has_fallback)
                        return {};
                    else
                        state_ptr = nullptr;
                }
                break;
            }

            const auto part = partition_table[static_cast<uchar_type>(*it)];
            const table_row* next_ptr{ state_ptr->next[part] };

            if constexpr (has_backlinks)
                backlink = transition_table.backlinks[state][part];

            if constexpr (has_fallback)
            {
                // TODO: improve this code here?
                // note: if we record states, we don't need this at all -> 1 fewer branch
                if (fallback_bitset[state])
                {
                    fallback.state_ptr = state_ptr;
                    if constexpr (not DFA.flags.return_bool)
                        fallback.it = it;
                }
            }

            if (next_ptr == nullptr) [[unlikely]]
            {
                if constexpr (not has_fallback)
                    return {};
                else
                    if (not fallback_bitset[state])
                        state_ptr = nullptr;
                break;
            }

            ++it;
            state_ptr = next_ptr;

            if constexpr (has_backlinks)
                backlinks.push_back(backlink);
        }

        /* process fallback */

        if constexpr (has_fallback)
        {
            if (state_ptr == nullptr)
            {
                if (fallback.state_ptr == nullptr)
                    return {};

                if constexpr (has_backlinks)
                {
                    std::ptrdiff_t to_erase{ std::ranges::distance(fallback.it, it) };

                    /* this is always true since we only ever call this function
                        when the current state itself is not a fallback state */
                    [[assume(to_erase > 0)]];

                    while (to_erase-- > 0)
                        backlinks.pop_back();
                }

                state_ptr = fallback.state_ptr;
                if constexpr (not DFA.flags.return_bool)
                    it = fallback.it;

                /* assume state_table[state] now holds */
            }
        }

        if constexpr (DFA.flags.return_bool or accepting_state_empty)
            return fwd_result{ it };
        else
            return fwd_result{ it, state_table[state_ptr - &transition_table.transitions[0]] };
    }


public:
    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires (DFA.flags.return_bool)
    static constexpr bool operator()(const I first, const S last)
    {
        return forward_pass(terminal_object{}, first, last, DFA.match_start);
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires (not DFA.flags.return_bool)
    static constexpr result<I> operator()(const I first, const S last)
    {
        result<I> res{ first };
        backlink_buffer_t buf{};

        if (const auto& fwd = forward_pass(buf, first, last, DFA.match_start))
            backward_pass(bwd_result{ res }, buf, fwd);
        return res;
    }
};


template<std::meta::info Info>
template<std::bidirectional_iterator I>
    requires std::is_nothrow_convertible_v<std::iter_value_t<I>, typename table_dfa<Info>::char_type>
struct table_dfa<Info>::iterated_result
{
    static constexpr bool needs_begin{ not never_empty };

    using buffer_type = maybe_type_t<has_backlinks, backlink_buffer_t>;
    using continue_type = fwd_result<I>::continue_type;

    iterated_result() = default;

    template<std::sentinel_for<I> S>
    constexpr iterated_result(const I first, const S last)
        : cached_res{ first }
    {
        start(first, last);
    }

    template<std::sentinel_for<I> S>
    constexpr iterated_result(const I first, const S last, match_non_empty_t)
        : cached_res{ first }
    {
        start(first, last, true);
    }

    template<std::sentinel_for<I> S>
    constexpr iterated_result(const I /* first */, const I it, const S last, continue_type cont, bool prev_empty)
        : cached_res{ it }
    {
        fwd_res.continue_at = cont;

        const bool nonempty = [&]{
            if constexpr (not never_empty)
                return prev_empty;
            return false;
        }();

        resume(it, last, nonempty);
    }

    constexpr operator bool() const noexcept
    {
        return fwd_res.has_value();
    }

    constexpr const result<I>& operator*() const
    {
        if (not cache_engaged_)
        {
            if (fwd_res)
                backward_pass(bwd_result{ cached_res }, buf, fwd_res);
            cache_engaged_ = true;
        }

        return cached_res;
    }

    constexpr continue_type get_continue() const noexcept
    {
        return fwd_res.continue_at;
    }

    template<std::sentinel_for<I> S>
    constexpr I advance(const S last)
        requires never_empty
    {
        I current{ fwd_res.it };

        if constexpr (largest_offset > 0)
            std::ranges::advance(current, -fwd_res.offset);

        cached_res.reset(current);
        resume(current, last);
        return current;
    }

    template<std::sentinel_for<I> S>
    constexpr I advance(const I first, const S last)
    {
        I current{ fwd_res.it };

        if constexpr (largest_offset > 0)
            std::ranges::advance(current, -fwd_res.offset);

        if constexpr (not never_empty)
        {
            const auto& match = cached_res.template force_get<0>();

            if (match.empty())
            {
                if (current == last)
                {
                    fwd_res.reset();
                    cached_res.clear_match();
                    return current;
                }

                cached_res.reset(current);

                if (current == first)
                    start(current, last, true);
                else
                    resume(current, last, true);
                return current;
            }
        }

        cached_res.reset(current);
        resume(current, last);
        return current;
    }

    constexpr iterated_result(const iterated_result&) = delete;
    constexpr iterated_result(iterated_result&&) noexcept = default;
    constexpr iterated_result& operator=(const iterated_result&) = delete;
    constexpr iterated_result& operator=(iterated_result&&) noexcept = default;
    constexpr ~iterated_result() = default;

private:
    template<std::sentinel_for<I> S>
    constexpr bool start(const I first, const S last, const bool nonempty_match = false)
    {
        const std::size_t start_state = [&](){
            if constexpr (not never_empty)
                if (nonempty_match)
                    return DFA.additional_continue_nodes.back();
            return DFA.match_start;
        }();

        if constexpr (has_backlinks and not has_finite_length)
            buf.clear();

        fwd_res = forward_pass(buf, first, last, start_state);
        cache_engaged_ = false;
        return fwd_res;
    }

    template<std::sentinel_for<I> S>
    constexpr bool resume(const I first, const S last, const bool nonempty_match = false)
    {
        if constexpr (not has_continue)
        {
            return start(first, last, nonempty_match);
        }
        else
        {
            const std::size_t resume_state = [&]{
                if constexpr (not never_empty)
                    if (nonempty_match)
                        return DFA.additional_continue_nodes[fwd_res.continue_at];
                return DFA.continue_nodes[fwd_res.continue_at];
            }();

            fwd_res.reset();
            if constexpr (has_backlinks and not has_finite_length)
                buf.clear();

            fwd_res = forward_pass(buf, first, last, resume_state);
            cache_engaged_ = false;
            return fwd_res.has_value();
        }
    }

    fwd_result<I> fwd_res;
    mutable result<I> cached_res;
    [[no_unique_address]] mutable buffer_type buf;
    mutable bool cache_engaged_{ false };
};


template<string_literal Pattern, fsm_flags Flags>
using table_dfa_matcher = table_dfa<(^^rf<std::meta::reflect_constant_string(Pattern.view()), pack_flags(Flags)>)>;

} // namepace detail
} // namepace srx