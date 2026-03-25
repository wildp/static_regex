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

    template<rx::string_literal S>
    consteval bool match(rx::static_regex<S, rx::mode::naive> pattern, std::string_view str)
    {
        return pattern.is_match(str);
    }

    template<rx::string_literal S>
    consteval bool match(rx::static_regex<S, rx::mode::naive> pattern, std::string_view str, const std::vector<std::size_t>& captures)
    {
        return submatch_check(pattern.match(str), captures, str.begin(), 1);
    }

    template<rx::string_literal S>
    consteval bool prefix_match(rx::static_regex<S, rx::mode::naive> pattern, std::string_view str)
    {
       return pattern.starts_with_match(str);
    }

    template<rx::string_literal S>
    consteval bool prefix_match(rx::static_regex<S, rx::mode::naive> pattern, std::string_view str, const std::vector<std::size_t>& captures)
    {
        return submatch_check(pattern.prefix_match(str), captures, str.begin());
    }

    template<rx::string_literal S>
    consteval bool search(rx::static_regex<S, rx::mode::naive> pattern, std::string_view str)
    {
       return pattern.contains_match(str);
    }

    template<rx::string_literal S>
    consteval bool search(rx::static_regex<S, rx::mode::naive> pattern, std::string_view str, const std::vector<std::size_t>& captures)
    {
        return submatch_check(pattern.search(str), captures, str.begin());
    }

    template<rx::string_literal S>
    consteval bool search_all(rx::static_regex<S, rx::mode::naive> pattern, std::string_view str, const std::vector<std::vector<std::size_t>>& captures)
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
static_assert(match(""_rxn, ""));
static_assert(not match(""_rxn, "a"));
static_assert(not match("a"_rxn, ""));
static_assert(match("a"_rxn, "a"));
static_assert(not match("a"_rxn, "b"));

/* grouping tests */
static_assert(not match("a|b"_rxn, ""));
static_assert(match("a|b"_rxn, "a"));
static_assert(match("a|b"_rxn, "b"));
static_assert(not match("a|b"_rxn, "c"));
static_assert(match("a|b|c"_rxn, "c"));
static_assert(match("a|b|c"_rxn, "b"));
static_assert(match("a|b|c"_rxn, "a"));
static_assert(not match("aa"_rxn, ""));
static_assert(not match("aa"_rxn, "a"));
static_assert(match("aa"_rxn, "aa"));
static_assert(not match("aa"_rxn, "aaa"));
static_assert(not match("ab"_rxn, ""));
static_assert(not match("ab"_rxn, "a"));
static_assert(not match("ab"_rxn, "b"));
static_assert(match("ab"_rxn, "ab"));
static_assert(not match("ab"_rxn, "ba"));
static_assert(not match("ab"_rxn, "abb"));
static_assert(not match("ab"_rxn, "aab"));
static_assert(not match("ab"_rxn, "bb"));

/* character class tests */
static_assert(match("[a-c]"_rxn, "a"));
static_assert(match("[a-c]"_rxn, "b"));
static_assert(match("[a-c]"_rxn, "c"));
static_assert(not match("[a-c]"_rxn, "d"));

/* repeater tests */
static_assert(match("a*"_rxn, ""));
static_assert(match("a*"_rxn, "a"));
static_assert(match("a*"_rxn, "aa"));
static_assert(match("a*"_rxn, "aaaaa"));
static_assert(not match("a+"_rxn, ""));
static_assert(match("a+"_rxn, "a"));
static_assert(match("a+"_rxn, "aa"));
static_assert(match("a+"_rxn, "aaaaa"));
static_assert(match("a?"_rxn, ""));
static_assert(match("a?"_rxn, "a"));
static_assert(not match("a?"_rxn, "aa"));
static_assert(not match("a?"_rxn, "aaaaa"));
static_assert(not match("a{1}"_rxn, ""));
static_assert(match("a{1}"_rxn, "a"));
static_assert(not match("a{1}"_rxn, "aa"));
static_assert(not match("a{2,5}"_rxn, "a"));
static_assert(match("a{2,5}"_rxn, "aa"));
static_assert(match("a{2,5}"_rxn, "aaa"));
static_assert(match("a{2,5}"_rxn, "aaaa"));
static_assert(match("a{2,5}"_rxn, "aaaaa"));
static_assert(not match("a{2,5}"_rxn, "aaaaaaa"));
static_assert(not match("a{2,}"_rxn, "a"));
static_assert(match("a{2,}"_rxn, "aa"));
static_assert(match("a{2,}"_rxn, "aaa"));
static_assert(match("a{2,}"_rxn, "aaaa"));
static_assert(match("a{2,}"_rxn, "aaaaa"));
static_assert(not match("a{3},"_rxn, "aa"));
static_assert(match("a{3}"_rxn, "aaa"));
static_assert(not match("a{3}"_rxn, "aaaa"));

