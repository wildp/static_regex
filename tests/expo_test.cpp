#include "rx/tree.hpp"
#include "util/tree_matcher.hpp"

namespace
{
    template<typename CharT>
    consteval bool tree_match(const CharT* pattern, const CharT* str)
    {
        rx::testing::tree_matcher<CharT> tree{ pattern , rx::detail::parser_flags{ .enable_possessive=true, .enable_backrefs=true, .enable_branchreset=true } };

        return tree.match(std::string_view{ str });
    }

    template<typename CharT>
    consteval bool tree_capture(const CharT* pattern, const CharT* str, const std::vector<std::size_t>& captures)
    {
        rx::testing::tree_matcher<CharT> tree{ pattern, rx::detail::parser_flags{ .enable_possessive=true, .enable_backrefs=true, .enable_branchreset=true }  };
        auto match_result{ tree.submatches(std::string_view{ str }) };

        if (not match_result.has_value())
            return false;

        return std::ranges::equal(match_result.value(), captures);
    }
}

using rx::detail::no_tag;

/* basic tests */
static_assert(tree_match("", ""));
static_assert(not tree_match("", "a"));
static_assert(not tree_match("a", ""));
static_assert(tree_match("a", "a"));
static_assert(not tree_match("a", "b"));

/* grouping tests */
static_assert(not tree_match("a|b", ""));
static_assert(tree_match("a|b", "a"));
static_assert(tree_match("a|b", "b"));
static_assert(tree_match("a|b|c", "c"));
static_assert(tree_match("a|b|c", "b"));
static_assert(tree_match("a|b|c", "a"));
static_assert(not tree_match("aa", ""));
static_assert(not tree_match("aa", "a"));
static_assert(tree_match("aa", "aa"));
static_assert(not tree_match("aa", "aaa"));
static_assert(not tree_match("ab", ""));
static_assert(not tree_match("ab", "a"));
static_assert(not tree_match("ab", "b"));
static_assert(tree_match("ab", "ab"));
static_assert(not tree_match("ab", "ba"));
static_assert(not tree_match("ab", "abb"));
static_assert(not tree_match("ab", "aab"));
static_assert(not tree_match("ab", "bb"));

/* repeater tests */
static_assert(tree_match("a*", ""));
static_assert(tree_match("a*", "a"));
static_assert(tree_match("a*", "aa"));
static_assert(tree_match("a*", "aaaaa"));
static_assert(not tree_match("a+", ""));
static_assert(tree_match("a+", "a"));
static_assert(tree_match("a+", "aa"));
static_assert(tree_match("a+", "aaaaa"));
static_assert(tree_match("a?", ""));
static_assert(tree_match("a?", "a"));
static_assert(not tree_match("a?", "aa"));
static_assert(not tree_match("a?", "aaaaa"));
static_assert(not tree_match("a{1}", ""));
static_assert(tree_match("a{1}", "a"));
static_assert(not tree_match("a{1}", "aa"));
static_assert(not tree_match("a{2,5}", "a"));
static_assert(tree_match("a{2,5}", "aa"));
static_assert(tree_match("a{2,5}", "aaa"));
static_assert(tree_match("a{2,5}", "aaaa"));
static_assert(tree_match("a{2,5}", "aaaaa"));
static_assert(not tree_match("a{2,5}", "aaaaaaa"));
static_assert(not tree_match("a{2,}", "a"));
static_assert(tree_match("a{2,}", "aa"));
static_assert(tree_match("a{2,}", "aaa"));
static_assert(tree_match("a{2,}", "aaaa"));
static_assert(tree_match("a{2,}", "aaaaa"));
static_assert(not tree_match("a{2,}?", "a"));
static_assert(tree_match("a{2,}?", "aa"));
static_assert(tree_match("a{2,}?", "aaa"));
static_assert(tree_match("a{2,}?", "aaaa"));
static_assert(tree_match("a{2,}?", "aaaaa"));
static_assert(not tree_match("a{3},", "aa"));
static_assert(tree_match("a{3}", "aaa"));
static_assert(not tree_match("a{3}", "aaaa"));
static_assert(tree_match("a{0}", ""));
static_assert(not tree_match("a{0}", "a"));
static_assert(tree_match("a{0,1}", ""));
static_assert(tree_match("a{0,1}", "a"));
static_assert(not tree_match("a{0,1}", "aa"));
static_assert(not tree_match("a{0,1}", "aaa"));
static_assert(not tree_match("a{1,2}", ""));
static_assert(tree_match("a{1,2}", "a"));
static_assert(tree_match("a{1,2}", "aa"));
static_assert(not tree_match("a{1,2}", "aaa"));
static_assert(not tree_match("a{1,2}", "aaa"));

/* laziness tests */
static_assert(tree_capture("(a)?a*", "", { 0, no_tag, no_tag }));
static_assert(tree_capture("(a)?a*", "a", { 0, 0, 1 }));
static_assert(tree_capture("(a)?a*", "aa", { 0, 0, 1 }));
static_assert(tree_capture("(a)??a*", "aa", { 0, no_tag, no_tag }));

