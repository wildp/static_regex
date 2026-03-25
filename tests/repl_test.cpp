// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <rx/regex.hpp>


namespace
{
    template<rx::string_literal Pattern, rx::mode Mode>
    consteval bool test(rx::static_regex<Pattern, Mode> pattern, std::string_view fmt, std::string_view input, std::string_view result)
    {
        return rx::regex_replace(input, pattern, fmt) == result;
    }

    template<rx::string_literal Pattern, rx::mode Mode, rx::string_literal Fmt>
    consteval bool test(rx::static_regex<Pattern, Mode> pattern, rx::fmt_t<Fmt>, std::string_view input, std::string_view result)
    {
        return rx::regex_replace(input, pattern, rx::fmt<Fmt>) == result;
    }

    template<rx::string_literal Pattern, rx::mode Mode>
    consteval bool lazy_test(rx::static_regex<Pattern, Mode> pattern, std::string_view fmt, std::string_view input, std::string_view result)
    {
        std::string tmp{ std::from_range, input | rx::views::regex_match(pattern) | rx::views::replace(fmt) };
        return tmp == result;
    }

    template<rx::string_literal Pattern, rx::mode Mode, rx::string_literal Fmt>
    consteval bool lazy_test(rx::static_regex<Pattern, Mode> pattern, rx::fmt_t<Fmt>, std::string_view input, std::string_view result)
    {
        std::string tmp{ std::from_range, input | rx::views::regex_match(pattern) | rx::views::replace(rx::fmt<Fmt>) };
        return tmp == result;
    }

    template<rx::string_literal Pattern, rx::mode Mode>
    consteval bool lazy_test_cstr(rx::static_regex<Pattern, Mode> pattern, const char* fmt, std::string_view input, std::string_view result)
    {
        std::string tmp{ std::from_range, input | rx::views::regex_match(pattern) | rx::views::replace(fmt) };
        return tmp == result;
    }
}


using namespace rx::literals;
using rx::fmt;

/* dynamic replace format tests */
static_assert(test("a"_rx, "x", "abc", "xbc"));
static_assert(test("b"_rx, "x", "abc", "axc"));
static_assert(test("c"_rx, "x", "abc", "abx"));
static_assert(test("a"_rx, "d", "acacac", "dcdcdc"));
static_assert(test(""_rx, "-", "abc", "-a-b-c-"));
static_assert(test("-"_rx, "", "a-b-c", "abc"));
static_assert(test("^|$"_rx, "-", "abc", "-abc-"));
static_assert(test("[ac]"_rx, "", "abcd", "bd"));
static_assert(test("[ac]"_rx, "-", "abcd", "-b-d"));
static_assert(test("a"_rx, "$", "aba", "$b$"));
static_assert(test("([ac])"_rx, "$1", "abcd", "abcd"));
static_assert(test("([ac])"_rx, "$1$1", "abcd", "aabccd"));
static_assert(test("(a)"_rx, "[$1]", "abbab", "[a]bb[a]b"));
static_assert(test("(a)"_rx, "[$1]", "aabbaaab", "[a][a]bb[a][a][a]b"));
static_assert(test("(a+)"_rx, "[$1]", "aabbaaab", "[aa]bb[aaa]b"));
static_assert(test("(a.?c|d.?c)"_rx, "-", "abcdcba", "--ba"));
static_assert(test("(.*)ab(.*)(cd)"_rx, "$1-$2", "abcd", "-"));
static_assert(test("(.*)ab(.*)(cd)"_rx, "$1-$2", "aabccd", "a-c"));
static_assert(test("(.*)ab(.*)(cd)"_rx, "$1-$2", "ababcdcd", "ab-cd"));
static_assert(test("(?:)|abc"_rx, "-", "abc", "---"));
static_assert(test("\\b|abc"_rx, "-", "abc", "---"));
static_assert(test("(?:)|abc"_rx, "-", "abcabc", "-----"));
static_assert(test("\\b|abc"_rx, "-", "abcabc", "----"));

