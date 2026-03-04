// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <concepts>
#include <meta>
#include <limits>
#include <stdexcept>
#include <variant>


namespace rx::detail
{
    template<class... Ts>
    struct overloads : Ts... { using Ts::operator()...; };

    template<typename T, typename... Ts>
    concept one_of = (std::same_as<T, Ts> or ...);

    template<typename CharT>
    concept character = one_of<CharT, char, wchar_t, char8_t, char16_t, char32_t>;

    consteval bool is_template_instantiation_of_impl(std::meta::info type, std::meta::info templ)
    {
        if (not is_type(type) or not is_class_template(templ))
            return false;

        if (not has_template_arguments(dealias(type)))
            return false;

        return template_of(dealias(type)) == templ;
    }

    template<typename T, std::meta::info Template>
    concept template_instantiation_of = is_template_instantiation_of_impl(^^T, Template);

    consteval bool in_variant_impl(std::meta::info type, std::meta::info variant)
    {
        if (not is_template_instantiation_of_impl(dealias(variant), ^^std::variant))
            throw std::invalid_argument("in_variant_impl: variant is not an instantiation of std::variant");

        return std::ranges::contains(template_arguments_of(dealias(variant)), dealias(type), std::meta::dealias);
    }

    template<typename T, typename Variant>
    concept in_variant = in_variant_impl(^^T, ^^Variant);

    consteval std::size_t index_in_variant(std::meta::info type, std::meta::info variant)
    {
        if (not is_template_instantiation_of_impl(dealias(variant), ^^std::variant))
            throw std::invalid_argument("index_in_variant: variant is not an instantiation of std::variant");

        const auto vartypes{ template_arguments_of(dealias(variant)) };
        const auto it{ std::ranges::find(vartypes, dealias(type), std::meta::dealias) };

        if (it == vartypes.end())
            throw std::invalid_argument("index_in_variant: type is not an alternative in variant");

        return static_cast<std::size_t>(std::ranges::distance(vartypes.begin(), it));
    }


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


    namespace hash
    {
        static constexpr std::size_t fnv_offset_basis{ 0xcbf29ce484222325 };
        static constexpr std::size_t fnv_prime{ 0xcbf29ce484222325 };

        consteval std::size_t init() { return fnv_offset_basis; }

        template<typename T>
        concept memberwise_hashable = std::is_class_v<T> and not std::ranges::range<T>;

        template<std::integral T>
        constexpr void append(std::size_t& hash, const T& value);

        template<std::ranges::range T>
        constexpr void append(std::size_t& hash, T&& value);

        template<memberwise_hashable T>
        constexpr void append(std::size_t& hash, const T& value);

        template<std::integral T>
        constexpr void append(std::size_t& hash, const T& value)
        {
            for (unsigned char byte : std::bit_cast<std::array<unsigned char, sizeof(T)>>(value))
            {
                hash ^= byte;
                hash *= fnv_prime;
            }
        }

        template<std::ranges::range T>
        constexpr void append(std::size_t& hash, T&& value)
        {
            for (auto&& elem : value)
                append(hash, elem);
        }

        template<memberwise_hashable T>
        constexpr void append(std::size_t& hash, const T& value)
        {
            template for (constexpr auto member : define_static_array(nonstatic_data_members_of(dealias(^^T), std::meta::access_context::unchecked())))
                append(hash, value.[:member:]);
        }
    };


    inline constexpr std::size_t no_tag{ std::numeric_limits<std::size_t>::max() };


    struct cstr_sentinel_t
    {
        template<character CharT>
        friend constexpr bool operator==(const CharT* c, cstr_sentinel_t)
        {
            return *c == CharT{};
        }
    };

    inline constexpr cstr_sentinel_t cstr_sentinel;


    struct terminal_object
    {
        template<typename... Ts>
        constexpr explicit terminal_object(Ts&&...) {}
    };

    template<bool Enabled, typename T>
    using maybe_type_t = std::conditional_t<Enabled, T, terminal_object>;

    template<bool Const, typename T>
    using maybe_const_t = std::conditional_t<Const, const T, T>;
}
