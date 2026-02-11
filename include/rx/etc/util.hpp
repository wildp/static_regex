// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <concepts>
#include <meta>
#include <limits>
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
        if (not std::meta::is_type(type) or not std::meta::is_class_template(templ))
            return false;

        auto dealiased{ std::meta::dealias(type) };

        if (not std::meta::has_template_arguments(dealiased))
            return false;

        return std::meta::template_of(dealiased) == templ;
    }

    template<typename T, std::meta::info Template>
    concept template_instantiation_of = is_template_instantiation_of_impl(^^T, Template);

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
            template for (constexpr std::meta::info member : std::define_static_array(std::meta::nonstatic_data_members_of(std::meta::dealias(^^T), std::meta::access_context::unchecked())))
            {
                const auto& ref = value.[:member:]; // TODO: replace with `append(hash, value.[:member:])`
                append(hash, ref);
            }
        }
    };

    template<typename Derived>
    struct flag_base
    {
        friend constexpr Derived operator|(Derived x, Derived y)
        {
            Derived result;
            template for (constexpr std::meta::info member : std::define_static_array(std::meta::nonstatic_data_members_of(std::meta::dealias(^^Derived), std::meta::access_context::current())))
            {
                result.[:member:] = x.[:member:] or y.[:member:];
            }
            return result;
        }

        friend constexpr Derived operator&(Derived x, Derived y)
        {
            Derived result;
            template for (constexpr std::meta::info member : std::define_static_array(std::meta::nonstatic_data_members_of(std::meta::dealias(^^Derived), std::meta::access_context::current())))
            {
                result.[:member:] = x.[:member:] and y.[:member:];
            }
            return result;
        }

        friend constexpr Derived operator^(Derived x, Derived y)
        {
            Derived result;
            template for (constexpr std::meta::info member : std::define_static_array(std::meta::nonstatic_data_members_of(std::meta::dealias(^^Derived), std::meta::access_context::current())))
            {
                result.[:member:] = x.[:member:] != y.[:member:];
            }
            return result;
        }
    };

    inline constexpr std::size_t no_tag{ std::numeric_limits<std::size_t>::max() };

    template<bool Const, typename T>
    using maybe_const_t = std::conditional_t<Const, const T, T>;

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
}