/* static replace format test */
static_assert(test("a"_rx, fmt<"x">, "abc", "xbc"));
static_assert(test("b"_rx, fmt<"x">, "abc", "axc"));
static_assert(test("c"_rx, fmt<"x">, "abc", "abx"));
static_assert(test("a"_rx, fmt<"d">, "acacac", "dcdcdc"));
static_assert(test(""_rx, fmt<"-">, "abc", "-a-b-c-"));
static_assert(test("-"_rx, fmt<"">, "a-b-c", "abc"));
static_assert(test("^|$"_rx, fmt<"-">, "abc", "-abc-"));
static_assert(test("[ac]"_rx, fmt<"">, "abcd", "bd"));
static_assert(test("[ac]"_rx, fmt<"-">, "abcd", "-b-d"));
static_assert(test("a"_rx, fmt<"$">, "aba", "$b$"));
static_assert(test("([ac])"_rx, fmt<"$1">, "abcd", "abcd"));
static_assert(test("([ac])"_rx, fmt<"$1$1">, "abcd", "aabccd"));
static_assert(test("(a)"_rx, fmt<"[$1]">, "abbab", "[a]bb[a]b"));
static_assert(test("(a)"_rx, fmt<"[$1]">, "aabbaaab", "[a][a]bb[a][a][a]b"));
static_assert(test("(a+)"_rx, fmt<"[$1]">, "aabbaaab", "[aa]bb[aaa]b"));
static_assert(test("(a.?c|d.?c)"_rx, fmt<"-">, "abcdcba", "--ba"));
static_assert(test("(.*)ab(.*)(cd)"_rx, fmt<"$1-$2">, "abcd", "-"));
static_assert(test("(.*)ab(.*)(cd)"_rx, fmt<"$1-$2">, "aabccd", "a-c"));
static_assert(test("(.*)ab(.*)(cd)"_rx, fmt<"$1-$2">, "ababcdcd", "ab-cd"));
static_assert(test("(?:)|abc"_rx, fmt<"-">, "abc", "---"));
static_assert(test("\\b|abc"_rx, fmt<"-">, "abc", "---"));
static_assert(test("(?:)|abc"_rx, fmt<"-">, "abcabc", "-----"));
static_assert(test("\\b|abc"_rx, fmt<"-">, "abcabc", "----"));

/* dynamic replace format tests (lazy) */
static_assert(lazy_test("a"_rx, "x", "abc", "xbc"));
static_assert(lazy_test("b"_rx, "x", "abc", "axc"));
static_assert(lazy_test("c"_rx, "x", "abc", "abx"));
static_assert(lazy_test("a"_rx, "d", "acacac", "dcdcdc"));
static_assert(lazy_test(""_rx, "-", "abc", "-a-b-c-"));
static_assert(lazy_test("-"_rx, "", "a-b-c", "abc"));
static_assert(lazy_test("^|$"_rx, "-", "abc", "-abc-"));
static_assert(lazy_test("[ac]"_rx, "", "abcd", "bd"));
static_assert(lazy_test("[ac]"_rx, "-", "abcd", "-b-d"));
static_assert(lazy_test("a"_rx, "$", "aba", "$b$"));
static_assert(lazy_test("([ac])"_rx, "$1", "abcd", "abcd"));
static_assert(lazy_test("([ac])"_rx, "$1$1", "abcd", "aabccd"));
static_assert(lazy_test("(a)"_rx, "[$1]", "abbab", "[a]bb[a]b"));
static_assert(lazy_test("(a)"_rx, "[$1]", "aabbaaab", "[a][a]bb[a][a][a]b"));
static_assert(lazy_test("(a+)"_rx, "[$1]", "aabbaaab", "[aa]bb[aaa]b"));
static_assert(lazy_test("(a.?c|d.?c)"_rx, "-", "abcdcba", "--ba"));
static_assert(lazy_test("(.*)ab(.*)(cd)"_rx, "$1-$2", "abcd", "-"));
static_assert(lazy_test("(.*)ab(.*)(cd)"_rx, "$1-$2", "aabccd", "a-c"));
static_assert(lazy_test("(.*)ab(.*)(cd)"_rx, "$1-$2", "ababcdcd", "ab-cd"));
static_assert(lazy_test("(?:)|abc"_rx, "-", "abc", "---"));
static_assert(lazy_test("\\b|abc"_rx, "-", "abc", "---"));
static_assert(lazy_test("(?:)|abc"_rx, "-", "abcabc", "-----"));
static_assert(lazy_test("\\b|abc"_rx, "-", "abcabc", "----"));

