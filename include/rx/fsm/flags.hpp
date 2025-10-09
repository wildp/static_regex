#pragma once

namespace rx::detail
{
    struct fsm_flags
    {
        bool is_search          : 1;
        bool longest_match      : 1;
        bool enable_fallback    : 1;
    };

    namespace default_fsm_flags
    {
        inline constexpr fsm_flags full_match{
            .is_search = false,
            .longest_match = true,
            .enable_fallback = false,
        };

        inline constexpr fsm_flags partial_match{
            .is_search = false,
            .longest_match = false,
            .enable_fallback = true,
        };

        inline constexpr fsm_flags search{
            .is_search = true,
            .longest_match = false,
            .enable_fallback = true,
        };
    }
}