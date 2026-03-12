// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <rx/regex.hpp>


namespace
{
    template<typename MatchResult, std::bidirectional_iterator Iter>
    consteval bool submatch_check(const MatchResult& mr, const std::vector<std::size_t>& captures, const Iter start, std::size_t init = 0)
    {
        using rx::detail::no_tag;

        if (((mr.size() - init) * 2) != captures.size())
            return false;

        for (std::size_t i{ init }, i_end{ mr.size() }; i < i_end; ++i)
        {
            const auto submatch{ mr.at(i) };
            auto lhs{ captures.at((i - init) * 2) };
            auto rhs{ captures.at(((i - init) * 2) + 1) };

            if (lhs == no_tag and rhs == no_tag)
            {
                if (submatch.matched())
                    return false;
            }
            else if (lhs != no_tag or rhs != no_tag)
            {
                if (not submatch.matched())
                    return false;

                if (not (submatch.begin() == std::ranges::next(start, lhs) and submatch.end() == std::ranges::next(start, rhs)))
                    return false;
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    template<rx::string_literal S>
    consteval bool match(rx::static_regex<S> pattern, std::string_view str)
    {
        return pattern.is_match(str);
    }

    template<rx::string_literal S>
    consteval bool match(rx::static_regex<S> pattern, std::string_view str, const std::vector<std::size_t>& captures)
    {
        return submatch_check(pattern.match(str), captures, str.begin(), 1);
    }

    template<rx::string_literal S>
    consteval bool prefix_match(rx::static_regex<S> pattern, std::string_view str)
    {
       return pattern.starts_with_match(str);
    }

    template<rx::string_literal S>
    consteval bool prefix_match(rx::static_regex<S> pattern, std::string_view str, const std::vector<std::size_t>& captures)
    {
        return submatch_check(pattern.prefix_match(str), captures, str.begin());
    }

    template<rx::string_literal S>
    consteval bool search(rx::static_regex<S> pattern, std::string_view str)
    {
       return pattern.contains_match(str);
    }

    template<rx::string_literal S>
    consteval bool search(rx::static_regex<S> pattern, std::string_view str, const std::vector<std::size_t>& captures)
    {
        return submatch_check(pattern.search(str), captures, str.begin());
    }

    template<rx::string_literal S>
    consteval bool search_all(rx::static_regex<S> pattern, std::string_view str, const std::vector<std::vector<std::size_t>>& captures)
    {
        for (const auto& [match, caps] : std::views::zip(pattern.range(str), captures))
        {
            if (not submatch_check(match, caps, str.begin()))
                return false;
        }
        return true;
    }
}

using rx::detail::no_tag;
using namespace rx::literals;

/* basic tests */
static_assert(match(""_rx, ""));
static_assert(not match(""_rx, "a"));
static_assert(not match("a"_rx, ""));
static_assert(match("a"_rx, "a"));
static_assert(not match("a"_rx, "b"));

/* grouping tests */
static_assert(not match("a|b"_rx, ""));
static_assert(match("a|b"_rx, "a"));
static_assert(match("a|b"_rx, "b"));
static_assert(not match("a|b"_rx, "c"));
static_assert(match("a|b|c"_rx, "c"));
static_assert(match("a|b|c"_rx, "b"));
static_assert(match("a|b|c"_rx, "a"));
static_assert(not match("aa"_rx, ""));
static_assert(not match("aa"_rx, "a"));
static_assert(match("aa"_rx, "aa"));
static_assert(not match("aa"_rx, "aaa"));
static_assert(not match("ab"_rx, ""));
static_assert(not match("ab"_rx, "a"));
static_assert(not match("ab"_rx, "b"));
static_assert(match("ab"_rx, "ab"));
static_assert(not match("ab"_rx, "ba"));
static_assert(not match("ab"_rx, "abb"));
static_assert(not match("ab"_rx, "aab"));
static_assert(not match("ab"_rx, "bb"));

/* character class tests */
static_assert(match("[a-c]"_rx, "a"));
static_assert(match("[a-c]"_rx, "b"));
static_assert(match("[a-c]"_rx, "c"));
static_assert(not match("[a-c]"_rx, "d"));

/* repeater tests */
static_assert(match("a*"_rx, ""));
static_assert(match("a*"_rx, "a"));
static_assert(match("a*"_rx, "aa"));
static_assert(match("a*"_rx, "aaaaa"));
static_assert(not match("a+"_rx, ""));
static_assert(match("a+"_rx, "a"));
static_assert(match("a+"_rx, "aa"));
static_assert(match("a+"_rx, "aaaaa"));
static_assert(match("a?"_rx, ""));
static_assert(match("a?"_rx, "a"));
static_assert(not match("a?"_rx, "aa"));
static_assert(not match("a?"_rx, "aaaaa"));
static_assert(not match("a{1}"_rx, ""));
static_assert(match("a{1}"_rx, "a"));
static_assert(not match("a{1}"_rx, "aa"));
static_assert(not match("a{2,5}"_rx, "a"));
static_assert(match("a{2,5}"_rx, "aa"));
static_assert(match("a{2,5}"_rx, "aaa"));
static_assert(match("a{2,5}"_rx, "aaaa"));
static_assert(match("a{2,5}"_rx, "aaaaa"));
static_assert(not match("a{2,5}"_rx, "aaaaaaa"));
static_assert(not match("a{2,}"_rx, "a"));
static_assert(match("a{2,}"_rx, "aa"));
static_assert(match("a{2,}"_rx, "aaa"));
static_assert(match("a{2,}"_rx, "aaaa"));
static_assert(match("a{2,}"_rx, "aaaaa"));
static_assert(not match("a{3},"_rx, "aa"));
static_assert(match("a{3}"_rx, "aaa"));
static_assert(not match("a{3}"_rx, "aaaa"));

/* laziness tests */
static_assert(match("(a)?a*"_rx, "", { no_tag, no_tag }));
static_assert(match("(a)?a*"_rx, "a", { 0, 1 }));
static_assert(match("(a)?a*"_rx, "aa", { 0, 1 }));
static_assert(match("(a)??a*"_rx, "aa", { no_tag, no_tag }));
static_assert(match("(a)+?a?"_rx, "aa", { 0, 1 }));
static_assert(match("(a)+a?"_rx, "aa", { 1, 2 }));
static_assert(match("(a){2,3}?a?"_rx, "aaa", { 1, 2 }));
static_assert(match("(a){2,3}a?"_rx, "aaa", { 2, 3 }));
static_assert(match("(ab+c)+?(ab+c|.*d)"_rx, "abcabbcacd", { 0, 3, 3, 10 }));
static_assert(match("(ab+c)+(ab+c|.*d)"_rx, "abcabbcacd", { 3, 7, 7, 10 }));

/* submatch disambiguation tests */
static_assert(match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_rx, "abcdefg", { 6, 7 }));     /* [perl]: a bcdef g */
static_assert(not match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_rx, "abcdefg", { 4, 7 })); /* [posix]: ab c d efg */
static_assert(not match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_rx, "abcdefg", { 5, 7 })); /* [incorrect]: ab c d e fg */

