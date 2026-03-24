// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "headers/tdfa_matcher.hpp"


namespace
{
    template<typename CharT>
    consteval bool match(const CharT* pattern, const CharT* str, const std::vector<std::size_t>& captures = {})
    {
        using namespace rx::detail;
        const expr_tree ast{ pattern };
        tagged_nfa nfa{ ast, default_fsm_flags::full_match };
        nfa.rewrite_assertions();

        const rx::testing::tdfa_matcher dfa{ nfa };
        const auto match_result{ dfa.match(str) };

        if (captures.empty())
            return match_result.has_value();
        else if (not match_result.has_value())
            return false;
        else
            return std::ranges::equal(match_result.value() | std::views::drop(2), captures);
    }

    template<typename CharT>
    consteval bool prefix_match(const CharT* pattern, const CharT* str, const std::vector<std::size_t>& captures = {})
    {
        using namespace rx::detail;
        const expr_tree ast{ pattern };
        tagged_nfa nfa{ ast, default_fsm_flags::partial_match };
        nfa.rewrite_assertions();

        const rx::testing::tdfa_matcher dfa{ nfa };
        const auto match_result{ dfa.partial_match(str) };

        if (captures.empty())
            return match_result.has_value();
        else if (not match_result.has_value())
            return false;
        else
            return std::ranges::equal(match_result.value(), captures);
    }

    template<typename CharT>
    consteval bool search(const CharT* pattern, const CharT* str, const std::vector<std::size_t>& captures = {})
    {
        using namespace rx::detail;
        expr_tree ast{ pattern };
        ast.insert_search_prefix();
        tagged_nfa nfa{ ast, default_fsm_flags::search_single };
        nfa.rewrite_assertions();

        const rx::testing::tdfa_matcher dfa{ nfa };
        const auto match_result{ dfa.partial_match(str) };

        if (captures.empty())
            return match_result.has_value();
        else if (not match_result.has_value())
            return false;
        else
            return std::ranges::equal(match_result.value(), captures);
    }

    template<typename CharT>
    consteval bool search_all(const CharT* pattern, const CharT* str, const std::vector<std::vector<std::size_t>>& captures = {})
    {
        using namespace rx::detail;
        expr_tree ast{ pattern };
        ast.insert_search_prefix();
        tagged_nfa nfa{ ast, default_fsm_flags::search_all };
        nfa.rewrite_assertions();
        if (ast.empty_match_possible())
            nfa.add_non_empty_match_pathway();

        const rx::testing::tdfa_matcher dfa{ nfa };
        const auto match_result{ dfa.match_all(str) };

        if (captures.empty())
            return not match_result.empty();
        else if (match_result.empty())
            return false;
        else
            return std::ranges::equal(match_result, captures);
    }
}

using rx::detail::no_tag;

/* basic tests */
static_assert(match("", ""));
static_assert(not match("", "a"));
static_assert(not match("a", ""));
static_assert(match("a", "a"));
static_assert(not match("a", "b"));

/* grouping tests */
static_assert(not match("a|b", ""));
static_assert(match("a|b", "a"));
static_assert(match("a|b", "b"));
static_assert(not match("a|b", "c"));
static_assert(match("a|b|c", "c"));
static_assert(match("a|b|c", "b"));
static_assert(match("a|b|c", "a"));
static_assert(not match("aa", ""));
static_assert(not match("aa", "a"));
static_assert(match("aa", "aa"));
static_assert(not match("aa", "aaa"));
static_assert(not match("ab", ""));
static_assert(not match("ab", "a"));
static_assert(not match("ab", "b"));
static_assert(match("ab", "ab"));
static_assert(not match("ab", "ba"));
static_assert(not match("ab", "abb"));
static_assert(not match("ab", "aab"));
static_assert(not match("ab", "bb"));

/* character class tests */
static_assert(match("[a-c]", "a"));
static_assert(match("[a-c]", "b"));
static_assert(match("[a-c]", "c"));
static_assert(not match("[a-c]", "d"));

/* repeater tests */
static_assert(match("a*", ""));
static_assert(match("a*", "a"));
static_assert(match("a*", "aa"));
static_assert(match("a*", "aaaaa"));
static_assert(not match("a+", ""));
static_assert(match("a+", "a"));
static_assert(match("a+", "aa"));
static_assert(match("a+", "aaaaa"));
static_assert(match("a?", ""));
static_assert(match("a?", "a"));
static_assert(not match("a?", "aa"));
static_assert(not match("a?", "aaaaa"));
static_assert(not match("a{1}", ""));
static_assert(match("a{1}", "a"));
static_assert(not match("a{1}", "aa"));
static_assert(not match("a{2,5}", "a"));
static_assert(match("a{2,5}", "aa"));
static_assert(match("a{2,5}", "aaa"));
static_assert(match("a{2,5}", "aaaa"));
static_assert(match("a{2,5}", "aaaaa"));
static_assert(not match("a{2,5}", "aaaaaaa"));
static_assert(not match("a{2,}", "a"));
static_assert(match("a{2,}", "aa"));
static_assert(match("a{2,}", "aaa"));
static_assert(match("a{2,}", "aaaa"));
static_assert(match("a{2,}", "aaaaa"));
static_assert(not match("a{3},", "aa"));
static_assert(match("a{3}", "aaa"));
static_assert(not match("a{3}", "aaaa"));

