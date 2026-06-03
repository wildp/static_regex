// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <srx/regex.hpp>


namespace
{
    template<srx::string_literal Pattern, srx::mode Mode>
    consteval bool test(srx::static_regex<Pattern, Mode> pattern, std::string_view fmt, std::string_view input, std::string_view result)
    {
        return srx::regex_replace(input, pattern, fmt) == result;
    }

    template<srx::string_literal Pattern, srx::mode Mode, srx::string_literal Fmt>
    consteval bool test(srx::static_regex<Pattern, Mode> pattern, srx::fmt_t<Fmt>, std::string_view input, std::string_view result)
    {
        return srx::regex_replace(input, pattern, srx::fmt<Fmt>) == result;
    }

    template<srx::string_literal Pattern, srx::mode Mode>
    consteval bool lazy_test(srx::static_regex<Pattern, Mode> pattern, std::string_view fmt, std::string_view input, std::string_view result)
    {
        auto tmp = input | srx::views::regex_match(pattern) | srx::views::replace(fmt) | std::ranges::to<std::basic_string>();
        return tmp == result;
    }

    template<srx::string_literal Pattern, srx::mode Mode, srx::string_literal Fmt>
    consteval bool lazy_test(srx::static_regex<Pattern, Mode> pattern, srx::fmt_t<Fmt>, std::string_view input, std::string_view result)
    {
        auto tmp = input | srx::views::regex_match(pattern) | srx::views::replace(srx::fmt<Fmt>) | std::ranges::to<std::basic_string>();
        return tmp == result;
    }

    template<srx::string_literal Pattern, srx::mode Mode>
    consteval bool lazy_test_cstr(srx::static_regex<Pattern, Mode> pattern, const char* fmt, std::string_view input, std::string_view result)
    {
        auto tmp = input | srx::views::regex_match(pattern) | srx::views::replace(fmt) | std::ranges::to<std::basic_string>();
        return tmp == result;
    }
}


using namespace srx::literals;
using srx::fmt;

/* dynamic replace format tests */
static_assert(test("a"_srx, "x", "abc", "xbc"));
static_assert(test("b"_srx, "x", "abc", "axc"));
static_assert(test("c"_srx, "x", "abc", "abx"));
static_assert(test("a"_srx, "d", "acacac", "dcdcdc"));
static_assert(test(""_srx, "-", "abc", "-a-b-c-"));
static_assert(test("-"_srx, "", "a-b-c", "abc"));
static_assert(test("^|$"_srx, "-", "abc", "-abc-"));
static_assert(test("[ac]"_srx, "", "abcd", "bd"));
static_assert(test("[ac]"_srx, "-", "abcd", "-b-d"));
static_assert(test("a"_srx, "$", "aba", "$b$"));
static_assert(test("([ac])"_srx, "$1", "abcd", "abcd"));
static_assert(test("([ac])"_srx, "$1$1", "abcd", "aabccd"));
static_assert(test("(a)"_srx, "[$1]", "abbab", "[a]bb[a]b"));
static_assert(test("(a)"_srx, "[$1]", "aabbaaab", "[a][a]bb[a][a][a]b"));
static_assert(test("(a+)"_srx, "[$1]", "aabbaaab", "[aa]bb[aaa]b"));
static_assert(test("(a.?c|d.?c)"_srx, "-", "abcdcba", "--ba"));
static_assert(test("(.*)ab(.*)(cd)"_srx, "$1-$2", "abcd", "-"));
static_assert(test("(.*)ab(.*)(cd)"_srx, "$1-$2", "aabccd", "a-c"));
static_assert(test("(.*)ab(.*)(cd)"_srx, "$1-$2", "ababcdcd", "ab-cd"));
static_assert(test("(?:)|abc"_srx, "-", "abc", "---"));
static_assert(test("\\b|abc"_srx, "-", "abc", "---"));
static_assert(test("(?:)|abc"_srx, "-", "abcabc", "-----"));
static_assert(test("\\b|abc"_srx, "-", "abcabc", "----"));
static_assert(test("a"_srx, "[$0]", "abbab", "[a]bb[a]b"));
static_assert(test("a"_srx, "[$&]", "abbab", "[a]bb[a]b"));
static_assert(test("a"_srx, "[$`]", "abbab", "[]bb[bb]b"));
static_assert(test("a"_srx, "[$']", "abbab", "[bbab]bb[b]b"));
static_assert(test("()(a)"_srx, "[$+]", "abbab", "[a]bb[a]b"));
static_assert(test("()(a)"_srx, "[$2]", "abbab", "[a]bb[a]b"));
static_assert(test("(a)+(b)"_srx, "$1[$+]", "aabbaaaba", "a[b]ba[b]a"));
static_assert(test("a"_srx, "$$", "abbab", "$bb$b"));
static_assert(test("a"_srx, "$$0", "abbab", "$0bb$0b"));
static_assert(test("a"_srx, "$$$0", "abbab", "$abb$ab"));

