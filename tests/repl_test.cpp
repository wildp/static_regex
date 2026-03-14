// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "rx/api/replace.hpp"
#include "rx/etc/string_literal.hpp"
#include <rx/regex.hpp>

namespace
{
    template<rx::string_literal Pattern, rx::mode Mode>
    consteval bool test(rx::static_regex<Pattern, Mode> pattern, std::string_view fmt, std::string_view input, std::string_view result)
    {
        return rx::regex_replace(input.begin(), input.end(), pattern, rx::replace_fmt{ fmt }) == result;
    }

    template<rx::string_literal Pattern, rx::mode Mode, rx::string_literal Fmt>
    consteval bool test(rx::static_regex<Pattern, Mode> pattern, rx::static_replace_fmt<Fmt> fmt, std::string_view input, std::string_view result)
    {
        return rx::regex_replace(input, pattern, fmt) == result;
    }
}


using namespace rx::literals;
using rx::fmt;

/* dynamic format tests */
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
