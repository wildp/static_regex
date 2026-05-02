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
    consteval bool match(srx::static_regex<S, srx::mode::backtrack> pattern, std::string_view str)
    {
        return pattern.is_match(str);
    }

    template<srx::string_literal S>
    consteval bool match(srx::static_regex<S, srx::mode::backtrack> pattern, std::string_view str, const std::vector<std::size_t>& captures)
    {
        return submatch_check(pattern.match(str), captures, str.begin(), 1);
    }

    template<srx::string_literal S>
    consteval bool prefix_match(srx::static_regex<S, srx::mode::backtrack> pattern, std::string_view str)
    {
       return pattern.starts_with_match(str);
    }

    template<srx::string_literal S>
    consteval bool prefix_match(srx::static_regex<S, srx::mode::backtrack> pattern, std::string_view str, const std::vector<std::size_t>& captures)
    {
        return submatch_check(pattern.prefix_match(str), captures, str.begin());
    }

    template<srx::string_literal S>
    consteval bool search(srx::static_regex<S, srx::mode::backtrack> pattern, std::string_view str)
    {
       return pattern.contains_match(str);
    }

    template<srx::string_literal S>
    consteval bool search(srx::static_regex<S, srx::mode::backtrack> pattern, std::string_view str, const std::vector<std::size_t>& captures)
    {
        return submatch_check(pattern.search(str), captures, str.begin());
    }

    template<srx::string_literal S>
    consteval bool search_all(srx::static_regex<S, srx::mode::backtrack> pattern, std::string_view str, const std::vector<std::vector<std::size_t>>& captures)
    {
        for (const auto& [match, caps] : std::views::zip(pattern.range(str), captures))
        {
            if (not submatch_check(match, caps, str.begin()))
                return false;
        }
        return true;
    }

    template<srx::string_literal Pattern>
    consteval srx::static_regex<Pattern, srx::mode::backtrack> operator ""_srxn() { return {}; }
}

using srx::detail::no_tag;

/* basic tests */
static_assert(match(""_srxn, ""));
static_assert(not match(""_srxn, "a"));
static_assert(not match("a"_srxn, ""));
static_assert(match("a"_srxn, "a"));
static_assert(not match("a"_srxn, "b"));

/* grouping tests */
static_assert(not match("a|b"_srxn, ""));
static_assert(match("a|b"_srxn, "a"));
static_assert(match("a|b"_srxn, "b"));
static_assert(not match("a|b"_srxn, "c"));
static_assert(match("a|b|c"_srxn, "c"));
static_assert(match("a|b|c"_srxn, "b"));
static_assert(match("a|b|c"_srxn, "a"));
static_assert(not match("aa"_srxn, ""));
static_assert(not match("aa"_srxn, "a"));
static_assert(match("aa"_srxn, "aa"));
static_assert(not match("aa"_srxn, "aaa"));
static_assert(not match("ab"_srxn, ""));
static_assert(not match("ab"_srxn, "a"));
static_assert(not match("ab"_srxn, "b"));
static_assert(match("ab"_srxn, "ab"));
static_assert(not match("ab"_srxn, "ba"));
static_assert(not match("ab"_srxn, "abb"));
static_assert(not match("ab"_srxn, "aab"));
static_assert(not match("ab"_srxn, "bb"));

/* character class tests */
static_assert(match("[a-c]"_srxn, "a"));
static_assert(match("[a-c]"_srxn, "b"));
static_assert(match("[a-c]"_srxn, "c"));
static_assert(not match("[a-c]"_srxn, "d"));

/* repeater tests */
static_assert(match("a*"_srxn, ""));
static_assert(match("a*"_srxn, "a"));
static_assert(match("a*"_srxn, "aa"));
static_assert(match("a*"_srxn, "aaaaa"));
static_assert(not match("a+"_srxn, ""));
static_assert(match("a+"_srxn, "a"));
static_assert(match("a+"_srxn, "aa"));
static_assert(match("a+"_srxn, "aaaaa"));
static_assert(match("a?"_srxn, ""));
static_assert(match("a?"_srxn, "a"));
static_assert(not match("a?"_srxn, "aa"));
static_assert(not match("a?"_srxn, "aaaaa"));
static_assert(not match("a{1}"_srxn, ""));
static_assert(match("a{1}"_srxn, "a"));
static_assert(not match("a{1}"_srxn, "aa"));
static_assert(not match("a{2,5}"_srxn, "a"));
static_assert(match("a{2,5}"_srxn, "aa"));
static_assert(match("a{2,5}"_srxn, "aaa"));
static_assert(match("a{2,5}"_srxn, "aaaa"));
static_assert(match("a{2,5}"_srxn, "aaaaa"));
static_assert(not match("a{2,5}"_srxn, "aaaaaaa"));
static_assert(not match("a{2,}"_srxn, "a"));
static_assert(match("a{2,}"_srxn, "aa"));
static_assert(match("a{2,}"_srxn, "aaa"));
static_assert(match("a{2,}"_srxn, "aaaa"));
static_assert(match("a{2,}"_srxn, "aaaaa"));
static_assert(not match("a{3},"_srxn, "aa"));
static_assert(match("a{3}"_srxn, "aaa"));
static_assert(not match("a{3}"_srxn, "aaaa"));