/* laziness tests */
static_assert(match("(a)?a*"_rxn, "", { no_tag, no_tag }));
static_assert(match("(a)?a*"_rxn, "a", { 0, 1 }));
static_assert(match("(a)?a*"_rxn, "aa", { 0, 1 }));
static_assert(match("(a)??a*"_rxn, "aa", { no_tag, no_tag }));
static_assert(match("(a)+?a?"_rxn, "aa", { 0, 1 }));
static_assert(match("(a)+a?"_rxn, "aa", { 1, 2 }));
static_assert(match("(a){2,3}?a?"_rxn, "aaa", { 1, 2 }));
static_assert(match("(a){2,3}a?"_rxn, "aaa", { 2, 3 }));
static_assert(match("(ab+c)+?(ab+c|.*d)"_rxn, "abcabbcacd", { 0, 3, 3, 10 }));
static_assert(match("(ab+c)+(ab+c|.*d)"_rxn, "abcabbcacd", { 3, 7, 7, 10 }));

/* submatch disambiguation tests */
static_assert(match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_rxn, "abcdefg", { 6, 7 }));     /* [perl]: a bcdef g */
static_assert(not match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_rxn, "abcdefg", { 4, 7 })); /* [posix]: ab c d efg */
static_assert(not match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_rxn, "abcdefg", { 5, 7 })); /* [incorrect]: ab c d e fg */

/* capture location tests */
static_assert(match("(a)"_rxn, "a", { 0, 1 }));
static_assert(match("a(a)"_rxn, "aa", { 1, 2 }));
static_assert(match("a(a)a"_rxn, "aaa", { 1, 2 }));
static_assert(match("(a)*"_rxn, "a", { 0, 1 }));
static_assert(match("(a)*"_rxn, "aa", { 1, 2 }));
static_assert(match("(?:(a)|(d))c"_rxn, "ac", { 0, 1, no_tag, no_tag }));
static_assert(match("(?:(a)|(d))c"_rxn, "dc", { no_tag, no_tag, 0, 1 }));
static_assert(match("(a)+(b)*"_rxn, "ab", { 0, 1, 1, 2 }));
static_assert(match("(a)+(b)*"_rxn, "aab", { 1, 2, 2, 3 }));
static_assert(match("(aa)+a*"_rxn, "aaaaa", { 2, 4 }));
static_assert(match("(aa)+?a*"_rxn, "aaaaa", { 0, 2 }));
static_assert(match("(a((a)))(a)c"_rxn, "aaac", { 0, 2, 1, 2, 1, 2, 2, 3 }));
static_assert(match("a(aa)b|(aa)ac"_rxn, "aaab", { 1, 3, no_tag, no_tag }));
static_assert(match("a(aa)b|(aa)ac"_rxn, "aaac", { no_tag, no_tag, 0, 2 }));

/* wildcard tests */
static_assert(not match("."_rxn, ""));
static_assert(match("."_rxn, "0"));
static_assert(match("."_rxn, "@"));
static_assert(match("."_rxn, "$"));
static_assert(match("."_rxn, "z"));
static_assert(match("."_rxn, "A"));
static_assert(match("."_rxn, "."));
static_assert(not match("."_rxn, "\n"));
static_assert(match("(?s)."_rxn, "\n"));
// TODO: add test cases for non ascii chars

/* backtracking tests */
static_assert(match("a(b|bb)b"_rxn, "abb"));
static_assert(match("a(b|bb)b"_rxn, "abbb"));
static_assert(match("a(bb|b)b"_rxn, "abb"));
static_assert(match("a(bb|b)b"_rxn, "abbb"));
static_assert(match("(a*b)+"_rxn, "aaababaaaab"));
static_assert(match("(a+){4}"_rxn, "aaaaaaaaaaaa"));
static_assert(match("(aa|aaa)*a"_rxn, "aaaaaa"));

