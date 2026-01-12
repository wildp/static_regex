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
    };

    namespace default_fsm_flags
    {
        inline constexpr fsm_flags full_match{
            .is_search = false,
            .longest_match = true,
            .enable_fallback = false,
            .is_iterator = false,
        };

        inline constexpr fsm_flags partial_match{
            .is_search = false,
            .longest_match = false,
            .enable_fallback = true,
            .is_iterator = false,
        };

        inline constexpr fsm_flags search_single{
            .is_search = true,
            .longest_match = false,
            .enable_fallback = true,
            .is_iterator = false,
        };

        inline constexpr fsm_flags search_all{
            .is_search = true,
            .longest_match = false,
            .enable_fallback = true,
            .is_iterator = true,
        };
    }
}