/* static replace format test */
static_assert(test("a"_srx, fmt<"x">, "abc", "xbc"));
static_assert(test("b"_srx, fmt<"x">, "abc", "axc"));
static_assert(test("c"_srx, fmt<"x">, "abc", "abx"));
static_assert(test("a"_srx, fmt<"d">, "acacac", "dcdcdc"));
static_assert(test(""_srx, fmt<"-">, "abc", "-a-b-c-"));
static_assert(test("-"_srx, fmt<"">, "a-b-c", "abc"));
static_assert(test("^|$"_srx, fmt<"-">, "abc", "-abc-"));
static_assert(test("[ac]"_srx, fmt<"">, "abcd", "bd"));
static_assert(test("[ac]"_srx, fmt<"-">, "abcd", "-b-d"));
static_assert(test("a"_srx, fmt<"$">, "aba", "$b$"));
static_assert(test("([ac])"_srx, fmt<"$1">, "abcd", "abcd"));
static_assert(test("([ac])"_srx, fmt<"$1$1">, "abcd", "aabccd"));
static_assert(test("(a)"_srx, fmt<"[$1]">, "abbab", "[a]bb[a]b"));
static_assert(test("(a)"_srx, fmt<"[$1]">, "aabbaaab", "[a][a]bb[a][a][a]b"));
static_assert(test("(a+)"_srx, fmt<"[$1]">, "aabbaaab", "[aa]bb[aaa]b"));
static_assert(test("(a.?c|d.?c)"_srx, fmt<"-">, "abcdcba", "--ba"));
static_assert(test("(.*)ab(.*)(cd)"_srx, fmt<"$1-$2">, "abcd", "-"));
static_assert(test("(.*)ab(.*)(cd)"_srx, fmt<"$1-$2">, "aabccd", "a-c"));
static_assert(test("(.*)ab(.*)(cd)"_srx, fmt<"$1-$2">, "ababcdcd", "ab-cd"));
static_assert(test("(?:)|abc"_srx, fmt<"-">, "abc", "---"));
static_assert(test("\\b|abc"_srx, fmt<"-">, "abc", "---"));
static_assert(test("(?:)|abc"_srx, fmt<"-">, "abcabc", "-----"));
static_assert(test("\\b|abc"_srx, fmt<"-">, "abcabc", "----"));
static_assert(test("a"_srx, fmt<"[$0]">, "abbab", "[a]bb[a]b"));
static_assert(test("a"_srx, fmt<"[$&]">, "abbab", "[a]bb[a]b"));
static_assert(test("a"_srx, fmt<"[$`]">, "abbab", "[]bb[bb]b"));
static_assert(test("a"_srx, fmt<"[$']">, "abbab", "[bbab]bb[b]b"));
static_assert(test("()(a)"_srx, fmt<"[$+]">, "abbab", "[a]bb[a]b"));
static_assert(test("()(a)"_srx, fmt<"[$2]">, "abbab", "[a]bb[a]b"));
static_assert(test("(a)+(b)"_srx, fmt<"$1[$+]">, "aabbaaaba", "a[b]ba[b]a"));
static_assert(test("a"_srx, fmt<"$$">, "abbab", "$bb$b"));
static_assert(test("a"_srx, fmt<"$$0">, "abbab", "$0bb$0b"));
static_assert(test("a"_srx, fmt<"$$$0">, "abbab", "$abb$ab"));

