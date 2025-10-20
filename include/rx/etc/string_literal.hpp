#pragma once

#include <array>
#include <algorithm>

namespace rx
{
    template<typename CharT, std::size_t N>
    struct string_literal
    {
        using char_type = CharT;

        consteval string_literal(const char_type (&str)[N])
        {
            std::copy_n(str, N, value.begin());
        }

        std::array<char_type, N> value;
    };
}