/* laziness tests */
static_assert(match("(a)?a*"_srxn, "", { no_tag, no_tag }));
static_assert(match("(a)?a*"_srxn, "a", { 0, 1 }));
static_assert(match("(a)?a*"_srxn, "aa", { 0, 1 }));
static_assert(match("(a)??a*"_srxn, "aa", { no_tag, no_tag }));
static_assert(match("(a)+?a?"_srxn, "aa", { 0, 1 }));
static_assert(match("(a)+a?"_srxn, "aa", { 1, 2 }));
static_assert(match("(a){2,3}?a?"_srxn, "aaa", { 1, 2 }));
static_assert(match("(a){2,3}a?"_srxn, "aaa", { 2, 3 }));
static_assert(match("(ab+c)+?(ab+c|.*d)"_srxn, "abcabbcacd", { 0, 3, 3, 10 }));
static_assert(match("(ab+c)+(ab+c|.*d)"_srxn, "abcabbcacd", { 3, 7, 7, 10 }));

/* submatch disambiguation tests */
static_assert(match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_srxn, "abcdefg", { 6, 7 }));     /* [perl]: a bcdef g */
static_assert(not match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_srxn, "abcdefg", { 4, 7 })); /* [posix]: ab c d efg */
static_assert(not match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_srxn, "abcdefg", { 5, 7 })); /* [incorrect]: ab c d e fg */

/* capture location tests */
static_assert(match("(a)"_srxn, "a", { 0, 1 }));
static_assert(match("a(a)"_srxn, "aa", { 1, 2 }));
static_assert(match("a(a)a"_srxn, "aaa", { 1, 2 }));
static_assert(match("(a)*"_srxn, "a", { 0, 1 }));
static_assert(match("(a)*"_srxn, "aa", { 1, 2 }));
static_assert(match("(?:(a)|(d))c"_srxn, "ac", { 0, 1, no_tag, no_tag }));
static_assert(match("(?:(a)|(d))c"_srxn, "dc", { no_tag, no_tag, 0, 1 }));
static_assert(match("(a)+(b)*"_srxn, "ab", { 0, 1, 1, 2 }));
static_assert(match("(a)+(b)*"_srxn, "aab", { 1, 2, 2, 3 }));
static_assert(match("(aa)+a*"_srxn, "aaaaa", { 2, 4 }));
static_assert(match("(aa)+?a*"_srxn, "aaaaa", { 0, 2 }));
static_assert(match("(a((a)))(a)c"_srxn, "aaac", { 0, 2, 1, 2, 1, 2, 2, 3 }));
static_assert(match("a(aa)b|(aa)ac"_srxn, "aaab", { 1, 3, no_tag, no_tag }));
static_assert(match("a(aa)b|(aa)ac"_srxn, "aaac", { no_tag, no_tag, 0, 2 }));

/* wildcard tests */
static_assert(not match("."_srxn, ""));
static_assert(match("."_srxn, "0"));
static_assert(match("."_srxn, "@"));
static_assert(match("."_srxn, "$"));
static_assert(match("."_srxn, "z"));
static_assert(match("."_srxn, "A"));
static_assert(match("."_srxn, "."));
static_assert(not match("."_srxn, "\n"));
static_assert(match("(?s)."_srxn, "\n"));
// TODO: add test cases for non ascii chars

/* backtracking tests */
static_assert(match("a(b|bb)b"_srxn, "abb"));
static_assert(match("a(b|bb)b"_srxn, "abbb"));
static_assert(match("a(bb|b)b"_srxn, "abb"));
static_assert(match("a(bb|b)b"_srxn, "abbb"));
static_assert(match("(a*b)+"_srxn, "aaababaaaab"));
static_assert(match("(a+){4}"_srxn, "aaaaaaaaaaaa"));
static_assert(match("(aa|aaa)*a"_srxn, "aaaaaa"));

