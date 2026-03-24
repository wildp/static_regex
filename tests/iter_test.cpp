// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <rx/regex.hpp>

#include <deque>
#include <list>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>


namespace
{
    template<int... Is>
    using ints = std::integer_sequence<int, Is...>;

    template<rx::string_literal Pattern, std::ranges::bidirectional_range T>
    constexpr int match_count(T input)
    {
        int count{ 0 };
        for (const auto& _ : input | rx::views::regex_match(rx::static_regex<Pattern>{}))
            ++count;
        return count;
    }

    template<rx::string_literal Pattern, typename CharT>
    constexpr int match_count(const CharT* input)
    {
        return match_count<Pattern>(std::ranges::subrange(input, rx::detail::cstr_sentinel));
    }

    template<rx::string_literal Pattern, typename Ints, std::ranges::bidirectional_range T>
    constexpr int submatch_count(T input)
    {
        int count{ 0 };
        for (const auto& _ : input | rx::views::regex_match(rx::static_regex<Pattern>{}) | rx::views::submatches(Ints{}))
            ++count;
        return count;
    }

    template<rx::string_literal Pattern, std::ranges::bidirectional_range T>
    constexpr int submatch_count(T input, const std::vector<int>& vec)
    {
        int count{ 0 };
        for (const auto& _ : input | rx::views::regex_match(rx::static_regex<Pattern>{}) | rx::views::submatches(vec))
            ++count;
        return count;
    }

    template<rx::string_literal Pattern, typename Ints, typename CharT>
    constexpr int submatch_count(const CharT* input)
    {
        return submatch_count<Pattern, Ints>(std::ranges::subrange(input, rx::detail::cstr_sentinel));
    }

    template<rx::string_literal Pattern, typename CharT>
    constexpr int submatch_count(const CharT* input, const std::vector<int>& vec)
    {
        return submatch_count<Pattern>(std::ranges::subrange(input, rx::detail::cstr_sentinel), vec);
    }

    template<std::size_t I, std::ranges::bidirectional_range R>
    constexpr bool test(R&& r)
    {
        using namespace rx::literals;
        auto matcher = "(Hello)|(World)"_rx;
        auto result = matcher.match(std::forward<R>(r));
        return result and get<0>(result) == get<I>(result);
    }

    template<std::size_t I, std::ranges::bidirectional_range R>
    constexpr bool owning_test(R r)
    {
        using namespace rx::literals;
        auto matcher = "(Hello)|(World)"_rx;
        auto result = matcher.match(r);
        return result and get<0>(result) == get<I>(result);
    }
}


using namespace std::literals;
using namespace rx::literals;

/* iterator category tests */
static_assert(std::random_access_iterator<rx::detail::p1306_matcher<"", rx::detail::default_fsm_flags::search_all>::template result<const char*>::iterator>);

/* range category tests */
using range_category_test_1 = decltype(std::declval<std::string>() | rx::views::regex_match(""_rx));
using range_category_test_2 = decltype(std::ranges::subrange(std::declval<const char*>(), rx::detail::cstr_sentinel) | rx::views::regex_match(""_rx));
using range_category_test_3 = decltype(std::declval<range_category_test_1>() | rx::views::submatches(std::integer_sequence<int, 0, -1>{}));
using range_category_test_4 = decltype(std::declval<range_category_test_2>() | rx::views::submatches(std::integer_sequence<int, 0, -1>{}));
using range_category_test_5 = decltype(std::declval<range_category_test_1>() | rx::views::submatches(std::vector{ 0, -1 }));
using range_category_test_6 = decltype(std::declval<range_category_test_2>() | rx::views::submatches(std::vector{ 0, -1 }));
static_assert(std::ranges::input_range<range_category_test_1>);
static_assert(std::ranges::input_range<range_category_test_2>);
static_assert(std::ranges::input_range<range_category_test_3>);
static_assert(std::ranges::input_range<range_category_test_4>);
static_assert(std::ranges::input_range<range_category_test_5>);
static_assert(std::ranges::input_range<range_category_test_6>);

