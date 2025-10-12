#pragma once

#include <array>
#include <algorithm>

namespace rx::detail
{
    template<typename CharT, std::size_t N>
    struct string_literal
    {
        using char_type = CharT;

        constexpr string_literal(const CharT (&str)[N])
        {
            std::copy_n(str, N, value.begin());
        }

        std::array<CharT, N> value;
    };
}