/* atomic matching tests */
static_assert(match("(a)++[ab]"_rxn, "aaab", { 2, 3 }));
static_assert(not match("(a)++[ab]"_rxn, "aaaa"));
static_assert(match("(?:(a)++[ab])|(a*)"_rxn, "aaab", { 2, 3, no_tag, no_tag }));
static_assert(match("(?:(a)++[ab])|(a*)"_rxn, "aaaa", { no_tag, no_tag, 0, 4 }));

/* backreference tests */
static_assert(match("(abc)\\1"_rxn, "abcabc"));
static_assert(match("(a|b|c)\\g1"_rxn, "bb"));
static_assert(match("(a|\1){2}"_rxn, "aa"));
static_assert(match("(a)(b)\\g2\\g1"_rxn, "abba"));
static_assert(match("(ab(cd))\\1\\2"_rxn, "abcdabcdcd"));
static_assert(not match("(ab(cd))\\1\\2"_rxn, "abcdcdabcd"));
static_assert(match("((ab|e\\3)(c*)d){2}"_rxn, "abccdeccd"));
static_assert(match("((a)(b)\\3\\2){2}"_rxn, "abbaabba"));
static_assert(not match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\10"_rxn, "abcdefghijkj"));
static_assert(not match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g10"_rxn, "abcdefghijkj"));
static_assert(match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g10"_rxn, "abcdefghijka0"));
static_assert(match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g{10}"_rxn, "abcdefghijkj"));
static_assert(match("(.*)\\1"_rxn, "abcdeabcde"));
static_assert(not match("\\1(abc)\\1"_rxn, "abc"));
static_assert(match("(abc|a\\1c)+"_rxn, "abc"));
static_assert(match("(abc|a\\1c)+"_rxn, "abcaabcc"));
static_assert(match("(abc|a\\1c)+"_rxn, "abcaabccaaabccc"));
static_assert(match(".?|(..+?)\\1+"_rxn, "a"));
static_assert(not match(".?|(..+?)\\1+"_rxn, "aa"));
static_assert(not match(".?|(..+?)\\1+"_rxn, "aaa"));
static_assert(match(".?|(..+?)\\1+"_rxn, "aaaa"));
static_assert(not match(".?|(..+?)\\1+"_rxn, "aaaaa"));
static_assert(match(".?|(..+?)\\1+"_rxn, "aaaaaaaaaa"));
static_assert(not match(".?|(..+?)\\1+"_rxn, "aaaaaaaaaaa"));

/* capture features */
static_assert(match("(a)"_rxn, "a"));
static_assert(match("(a)(b)"_rxn, "ab"));
static_assert(match("(a(b))"_rxn, "ab"));
static_assert(match("(?:a)"_rxn, "a"));
static_assert(match("(?#comment)"_rxn, ""));
static_assert(match("(?:c)(a)(b)\\2"_rxn, "cabb"));
static_assert(not match("(?:c)(a)(b)\\2"_rxn, "caba"));
static_assert(match("(?|(a)|(b))\\1"_rxn, "aa"));
static_assert(match("(?|(a)|(b))\\1"_rxn, "bb"));
static_assert(match("(?|(a)|(b))(c)\\1\\2"_rxn, "acac"));
static_assert(match("(?|(a)|(b))(c)\\1\\2"_rxn, "bcbc"));
static_assert(match("(?|(a)|(b)(c))(d)\\3"_rxn, "add"));
static_assert(match("(?|(a)|(b)(c))(d)\\1\\2"_rxn, "bcdbc"));
static_assert(match("(?|(a)|(b)(c)|(d)(e))(f)\\1\\2"_rxn, "bcfbc"));
static_assert(match("(?|(a)|(b)(c)|(d)(e))(f)\\1\\2"_rxn, "defde"));

/* caseless flag */
static_assert(match("(?i)abc"_rxn, "ABC"));
static_assert(match("(?i)abc"_rxn, "Abc"));
static_assert(match("(?i)abc"_rxn, "aBc"));
static_assert(match("(?i)a[be]"_rxn, "aB"));
static_assert(match("(?i)a[Z-a]"_rxn, "az"));
static_assert(match("a(?i:b)c"_rxn, "abc"));
static_assert(match("a(?i:b)c"_rxn, "aBc"));
static_assert(not match("a(?i:b)c"_rxn, "abC"));

/* dotall flag */
static_assert(match("(?s)."_rxn, "\n"));
static_assert(match("a.b"_rxn, "a-b"));
static_assert(not match("a.b"_rxn, "a\nb"));
static_assert(match("(?s)a.b"_rxn, "a\nb"));
static_assert(not match("((?s))a.b"_rxn, "a\nb"));
static_assert(not match("(?:(?s))a.b"_rxn, "a\nb"));

/* ungreedy flag */
static_assert(match("(?U)(a)?a*"_rxn, "aa", { no_tag, no_tag }));
static_assert(match("(?U)(a)??a*"_rxn, "aa", { 0, 1 }));
static_assert(match("(?U:)(a)?a*"_rxn, "aa", { 0, 1 }));
static_assert(match("(?U:)(a)??a*"_rxn, "aa", { no_tag, no_tag }));
static_assert(match("(?U)(aa)+a*"_rxn, "aaaaa", { 0, 2 }));
static_assert(match("(?U)(aa)+?a*"_rxn, "aaaaa", { 2, 4 }));
static_assert(match("(?U-U)(a)?a*"_rxn, "aa", { 0, 1 }));
static_assert(match("(?U-U)(a)??a*"_rxn, "aa", { no_tag, no_tag }));
static_assert(match("(?U)(a)?a*(?-U)(b)?b*"_rxn, "aabb", { no_tag, no_tag, 2, 3 }));
static_assert(match("(?U)(a)??a*(?-U)(b)??b*"_rxn, "aabb", { 0, 1, no_tag, no_tag }));

/* additional parser tests */
static_assert(match("\\Q...\\E"_rxn, "..."));
static_assert(not match("\\Q...\\E"_rxn, "aaa"));

/* fallback tests */
static_assert(prefix_match("abc"_rxn, "abcdef", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_rxn, "abc", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_rxn, "abcdef", { 0, 6 }));
static_assert(prefix_match("abc|abcdef"_rxn, "abcdef", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_rxn, "abcde", { 0, 3 }));

/* lazy prefix matching tests */
static_assert(prefix_match("(abc)+"_rxn, "abcabc", { 0, 6, 3, 6 }));
static_assert(prefix_match("(abc)+?"_rxn, "abcabc", { 0, 3, 0, 3 }));
static_assert(prefix_match("(abc)+?a"_rxn, "abcabc", { 0, 4, 0, 3 }));
static_assert(prefix_match("(abc)+?a"_rxn, "abcabca", { 0, 4, 0, 3 }));

/* additional capture location tests */
static_assert(match("(ab+c)+?(ab+c|.*d)"_rxn, "abcabbcacd", { 0, 3, 3, 10 }));
static_assert(prefix_match("(ab+c)+?(ab+c|.*d)"_rxn, "abcabbcacd", { 0, 7, 0, 3, 3, 7 }));
static_assert(prefix_match("(ab+c)+(ab+c|.*d)"_rxn, "abcabbcacd", { 0, 10, 3, 7, 7, 10 }));
static_assert(search("([ad]b+c)+?([ad])"_rxn, "aaabacabcdbbcacd", { 6, 10, 6, 9, 9, 10 }));
static_assert(search("([ad]b+c)+([ad])"_rxn, "aaabacabcdbbcacd", { 6, 14, 9, 13, 13, 14 }));

/* search tests */
static_assert(search("a"_rxn, "abcd", { 0, 1 }));
static_assert(search("b"_rxn, "abcd", { 1, 2 }));
static_assert(search("d"_rxn, "abcd", { 3, 4 }));
static_assert(not search("e"_rxn, "abcd"));
static_assert(search("aa"_rxn, "abaab", { 2, 4 }));
static_assert(search("ab"_rxn, "abaab", { 0, 2 }));
static_assert(search("bc"_rxn, "abcd", { 1, 3 }));
static_assert(search_all("a"_rxn, "a", { { 0, 1 } }));
static_assert(search_all("a"_rxn, "aaa", { { 0, 1 }, { 1, 2 }, { 2, 3 } }));
static_assert(search_all("ab"_rxn, "abab", { { 0, 2 }, { 2, 4 } }));
static_assert(search_all("ab"_rxn, "abaab", { { 0, 2 }, { 3, 5 } }));
static_assert(search_all("(?:)|abc"_rxn, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
static_assert(search_all("(?:)|abc"_rxn, "abcabc", { { 0, 0 }, { 0, 3 }, { 3, 3 }, { 3, 6 }, { 6, 6 } }));

/* sof+eof anchor tests */
static_assert(match("a$"_rxn, "a"));
static_assert(match("^a"_rxn, "a"));
static_assert(match("^a$"_rxn, "a"));
static_assert(match("^$"_rxn, ""));
static_assert(prefix_match("a$"_rxn, "a"));
static_assert(prefix_match("^a"_rxn, "a"));
static_assert(prefix_match("^a$"_rxn, "a"));
static_assert(prefix_match("^$"_rxn, ""));
static_assert(not search("^$"_rxn, "a"));
static_assert(search("^$"_rxn, "", { 0, 0 }));
static_assert(search("$^"_rxn, "", { 0, 0 }));
static_assert(search("a$"_rxn, "a", { 0, 1 }));
static_assert(search("^a"_rxn, "a", { 0, 1 }));
static_assert(search("^a$"_rxn, "a", { 0, 1 }));
static_assert(prefix_match("(a)+?$$"_rxn, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?$$"_rxn, "aa", { 0, 2, 1, 2 }));
static_assert(prefix_match("(a)+?$"_rxn, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?$"_rxn, "aa", { 0, 2, 1, 2 }));
static_assert(prefix_match("(a)+?"_rxn, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?"_rxn, "aa", { 0, 1, 0, 1 }));
static_assert(not search("^^ab"_rxn, "bab"));
static_assert(search("^^ab"_rxn, "aba", { 0, 2 }));
static_assert(not search("^ab"_rxn, "bab"));
static_assert(search("^ab"_rxn, "aba", { 0, 2 }));
static_assert(search("ab"_rxn, "bab", { 1, 3 }));
static_assert(search("ab"_rxn, "aba", { 0, 2 }));
static_assert(search("(abc)|(^abc)"_rxn, "abc", { 0, 3, 0, 3, no_tag, no_tag }));

/* sol+eol anchor tests */
static_assert(prefix_match("(?m:a$)"_rxn, "a", { 0, 1 }));
static_assert(prefix_match("(?m:a$)"_rxn, "a\na", { 0, 1 }));
static_assert(prefix_match("(?m:a$\na)"_rxn, "a\na", { 0, 3 }));
static_assert(prefix_match("(?m:a$\na)"_rxn, "a\na", { 0, 3 }));
static_assert(prefix_match("(?m)($\na)+"_rxn, "\na", { 0, 2, 0, 2 }));
static_assert(prefix_match("(?m)($\na)+"_rxn, "\na\na", { 0, 4, 2, 4 }));
static_assert(prefix_match("(?m)(\n$)+"_rxn, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(prefix_match("(?m)(a$)"_rxn, "a\na", { 0, 1, 0, 1 }));
static_assert(search("(?m:^a)"_rxn, "a", { 0, 1 }));
static_assert(search("(?m:^a)"_rxn, "\na", { 1, 2 }));
static_assert(search("(?ms)(^a.?)*"_rxn, "a\na", { 0, 3, 2, 3 }));
static_assert(search("(?m:^\n*$)"_rxn, "\n\n\n\n", { 0, 4 }));
static_assert(search("(?m:^\n*?$)"_rxn, "\n\n\n\n", { 0, 0 }));
static_assert(prefix_match("(?m)(\n$)+"_rxn, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)(^\n$)+"_rxn, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)($\n^)+"_rxn, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)(^a\n)+"_rxn, "a\na\na", { 0, 4, 2, 4 }));

/* word boundary tests */
static_assert(not search(R"(\b)"_rxn, ""));
static_assert(search(R"(\B)"_rxn, "", { 0, 0 }));
static_assert(search_all(R"(\b)"_rxn, "ab+-cd", { { 0, 0 }, { 2, 2 }, { 4, 4 }, { 6, 6 } }));
static_assert(search_all(R"(\B)"_rxn, "ab<>cd", { { 1, 1 }, { 3, 3 }, { 5, 5 } }));
static_assert(search(R"(\B)"_rxn, "a_", { 1, 1 }));
static_assert(search(R"(a\b)"_rxn, "a+", { 0, 1 }));
static_assert(search(R"(a\b)"_rxn, "a.", { 0, 1 }));
static_assert(not search(R"(a\b)"_rxn, "a_"));
static_assert(search_all(R"(\b|abc)"_rxn, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
static_assert(search_all(R"(\b|^abc)"_rxn, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));

