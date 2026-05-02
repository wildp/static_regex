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
    consteval bool search(srx::static_regex<S, srx::mode::linear> pattern, std::string_view str)
    {
       return pattern.contains_match(str);
    }

    template<srx::string_literal S>
    consteval bool search(srx::static_regex<S, srx::mode::linear> pattern, std::string_view str, const std::vector<std::size_t>& captures)
    {
        return submatch_check(pattern.search(str), captures, str.begin());
    }

    template<srx::string_literal S>
    consteval bool search_all(srx::static_regex<S, srx::mode::linear> pattern, std::string_view str, const std::vector<std::vector<std::size_t>>& captures)
    {
        for (const auto& [match, caps] : std::views::zip(pattern.range(str), captures))
        {
            if (not submatch_check(match, caps, str.begin()))
                return false;
        }
        return true;
    }

    template<srx::string_literal Pattern>
    consteval srx::static_regex<Pattern, srx::mode::linear> operator ""_srxf() { return {}; }
}

using srx::detail::no_tag;

/* these tests have been copied from srx_1 */

/* additional capture location tests */
static_assert(search("([ad]b+c)+?([ad])"_srxf, "aaabacabcdbbcacd", { 6, 10, 6, 9, 9, 10 }));
static_assert(search("([ad]b+c)+([ad])"_srxf, "aaabacabcdbbcacd", { 6, 14, 9, 13, 13, 14 }));

/* search tests */
static_assert(search("a"_srxf, "abcd", { 0, 1 }));
static_assert(search("b"_srxf, "abcd", { 1, 2 }));
static_assert(search("d"_srxf, "abcd", { 3, 4 }));
static_assert(not search("e"_srxf, "abcd"));
static_assert(search("aa"_srxf, "abaab", { 2, 4 }));
static_assert(search("ab"_srxf, "abaab", { 0, 2 }));
static_assert(search("bc"_srxf, "abcd", { 1, 3 }));
static_assert(search_all("a"_srxf, "a", { { 0, 1 } }));
static_assert(search_all("a"_srxf, "aaa", { { 0, 1 }, { 1, 2 }, { 2, 3 } }));
static_assert(search_all("ab"_srxf, "abab", { { 0, 2 }, { 2, 4 } }));
static_assert(search_all("ab"_srxf, "abaab", { { 0, 2 }, { 3, 5 } }));
static_assert(search_all("(?:)|abc"_srxf, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
static_assert(search_all("(?:)|abc"_srxf, "abcabc", { { 0, 0 }, { 0, 3 }, { 3, 3 }, { 3, 6 }, { 6, 6 } }));

/* sof+eof anchor tests */
static_assert(not search("^$"_srxf, "a"));
static_assert(search("^$"_srxf, "", { 0, 0 }));
static_assert(search("$^"_srxf, "", { 0, 0 }));
static_assert(search("a$"_srxf, "a", { 0, 1 }));
static_assert(search("^a"_srxf, "a", { 0, 1 }));
static_assert(search("^a$"_srxf, "a", { 0, 1 }));
static_assert(not search("^^ab"_srxf, "bab"));
static_assert(search("^^ab"_srxf, "aba", { 0, 2 }));
static_assert(not search("^ab"_srxf, "bab"));
static_assert(search("^ab"_srxf, "aba", { 0, 2 }));
static_assert(search("ab"_srxf, "bab", { 1, 3 }));
static_assert(search("ab"_srxf, "aba", { 0, 2 }));
static_assert(search("(abc)|(^abc)"_srxf, "abc", { 0, 3, 0, 3, no_tag, no_tag }));

/* sol+eol anchor tests */
static_assert(search("(?m:^a)"_srxf, "a", { 0, 1 }));
static_assert(search("(?m:^a)"_srxf, "\na", { 1, 2 }));
static_assert(search("(?ms)(^a.?)*"_srxf, "a\na", { 0, 3, 2, 3 }));
static_assert(search("(?m:^\n*$)"_srxf, "\n\n\n\n", { 0, 4 }));
static_assert(search("(?m:^\n*?$)"_srxf, "\n\n\n\n", { 0, 0 }));
static_assert(search("(?m)(^\n$)+"_srxf, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)($\n^)+"_srxf, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)(^a\n)+"_srxf, "a\na\na", { 0, 4, 2, 4 }));
static_assert(search_all("(?m)^a"_srxf, "a\na\na", { { 0, 1 }, { 2, 3 }, { 4, 5 } }));

/* word boundary tests */
static_assert(not search(R"(\b)"_srxf, ""));
static_assert(search(R"(\B)"_srxf, "", { 0, 0 }));
static_assert(search_all(R"(\b)"_srxf, "ab+-cd", { { 0, 0 }, { 2, 2 }, { 4, 4 }, { 6, 6 } }));
static_assert(search_all(R"(\B)"_srxf, "ab<>cd", { { 1, 1 }, { 3, 3 }, { 5, 5 } }));
static_assert(search(R"(\B)"_srxf, "a_", { 1, 1 }));
static_assert(search(R"(a\b)"_srxf, "a+", { 0, 1 }));
static_assert(search(R"(a\b)"_srxf, "a.", { 0, 1 }));
static_assert(not search(R"(a\b)"_srxf, "a_"));
static_assert(search_all(R"(\b|abc)"_srxf, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
static_assert(search_all(R"(\b|^abc)"_srxf, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