/* capture location tests */
static_assert(match("(a)"_rx, "a", { 0, 1 }));
static_assert(match("a(a)"_rx, "aa", { 1, 2 }));
static_assert(match("a(a)a"_rx, "aaa", { 1, 2 }));
static_assert(match("(a)*"_rx, "a", { 0, 1 }));
static_assert(match("(a)*"_rx, "aa", { 1, 2 }));
static_assert(match("(?:(a)|(d))c"_rx, "ac", { 0, 1, no_tag, no_tag }));
static_assert(match("(?:(a)|(d))c"_rx, "dc", { no_tag, no_tag, 0, 1 }));
static_assert(match("(a)+(b)*"_rx, "ab", { 0, 1, 1, 2 }));
static_assert(match("(a)+(b)*"_rx, "aab", { 1, 2, 2, 3 }));
static_assert(match("(aa)+a*"_rx, "aaaaa", { 2, 4 }));
static_assert(match("(aa)+?a*"_rx, "aaaaa", { 0, 2 }));
static_assert(match("(a((a)))(a)c"_rx, "aaac", { 0, 2, 1, 2, 1, 2, 2, 3 }));
static_assert(match("a(aa)b|(aa)ac"_rx, "aaab", { 1, 3, no_tag, no_tag }));
static_assert(match("a(aa)b|(aa)ac"_rx, "aaac", { no_tag, no_tag, 0, 2 }));

