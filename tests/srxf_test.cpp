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
    consteval bool search(rx::static_regex<S, rx::mode::fast> pattern, std::string_view str)
    {
       return pattern.contains_match(str);
    }

    template<rx::string_literal S>
    consteval bool search(rx::static_regex<S, rx::mode::fast> pattern, std::string_view str, const std::vector<std::size_t>& captures)
    {
        return submatch_check(pattern.search(str), captures, str.begin());
    }

    template<rx::string_literal S>
    consteval bool search_all(rx::static_regex<S, rx::mode::fast> pattern, std::string_view str, const std::vector<std::vector<std::size_t>>& captures)
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

/* these tests have been copied from srx_1 */

/* additional capture location tests */
static_assert(search("([ad]b+c)+?([ad])"_rxf, "aaabacabcdbbcacd", { 6, 10, 6, 9, 9, 10 }));
static_assert(search("([ad]b+c)+([ad])"_rxf, "aaabacabcdbbcacd", { 6, 14, 9, 13, 13, 14 }));

/* search tests */
static_assert(search("a"_rxf, "abcd", { 0, 1 }));
static_assert(search("b"_rxf, "abcd", { 1, 2 }));
static_assert(search("d"_rxf, "abcd", { 3, 4 }));
static_assert(not search("e"_rxf, "abcd"));
static_assert(search("aa"_rxf, "abaab", { 2, 4 }));
static_assert(search("ab"_rxf, "abaab", { 0, 2 }));
static_assert(search("bc"_rxf, "abcd", { 1, 3 }));
static_assert(search_all("a"_rxf, "a", { { 0, 1 } }));
static_assert(search_all("a"_rxf, "aaa", { { 0, 1 }, { 1, 2 }, { 2, 3 } }));
static_assert(search_all("ab"_rxf, "abab", { { 0, 2 }, { 2, 4 } }));
static_assert(search_all("ab"_rxf, "abaab", { { 0, 2 }, { 3, 5 } }));
static_assert(search_all("(?:)|abc"_rxf, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
static_assert(search_all("(?:)|abc"_rxf, "abcabc", { { 0, 0 }, { 0, 3 }, { 3, 3 }, { 3, 6 }, { 6, 6 } }));

/* sof+eof anchor tests */
static_assert(not search("^$"_rxf, "a"));
static_assert(search("^$"_rxf, "", { 0, 0 }));
static_assert(search("$^"_rxf, "", { 0, 0 }));
static_assert(search("a$"_rxf, "a", { 0, 1 }));
static_assert(search("^a"_rxf, "a", { 0, 1 }));
static_assert(search("^a$"_rxf, "a", { 0, 1 }));
static_assert(not search("^^ab"_rxf, "bab"));
static_assert(search("^^ab"_rxf, "aba", { 0, 2 }));
static_assert(not search("^ab"_rxf, "bab"));
static_assert(search("^ab"_rxf, "aba", { 0, 2 }));
static_assert(search("ab"_rxf, "bab", { 1, 3 }));
static_assert(search("ab"_rxf, "aba", { 0, 2 }));
static_assert(search("(abc)|(^abc)"_rxf, "abc", { 0, 3, 0, 3, no_tag, no_tag }));

/* sol+eol anchor tests */
static_assert(search("(?m:^a)"_rxf, "a", { 0, 1 }));
static_assert(search("(?m:^a)"_rxf, "\na", { 1, 2 }));
static_assert(search("(?ms)(^a.?)*"_rxf, "a\na", { 0, 3, 2, 3 }));
static_assert(search("(?m:^\n*$)"_rxf, "\n\n\n\n", { 0, 4 }));
static_assert(search("(?m:^\n*?$)"_rxf, "\n\n\n\n", { 0, 0 }));
static_assert(search("(?m)(^\n$)+"_rxf, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)($\n^)+"_rxf, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)(^a\n)+"_rxf, "a\na\na", { 0, 4, 2, 4 }));

/* word boundary tests */
static_assert(not search(R"(\b)"_rxf, ""));
static_assert(search(R"(\B)"_rxf, "", { 0, 0 }));
static_assert(search_all(R"(\b)"_rxf, "ab+-cd", { { 0, 0 }, { 2, 2 }, { 4, 4 }, { 6, 6 } }));
static_assert(search_all(R"(\B)"_rxf, "ab<>cd", { { 1, 1 }, { 3, 3 }, { 5, 5 } }));
static_assert(search(R"(\B)"_rxf, "a_", { 1, 1 }));
static_assert(search(R"(a\b)"_rxf, "a+", { 0, 1 }));
static_assert(search(R"(a\b)"_rxf, "a.", { 0, 1 }));
static_assert(not search(R"(a\b)"_rxf, "a_"));
static_assert(search_all(R"(\b|abc)"_rxf, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
static_assert(search_all(R"(\b|^abc)"_rxf, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