/* laziness tests */
static_assert(match("(a)?a*", "", { no_tag, no_tag }));
static_assert(match("(a)?a*", "a", { 0, 1 }));
static_assert(match("(a)?a*", "aa", { 0, 1 }));
static_assert(match("(a)??a*", "aa", { no_tag, no_tag }));
static_assert(match("(a)+?a?", "aa", { 0, 1 }));
static_assert(match("(a)+a?", "aa", { 1, 2 }));
static_assert(match("(a){2,3}?a?", "aaa", { 1, 2 }));
static_assert(match("(a){2,3}a?", "aaa", { 2, 3 }));
static_assert(match("(ab+c)+?(ab+c|.*d)", "abcabbcacd", { 0, 3, 3, 10 }));
static_assert(match("(ab+c)+(ab+c|.*d)", "abcabbcacd", { 3, 7, 7, 10 }));

/* submatch disambiguation tests */
static_assert(match("(a|bcdef|g|ab|c|d|e|efg|fg)*", "abcdefg", { 6, 7 }));     /* [perl]: a bcdef g */
static_assert(not match("(a|bcdef|g|ab|c|d|e|efg|fg)*", "abcdefg", { 4, 7 })); /* [posix]: ab c d efg */
static_assert(not match("(a|bcdef|g|ab|c|d|e|efg|fg)*", "abcdefg", { 5, 7 })); /* [incorrect]: ab c d e fg */

/* capture location tests */
static_assert(match("(a)", "a", { 0, 1 }));
static_assert(match("a(a)", "aa", { 1, 2 }));
static_assert(match("a(a)a", "aaa", { 1, 2 }));
static_assert(match("(a)*", "a", { 0, 1 }));
static_assert(match("(a)*", "aa", { 1, 2 }));
static_assert(match("(?:(a)|(d))c", "ac", { 0, 1, no_tag, no_tag }));
static_assert(match("(?:(a)|(d))c", "dc", { no_tag, no_tag, 0, 1 }));
static_assert(match("(a)+(b)*", "ab", { 0, 1, 1, 2 }));
static_assert(match("(a)+(b)*", "aab", { 1, 2, 2, 3 }));
static_assert(match("(aa)+a*", "aaaaa", { 2, 4 }));
static_assert(match("(aa)+?a*", "aaaaa", { 0, 2 }));
static_assert(match("(a((a)))(a)c", "aaac", { 0, 2, 1, 2, 1, 2, 2, 3 }));
static_assert(match("a(aa)b|(aa)ac", "aaab", { 1, 3, no_tag, no_tag }));
static_assert(match("a(aa)b|(aa)ac", "aaac", { no_tag, no_tag, 0, 2 }));

/* wildcard tests */
static_assert(not match(".", ""));
static_assert(match(".", "0"));
static_assert(match(".", "@"));
static_assert(match(".", "$"));
static_assert(match(".", "z"));
static_assert(match(".", "A"));
static_assert(match(".", "."));
static_assert(not match(".", "\n"));
static_assert(match("(?s).", "\n"));
// TODO: add test cases for non ascii chars


/* fallback tests */
static_assert(prefix_match("abc", "abcdef", { 0, 3 }));
static_assert(prefix_match("abcdef|abc", "abc", { 0, 3 }));
static_assert(prefix_match("abcdef|abc", "abcdef", { 0, 6 }));
static_assert(prefix_match("abc|abcdef", "abcdef", { 0, 3 }));
static_assert(prefix_match("abcdef|abc", "abcde", { 0, 3 }));

/* lazy prefix matching tests */
static_assert(prefix_match("(abc)+", "abcabc", { 0, 6, 3, 6 }));
static_assert(prefix_match("(abc)+?", "abcabc", { 0, 3, 0, 3 }));
static_assert(prefix_match("(abc)+?a", "abcabc", { 0, 4, 0, 3 }));
static_assert(prefix_match("(abc)+?a", "abcabca", { 0, 4, 0, 3 }));

/* additional capture location tests */
static_assert(match("(ab+c)+?(ab+c|.*d)", "abcabbcacd", { 0, 3, 3, 10 }));
static_assert(prefix_match("(ab+c)+?(ab+c|.*d)", "abcabbcacd", { 0, 7, 0, 3, 3, 7 }));
static_assert(prefix_match("(ab+c)+(ab+c|.*d)", "abcabbcacd", { 0, 10, 3, 7, 7, 10 }));
static_assert(search("([ad]b+c)+?([ad])", "aaabacabcdbbcacd", { 6, 10, 6, 9, 9, 10 }));
static_assert(search("([ad]b+c)+([ad])", "aaabacabcdbbcacd", { 6, 14, 9, 13, 13, 14 }));

