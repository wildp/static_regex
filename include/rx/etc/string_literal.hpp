#pragma once

#include <array>
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
            std::copy_n(str, N, value_.begin());
        }

        constexpr std::basic_string_view<char_type> view() const
        {
            return { value_.data(), N - 1 };
        }

        std::array<char_type, N> value_;
    };
}