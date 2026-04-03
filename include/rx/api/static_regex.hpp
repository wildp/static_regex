// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>

#include "rx/etc/string_literal.hpp"
#include "rx/etc/util.hpp"
#include "rx/fsm/flags.hpp"
#include "rx/gen/naive.hpp"
#include "rx/gen/p1306.hpp"
#include "rx/gen/result.hpp"


namespace rx
{
    enum class mode : unsigned char
    {
        standard,
        fast,
        naive
    };

    namespace detail
    {
        consteval auto get_matcher_refl(mode i, bool is_search = false)
        {
            if (i == mode::naive)
                return ^^detail::naive_matcher_adaptor;
            else if (i == mode::standard and is_search)
                return ^^detail::p1306_searcher;
            else
                return ^^detail::p1306_matcher;
        }
    }

    template<string_literal Pattern, mode Mode = mode::standard>
    struct static_regex
    {
        using char_type = decltype(Pattern)::value_type;
        static_assert(std::same_as<char, char_type>); /* temporary: remove later */

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto match(I first, S last)
        {
            using namespace detail::default_fsm_flags;
            using matcher_t = [: detail::get_matcher_refl(Mode) :]<Pattern, full_match>;
            return matcher_t{}(first, last);
        }

        template<std::ranges::bidirectional_range R>
            requires std::same_as<std::ranges::range_value_t<R>, char_type> and std::ranges::borrowed_range<R>
        [[nodiscard]] static constexpr auto match(R&& r)
        {
            return match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
            requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr auto match(const CharT* cstr)
        {
            return match(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto prefix_match(I first, S last)
        {
            using namespace detail::default_fsm_flags;
            using matcher_t = [: detail::get_matcher_refl(Mode) :]<Pattern, partial_match>;
            return matcher_t{}(first, last);
        }

        template<std::ranges::bidirectional_range R>
            requires std::same_as<std::ranges::range_value_t<R>, char_type> and std::ranges::borrowed_range<R>
        [[nodiscard]] static constexpr auto prefix_match(R&& r)
        {
            return prefix_match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
            requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr auto prefix_match(const CharT* cstr)
        {
            return prefix_match(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto search(I first, S last)
        {
            using namespace detail::default_fsm_flags;
            using matcher_t = [: detail::get_matcher_refl(Mode, true) :]<Pattern, search_single>;
            return matcher_t{}(first, last);
        }

        template<std::ranges::bidirectional_range R>
            requires std::same_as<std::ranges::range_value_t<R>, char_type> and std::ranges::borrowed_range<R>
        [[nodiscard]] static constexpr auto search(R&& r)
        {
            return search(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
            requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr auto search(const CharT* cstr)
        {
            return search(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr bool is_match(I first, S last)
        {
            using namespace detail::default_fsm_flags;
            using matcher_t = [: detail::get_matcher_refl(Mode) :]<Pattern, detail::adapt_flags_return_bool(full_match)>;
            return matcher_t{}(first, last);
        }

        template<std::ranges::bidirectional_range R>
            requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] static constexpr bool is_match(R&& r)
        {
            return is_match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
            requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr bool is_match(const CharT* cstr)
        {
            return is_match(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr bool starts_with_match(I first, S last)
        {
            using namespace detail::default_fsm_flags;
            using matcher_t = [: detail::get_matcher_refl(Mode) :]<Pattern, detail::adapt_flags_return_bool(partial_match)>;
            return matcher_t{}(first, last);
        }

        template<std::ranges::bidirectional_range R>
            requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] static constexpr bool starts_with_match(R&& r)
        {
            return starts_with_match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
            requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr bool starts_with_match(const CharT* cstr)
        {
            return starts_with_match(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr bool contains_match(I first, S last)
        {
            using namespace detail::default_fsm_flags;
            using matcher_t = [: detail::get_matcher_refl(Mode, true) :]<Pattern, detail::adapt_flags_return_bool(search_single)>;
            return matcher_t{}(first, last);
        }

        template<std::ranges::bidirectional_range R>
            requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] static constexpr bool contains_match(R&& r)
        {
            return contains_match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
            requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr bool contains_match(const CharT* cstr)
        {
            return contains_match(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto range(I first, S last)
        {
            return range(std::ranges::subrange(first, last));
        }

        template<std::ranges::bidirectional_range R>
            requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] static constexpr auto range(R&& r);

        template<typename CharT>
            requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr auto range(const CharT* cstr)
        {
            return range(std::ranges::subrange(cstr, detail::cstr_sentinel));
        }
    };


    namespace detail
    {
        template<typename R>
        concept static_regex_match_view_like = template_instantiation_of<std::ranges::range_value_t<R>, ^^static_regex_match_result>;

        template<typename Regex>
        concept static_regex_like = template_instantiation_of<Regex, ^^static_regex>;

        template<typename Regex>
        concept regex_like = static_regex_like<Regex>;

        template<typename Regex, typename CharT>
        concept typed_static_regex_like = static_regex_like<Regex> and std::same_as<CharT, typename Regex::char_type>;

        template<typename Regex, typename CharT>
        concept typed_regex_like = typed_static_regex_like<Regex, CharT> ; // or ...
    }


    namespace literals
    {
        template<string_literal Pattern>
        consteval static_regex<Pattern> operator ""_rx()
        {
            return {};
        }

        template<string_literal Pattern>
        consteval static_regex<Pattern, mode::fast> operator ""_rxf()
        {
            return {};
        }

        template<string_literal Pattern>
        consteval static_regex<Pattern, mode::fast> operator ""_rx_fast()
        {
            return {};
        }

        template<string_literal Pattern>
        consteval static_regex<Pattern, mode::naive> operator ""_rxn()
        {
            return {};
        }

        template<string_literal Pattern>
        consteval static_regex<Pattern, mode::naive> operator ""_rx_naive()
        {
            return {};
        }
    }
}

