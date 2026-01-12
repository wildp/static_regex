// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <concepts>
#include <limits>
#include <variant>


namespace rx::detail
{
    template<class... Ts>
    struct overloads : Ts... { using Ts::operator()...; };

    template<typename T, typename... Ts>
    concept one_of = (std::same_as<T, Ts> or ...);

    // TODO: replace with P2996 implementation
    template<typename, typename>
    struct type_in_variant_impl {};

    // TODO: replace with P2996 implementation
    template<typename T, typename... Ts>
    struct type_in_variant_impl<T, std::variant<Ts...>> : std::bool_constant<one_of<T, Ts...>> {};

    template<typename T, typename Variant>
    concept in_variant = type_in_variant_impl<T, Variant>::value;

    template<typename CharT>
    concept char_is_utf8 = std::same_as<CharT, char8_t>; /* Assume a regular char is ascii (or some single byte superset of) */

    template<typename CharT>
    concept char_is_utf16 = std::same_as<CharT, char16_t>
                            or (std::same_as<CharT, wchar_t> and sizeof(CharT) == sizeof(char16_t)); /* Assume a 16-bit wchar_t is encoded in utf16 */

    template<typename CharT>
    concept char_is_utf32 = std::same_as<CharT, char32_t>
                            or (std::same_as<CharT, wchar_t> and sizeof(CharT) == sizeof(char32_t)); /* Assume a 32-bit wchar_t is encoded in utf32 */

    template<typename CharT>
    concept char_is_multibyte = char_is_utf8<CharT> or char_is_utf16<CharT>;


    template<typename V, typename T, std::size_t I = 0>
    struct index_of_impl : std::conditional_t<std::same_as<T, std::variant_alternative_t<I, V>>,
                                              std::integral_constant<std::size_t, I>,
                                              index_of_impl<V, T, I + 1>> {};

    template<typename V, typename T, std::size_t I>
    requires (I >= std::variant_size_v<V>)
    struct index_of_impl<V, T, I> {};

    inline constexpr std::size_t no_tag{ std::numeric_limits<std::size_t>::max() };
}
