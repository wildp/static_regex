// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <srx/regex.hpp>


namespace
{
    template<typename MatchResult, std::bidirectional_iterator Iter>
    consteval bool submatch_check(const MatchResult& mr, const std::vector<std::size_t>& captures, const Iter start, std::size_t init = 0)
    {
        using srx::detail::no_tag;

        if (((mr.size() - init) * 2) != captures.size())
            return false;

        for (std::size_t i{ init }, i_end{ mr.size() }; i < i_end; ++i)
        {
            const auto submatch = mr.at(i);
            auto lhs = captures.at((i - init) * 2);
            auto rhs = captures.at(((i - init) * 2) + 1);

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

    template<srx::string_literal S>
    consteval bool match(srx::static_regex<S> pattern, std::string_view str)
    {
        return pattern.is_match(str);
    }

    template<srx::string_literal S>
    consteval bool match(srx::static_regex<S> pattern, std::string_view str, const std::vector<std::size_t>& captures)
    {
        return submatch_check(pattern.match(str), captures, str.begin(), 1);
    }

    template<srx::string_literal S>
    consteval bool prefix_match(srx::static_regex<S> pattern, std::string_view str)
    {
       return pattern.starts_with_match(str);
    }

    template<srx::string_literal S>
    consteval bool prefix_match(srx::static_regex<S> pattern, std::string_view str, const std::vector<std::size_t>& captures)
    {
        return submatch_check(pattern.prefix_match(str), captures, str.begin());
    }

    template<srx::string_literal S>
    consteval bool search(srx::static_regex<S> pattern, std::string_view str)
    {
       return pattern.contains_match(str);
    }

    template<srx::string_literal S>
    consteval bool search(srx::static_regex<S> pattern, std::string_view str, const std::vector<std::size_t>& captures)
    {
        return submatch_check(pattern.search(str), captures, str.begin());
    }

    template<srx::string_literal S>
    consteval bool search_all(srx::static_regex<S> pattern, std::string_view str, const std::vector<std::vector<std::size_t>>& captures)
    {
        for (const auto& [match, caps] : std::views::zip(pattern.range(str), captures))
        {
            if (not submatch_check(match, caps, str.begin()))
                return false;
        }
        return true;
    }
}

using srx::detail::no_tag;
using namespace srx::literals;

/* basic tests */
static_assert(match(""_srx, ""));
static_assert(not match(""_srx, "a"));
static_assert(not match("a"_srx, ""));
static_assert(match("a"_srx, "a"));
static_assert(not match("a"_srx, "b"));

/* grouping tests */
static_assert(not match("a|b"_srx, ""));
static_assert(match("a|b"_srx, "a"));
static_assert(match("a|b"_srx, "b"));
static_assert(not match("a|b"_srx, "c"));
static_assert(match("a|b|c"_srx, "c"));
static_assert(match("a|b|c"_srx, "b"));
static_assert(match("a|b|c"_srx, "a"));
static_assert(not match("aa"_srx, ""));
static_assert(not match("aa"_srx, "a"));
static_assert(match("aa"_srx, "aa"));
static_assert(not match("aa"_srx, "aaa"));
static_assert(not match("ab"_srx, ""));
static_assert(not match("ab"_srx, "a"));
static_assert(not match("ab"_srx, "b"));
static_assert(match("ab"_srx, "ab"));
static_assert(not match("ab"_srx, "ba"));
static_assert(not match("ab"_srx, "abb"));
static_assert(not match("ab"_srx, "aab"));
static_assert(not match("ab"_srx, "bb"));

/* character class tests */
static_assert(match("[a-c]"_srx, "a"));
static_assert(match("[a-c]"_srx, "b"));
static_assert(match("[a-c]"_srx, "c"));
static_assert(not match("[a-c]"_srx, "d"));

/* repeater tests */
static_assert(match("a*"_srx, ""));
static_assert(match("a*"_srx, "a"));
static_assert(match("a*"_srx, "aa"));
static_assert(match("a*"_srx, "aaaaa"));
static_assert(not match("a+"_srx, ""));
static_assert(match("a+"_srx, "a"));
static_assert(match("a+"_srx, "aa"));
static_assert(match("a+"_srx, "aaaaa"));
static_assert(match("a?"_srx, ""));
static_assert(match("a?"_srx, "a"));
static_assert(not match("a?"_srx, "aa"));
static_assert(not match("a?"_srx, "aaaaa"));
static_assert(not match("a{1}"_srx, ""));
static_assert(match("a{1}"_srx, "a"));
static_assert(not match("a{1}"_srx, "aa"));
static_assert(not match("a{2,5}"_srx, "a"));
static_assert(match("a{2,5}"_srx, "aa"));
static_assert(match("a{2,5}"_srx, "aaa"));
static_assert(match("a{2,5}"_srx, "aaaa"));
static_assert(match("a{2,5}"_srx, "aaaaa"));
static_assert(not match("a{2,5}"_srx, "aaaaaaa"));
static_assert(not match("a{2,}"_srx, "a"));
static_assert(match("a{2,}"_srx, "aa"));
static_assert(match("a{2,}"_srx, "aaa"));
static_assert(match("a{2,}"_srx, "aaaa"));
static_assert(match("a{2,}"_srx, "aaaaa"));
static_assert(not match("a{3},"_srx, "aa"));
static_assert(match("a{3}"_srx, "aaa"));
static_assert(not match("a{3}"_srx, "aaaa"));

/* laziness tests */
static_assert(match("(a)?a*"_srx, "", { no_tag, no_tag }));
static_assert(match("(a)?a*"_srx, "a", { 0, 1 }));
static_assert(match("(a)?a*"_srx, "aa", { 0, 1 }));
static_assert(match("(a)??a*"_srx, "aa", { no_tag, no_tag }));
static_assert(match("(a)+?a?"_srx, "aa", { 0, 1 }));
static_assert(match("(a)+a?"_srx, "aa", { 1, 2 }));
static_assert(match("(a){2,3}?a?"_srx, "aaa", { 1, 2 }));
static_assert(match("(a){2,3}a?"_srx, "aaa", { 2, 3 }));
static_assert(match("(ab+c)+?(ab+c|.*d)"_srx, "abcabbcacd", { 0, 3, 3, 10 }));
static_assert(match("(ab+c)+(ab+c|.*d)"_srx, "abcabbcacd", { 3, 7, 7, 10 }));

/* submatch disambiguation tests */
static_assert(match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_srx, "abcdefg", { 6, 7 }));     /* [perl]: a bcdef g */
static_assert(not match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_srx, "abcdefg", { 4, 7 })); /* [posix]: ab c d efg */
static_assert(not match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_srx, "abcdefg", { 5, 7 })); /* [incorrect]: ab c d e fg */

