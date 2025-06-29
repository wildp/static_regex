module;

#include <concepts>
#include <limits>
#include <variant>
#include <algorithm>

export module rx.util;

// import std;
export import :error;
export import :cdarray;

export namespace rx::detail 
{
    template<class... Ts>
    struct overloads : Ts... { using Ts::operator()...; };

    template<typename T, typename... Ts>
    concept one_of = (std::same_as<T, Ts> or ...);
}

namespace
{
    template<typename, typename>
    struct type_in_variant_impl {};

    template<typename T, typename... Ts>
    struct type_in_variant_impl<T, std::variant<Ts...>> : std::bool_constant<rx::detail::one_of<T, Ts...>> {};

}

export namespace rx::detail
{
    template<typename T, typename Variant>
    concept in_variant = type_in_variant_impl<T, Variant>::value;

    template<typename CharT>
    concept char_is_utf8 = std::same_as<CharT, char8_t>; /* Assume a regular char is ascii (or some single byte superset of) */

    template<typename CharT>
    concept char_is_utf16 = std::same_as<CharT, char16_t>
                            or (std::same_as<CharT, wchar_t> and sizeof(wchar_t) == sizeof(char16_t)); /* Assume a 16-bit wchar_t is encoded in utf16 */

    template<typename CharT>
    concept char_is_utf32 = std::same_as<CharT, char32_t>
                            or (std::same_as<CharT, wchar_t> and sizeof(wchar_t) == sizeof(char32_t)); /* Assume a 32-bit wchar_t is encoded in utf32 */
    

    template<typename CharT>
    concept char_is_multibyte = char_is_utf8<CharT> or char_is_utf16<CharT>;

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

    inline constexpr int start_tag_number{ 1 };

    inline constexpr std::size_t no_tag{ std::numeric_limits<std::size_t>::max() };
}

















#if RX_TREE_DEBUG_PARSER
#include <meta>
#include <print>

namespace rx::detail
{
     /* print-statement debugging tools */

    template <typename E>
    requires std::is_enum_v<E>
    constexpr std::string enum_to_string(E value)
    {
        template for (constexpr auto e : std::define_static_array(std::meta::enumerators_of(^^E)))
        {
            if (value == [:e:]) 
                return std::string(std::meta::identifier_of(e));
        }

        return "<unnamed>";
    }

    template <typename... Ts>
    constexpr std::string variant_alternative_name(const std::variant<Ts...>& value)
    {
        template for (constexpr auto e : { (^^Ts)... })
        {
            if (std::holds_alternative<[: e :]>(value))
                return std::string{ std::meta::display_string_of(e) };
        }

        return "<error>";
    }

    template<typename... Ts>
    constexpr std::string pretty_print_2(const std::variant<Ts...>& value)
    {
        template for (constexpr auto e : { (^^Ts)... })
        {
            if (std::holds_alternative<[: e :]>(value))
            {
                if constexpr (std::meta::is_enum_type(e))
                    return enum_to_string(std::get<[: e :]>(value));
                if constexpr (std::meta::has_template_arguments(e) and std::meta::template_of(e) == ^^std::variant)
                    return variant_alternative_name(std::get<[: e :]>(value));
                else
                    return std::string{ std::meta::display_string_of(e) };
            }
        }

        return "<error>";
    }
}
#endif // RX_TREE_DEBUG_PARSER