/* capture location tests */
static_assert(tree_capture("(a)+b*", "ab", { 0, 0, 1 }));
static_assert(tree_capture("(a)+b*", "aab", { 0, 1, 2 }));
static_assert(tree_capture("(aa)+a*", "aaaaa", { 0, 2, 4 }));
static_assert(tree_capture("(aa)+?a*", "aaaaa", { 0, 0, 2 }));
static_assert(tree_capture("a(aa)b|(aa)ac", "aaab", {0, 1, 3, no_tag, no_tag }));
static_assert(tree_capture("a(aa)b|(aa)ac", "aaac", {0, no_tag, no_tag, 0, 2 }));

/* wildcard tests */
static_assert(not tree_match(".", ""));
static_assert(tree_match(".", "0"));
static_assert(tree_match(".", "@"));
static_assert(tree_match(".", "$"));
static_assert(tree_match(".", "z"));
static_assert(tree_match(".", "A"));
static_assert(tree_match(".", "."));
static_assert(tree_match(".", "\n")); // NOTE: this should not match!
// TODO: add test cases for non ascii chars

/* backtracking tests */
static_assert(tree_match("a(b|bb)b", "abb"));
static_assert(tree_match("a(b|bb)b", "abbb"));
static_assert(tree_match("a(bb|b)b", "abb"));
static_assert(tree_match("a(bb|b)b", "abbb"));
static_assert(tree_match("(a*b)+", "aaababaaaab"));
static_assert(tree_match("(a+){4}", "aaaaaaaaaaaa"));
static_assert(tree_match("(aa|aaa)*a", "aaaaaa"));

/* backreference tests */
static_assert(tree_match("(abc)\\1", "abcabc"));
static_assert(tree_match("(a|b|c)\\g1", "bb"));
static_assert(tree_match("(a|\1){2}", "aa"));
static_assert(tree_match("(a)(b)\\g2\\g1", "abba"));
static_assert(tree_match("(ab(cd))\\1\\2", "abcdabcdcd"));
static_assert(not tree_match("(ab(cd))\\1\\2", "abcdcdabcd"));
static_assert(tree_match("((ab|e\\3)(c*)d){2}", "abccdeccd"));
static_assert(tree_match("((a)(b)\\3\\2){2}", "abbaabba"));
static_assert(not tree_match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\10", "abcdefghijkj"));
static_assert(not tree_match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g10", "abcdefghijkj"));
static_assert(tree_match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g10", "abcdefghijka0"));
static_assert(tree_match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g{10}", "abcdefghijkj"));

/* capture features */
static_assert(tree_match("(a)", "a"));
static_assert(tree_match("(a)(b)", "ab"));
static_assert(tree_match("(a(b))", "ab"));
static_assert(tree_match("(?:a)", "a"));
static_assert(tree_match("(?#comment)", ""));
static_assert(tree_match("(?:c)(a)(b)\\2", "cabb"));
static_assert(not tree_match("(?:c)(a)(b)\\2", "caba"));
static_assert(tree_match("(?|(a)|(b))\\1", "aa"));
static_assert(tree_match("(?|(a)|(b))\\1", "bb"));
static_assert(tree_match("(?|(a)|(b))(c)\\1\\2", "acac"));
static_assert(tree_match("(?|(a)|(b))(c)\\1\\2", "bcbc"));
static_assert(tree_match("(?|(a)|(b)(c))(d)\\3", "add"));
static_assert(tree_match("(?|(a)|(b)(c))(d)\\1\\2", "bcdbc"));
static_assert(tree_match("(?|(a)|(b)(c)|(d)(e))(f)\\1\\2", "bcfbc"));
static_assert(tree_match("(?|(a)|(b)(c)|(d)(e))(f)\\1\\2", "defde"));
static_assert(tree_capture("(?U)(a)?a*", "aa", { 0, no_tag, no_tag }));
static_assert(tree_capture("(?U)(a)??a*", "aa", { 0, 0, 1 }));
static_assert(tree_capture("(?U:)(a)?a*", "", { 0, no_tag, no_tag }));
static_assert(tree_capture("(?U)(aa)+a*", "aaaaa", { 0, 0, 2 }));
static_assert(tree_capture("(?U)(aa)+?a*", "aaaaa", { 0, 2, 4 }));
static_assert(tree_capture("(?U-U)(a)?a*", "aa", { 0, 0, 1 }));
static_assert(tree_capture("(?U-U)(a)??a*", "aa", { 0, no_tag, no_tag }));
static_assert(tree_capture("(?U)(a)?a*(?-U)(b)?b*", "aabb", { 0, no_tag, no_tag, 2, 3 }));
static_assert(tree_capture("(?U)(a)??a*(?-U)(b)??b*", "aabb", { 0, 0, 1, no_tag, no_tag }));