/* capture location tests */
static_assert(match("(a)"_srx, "a", { 0, 1 }));
static_assert(match("a(a)"_srx, "aa", { 1, 2 }));
static_assert(match("a(a)a"_srx, "aaa", { 1, 2 }));
static_assert(match("(a)*"_srx, "a", { 0, 1 }));
static_assert(match("(a)*"_srx, "aa", { 1, 2 }));
static_assert(match("(?:(a)|(d))c"_srx, "ac", { 0, 1, no_tag, no_tag }));
static_assert(match("(?:(a)|(d))c"_srx, "dc", { no_tag, no_tag, 0, 1 }));
static_assert(match("(a)+(b)*"_srx, "ab", { 0, 1, 1, 2 }));
static_assert(match("(a)+(b)*"_srx, "aab", { 1, 2, 2, 3 }));
static_assert(match("(aa)+a*"_srx, "aaaaa", { 2, 4 }));
static_assert(match("(aa)+?a*"_srx, "aaaaa", { 0, 2 }));
static_assert(match("(a((a)))(a)c"_srx, "aaac", { 0, 2, 1, 2, 1, 2, 2, 3 }));
static_assert(match("a(aa)b|(aa)ac"_srx, "aaab", { 1, 3, no_tag, no_tag }));
static_assert(match("a(aa)b|(aa)ac"_srx, "aaac", { no_tag, no_tag, 0, 2 }));