/* wildcard tests */
static_assert(not match("."_rx, ""));
static_assert(match("."_rx, "0"));
static_assert(match("."_rx, "@"));
static_assert(match("."_rx, "$"));
static_assert(match("."_rx, "z"));
static_assert(match("."_rx, "A"));
static_assert(match("."_rx, "."));
static_assert(not match("."_rx, "\n"));
static_assert(match("(?s)."_rx, "\n"));
// TODO: add test cases for non ascii chars


/* fallback tests */
static_assert(prefix_match("abc"_rx, "abcdef", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_rx, "abc", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_rx, "abcdef", { 0, 6 }));
static_assert(prefix_match("abc|abcdef"_rx, "abcdef", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_rx, "abcde", { 0, 3 }));

/* lazy prefix matching tests */
static_assert(prefix_match("(abc)+"_rx, "abcabc", { 0, 6, 3, 6 }));
static_assert(prefix_match("(abc)+?"_rx, "abcabc", { 0, 3, 0, 3 }));
static_assert(prefix_match("(abc)+?a"_rx, "abcabc", { 0, 4, 0, 3 }));
static_assert(prefix_match("(abc)+?a"_rx, "abcabca", { 0, 4, 0, 3 }));

/* additional capture location tests */
static_assert(match("(ab+c)+?(ab+c|.*d)"_rx, "abcabbcacd", { 0, 3, 3, 10 }));
static_assert(prefix_match("(ab+c)+?(ab+c|.*d)"_rx, "abcabbcacd", { 0, 7, 0, 3, 3, 7 }));
static_assert(prefix_match("(ab+c)+(ab+c|.*d)"_rx, "abcabbcacd", { 0, 10, 3, 7, 7, 10 }));
static_assert(search("([ad]b+c)+?([ad])"_rx, "aaabacabcdbbcacd", { 6, 10, 6, 9, 9, 10 }));
static_assert(search("([ad]b+c)+([ad])"_rx, "aaabacabcdbbcacd", { 6, 14, 9, 13, 13, 14 }));

/* search tests */
static_assert(search("a"_rx, "abcd", { 0, 1 }));
static_assert(search("b"_rx, "abcd", { 1, 2 }));
static_assert(search("d"_rx, "abcd", { 3, 4 }));
static_assert(not search("e"_rx, "abcd"));
static_assert(search("aa"_rx, "abaab", { 2, 4 }));
static_assert(search("ab"_rx, "abaab", { 0, 2 }));
static_assert(search("bc"_rx, "abcd", { 1, 3 }));
static_assert(search_all("a"_rx, "a", { { 0, 1 } }));
static_assert(search_all("a"_rx, "aaa", { { 0, 1 }, { 1, 2 }, { 2, 3 } }));
static_assert(search_all("ab"_rx, "abab", { { 0, 2 }, { 2, 4 } }));
static_assert(search_all("ab"_rx, "abaab", { { 0, 2 }, { 3, 5 } }));

