// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once


namespace rx::detail
{
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

    namespace default_fsm_flags
    {
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
    }

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
}