#pragma once

#include <concepts>
#include <ranges>

#include <rx/etc/string_literal.hpp>
#include <rx/gen/p1306.hpp>


namespace rx
{
    template<string_literal Pattern>
    requires std::same_as<char, typename decltype(Pattern)::char_type> /* temporary: remove later */
    struct static_regex
    {
        using char_type = decltype(Pattern)::char_type;

        template<std::bidirectional_iterator I>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] constexpr auto match(const I first, const I last) const
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::full_match> m{};
            return m(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] constexpr auto match(R&& r) const
        {
            return match(std::ranges::cbegin(r), std::ranges::cend(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] constexpr auto match(const CharT* cstr) const
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::full_match> m{};
            return m(cstr);
        }

        template<std::bidirectional_iterator I>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] constexpr auto starts_with(const I first, const I last) const
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::partial_match> m{};
            return m(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] constexpr auto starts_with(R&& r) const
        {
            return starts_with(std::ranges::cbegin(r), std::ranges::cend(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] constexpr auto starts_with(const CharT* cstr) const
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::partial_match> m{};
            return m(cstr);
        }

        template<std::bidirectional_iterator I>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] constexpr auto search(const I first, const I last) const
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::search_single> m{};
            return m(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] constexpr auto search(R&& r) const
        {
            return search(std::ranges::cbegin(r), std::ranges::cend(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] constexpr auto search(const CharT* cstr) const
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::search_single> m{};
            return m(cstr);
        }
    };

    namespace literals
    {
        template<string_literal P>
        consteval auto operator ""_rx()
        {
            return static_regex<P>();
        }
    }
}


