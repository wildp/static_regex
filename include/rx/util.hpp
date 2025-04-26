#pragma once

#include <concepts>
#include <variant>
#include <algorithm>

namespace rx::detail 
{
    template<class... Ts>
    struct overloads : Ts... { using Ts::operator()...; };

    template<typename T, typename... Ts>
    concept one_of = (std::same_as<T, Ts> or ...);

    template<typename, typename>
    struct type_in_variant_impl {};

    template<typename T, typename... Ts>
    struct type_in_variant_impl<T, std::variant<Ts...>> : std::bool_constant<one_of<T, Ts...>> {};

    template<typename T, typename Variant>
    concept in_variant = type_in_variant_impl<T, Variant>::value;

    template<typename CharT, std::size_t N>
    struct string_literal
    {
        constexpr string_literal(const CharT (&str)[N])
        {
            std::copy_n(str, N, value.begin());
        }

        std::array<CharT, N> value;
    };

    template<typename V, typename T, std::size_t I = 0>
    struct index_of_impl : std::conditional_t<std::same_as<T, std::variant_alternative_t<I, V>>,
                                            std::integral_constant<std::size_t, I>,
                                            index_of_impl<V, T, I + 1>> {};

    template<typename V, typename T, std::size_t I>
    requires (I >= std::variant_size_v<V>)
    struct index_of_impl<V, T, I> {};

    constexpr int cap_num_to_tag(const std::uint_least16_t capture_num, const bool is_right)
    {
        /* tags start at 1, and we reserve tag 1 for the start tag */
        return ((capture_num + 1) * 2) + is_right;
    }

    inline static constexpr int start_tag{ 1 };
}