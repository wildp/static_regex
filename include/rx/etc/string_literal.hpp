// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <string_view>

namespace rx
{
    template<typename CharT, std::size_t N>
    struct string_literal
    {
        static_assert(N != 0);
        using char_type = CharT;

        consteval string_literal(const char_type (&str)[N])
        {
            std::ranges::copy_n(str, N, value_);
        }

        [[nodiscard]] constexpr std::basic_string_view<char_type> view() const
        {
            return { value_, N - 1 };
        }

        char_type value_[N]{};
    };
}
