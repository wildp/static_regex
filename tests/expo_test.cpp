#include "headers/tree_matcher.hpp"


namespace
{
    constexpr rx::detail::parser_flags expo_test_flags{ .enable_possessive=true, .enable_backrefs=true, .enable_branchreset=true };

    template<typename CharT>
    consteval bool match(const CharT* pattern, const CharT* str, const std::vector<std::size_t>& captures = {})
    {
        rx::testing::tree_matcher<CharT> tree{ pattern, expo_test_flags };
        auto match_result{ tree.match(std::string_view{ str }) };

        if (captures.empty())
            return match_result.has_value();
        else
            return std::ranges::equal(match_result.value() | std::views::drop(2) , captures);
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
static_assert(not match("a{2,}?", "a"));
static_assert(match("a{2,}?", "aa"));
static_assert(match("a{2,}?", "aaa"));
static_assert(match("a{2,}?", "aaaa"));
static_assert(match("a{2,}?", "aaaaa"));
static_assert(not match("a{3},", "aa"));
static_assert(match("a{3}", "aaa"));
static_assert(not match("a{3}", "aaaa"));
static_assert(match("a{0}", ""));
static_assert(not match("a{0}", "a"));
static_assert(match("a{0,1}", ""));
static_assert(match("a{0,1}", "a"));
static_assert(not match("a{0,1}", "aa"));
static_assert(not match("a{0,1}", "aaa"));
static_assert(not match("a{1,2}", ""));
static_assert(match("a{1,2}", "a"));
static_assert(match("a{1,2}", "aa"));
static_assert(not match("a{1,2}", "aaa"));
static_assert(not match("a{1,2}", "aaa"));

/* laziness tests */
static_assert(match("(a)?a*", "", { no_tag, no_tag }));
static_assert(match("(a)?a*", "a", { 0, 1 }));
static_assert(match("(a)?a*", "aa", { 0, 1 }));
static_assert(match("(a)??a*", "aa", { no_tag, no_tag }));
static_assert(match("(a)+?a?", "aa", { 0, 1 }));
static_assert(match("(a)+a?", "aa", { 1, 2 }));
static_assert(match("(a){2,3}?a?", "aaa", { 1, 2 }));
static_assert(match("(a){2,3}a?", "aaa", { 2, 3 }));

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
// TODO: add test cases for non ascii chars

/* backtracking tests */
static_assert(match("a(b|bb)b", "abb"));
static_assert(match("a(b|bb)b", "abbb"));
static_assert(match("a(bb|b)b", "abb"));
static_assert(match("a(bb|b)b", "abbb"));
static_assert(match("(a*b)+", "aaababaaaab"));
static_assert(match("(a+){4}", "aaaaaaaaaaaa"));
static_assert(match("(aa|aaa)*a", "aaaaaa"));

/* backreference tests */
static_assert(match("(abc)\\1", "abcabc"));
static_assert(match("(a|b|c)\\g1", "bb"));
static_assert(match("(a|\1){2}", "aa"));
static_assert(match("(a)(b)\\g2\\g1", "abba"));
static_assert(match("(ab(cd))\\1\\2", "abcdabcdcd"));
static_assert(not match("(ab(cd))\\1\\2", "abcdcdabcd"));
static_assert(match("((ab|e\\3)(c*)d){2}", "abccdeccd"));
static_assert(match("((a)(b)\\3\\2){2}", "abbaabba"));
static_assert(not match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\10", "abcdefghijkj"));
static_assert(not match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g10", "abcdefghijkj"));
static_assert(match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g10", "abcdefghijka0"));
static_assert(match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g{10}", "abcdefghijkj"));

/* capture features */
static_assert(match("(a)", "a"));
static_assert(match("(a)(b)", "ab"));
static_assert(match("(a(b))", "ab"));
static_assert(match("(?:a)", "a"));
static_assert(match("(?#comment)", ""));
static_assert(match("(?:c)(a)(b)\\2", "cabb"));
static_assert(not match("(?:c)(a)(b)\\2", "caba"));
static_assert(match("(?|(a)|(b))\\1", "aa"));
static_assert(match("(?|(a)|(b))\\1", "bb"));
static_assert(match("(?|(a)|(b))(c)\\1\\2", "acac"));
static_assert(match("(?|(a)|(b))(c)\\1\\2", "bcbc"));
static_assert(match("(?|(a)|(b)(c))(d)\\3", "add"));
static_assert(match("(?|(a)|(b)(c))(d)\\1\\2", "bcdbc"));
static_assert(match("(?|(a)|(b)(c)|(d)(e))(f)\\1\\2", "bcfbc"));
static_assert(match("(?|(a)|(b)(c)|(d)(e))(f)\\1\\2", "defde"));

/* caseless flag */
static_assert(match("(?i)abc", "ABC"));
static_assert(match("(?i)abc", "Abc"));
static_assert(match("(?i)abc", "aBc"));
static_assert(match("(?i)a[be]", "aB"));
static_assert(match("(?i)a[Z-a]", "az"));
static_assert(match("a(?i:b)c", "abc"));
static_assert(match("a(?i:b)c", "aBc"));
static_assert(not match("a(?i:b)c", "abC"));

/* dotall flag */
static_assert(match("(?s).", "\n"));
static_assert(match("a.b", "a-b"));
static_assert(not match("a.b", "a\nb"));
static_assert(match("(?s)a.b", "a\nb"));
static_assert(not match("((?s))a.b", "a\nb"));
static_assert(not match("(?:(?s))a.b", "a\nb"));

/* ungreedy flag */
static_assert(match("(?U)(a)?a*", "aa", { no_tag, no_tag }));
static_assert(match("(?U)(a)??a*", "aa", { 0, 1 }));
static_assert(match("(?U:)(a)?a*", "aa", { 0, 1 }));
static_assert(match("(?U:)(a)??a*", "aa", { no_tag, no_tag }));
static_assert(match("(?U)(aa)+a*", "aaaaa", { 0, 2 }));
static_assert(match("(?U)(aa)+?a*", "aaaaa", { 2, 4 }));
static_assert(match("(?U-U)(a)?a*", "aa", { 0, 1 }));
static_assert(match("(?U-U)(a)??a*", "aa", { no_tag, no_tag }));
static_assert(match("(?U)(a)?a*(?-U)(b)?b*", "aabb", { no_tag, no_tag, 2, 3 }));
static_assert(match("(?U)(a)??a*(?-U)(b)??b*", "aabb", { 0, 1, no_tag, no_tag }));

/* additional parser tests */
static_assert(match("\\Q...\\E", "..."));
static_assert(not match("\\Q...\\E", "aaa"));
