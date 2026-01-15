#include <rx/regex.hpp>

#include <deque>
#include <list>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace 
{
    template<rx::string_literal Pattern, typename T>
    constexpr int match_count(T input)
    {
        int count{ 0 };
        rx::static_regex_iterator it{ input, rx::static_regex<Pattern>{} };
        const rx::static_regex_iterator end;

        while (it != end)
        {
            ++count;
            ++it;
        }

        return count;
    }

    template<std::size_t I, typename R>
    constexpr bool test(R&& r)
    {
        using namespace rx::literals;
        auto matcher = "(Hello)|(World)"_rx;
        auto result = matcher.match(std::forward<R>(r));
        return result and result.template get<0>() == result.template get<I>();
    }
}

using namespace std::literals;

/* iterator category tests */
static_assert(std::random_access_iterator<rx::static_regex_match_result<const char*, "", rx::detail::default_fsm_flags::search_all>::iterator>);
static_assert(std::input_iterator<rx::static_regex_iterator<"", const char*, rx::detail::cstr_sentinel_t>>);
static_assert(std::input_iterator<rx::static_regex_iterator<"", std::string::iterator, std::string::iterator>>);
static_assert(std::input_iterator<rx::static_regex_token_iterator<"", const char*, rx::detail::cstr_sentinel_t, 0>>);
static_assert(std::input_iterator<rx::static_regex_token_iterator<"", std::string::iterator, std::string::iterator, 0>>);
static_assert(std::input_iterator<rx::static_regex_token_iterator<"", const char*, rx::detail::cstr_sentinel_t, 0, -1>>);
static_assert(std::input_iterator<rx::static_regex_token_iterator<"", std::string::iterator, std::string::iterator, 0, -1>>);

/* sentinel correctness tests */
static_assert(std::sentinel_for<rx::static_regex_sentinel_t, rx::static_regex_iterator<"", const char*, rx::detail::cstr_sentinel_t>>);
static_assert(std::sentinel_for<rx::static_regex_sentinel_t, rx::static_regex_iterator<"", std::string::iterator, std::string::iterator>>);
static_assert(std::sentinel_for<rx::static_regex_token_sentinel_t, rx::static_regex_token_iterator<"", const char*, rx::detail::cstr_sentinel_t, 0>>);
static_assert(std::sentinel_for<rx::static_regex_token_sentinel_t, rx::static_regex_token_iterator<"", std::string::iterator, std::string::iterator, 0>>);
static_assert(std::sentinel_for<rx::static_regex_token_sentinel_t, rx::static_regex_token_iterator<"", const char*, rx::detail::cstr_sentinel_t, 0, -1>>);
static_assert(std::sentinel_for<rx::static_regex_token_sentinel_t, rx::static_regex_token_iterator<"", std::string::iterator, std::string::iterator, 0, -1>>);

/* static_regex_match_result iterator type tests */
static_assert(test<1>("Hello"));
static_assert(test<2>("World"s));
static_assert(test<1>("Hello"sv));
static_assert(test<2>(std::vector<char>{ 'W', 'o', 'r', 'l', 'd' }));
static_assert(test<1>(std::list<char>{ 'H', 'e', 'l', 'l', 'o' }));
#if __cpp_lib_constexpr_deque >= 202502L
static_assert(test<2>(std::deque<char>{ 'W', 'o', 'r', 'l', 'd' }));
#endif
// static_assert(test<2>(std::vector<char>{ 'V', 'n', 'q', 'k', 'c' } | std::views::transform([](char c) -> char { return c + 1; })));
static_assert(test<1>("H e l l o"s | std::views::filter([](char c) { return c != ' '; })));
// static_assert(test<1>("Gdkkn"s | std::views::transform([](char c) -> char { return c + 1; })));
static_assert(test<1>("Hello World"s | std::views::take(5)));
static_assert(test<2>("Hello World"s | std::views::drop(6)));
static_assert(test<1>("Hello World"sv | std::views::take_while([](char c) { return c != ' '; })));
static_assert(test<1>(std::vector{ "He"sv, "ll"sv, "o"sv } | std::views::join));
static_assert(test<2>(std::vector{ "Wo"sv, "ld"sv} | std::views::join_with("r"sv)));
// TODO: fix transform view iterators 


/* match result count tests */
static_assert(match_count<"">("abcdefg") == 8);
static_assert(match_count<".">("abcdefg") == 7);
static_assert(match_count<"">("abcdefg"sv) == 8);
static_assert(match_count<".">("abcdefg"sv) == 7);
