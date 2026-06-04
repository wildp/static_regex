// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <srx/regex.hpp>

namespace {

template<srx::string_literal S>
consteval bool match(srx::static_regex<S> m, std::string_view str, const std::vector<std::string_view>& captures = {})
{
    if (captures.empty())
        return m.is_match(str);
    return std::ranges::equal(m.match(str), captures);
}

template<srx::string_literal S>
consteval bool prefix_match(srx::static_regex<S> m, std::string_view str, const std::vector<std::string_view>& captures = {})
{
    if (captures.empty())
        return m.starts_with_match(str);
    return std::ranges::equal(m.prefix_match(str), captures);
}

template<srx::string_literal S>
consteval bool search(srx::static_regex<S> m, std::string_view str, const std::vector<std::string_view>& captures = {})
{
    if (captures.empty())
        return m.contains_match(str);
    else
        return std::ranges::equal(m.search(str), captures);
}

template<srx::string_literal S, typename Ints>
consteval bool submatches(srx::static_regex<S> m, Ints ints, std::string_view str, const std::vector<std::string_view>& captures)
{
    return std::ranges::equal(str | srx::views::regex_match(m) | srx::views::submatches(ints), captures);
}

} // namespace

using namespace srx::literals;

template<int... Ints>
using ints = std::integer_sequence<int, Ints...>;

/* basic tests */
static_assert(not match("a"_srx, ""));
static_assert(match("a"_srx, "a"));
static_assert(match("(Hello|World)+"_srx, "HelloWorld"));
static_assert(prefix_match("a"_srx, "aa"));
static_assert(not prefix_match("a"_srx, "baa"));
static_assert(search("a"_srx, "baa"));
static_assert(not search("ab"_srx, "baa"));
static_assert(search("ab+c"_srx, "abbbbabbcaaac"));

/* laziness tests */
static_assert(match("([a-z])+?[a-z]?"_srx, "ab", { "ab", "a" }));
static_assert(match("([a-z])+[a-z]?"_srx, "ab", { "ab", "b" }));
static_assert(prefix_match("(ab+c)+?(ab+c|.*d)"_srx, "abcabbcacd", { "abcabbc", "abc", "abbc" }));
static_assert(not prefix_match("(ab+c)+?(ab+c|.*d)"_srx, "abcabbcacd", { "abcabbcacd", "abbc", "acd" }));
static_assert(prefix_match("(ab+c)+(ab+c|.*d)"_srx, "abcabbcacd", { "abcabbcacd", "abbc", "acd" }));
static_assert(search("([ad]b+c)+?([ad])"_srx, "aaabacabcdbbcacd", { "abcd", "abc", "d" }));
static_assert(search("([ad]b+c)+([ad])"_srx, "aaabacabcdbbcacd", { "abcdbbca", "dbbc", "a" }));

/* nontrival regex pattern tests */
constexpr auto email_regex = R"([[:alnum:].+/=?^_-]+@[[:alnum:]](?:[[:alnum:]-]*[[:alnum:]])?(?:\.[[:alnum:]](?:[[:alnum:]-]*[[:alnum:]])?)+)"_srx;
static_assert(match(email_regex, "hello@example.com"));
static_assert(not match(email_regex, "not_an_email@invalid_url"));
static_assert(not match(email_regex, "@example.com"));
static_assert(not match(email_regex, "test@example@example"));
static_assert(not match(email_regex, "invalid"));
static_assert(not match(email_regex, "address@"));
static_assert(not match(email_regex, "@example.example@"));

/* range submatch tests */
static_assert(submatches("a(.)c"_srx, ints<0, 1>{}, "abcbadce", { "abc" , "b", "adc", "d" }));
static_assert(submatches("a(.)c"_srx, ints<-1, 1>{}, "abcbadce", { "" , "b", "b", "d", "e" }));
static_assert(submatches("a(.)c"_srx, ints<-1, 1>{}, "abcbadc", { "" , "b", "b", "d" }));
static_assert(submatches("(.)([cde])"_srx, ints<0, 2, -1>{}, "adabcae", { "ad" , "d", "", "bc", "c", "a", "ae", "e", "" }));
static_assert(submatches("()|abc"_srx, ints<0>{}, "abc", { "" , "abc", "" }));
static_assert(submatches("\\b|abc"_srx, ints<0>{}, "abc", { "", "abc", "" }));