/* static_regex_match_result iterator type tests */ // TODO: fix transform view iterators
static_assert(test<1>("Hello"));
static_assert(test<1>("Hello"sv));
static_assert(test<1>("Hello World"sv | std::views::take(5)));
static_assert(test<2>("Hello World"sv | std::views::drop(6)));
static_assert(owning_test<2>("World"s));
static_assert(owning_test<2>(std::vector<char>{ 'W', 'o', 'r', 'l', 'd' }));
static_assert(owning_test<1>(std::list<char>{ 'H', 'e', 'l', 'l', 'o' }));
#if __cpp_lib_constexpr_deque >= 202502L
static_assert(owning_test<2>(std::deque<char>{ 'W', 'o', 'r', 'l', 'd' }));
#endif
// static_assert(owning_test<2>("Vnqkc"sv | std::views::transform([](char c) { return static_cast<char>(c + 1); })));
// static_assert(owning_test<2>(std::vector<char>{ 'V', 'n', 'q', 'k', 'c' } | std::views::transform([](char c) -> char { return c + 1; })));
static_assert(owning_test<1>("H e l l o"s | std::views::filter([](char c) { return c != ' '; })));
// static_assert(owning_test<1>("Gdkkn"sv | std::views::transform([](char c) { return static_cast<char>(c + 1); })));
static_assert(owning_test<1>("Hello World"s | std::views::take(5)));
static_assert(owning_test<2>("Hello World"s | std::views::drop(6)));
static_assert(owning_test<1>("Hello World"sv | std::views::take_while([](char c) { return c != ' '; })));
static_assert(owning_test<1>(std::vector{ "He"sv, "ll"sv, "o"sv } | std::views::join));
static_assert(owning_test<2>(std::vector{ "Wo"sv, "ld"sv} | std::views::join_with("r"sv)));

/* match result count tests */
static_assert(match_count<"">("abcdefg") == 8);
static_assert(match_count<".">("abcdefg") == 7);
static_assert(match_count<"">("abcdefg"sv) == 8);
static_assert(match_count<".">("abcdefg"sv) == 7);
static_assert(match_count<"()|abc">("abc"sv) == 3);
static_assert(match_count<"()|abc">("abcabc"sv) == 5);
static_assert(match_count<"\\b|abc">("abc"sv) == 3);
static_assert(match_count<"\\b|abc">("abcabc"sv) == 4);

/* submatch result count tests */
static_assert(submatch_count<"", ints<0>>("abcdefg") == 8);
static_assert(submatch_count<".", ints<0>>("abcdefg") == 7);
static_assert(submatch_count<"a", ints<0>>("abadafa") == 4);
static_assert(submatch_count<"a", ints<0>>("abadaf") == 3);
static_assert(submatch_count<"", ints<-1>>("abcdefg") == 8);
static_assert(submatch_count<".", ints<-1>>("abcdefg") == 7);
static_assert(submatch_count<"a", ints<-1>>("abadafa") == 4);
static_assert(submatch_count<"a", ints<-1>>("abadaf") == 4);
static_assert(submatch_count<"a", ints<-1, -1>>("abd") == 3);
static_assert(submatch_count<"a(b)|(c)d", ints<0, 1, 2>>("abcd") == 6);
static_assert(submatch_count<"b(c)|(e)f", ints<-1, 0, 1, 2>>("abcdefg") == 9);

/* dynamic submatch result count tests */
static_assert(submatch_count<"">("abcdefg", { 0 }) == 8);
static_assert(submatch_count<".">("abcdefg", { 0 }) == 7);
static_assert(submatch_count<"a">("abadafa", { 0 }) == 4);
static_assert(submatch_count<"a">("abadaf", { 0 }) == 3);
static_assert(submatch_count<"">("abcdefg", { -1 }) == 8);
static_assert(submatch_count<".">("abcdefg", { -1 }) == 7);
static_assert(submatch_count<"a">("abadafa", { -1 }) == 4);
static_assert(submatch_count<"a">("abadaf", { -1 }) == 4);
static_assert(submatch_count<"a">("abd", { -1, -1 }) == 3);
static_assert(submatch_count<"a(b)|(c)d">("abcd", { 0, 1, 2 }) == 6);
static_assert(submatch_count<"b(c)|(e)f">("abcdefg", { -1, 0, 1, 2 }) == 9);

