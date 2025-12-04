#include "headers/tnfa_matcher.hpp"


namespace
{
    template<typename CharT>
    consteval bool match(const CharT* pattern, const CharT* str, const std::vector<std::size_t>& captures = {})
    {
        using namespace rx::detail;
        expr_tree ast{ pattern };

        const rx::testing::tnfa_matcher nfa{ ast, default_fsm_flags::full_match };
        const auto match_result{ nfa.match(str) };

        if (captures.empty())
            return match_result.has_value();
        else if (not match_result.has_value())
            return false;
        else
            return std::ranges::equal(match_result.value() | std::views::drop(2), captures);
    }

    template<typename CharT>
    consteval bool ends_with(const CharT* pattern, const CharT* str, std::size_t start_pos)
    {
        using namespace rx::detail;
        expr_tree ast{ pattern };
        ast.insert_search_prefix();

        const rx::testing::tnfa_matcher nfa{ ast, default_fsm_flags::full_match };
        const auto match_result{ nfa.match(str) };

        if (not match_result.has_value())
            return false;
        else
            return match_result.value().at(0) == start_pos;
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

/* search prefix tests */
static_assert(ends_with("ab", "ab", 0));
static_assert(ends_with("aa", "aaa", 1));
static_assert(ends_with("cde", "abcde", 2));
static_assert(ends_with("cde", "cdecde", 3));
static_assert(ends_with("bcd", "abcdbcd", 4));