/* static replace format test (lazy) */
static_assert(lazy_test("a"_rx, fmt<"x">, "abc", "xbc"));
static_assert(lazy_test("b"_rx, fmt<"x">, "abc", "axc"));
static_assert(lazy_test("c"_rx, fmt<"x">, "abc", "abx"));
static_assert(lazy_test("a"_rx, fmt<"d">, "acacac", "dcdcdc"));
static_assert(lazy_test(""_rx, fmt<"-">, "abc", "-a-b-c-"));
static_assert(lazy_test("-"_rx, fmt<"">, "a-b-c", "abc"));
static_assert(lazy_test("^|$"_rx, fmt<"-">, "abc", "-abc-"));
static_assert(lazy_test("[ac]"_rx, fmt<"">, "abcd", "bd"));
static_assert(lazy_test("[ac]"_rx, fmt<"-">, "abcd", "-b-d"));
static_assert(lazy_test("a"_rx, fmt<"$">, "aba", "$b$"));
static_assert(lazy_test("([ac])"_rx, fmt<"$1">, "abcd", "abcd"));
static_assert(lazy_test("([ac])"_rx, fmt<"$1$1">, "abcd", "aabccd"));
static_assert(lazy_test("(a)"_rx, fmt<"[$1]">, "abbab", "[a]bb[a]b"));
static_assert(lazy_test("(a)"_rx, fmt<"[$1]">, "aabbaaab", "[a][a]bb[a][a][a]b"));
static_assert(lazy_test("(a+)"_rx, fmt<"[$1]">, "aabbaaab", "[aa]bb[aaa]b"));
static_assert(lazy_test("(a.?c|d.?c)"_rx, fmt<"-">, "abcdcba", "--ba"));
static_assert(lazy_test("(.*)ab(.*)(cd)"_rx, fmt<"$1-$2">, "abcd", "-"));
static_assert(lazy_test("(.*)ab(.*)(cd)"_rx, fmt<"$1-$2">, "aabccd", "a-c"));
static_assert(lazy_test("(.*)ab(.*)(cd)"_rx, fmt<"$1-$2">, "ababcdcd", "ab-cd"));
static_assert(lazy_test("(?:)|abc"_rx, fmt<"-">, "abc", "---"));
static_assert(lazy_test("\\b|abc"_rx, fmt<"-">, "abc", "---"));
static_assert(lazy_test("(?:)|abc"_rx, fmt<"-">, "abcabc", "-----"));
static_assert(lazy_test("\\b|abc"_rx, fmt<"-">, "abcabc", "----"));

/* dynamic replace format tests (lazy, using const char*) */
static_assert(lazy_test_cstr("a"_rx, "x", "abc", "xbc"));
static_assert(lazy_test_cstr("b"_rx, "x", "abc", "axc"));
static_assert(lazy_test_cstr("c"_rx, "x", "abc", "abx"));
static_assert(lazy_test_cstr("a"_rx, "d", "acacac", "dcdcdc"));
static_assert(lazy_test_cstr(""_rx, "-", "abc", "-a-b-c-"));
static_assert(lazy_test_cstr("-"_rx, "", "a-b-c", "abc"));
static_assert(lazy_test_cstr("^|$"_rx, "-", "abc", "-abc-"));
static_assert(lazy_test_cstr("[ac]"_rx, "", "abcd", "bd"));
static_assert(lazy_test_cstr("[ac]"_rx, "-", "abcd", "-b-d"));
static_assert(lazy_test_cstr("a"_rx, "$", "aba", "$b$"));
static_assert(lazy_test_cstr("([ac])"_rx, "$1", "abcd", "abcd"));
static_assert(lazy_test_cstr("([ac])"_rx, "$1$1", "abcd", "aabccd"));
static_assert(lazy_test_cstr("(a)"_rx, "[$1]", "abbab", "[a]bb[a]b"));
static_assert(lazy_test_cstr("(a)"_rx, "[$1]", "aabbaaab", "[a][a]bb[a][a][a]b"));
static_assert(lazy_test_cstr("(a+)"_rx, "[$1]", "aabbaaab", "[aa]bb[aaa]b"));
static_assert(lazy_test_cstr("(a.?c|d.?c)"_rx, "-", "abcdcba", "--ba"));
static_assert(lazy_test_cstr("(.*)ab(.*)(cd)"_rx, "$1-$2", "abcd", "-"));
static_assert(lazy_test_cstr("(.*)ab(.*)(cd)"_rx, "$1-$2", "aabccd", "a-c"));
static_assert(lazy_test_cstr("(.*)ab(.*)(cd)"_rx, "$1-$2", "ababcdcd", "ab-cd"));
static_assert(lazy_test_cstr("(?:)|abc"_rx, "-", "abc", "---"));
static_assert(lazy_test_cstr("\\b|abc"_rx, "-", "abc", "---"));
static_assert(lazy_test_cstr("(?:)|abc"_rx, "-", "abcabc", "-----"));
static_assert(lazy_test_cstr("\\b|abc"_rx, "-", "abcabc", "----"));