/* search tests */
static_assert(search("a", "abcd", { 0, 1 }));
static_assert(search("b", "abcd", { 1, 2 }));
static_assert(search("d", "abcd", { 3, 4 }));
static_assert(not search("e", "abcd"));
static_assert(search("aa", "abaab", { 2, 4 }));
static_assert(search("ab", "abaab", { 0, 2 }));
static_assert(search("bc", "abcd", { 1, 3 }));
static_assert(search_all("a", "a", { { 0, 1 } }));
static_assert(search_all("a", "aaa", { { 0, 1 }, { 1, 2 }, { 2, 3 } }));
static_assert(search_all("ab", "abab", { { 0, 2 }, { 2, 4 } }));
static_assert(search_all("ab", "abaab", { { 0, 2 }, { 3, 5 } }));
static_assert(search_all("(?:)|abc", "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
static_assert(search_all("(?:)|abc", "abcabc", { { 0, 0 }, { 0, 3 }, { 3, 3 }, { 3, 6 }, { 6, 6 } }));
// TODO: add more tests

/* sof+eof anchor tests */
static_assert(match("a$", "a"));
static_assert(match("^a", "a"));
static_assert(match("^a$", "a"));
static_assert(match("^$", ""));
static_assert(prefix_match("a$", "a"));
static_assert(prefix_match("^a", "a"));
static_assert(prefix_match("^a$", "a"));
static_assert(prefix_match("^$", ""));
static_assert(not search("^$", "a"));
static_assert(search("^$", "", { 0, 0 }));
static_assert(search("$^", "", { 0, 0 }));
static_assert(search("a$", "a", { 0, 1 }));
static_assert(search("^a", "a", { 0, 1 }));
static_assert(search("^a$", "a", { 0, 1 }));
static_assert(prefix_match("(a)+?$$", "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?$$", "aa", { 0, 2, 1, 2 }));
static_assert(prefix_match("(a)+?$", "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?$", "aa", { 0, 2, 1, 2 }));
static_assert(prefix_match("(a)+?", "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?", "aa", { 0, 1, 0, 1 }));
static_assert(not search("^^ab", "bab"));
static_assert(search("^^ab", "aba", { 0, 2 }));
static_assert(not search("^ab", "bab"));
static_assert(search("^ab", "aba", { 0, 2 }));
static_assert(search("ab", "bab", { 1, 3 }));
static_assert(search("ab", "aba", { 0, 2 }));
static_assert(search("(abc)|(^abc)", "abc", { 0, 3, 0, 3, no_tag, no_tag }));

/* sol+eol anchor tests */
static_assert(prefix_match("(?m:a$)", "a", { 0, 1 }));
static_assert(prefix_match("(?m:a$)", "a\na", { 0, 1 }));
static_assert(prefix_match("(?m:a$\na)", "a\na", { 0, 3 }));
static_assert(prefix_match("(?m:a$\na)", "a\na", { 0, 3 }));
static_assert(prefix_match("(?m)($\na)+", "\na", { 0, 2, 0, 2 }));
static_assert(prefix_match("(?m)($\na)+", "\na\na", { 0, 4, 2, 4 }));
static_assert(prefix_match("(?m)(\n$)+", "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(prefix_match("(?m)(a$)", "a\na", { 0, 1, 0, 1 }));
static_assert(search("(?m:^a)", "a", { 0, 1 }));
static_assert(search("(?m:^a)", "\na", { 1, 2 }));
static_assert(search("(?ms)(^a.?)*", "a\na", { 0, 3, 2, 3 }));
static_assert(search("(?m:^\n*$)", "\n\n\n\n", { 0, 4 }));
static_assert(search("(?m:^\n*?$)", "\n\n\n\n", { 0, 0 }));
static_assert(prefix_match("(?m)(\n$)+", "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)(^\n$)+", "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)($\n^)+", "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)(^a\n)+", "a\na\na", { 0, 4, 2, 4 }));

/* word boundary tests */
static_assert(not search(R"(\b)", ""));
static_assert(search(R"(\B)", "", { 0, 0 }));
static_assert(search_all(R"(\b)", "ab+-cd", { { 0, 0 }, { 2, 2 }, { 4, 4 }, { 6, 6 } }));
static_assert(search_all(R"(\B)", "ab<>cd", { { 1, 1 }, { 3, 3 }, { 5, 5 } }));
static_assert(search(R"(\B)", "a_", { 1, 1 }));
static_assert(search(R"(a\b)", "a+", { 0, 1 }));
static_assert(search(R"(a\b)", "a.", { 0, 1 }));
static_assert(not search(R"(a\b)", "a_"));
static_assert(search_all(R"(\b|abc)", "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
static_assert(search_all(R"(\b|^abc)", "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));


/* other tests */
// "R(est)|(Res)T" -> hopcroft