/* sof+eof anchor tests */
static_assert(match("a$"_rx, "a"));
static_assert(match("^a"_rx, "a"));
static_assert(match("^a$"_rx, "a"));
static_assert(match("^$"_rx, ""));
static_assert(prefix_match("a$"_rx, "a"));
static_assert(prefix_match("^a"_rx, "a"));
static_assert(prefix_match("^a$"_rx, "a"));
static_assert(prefix_match("^$"_rx, ""));
static_assert(not search("^$"_rx, "a"));
static_assert(search("^$"_rx, "", { 0, 0 }));
static_assert(search("$^"_rx, "", { 0, 0 }));
static_assert(search("a$"_rx, "a", { 0, 1 }));
static_assert(search("^a"_rx, "a", { 0, 1 }));
static_assert(search("^a$"_rx, "a", { 0, 1 }));
static_assert(prefix_match("(a)+?$$"_rx, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?$$"_rx, "aa", { 0, 2, 1, 2 }));
static_assert(prefix_match("(a)+?$"_rx, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?$"_rx, "aa", { 0, 2, 1, 2 }));
static_assert(prefix_match("(a)+?"_rx, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?"_rx, "aa", { 0, 1, 0, 1 }));
static_assert(not search("^^ab"_rx, "bab"));
static_assert(search("^^ab"_rx, "aba", { 0, 2 }));
static_assert(not search("^ab"_rx, "bab"));
static_assert(search("^ab"_rx, "aba", { 0, 2 }));
static_assert(search("ab"_rx, "bab", { 1, 3 }));
static_assert(search("ab"_rx, "aba", { 0, 2 }));
static_assert(search("(abc)|(^abc)"_rx, "abc", { 0, 3, 0, 3, no_tag, no_tag }));

/* sol+eol anchor tests */
static_assert(prefix_match("(?m:a$)"_rx, "a", { 0, 1 }));
static_assert(prefix_match("(?m:a$)"_rx, "a\na", { 0, 1 }));
static_assert(prefix_match("(?m:a$\na)"_rx, "a\na", { 0, 3 }));
static_assert(prefix_match("(?m:a$\na)"_rx, "a\na", { 0, 3 }));
static_assert(prefix_match("(?m)($\na)+"_rx, "\na", { 0, 2, 0, 2 }));
static_assert(prefix_match("(?m)($\na)+"_rx, "\na\na", { 0, 4, 2, 4 }));
static_assert(prefix_match("(?m)(\n$)+"_rx, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(prefix_match("(?m)(a$)"_rx, "a\na", { 0, 1, 0, 1 }));
static_assert(search("(?m:^a)"_rx, "a", { 0, 1 }));
static_assert(search("(?m:^a)"_rx, "\na", { 1, 2 }));
static_assert(search("(?ms)(^a.?)*"_rx, "a\na", { 0, 3, 2, 3 }));
static_assert(search("(?m:^\n*$)"_rx, "\n\n\n\n", { 0, 4 }));
static_assert(search("(?m:^\n*?$)"_rx, "\n\n\n\n", { 0, 0 }));
static_assert(prefix_match("(?m)(\n$)+"_rx, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)(^\n$)+"_rx, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)($\n^)+"_rx, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)(^a\n)+"_rx, "a\na\na", { 0, 4, 2, 4 }));

/* word boundary tests */
static_assert(not search(R"(\b)"_rx, ""));
static_assert(search(R"(\B)"_rx, "", { 0, 0 }));
static_assert(search_all(R"(\b)"_rx, "ab+-cd", { { 0, 0 }, { 2, 2 }, { 4, 4 }, { 6, 6 } }));
static_assert(search_all(R"(\B)"_rx, "ab<>cd", { { 1, 1 }, { 3, 3 }, { 5, 5 } }));
static_assert(search(R"(\B)"_rx, "a_", { 1, 1 }));
static_assert(search(R"(a\b)"_rx, "a+", { 0, 1 }));
static_assert(search(R"(a\b)"_rx, "a.", { 0, 1 }));
static_assert(not search(R"(a\b)"_rx, "a_"));


/* other tests */
// "R(est)|(Res)T" -> hopcroft
