#include "headers/tdfa_matcher.hpp"


namespace
{
    template<typename CharT>
    consteval bool tdfa_match(const CharT* pattern, const CharT* str)
    {
        rx::detail::expr_tree<CharT> ast{ pattern };
        rx::detail::tagged_nfa<CharT> tnfa{ ast };
        rx::testing::tdfa_matcher<CharT> tdfa{ tnfa };

        return tdfa.match(std::string_view{ str });
    }

    template<typename CharT>
    consteval bool tdfa_capture(const CharT* pattern, const CharT* str, const std::vector<std::size_t>& captures)
    {
        rx::detail::expr_tree<CharT> ast{ pattern };
        rx::detail::tagged_nfa<CharT> tnfa{ ast };
        rx::testing::tdfa_matcher<CharT> tdfa{ tnfa };

        auto match_result{ tdfa.submatches(std::string_view{ str }) };

        if (not match_result.has_value())
            return false;

        return std::ranges::equal(match_result.value(), captures);
    }
}

using rx::detail::no_tag;

/* basic tests */
static_assert(tdfa_match("", ""));
static_assert(not tdfa_match("", "a"));
static_assert(not tdfa_match("a", ""));
static_assert(tdfa_match("a", "a"));
static_assert(not tdfa_match("a", "b"));

/* grouping tests */
static_assert(not tdfa_match("a|b", ""));
static_assert(tdfa_match("a|b", "a"));
static_assert(tdfa_match("a|b", "b"));
static_assert(not tdfa_match("a|b", "c"));
static_assert(tdfa_match("a|b|c", "c"));
static_assert(tdfa_match("a|b|c", "b"));
static_assert(tdfa_match("a|b|c", "a"));
static_assert(not tdfa_match("aa", ""));
static_assert(not tdfa_match("aa", "a"));
static_assert(tdfa_match("aa", "aa"));
static_assert(not tdfa_match("aa", "aaa"));
static_assert(not tdfa_match("ab", ""));
static_assert(not tdfa_match("ab", "a"));
static_assert(not tdfa_match("ab", "b"));
static_assert(tdfa_match("ab", "ab"));
static_assert(not tdfa_match("ab", "ba"));
static_assert(not tdfa_match("ab", "abb"));
static_assert(not tdfa_match("ab", "aab"));
static_assert(not tdfa_match("ab", "bb"));

/* character class tests */
static_assert(tdfa_match("[a-c]", "a"));
static_assert(tdfa_match("[a-c]", "b"));
static_assert(tdfa_match("[a-c]", "c"));
static_assert(not tdfa_match("[a-c]", "d"));

/* repeater tests */
static_assert(tdfa_match("a*", ""));
static_assert(tdfa_match("a*", "a"));
static_assert(tdfa_match("a*", "aa"));
static_assert(tdfa_match("a*", "aaaaa"));
static_assert(not tdfa_match("a+", ""));
static_assert(tdfa_match("a+", "a"));
static_assert(tdfa_match("a+", "aa"));
static_assert(tdfa_match("a+", "aaaaa"));
static_assert(tdfa_match("a?", ""));
static_assert(tdfa_match("a?", "a"));
static_assert(not tdfa_match("a?", "aa"));
static_assert(not tdfa_match("a?", "aaaaa"));
static_assert(not tdfa_match("a{1}", ""));
static_assert(tdfa_match("a{1}", "a"));
static_assert(not tdfa_match("a{1}", "aa"));
static_assert(not tdfa_match("a{2,5}", "a"));
static_assert(tdfa_match("a{2,5}", "aa"));
static_assert(tdfa_match("a{2,5}", "aaa"));
static_assert(tdfa_match("a{2,5}", "aaaa"));
static_assert(tdfa_match("a{2,5}", "aaaaa"));
static_assert(not tdfa_match("a{2,5}", "aaaaaaa"));
static_assert(not tdfa_match("a{2,}", "a"));
static_assert(tdfa_match("a{2,}", "aa"));
static_assert(tdfa_match("a{2,}", "aaa"));
static_assert(tdfa_match("a{2,}", "aaaa"));
static_assert(tdfa_match("a{2,}", "aaaaa"));
static_assert(not tdfa_match("a{3},", "aa"));
static_assert(tdfa_match("a{3}", "aaa"));
static_assert(not tdfa_match("a{3}", "aaaa"));

/* laziness tests */
// static_assert(tdfa_capture("(a)?a*", "", { 0, no_tag, no_tag }));
// static_assert(tdfa_capture("(a)?a*", "a", { 0, 0, 1 }));
// static_assert(tdfa_capture("(a)?a*", "aa", { 0, 0, 1 }));
// static_assert(tdfa_capture("(a)??a*", "aa", { 0, no_tag, no_tag }));

/* capture location tests */
// static_assert(tdfa_capture("(a)", "a", { 0, 0, 1 }));
// static_assert(tdfa_capture("a(a)", "aa", { 0, 1, 2 }));
// static_assert(tdfa_capture("a(a)a", "aaa", { 0, 1, 2 }));
// static_assert(tdfa_capture("(a)*", "a", { 0, 0, 1 }));
// static_assert(tdfa_capture("(a)*", "aa", { 0, 1, 2 }));
// static_assert(tdfa_capture("(a)+b*", "ab", { 0, 0, 1 }));
// static_assert(tdfa_capture("(a)+b*", "aab", { 0, 1, 2 }));
// static_assert(tdfa_capture("(aa)+a*", "aaaaa", { 0, 2, 4 }));
// static_assert(tdfa_capture("(aa)+?a*", "aaaaa", { 0, 0, 2 }));
// static_assert(tdfa_capture("a(aa)b|(aa)ac", "aaab", { 0, 1, 3, no_tag, no_tag }));
// static_assert(tdfa_capture("a(aa)b|(aa)ac", "aaac", { 0, no_tag, no_tag, 0, 2 }));

/* wildcard tests */
static_assert(not tdfa_match(".", ""));
static_assert(tdfa_match(".", "0"));
static_assert(tdfa_match(".", "@"));
static_assert(tdfa_match(".", "$"));
static_assert(tdfa_match(".", "z"));
static_assert(tdfa_match(".", "A"));
static_assert(tdfa_match(".", "."));
static_assert(not tdfa_match(".", "\n"));
static_assert(tdfa_match("(?s).", "\n"));
// TODO: add test cases for non ascii chars