/* dynamic replace format tests (lazy) */
static_assert(lazy_test("a"_srx, "x", "abc", "xbc"));
static_assert(lazy_test("b"_srx, "x", "abc", "axc"));
static_assert(lazy_test("c"_srx, "x", "abc", "abx"));
static_assert(lazy_test("a"_srx, "d", "acacac", "dcdcdc"));
static_assert(lazy_test(""_srx, "-", "abc", "-a-b-c-"));
static_assert(lazy_test("-"_srx, "", "a-b-c", "abc"));
static_assert(lazy_test("^|$"_srx, "-", "abc", "-abc-"));
static_assert(lazy_test("[ac]"_srx, "", "abcd", "bd"));
static_assert(lazy_test("[ac]"_srx, "-", "abcd", "-b-d"));
static_assert(lazy_test("a"_srx, "$", "aba", "$b$"));
static_assert(lazy_test("([ac])"_srx, "$1", "abcd", "abcd"));
static_assert(lazy_test("([ac])"_srx, "$1$1", "abcd", "aabccd"));
static_assert(lazy_test("(a)"_srx, "[$1]", "abbab", "[a]bb[a]b"));
static_assert(lazy_test("(a)"_srx, "[$1]", "aabbaaab", "[a][a]bb[a][a][a]b"));
static_assert(lazy_test("(a+)"_srx, "[$1]", "aabbaaab", "[aa]bb[aaa]b"));
static_assert(lazy_test("(a.?c|d.?c)"_srx, "-", "abcdcba", "--ba"));
static_assert(lazy_test("(.*)ab(.*)(cd)"_srx, "$1-$2", "abcd", "-"));
static_assert(lazy_test("(.*)ab(.*)(cd)"_srx, "$1-$2", "aabccd", "a-c"));
static_assert(lazy_test("(.*)ab(.*)(cd)"_srx, "$1-$2", "ababcdcd", "ab-cd"));
static_assert(lazy_test("(?:)|abc"_srx, "-", "abc", "---"));
static_assert(lazy_test("\\b|abc"_srx, "-", "abc", "---"));
static_assert(lazy_test("(?:)|abc"_srx, "-", "abcabc", "-----"));
static_assert(lazy_test("\\b|abc"_srx, "-", "abcabc", "----"));
static_assert(lazy_test("a"_srx, "[$0]", "abbab", "[a]bb[a]b"));
static_assert(lazy_test("a"_srx, "[$&]", "abbab", "[a]bb[a]b"));
static_assert(lazy_test("a"_srx, "[$`]", "abbab", "[]bb[bb]b"));
static_assert(lazy_test("a"_srx, "[$']", "abbab", "[bbab]bb[b]b"));
static_assert(lazy_test("()(a)"_srx, "[$+]", "abbab", "[a]bb[a]b"));
static_assert(lazy_test("()(a)"_srx, "[$2]", "abbab", "[a]bb[a]b"));
static_assert(lazy_test("(a)+(b)"_srx, "$1[$+]", "aabbaaaba", "a[b]ba[b]a"));
static_assert(lazy_test("a"_srx, "$$", "abbab", "$bb$b"));
static_assert(lazy_test("a"_srx, "$$0", "abbab", "$0bb$0b"));
static_assert(lazy_test("a"_srx, "$$$0", "abbab", "$abb$ab"));

/* static replace format test (lazy) */
static_assert(lazy_test("a"_srx, fmt<"x">, "abc", "xbc"));
static_assert(lazy_test("b"_srx, fmt<"x">, "abc", "axc"));
static_assert(lazy_test("c"_srx, fmt<"x">, "abc", "abx"));
static_assert(lazy_test("a"_srx, fmt<"d">, "acacac", "dcdcdc"));
static_assert(lazy_test(""_srx, fmt<"-">, "abc", "-a-b-c-"));
static_assert(lazy_test("-"_srx, fmt<"">, "a-b-c", "abc"));
static_assert(lazy_test("^|$"_srx, fmt<"-">, "abc", "-abc-"));
static_assert(lazy_test("[ac]"_srx, fmt<"">, "abcd", "bd"));
static_assert(lazy_test("[ac]"_srx, fmt<"-">, "abcd", "-b-d"));
static_assert(lazy_test("a"_srx, fmt<"$">, "aba", "$b$"));
static_assert(lazy_test("([ac])"_srx, fmt<"$1">, "abcd", "abcd"));
static_assert(lazy_test("([ac])"_srx, fmt<"$1$1">, "abcd", "aabccd"));
static_assert(lazy_test("(a)"_srx, fmt<"[$1]">, "abbab", "[a]bb[a]b"));
static_assert(lazy_test("(a)"_srx, fmt<"[$1]">, "aabbaaab", "[a][a]bb[a][a][a]b"));
static_assert(lazy_test("(a+)"_srx, fmt<"[$1]">, "aabbaaab", "[aa]bb[aaa]b"));
static_assert(lazy_test("(a.?c|d.?c)"_srx, fmt<"-">, "abcdcba", "--ba"));
static_assert(lazy_test("(.*)ab(.*)(cd)"_srx, fmt<"$1-$2">, "abcd", "-"));
static_assert(lazy_test("(.*)ab(.*)(cd)"_srx, fmt<"$1-$2">, "aabccd", "a-c"));
static_assert(lazy_test("(.*)ab(.*)(cd)"_srx, fmt<"$1-$2">, "ababcdcd", "ab-cd"));
static_assert(lazy_test("(?:)|abc"_srx, fmt<"-">, "abc", "---"));
static_assert(lazy_test("\\b|abc"_srx, fmt<"-">, "abc", "---"));
static_assert(lazy_test("(?:)|abc"_srx, fmt<"-">, "abcabc", "-----"));
static_assert(lazy_test("\\b|abc"_srx, fmt<"-">, "abcabc", "----"));
static_assert(lazy_test("a"_srx, fmt<"[$0]">, "abbab", "[a]bb[a]b"));
static_assert(lazy_test("a"_srx, fmt<"[$&]">, "abbab", "[a]bb[a]b"));
static_assert(lazy_test("a"_srx, fmt<"[$`]">, "abbab", "[]bb[bb]b"));
static_assert(lazy_test("a"_srx, fmt<"[$']">, "abbab", "[bbab]bb[b]b"));
static_assert(lazy_test("()(a)"_srx, fmt<"[$+]">, "abbab", "[a]bb[a]b"));
static_assert(lazy_test("()(a)"_srx, fmt<"[$2]">, "abbab", "[a]bb[a]b"));
static_assert(lazy_test("(a)+(b)"_srx, fmt<"$1[$+]">, "aabbaaaba", "a[b]ba[b]a"));
static_assert(lazy_test("a"_srx, fmt<"$$">, "abbab", "$bb$b"));
static_assert(lazy_test("a"_srx, fmt<"$$0">, "abbab", "$0bb$0b"));
static_assert(lazy_test("a"_srx, fmt<"$$$0">, "abbab", "$abb$ab"));

