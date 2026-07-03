// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <bitset>
#include <limits>
#include <meta>
#include <ranges>


namespace srx::detail {

struct fsm_flags
{
    bool is_search       : 1;
    bool longest_match   : 1;
    bool enable_fallback : 1;
    bool is_iterator     : 1;
    bool no_captures     : 1;
    bool return_bool     : 1;
    bool maybe_no_empty  : 1;
};


namespace default_fsm_flags {

inline constexpr fsm_flags full_match{
    .is_search       = false,
    .longest_match   = true,
    .enable_fallback = false,
    .is_iterator     = false,
    .no_captures     = false,
    .return_bool     = false,
    .maybe_no_empty  = false,
};

inline constexpr fsm_flags partial_match{
    .is_search       = false,
    .longest_match   = false,
    .enable_fallback = true,
    .is_iterator     = false,
    .no_captures     = false,
    .return_bool     = false,
    .maybe_no_empty  = false,
};

inline constexpr fsm_flags search_single{
    .is_search       = true,
    .longest_match   = false,
    .enable_fallback = true,
    .is_iterator     = false,
    .no_captures     = false,
    .return_bool     = false,
    .maybe_no_empty  = false,
};

inline constexpr fsm_flags search_all{
    .is_search       = true,
    .longest_match   = false,
    .enable_fallback = true,
    .is_iterator     = true,
    .no_captures     = false,
    .return_bool     = false,
    .maybe_no_empty  = true,
};

inline constexpr fsm_flags match_sequential{
    .is_search       = false,
    .longest_match   = false,
    .enable_fallback = true,
    .is_iterator     = true,
    .no_captures     = false,
    .return_bool     = false,
    .maybe_no_empty  = true,
};

} // namespace default_fsm_flags;


/* flag adaptors */

constexpr fsm_flags adapt_searcher_flags_to_matcher(fsm_flags f)
{
    f.is_search = false;
    f.is_iterator = true;
    return f;
}

constexpr fsm_flags adapt_flags_return_bool(fsm_flags f)
{
    f.no_captures = true;
    f.return_bool = true;
    return f;
}

constexpr fsm_flags adapt_flags_non_capturing(fsm_flags f)
{
    f.no_captures = true;
    return f;
}


/* packed flag representation */

using ff = unsigned long long;

constexpr ff pack_flags(fsm_flags f)
{
    static constexpr auto nsdms{ define_static_array(nonstatic_data_members_of(^^fsm_flags, std::meta::access_context::unprivileged())) };
    static_assert(nsdms.size() <= std::numeric_limits<ff>::digits);

    std::bitset<nsdms.size()> bitset;

    template for (constexpr std::size_t i : std::views::iota(0uz, nsdms.size()))
        bitset[i] = f.[: nsdms[i] :];

    return bitset.to_ullong();
}

constexpr fsm_flags unpack_flags(ff f)
{
    static constexpr auto nsdms{ define_static_array(nonstatic_data_members_of(^^fsm_flags, std::meta::access_context::unprivileged())) };
    static_assert(nsdms.size() <= std::numeric_limits<ff>::digits);

    std::bitset<nsdms.size()> bitset{ f };
    fsm_flags result{};

    template for (constexpr std::size_t i : std::views::iota(0uz, nsdms.size()))
        result.[: nsdms[i]:] = bitset[i];

    return result;
}

} // namespace srx::detail