/* wildcard tests */
static_assert(not match("."_srx, ""));
static_assert(match("."_srx, "0"));
static_assert(match("."_srx, "@"));
static_assert(match("."_srx, "$"));
static_assert(match("."_srx, "z"));
static_assert(match("."_srx, "A"));
static_assert(match("."_srx, "."));
static_assert(not match("."_srx, "\n"));
static_assert(match("(?s)."_srx, "\n"));
// TODO: add test cases for non ascii chars


/* fallback tests */
static_assert(prefix_match("abc"_srx, "abcdef", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_srx, "abc", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_srx, "abcdef", { 0, 6 }));
static_assert(prefix_match("abc|abcdef"_srx, "abcdef", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_srx, "abcde", { 0, 3 }));

/* lazy prefix matching tests */
static_assert(prefix_match("(abc)+"_srx, "abcabc", { 0, 6, 3, 6 }));
static_assert(prefix_match("(abc)+?"_srx, "abcabc", { 0, 3, 0, 3 }));
static_assert(prefix_match("(abc)+?a"_srx, "abcabc", { 0, 4, 0, 3 }));
static_assert(prefix_match("(abc)+?a"_srx, "abcabca", { 0, 4, 0, 3 }));

/* additional capture location tests */
static_assert(match("(ab+c)+?(ab+c|.*d)"_srx, "abcabbcacd", { 0, 3, 3, 10 }));
static_assert(prefix_match("(ab+c)+?(ab+c|.*d)"_srx, "abcabbcacd", { 0, 7, 0, 3, 3, 7 }));
static_assert(prefix_match("(ab+c)+(ab+c|.*d)"_srx, "abcabbcacd", { 0, 10, 3, 7, 7, 10 }));
static_assert(search("([ad]b+c)+?([ad])"_srx, "aaabacabcdbbcacd", { 6, 10, 6, 9, 9, 10 }));
static_assert(search("([ad]b+c)+([ad])"_srx, "aaabacabcdbbcacd", { 6, 14, 9, 13, 13, 14 }));