/* atomic matching tests */
static_assert(match("(a)++[ab]"_srxn, "aaab", { 2, 3 }));
static_assert(not match("(a)++[ab]"_srxn, "aaaa"));
static_assert(match("(?:(a)++[ab])|(a*)"_srxn, "aaab", { 2, 3, no_tag, no_tag }));
static_assert(match("(?:(a)++[ab])|(a*)"_srxn, "aaaa", { no_tag, no_tag, 0, 4 }));

/* backreference tests */
static_assert(match("(abc)\\1"_srxn, "abcabc"));
static_assert(match("(a|b|c)\\g1"_srxn, "bb"));
static_assert(match("(a|\1){2}"_srxn, "aa"));
static_assert(match("(a)(b)\\g2\\g1"_srxn, "abba"));
static_assert(match("(ab(cd))\\1\\2"_srxn, "abcdabcdcd"));
static_assert(not match("(ab(cd))\\1\\2"_srxn, "abcdcdabcd"));
static_assert(match("((ab|e\\3)(c*)d){2}"_srxn, "abccdeccd"));
static_assert(match("((a)(b)\\3\\2){2}"_srxn, "abbaabba"));
static_assert(not match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\10"_srxn, "abcdefghijkj"));
static_assert(not match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g10"_srxn, "abcdefghijkj"));
static_assert(match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g10"_srxn, "abcdefghijka0"));
static_assert(match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g{10}"_srxn, "abcdefghijkj"));
static_assert(match("(.*)\\1"_srxn, "abcdeabcde"));
static_assert(not match("\\1(abc)\\1"_srxn, "abc"));
static_assert(match("(abc|a\\1c)+"_srxn, "abc"));
static_assert(match("(abc|a\\1c)+"_srxn, "abcaabcc"));
static_assert(match("(abc|a\\1c)+"_srxn, "abcaabccaaabccc"));
static_assert(match(".?|(..+?)\\1+"_srxn, "a"));
static_assert(not match(".?|(..+?)\\1+"_srxn, "aa"));
static_assert(not match(".?|(..+?)\\1+"_srxn, "aaa"));
static_assert(match(".?|(..+?)\\1+"_srxn, "aaaa"));
static_assert(not match(".?|(..+?)\\1+"_srxn, "aaaaa"));
static_assert(match(".?|(..+?)\\1+"_srxn, "aaaaaaaaaa"));
static_assert(not match(".?|(..+?)\\1+"_srxn, "aaaaaaaaaaa"));

/* capture features */
static_assert(match("(a)"_srxn, "a"));
static_assert(match("(a)(b)"_srxn, "ab"));
static_assert(match("(a(b))"_srxn, "ab"));
static_assert(match("(?:a)"_srxn, "a"));
static_assert(match("(?#comment)"_srxn, ""));
static_assert(match("(?:c)(a)(b)\\2"_srxn, "cabb"));
static_assert(not match("(?:c)(a)(b)\\2"_srxn, "caba"));
static_assert(match("(?|(a)|(b))\\1"_srxn, "aa"));
static_assert(match("(?|(a)|(b))\\1"_srxn, "bb"));
static_assert(match("(?|(a)|(b))(c)\\1\\2"_srxn, "acac"));
static_assert(match("(?|(a)|(b))(c)\\1\\2"_srxn, "bcbc"));
static_assert(match("(?|(a)|(b)(c))(d)\\3"_srxn, "add"));
static_assert(match("(?|(a)|(b)(c))(d)\\1\\2"_srxn, "bcdbc"));
static_assert(match("(?|(a)|(b)(c)|(d)(e))(f)\\1\\2"_srxn, "bcfbc"));
static_assert(match("(?|(a)|(b)(c)|(d)(e))(f)\\1\\2"_srxn, "defde"));

/* caseless flag */
static_assert(match("(?i)abc"_srxn, "ABC"));
static_assert(match("(?i)abc"_srxn, "Abc"));
static_assert(match("(?i)abc"_srxn, "aBc"));
static_assert(match("(?i)a[be]"_srxn, "aB"));
static_assert(match("(?i)a[Z-a]"_srxn, "az"));
static_assert(match("a(?i:b)c"_srxn, "abc"));
static_assert(match("a(?i:b)c"_srxn, "aBc"));
static_assert(not match("a(?i:b)c"_srxn, "abC"));

/* dotall flag */
static_assert(match("(?s)."_srxn, "\n"));
static_assert(match("a.b"_srxn, "a-b"));
static_assert(not match("a.b"_srxn, "a\nb"));
static_assert(match("(?s)a.b"_srxn, "a\nb"));
static_assert(not match("((?s))a.b"_srxn, "a\nb"));
static_assert(not match("(?:(?s))a.b"_srxn, "a\nb"));

/* ungreedy flag */
static_assert(match("(?U)(a)?a*"_srxn, "aa", { no_tag, no_tag }));
static_assert(match("(?U)(a)??a*"_srxn, "aa", { 0, 1 }));
static_assert(match("(?U:)(a)?a*"_srxn, "aa", { 0, 1 }));
static_assert(match("(?U:)(a)??a*"_srxn, "aa", { no_tag, no_tag }));
static_assert(match("(?U)(aa)+a*"_srxn, "aaaaa", { 0, 2 }));
static_assert(match("(?U)(aa)+?a*"_srxn, "aaaaa", { 2, 4 }));
static_assert(match("(?U-U)(a)?a*"_srxn, "aa", { 0, 1 }));
static_assert(match("(?U-U)(a)??a*"_srxn, "aa", { no_tag, no_tag }));
static_assert(match("(?U)(a)?a*(?-U)(b)?b*"_srxn, "aabb", { no_tag, no_tag, 2, 3 }));
static_assert(match("(?U)(a)??a*(?-U)(b)??b*"_srxn, "aabb", { 0, 1, no_tag, no_tag }));

/* additional parser tests */
static_assert(match("\\Q...\\E"_srxn, "..."));
static_assert(not match("\\Q...\\E"_srxn, "aaa"));

/* fallback tests */
static_assert(prefix_match("abc"_srxn, "abcdef", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_srxn, "abc", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_srxn, "abcdef", { 0, 6 }));
static_assert(prefix_match("abc|abcdef"_srxn, "abcdef", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_srxn, "abcde", { 0, 3 }));

/* lazy prefix matching tests */
static_assert(prefix_match("(abc)+"_srxn, "abcabc", { 0, 6, 3, 6 }));
static_assert(prefix_match("(abc)+?"_srxn, "abcabc", { 0, 3, 0, 3 }));
static_assert(prefix_match("(abc)+?a"_srxn, "abcabc", { 0, 4, 0, 3 }));
static_assert(prefix_match("(abc)+?a"_srxn, "abcabca", { 0, 4, 0, 3 }));

/* additional capture location tests */
static_assert(match("(ab+c)+?(ab+c|.*d)"_srxn, "abcabbcacd", { 0, 3, 3, 10 }));
static_assert(prefix_match("(ab+c)+?(ab+c|.*d)"_srxn, "abcabbcacd", { 0, 7, 0, 3, 3, 7 }));
static_assert(prefix_match("(ab+c)+(ab+c|.*d)"_srxn, "abcabbcacd", { 0, 10, 3, 7, 7, 10 }));
static_assert(search("([ad]b+c)+?([ad])"_srxn, "aaabacabcdbbcacd", { 6, 10, 6, 9, 9, 10 }));
static_assert(search("([ad]b+c)+([ad])"_srxn, "aaabacabcdbbcacd", { 6, 14, 9, 13, 13, 14 }));

/* search tests */
static_assert(search("a"_srxn, "abcd", { 0, 1 }));
static_assert(search("b"_srxn, "abcd", { 1, 2 }));
static_assert(search("d"_srxn, "abcd", { 3, 4 }));
static_assert(not search("e"_srxn, "abcd"));
static_assert(search("aa"_srxn, "abaab", { 2, 4 }));
static_assert(search("ab"_srxn, "abaab", { 0, 2 }));
static_assert(search("bc"_srxn, "abcd", { 1, 3 }));
static_assert(search_all("a"_srxn, "a", { { 0, 1 } }));
static_assert(search_all("a"_srxn, "aaa", { { 0, 1 }, { 1, 2 }, { 2, 3 } }));
static_assert(search_all("ab"_srxn, "abab", { { 0, 2 }, { 2, 4 } }));
static_assert(search_all("ab"_srxn, "abaab", { { 0, 2 }, { 3, 5 } }));
static_assert(search_all("(?:)|abc"_srxn, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
static_assert(search_all("(?:)|abc"_srxn, "abcabc", { { 0, 0 }, { 0, 3 }, { 3, 3 }, { 3, 6 }, { 6, 6 } }));

/* sof+eof anchor tests */
static_assert(match("a$"_srxn, "a"));
static_assert(match("^a"_srxn, "a"));
static_assert(match("^a$"_srxn, "a"));
static_assert(match("^$"_srxn, ""));
static_assert(prefix_match("a$"_srxn, "a"));
static_assert(prefix_match("^a"_srxn, "a"));
static_assert(prefix_match("^a$"_srxn, "a"));
static_assert(prefix_match("^$"_srxn, ""));
static_assert(not search("^$"_srxn, "a"));
static_assert(search("^$"_srxn, "", { 0, 0 }));
static_assert(search("$^"_srxn, "", { 0, 0 }));
static_assert(search("a$"_srxn, "a", { 0, 1 }));
static_assert(search("^a"_srxn, "a", { 0, 1 }));
static_assert(search("^a$"_srxn, "a", { 0, 1 }));
static_assert(prefix_match("(a)+?$$"_srxn, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?$$"_srxn, "aa", { 0, 2, 1, 2 }));
static_assert(prefix_match("(a)+?$"_srxn, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?$"_srxn, "aa", { 0, 2, 1, 2 }));
static_assert(prefix_match("(a)+?"_srxn, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?"_srxn, "aa", { 0, 1, 0, 1 }));
static_assert(not search("^^ab"_srxn, "bab"));
static_assert(search("^^ab"_srxn, "aba", { 0, 2 }));
static_assert(not search("^ab"_srxn, "bab"));
static_assert(search("^ab"_srxn, "aba", { 0, 2 }));
static_assert(search("ab"_srxn, "bab", { 1, 3 }));
static_assert(search("ab"_srxn, "aba", { 0, 2 }));
static_assert(search("(abc)|(^abc)"_srxn, "abc", { 0, 3, 0, 3, no_tag, no_tag }));

/* sol+eol anchor tests */
static_assert(prefix_match("(?m:a$)"_srxn, "a", { 0, 1 }));
static_assert(prefix_match("(?m:a$)"_srxn, "a\na", { 0, 1 }));
static_assert(prefix_match("(?m:a$\na)"_srxn, "a\na", { 0, 3 }));
static_assert(prefix_match("(?m:a$\na)"_srxn, "a\na", { 0, 3 }));
static_assert(prefix_match("(?m)($\na)+"_srxn, "\na", { 0, 2, 0, 2 }));
static_assert(prefix_match("(?m)($\na)+"_srxn, "\na\na", { 0, 4, 2, 4 }));
static_assert(prefix_match("(?m)(\n$)+"_srxn, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(prefix_match("(?m)(a$)"_srxn, "a\na", { 0, 1, 0, 1 }));
static_assert(search("(?m:^a)"_srxn, "a", { 0, 1 }));
static_assert(search("(?m:^a)"_srxn, "\na", { 1, 2 }));
static_assert(search("(?ms)(^a.?)*"_srxn, "a\na", { 0, 3, 2, 3 }));
static_assert(search("(?m:^\n*$)"_srxn, "\n\n\n\n", { 0, 4 }));
static_assert(search("(?m:^\n*?$)"_srxn, "\n\n\n\n", { 0, 0 }));
static_assert(prefix_match("(?m)(\n$)+"_srxn, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)(^\n$)+"_srxn, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)($\n^)+"_srxn, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)(^a\n)+"_srxn, "a\na\na", { 0, 4, 2, 4 }));
static_assert(search_all("(?m)^a"_srxn, "a\na\na", { { 0, 1 }, { 2, 3 }, { 4, 5 } }));

/* word boundary tests */
static_assert(not search(R"(\b)"_srxn, ""));
static_assert(search(R"(\B)"_srxn, "", { 0, 0 }));
static_assert(search_all(R"(\b)"_srxn, "ab+-cd", { { 0, 0 }, { 2, 2 }, { 4, 4 }, { 6, 6 } }));
static_assert(search_all(R"(\B)"_srxn, "ab<>cd", { { 1, 1 }, { 3, 3 }, { 5, 5 } }));
static_assert(search(R"(\B)"_srxn, "a_", { 1, 1 }));
static_assert(search(R"(a\b)"_srxn, "a+", { 0, 1 }));
static_assert(search(R"(a\b)"_srxn, "a.", { 0, 1 }));
static_assert(not search(R"(a\b)"_srxn, "a_"));
static_assert(search_all(R"(\b|abc)"_srxn, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
static_assert(search_all(R"(\b|^abc)"_srxn, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