/* dynamic replace format tests (lazy, using const char*) */
static_assert(lazy_test_cstr("a"_srx, "x", "abc", "xbc"));
static_assert(lazy_test_cstr("b"_srx, "x", "abc", "axc"));
static_assert(lazy_test_cstr("c"_srx, "x", "abc", "abx"));
static_assert(lazy_test_cstr("a"_srx, "d", "acacac", "dcdcdc"));
static_assert(lazy_test_cstr(""_srx, "-", "abc", "-a-b-c-"));
static_assert(lazy_test_cstr("-"_srx, "", "a-b-c", "abc"));
static_assert(lazy_test_cstr("^|$"_srx, "-", "abc", "-abc-"));
static_assert(lazy_test_cstr("[ac]"_srx, "", "abcd", "bd"));
static_assert(lazy_test_cstr("[ac]"_srx, "-", "abcd", "-b-d"));
static_assert(lazy_test_cstr("a"_srx, "$", "aba", "$b$"));
static_assert(lazy_test_cstr("([ac])"_srx, "$1", "abcd", "abcd"));
static_assert(lazy_test_cstr("([ac])"_srx, "$1$1", "abcd", "aabccd"));
static_assert(lazy_test_cstr("(a)"_srx, "[$1]", "abbab", "[a]bb[a]b"));
static_assert(lazy_test_cstr("(a)"_srx, "[$1]", "aabbaaab", "[a][a]bb[a][a][a]b"));
static_assert(lazy_test_cstr("(a+)"_srx, "[$1]", "aabbaaab", "[aa]bb[aaa]b"));
static_assert(lazy_test_cstr("(a.?c|d.?c)"_srx, "-", "abcdcba", "--ba"));
static_assert(lazy_test_cstr("(.*)ab(.*)(cd)"_srx, "$1-$2", "abcd", "-"));
static_assert(lazy_test_cstr("(.*)ab(.*)(cd)"_srx, "$1-$2", "aabccd", "a-c"));
static_assert(lazy_test_cstr("(.*)ab(.*)(cd)"_srx, "$1-$2", "ababcdcd", "ab-cd"));
static_assert(lazy_test_cstr("(?:)|abc"_srx, "-", "abc", "---"));
static_assert(lazy_test_cstr("\\b|abc"_srx, "-", "abc", "---"));
static_assert(lazy_test_cstr("(?:)|abc"_srx, "-", "abcabc", "-----"));
static_assert(lazy_test_cstr("\\b|abc"_srx, "-", "abcabc", "----"));
static_assert(lazy_test_cstr("a"_srx, "[$0]", "abbab", "[a]bb[a]b"));
static_assert(lazy_test_cstr("a"_srx, "[$&]", "abbab", "[a]bb[a]b"));
static_assert(lazy_test_cstr("a"_srx, "[$`]", "abbab", "[]bb[bb]b"));
static_assert(lazy_test_cstr("a"_srx, "[$']", "abbab", "[bbab]bb[b]b"));
static_assert(lazy_test_cstr("()(a)"_srx, "[$+]", "abbab", "[a]bb[a]b"));
static_assert(lazy_test_cstr("()(a)"_srx, "[$2]", "abbab", "[a]bb[a]b"));
static_assert(lazy_test_cstr("(a)+(b)"_srx, "$1[$+]", "aabbaaaba", "a[b]ba[b]a"));
static_assert(lazy_test_cstr("a"_srx, "$$", "abbab", "$bb$b"));
static_assert(lazy_test_cstr("a"_srx, "$$0", "abbab", "$0bb$0b"));
static_assert(lazy_test_cstr("a"_srx, "$$$0", "abbab", "$abb$ab"));