/* search tests */
static_assert(search("a"_srx, "abcd", { 0, 1 }));
static_assert(search("b"_srx, "abcd", { 1, 2 }));
static_assert(search("d"_srx, "abcd", { 3, 4 }));
static_assert(not search("e"_srx, "abcd"));
static_assert(search("aa"_srx, "abaab", { 2, 4 }));
static_assert(search("ab"_srx, "abaab", { 0, 2 }));
static_assert(search("bc"_srx, "abcd", { 1, 3 }));
static_assert(search_all("a"_srx, "a", { { 0, 1 } }));
static_assert(search_all("a"_srx, "aaa", { { 0, 1 }, { 1, 2 }, { 2, 3 } }));
static_assert(search_all("ab"_srx, "abab", { { 0, 2 }, { 2, 4 } }));
static_assert(search_all("ab"_srx, "abaab", { { 0, 2 }, { 3, 5 } }));
static_assert(search_all("(?:)|abc"_srx, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
static_assert(search_all("(?:)|abc"_srx, "abcabc", { { 0, 0 }, { 0, 3 }, { 3, 3 }, { 3, 6 }, { 6, 6 } }));

/* sof+eof anchor tests */
static_assert(match("a$"_srx, "a"));
static_assert(match("^a"_srx, "a"));
static_assert(match("^a$"_srx, "a"));
static_assert(match("^$"_srx, ""));
static_assert(prefix_match("a$"_srx, "a"));
static_assert(prefix_match("^a"_srx, "a"));
static_assert(prefix_match("^a$"_srx, "a"));
static_assert(prefix_match("^$"_srx, ""));
static_assert(not search("^$"_srx, "a"));
static_assert(search("^$"_srx, "", { 0, 0 }));
static_assert(search("$^"_srx, "", { 0, 0 }));
static_assert(search("a$"_srx, "a", { 0, 1 }));
static_assert(search("^a"_srx, "a", { 0, 1 }));
static_assert(search("^a$"_srx, "a", { 0, 1 }));
static_assert(prefix_match("(a)+?$$"_srx, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?$$"_srx, "aa", { 0, 2, 1, 2 }));
static_assert(prefix_match("(a)+?$"_srx, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?$"_srx, "aa", { 0, 2, 1, 2 }));
static_assert(prefix_match("(a)+?"_srx, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?"_srx, "aa", { 0, 1, 0, 1 }));
static_assert(not search("^^ab"_srx, "bab"));
static_assert(search("^^ab"_srx, "aba", { 0, 2 }));
static_assert(not search("^ab"_srx, "bab"));
static_assert(search("^ab"_srx, "aba", { 0, 2 }));
static_assert(search("ab"_srx, "bab", { 1, 3 }));
static_assert(search("ab"_srx, "aba", { 0, 2 }));
static_assert(search("(abc)|(^abc)"_srx, "abc", { 0, 3, 0, 3, no_tag, no_tag }));

/* sol+eol anchor tests */
static_assert(prefix_match("(?m:a$)"_srx, "a", { 0, 1 }));
static_assert(prefix_match("(?m:a$)"_srx, "a\na", { 0, 1 }));
static_assert(prefix_match("(?m:a$\na)"_srx, "a\na", { 0, 3 }));
static_assert(prefix_match("(?m:a$\na)"_srx, "a\na", { 0, 3 }));
static_assert(prefix_match("(?m)($\na)+"_srx, "\na", { 0, 2, 0, 2 }));
static_assert(prefix_match("(?m)($\na)+"_srx, "\na\na", { 0, 4, 2, 4 }));
static_assert(prefix_match("(?m)(\n$)+"_srx, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(prefix_match("(?m)(a$)"_srx, "a\na", { 0, 1, 0, 1 }));
static_assert(search("(?m:^a)"_srx, "a", { 0, 1 }));
static_assert(search("(?m:^a)"_srx, "\na", { 1, 2 }));
static_assert(search("(?ms)(^a.?)*"_srx, "a\na", { 0, 3, 2, 3 }));
static_assert(search("(?m:^\n*$)"_srx, "\n\n\n\n", { 0, 4 }));
static_assert(search("(?m:^\n*?$)"_srx, "\n\n\n\n", { 0, 0 }));
static_assert(prefix_match("(?m)(\n$)+"_srx, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)(^\n$)+"_srx, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)($\n^)+"_srx, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)(^a\n)+"_srx, "a\na\na", { 0, 4, 2, 4 }));
static_assert(search_all("(?m)^a"_srx, "a\na\na", { { 0, 1 }, { 2, 3 }, { 4, 5 } }));

/* word boundary tests */
static_assert(not search(R"(\b)"_srx, ""));
static_assert(search(R"(\B)"_srx, "", { 0, 0 }));
static_assert(search_all(R"(\b)"_srx, "ab+-cd", { { 0, 0 }, { 2, 2 }, { 4, 4 }, { 6, 6 } }));
static_assert(search_all(R"(\B)"_srx, "ab<>cd", { { 1, 1 }, { 3, 3 }, { 5, 5 } }));
static_assert(search(R"(\B)"_srx, "a_", { 1, 1 }));
static_assert(search(R"(a\b)"_srx, "a+", { 0, 1 }));
static_assert(search(R"(a\b)"_srx, "a.", { 0, 1 }));
static_assert(not search(R"(a\b)"_srx, "a_"));
static_assert(search_all(R"(\b|abc)"_srx, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
static_assert(search_all(R"(\b|^abc)"_srx, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
