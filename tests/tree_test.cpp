#include <rx/tree.hpp>

#include "util/printable_tree.hpp"

namespace 
{
    constexpr rx::detail::parser_flags tree_test_flags{ .enable_alttocc=false };

    template<typename CharT>
    consteval bool parse(const CharT* str)
    {
        const rx::testing::printable_expr_tree<CharT> ast{ str, tree_test_flags };
        return (ast.to_pattern() == str);
    }

    template<typename CharT>
    consteval bool parse(const CharT* str, const CharT* result)
    {
        const rx::testing::printable_expr_tree<CharT> ast{ str, tree_test_flags };
        return (ast.to_pattern() == result);
    }

    template<typename CharT>
    consteval bool alt_to_cc(const CharT* str, const CharT* result)
    {
        const rx::testing::printable_expr_tree<CharT> ast{ str };
        return (ast.to_pattern() == result);
    }

    template<typename CharT>
    consteval bool equal_to(const CharT* str1, const CharT* str2)
    {
        const rx::testing::printable_expr_tree<CharT> ast1{ str1, tree_test_flags };
        const rx::testing::printable_expr_tree<CharT> ast2{ str2, tree_test_flags };
        return (ast1.to_pattern() == ast2.to_pattern());
    }

    template<typename CharT>
    consteval bool parse_error(const CharT* str)
    {
        try
        {
            const rx::detail::expr_tree<CharT> ast{ str, tree_test_flags };
            return false;
        }
        catch (const rx::pattern_error&)
        {
            return true;
        }
    }

    template<typename CharT>
    consteval bool test(const CharT* str)
    {
        rx::detail::expr_tree<CharT> ast{ str };
        return true;
    }
}

/* single units */
static_assert(parse(""));
static_assert(parse("a"));
static_assert(parse("ab"));
static_assert(parse("a|b"));
static_assert(parse("()"));
static_assert(parse("(a)"));
static_assert(parse("a*"));
static_assert(parse("a+"));
static_assert(parse("a?"));
static_assert(parse("a*?"));
static_assert(parse("a+?"));
static_assert(parse("a??"));
static_assert(parse("a{3}"));
static_assert(parse("a{3,}"));
static_assert(parse("a{3,5}"));
static_assert(parse("a{3}?"));
static_assert(parse("a{3,}?"));
static_assert(parse("a{3,5}?"));
static_assert(parse("a{0,30}?"));
static_assert(parse("a{0,1000}"));

/* symbols */
static_assert(parse("."));
static_assert(parse("^"));
static_assert(parse("$"));
static_assert(parse("\n"));
static_assert(parse("\\n", "\n"));
static_assert(parse("\\t", "\t"));
static_assert(parse("\\.", "."));
static_assert(parse("\\^", "^"));
static_assert(parse("\\$", "$"));
static_assert(parse("\\*", "*"));
static_assert(parse("\\+", "+"));
static_assert(parse("\\?", "?"));
static_assert(parse("\\???", "???"));

/* chaining tests */
static_assert(parse("abcdefg"));
static_assert(parse("a|b|c|d"));
static_assert(parse("a|bcde|cefg|d"));
static_assert(parse("a|(aaaaa)|c{3,5}|d"));
static_assert(parse("a|bcde|cefg|d"));
static_assert(parse("a+b*c+d?"));
static_assert(parse("a|bcde|cefg|d"));
static_assert(parse("(ab+c)ab(ddee{0,2})"));
static_assert(parse("a|"));
static_assert(parse("|a"));
static_assert(parse("|"));

/* reject invalid syntax */
#if __cpp_constexpr_exceptions >= 202411L
static_assert(parse_error("+"));
static_assert(parse_error("*"));
static_assert(parse_error("?"));
static_assert(parse_error("??"));
static_assert(parse_error("{2,3}"));
static_assert(parse_error("a{2,0}"));
static_assert(parse_error("a{2,2}"));
static_assert(parse_error("a{-1,2}"));
static_assert(parse_error("a{2,-3}"));
static_assert(parse_error("a{ 2,5}"));
static_assert(parse_error("a{ 2,5}"));
static_assert(parse_error("a{0,1001}"));
static_assert(parse_error("a{1001, 1001}"))
static_assert(parse_error("a{1, 22, 333}"));
static_assert(parse_error("(a"));
static_assert(parse_error(")-("))
static_assert(parse_error("abcde:)"))
static_assert(parse_error("[]"));
static_assert(parse_error("[^]"));
#endif // __cpp_constexpr_exceptions >= 202411L

/* repeat de-generalisation in pretty printer */
static_assert(parse("a{0,}", "a*"));
static_assert(parse("a{1,}", "a+"));
static_assert(parse("a{0,1}", "a?"));
static_assert(parse("a{0,}?", "a*?"));
static_assert(parse("a{1,}?", "a+?"));
static_assert(parse("a{0,1}?", "a??"));
static_assert(test("a{1,512}"));

/* escape sequences */
static_assert(parse(R"(\33)", "\33"));
static_assert(parse(R"(\033)", "\033"));
static_assert(parse(R"(\60)", "0"));
static_assert(parse(R"(\141)", "a"));
static_assert(parse(R"(\x1)", "\1"));
static_assert(parse(R"(\x1b)", "\x1b"));
static_assert(parse(R"(\x21)", "!"));
static_assert(parse(R"(\x61)", "a"));
static_assert(parse(R"(\x4f)", "O"));
static_assert(parse(R"(\u0021)", "!"));
static_assert(parse(R"(\u0061)", "a"));
static_assert(parse(R"(\u004f)", "O"));
static_assert(parse(R"(\U00000021)", "!"));
static_assert(parse(R"(\U00000061)", "a"));
static_assert(parse(R"(\U0000004F)", "O"));
static_assert(parse(R"(\x{61})", "a"));
static_assert(parse(R"(\u{49})", "I"));
static_assert(parse(R"(\U{6F})", "o"));
static_assert(parse(uR"(\u2705)", u"✅"));
static_assert(parse(UR"(\x{1F30D})", U"🌍"));
static_assert(parse(UR"(\u{1F30E})", U"🌎"));
static_assert(parse(UR"(\U{1F30F})", U"🌏"));
static_assert(parse(UR"(\U0001F310)", U"🌐"));
// TODO: replace previous 5 tests with tests for multibyte chars

/* character classes */
static_assert(parse("."));
static_assert(parse("[az]"));
static_assert(parse("[a-z]"));
static_assert(parse("[abc]", "[a-c]"));
static_assert(parse("[a-cb]", "[a-c]"));
static_assert(parse("[a-ca-c]", "[a-c]"));
static_assert(parse("[a-da-c]", "[a-d]"));
static_assert(parse("[a-fc-j]", "[a-j]"));
static_assert(parse("[a-cd-f]", "[a-f]"));
static_assert(parse("[a-cde-g]", "[a-g]"));
static_assert(parse("[AZaz]"));
static_assert(parse("[aAzZ]", "[AZaz]"));
static_assert(parse("[a-zA-Z]", "[A-Za-z]"));
static_assert(parse("[A-zZ-a]", "[A-z]"));
static_assert(parse("[-]", "[\\-]"));
static_assert(parse("[--]", "[\\-]"));
static_assert(parse("[---]", "[\\-]"));
static_assert(parse("[----]", "[\\-]"));
static_assert(parse("[-----]", "[\\-]"));
static_assert(parse("[------]", "[\\-]"));
static_assert(parse("[a-]", "[\\-a]"));
static_assert(parse("[a\\-]", "[\\-a]"));
static_assert(parse("[-\\-a]", "[\\-a]"));
static_assert(parse("[^^]", "[^\\^]"));
static_assert(parse("[^-^]", "[^\\-\\^]"));
static_assert(parse("[^^]", "[^\\^]"));
static_assert(parse("[^\\^]", "[^\\^]"));
static_assert(parse("[\\^\\^]", "[\\^]"));
static_assert(parse("[]]", "[\\]]"));
static_assert(parse("[^]]", "[^\\]]"));
static_assert(parse("[\\[\\]]", "[\\[\\]]"));
static_assert(parse("[\\0]", "[\\0]"));
static_assert(parse("[\\60-\\71]", "[0-9]"));
static_assert(parse("[\\060-\\071]", "[0-9]"));
static_assert(parse("[\\141]", "[a]"));
static_assert(parse("[\\141-\\172]", "[a-z]"));
static_assert(parse("[\\x61]", "[a]"));
static_assert(parse("[\\x61-\\x64]", "[a-d]"));
static_assert(parse("[\\u0061]", "[a]"));
static_assert(parse("[\\U00000061]", "[a]"));
static_assert(parse("[\\u002b]", "[+]"));
static_assert(parse("[+-]", "[+\\-]"));
static_assert(parse("[*+-]", "[*-+\\-]"));
static_assert(parse("[*-+]"));
static_assert(parse("[+--]", "[+-\\-]"));

/* named character classes */
static_assert(parse("[[:alnum:]]", "[0-9A-Za-z]"));
static_assert(parse("[[:alpha:]]", "[A-Za-z]"));
static_assert(parse("[[:ascii:]]", R"([\0-\177])"));
static_assert(parse("[[:blank:]]", R"([\t ])"));
static_assert(parse("[[:cntrl:]]", R"([\0-\37\177])"));
static_assert(parse("[[:digit:]]", "[0-9]"));
static_assert(parse("[[:graph:]]", "[!-~]"));
static_assert(parse("[[:lower:]]", "[a-z]"));
static_assert(parse("[[:print:]]", "[ -~]"));
static_assert(parse("[[:punct:]]", R"([!-/:-@\[-`{-~])"));
static_assert(parse("[[:space:]]", R"([\t-\r ])"));
static_assert(parse("[[:upper:]]", "[A-Z]"));
static_assert(parse("[[:word:]]", "[0-9A-Z_a-z]"));
static_assert(parse("[[:xdigit:]]", "[0-9A-Fa-f]"));

/* perl character classes */
static_assert(equal_to(".", "[^\\n]"));
static_assert(equal_to("\\d", "[[:digit:]]"));
static_assert(equal_to("\\D", "[^[:digit:]]"));
static_assert(parse("\\s", R"([\t-\n\f-\r ])"));
static_assert(parse("\\S", R"([^\t-\n\f-\r ])"));
static_assert(equal_to("\\w", "[[:word:]]"));
static_assert(equal_to("\\W", "[^[:word:]]"));
static_assert(equal_to("[\\d]", "\\d"));
static_assert(equal_to("[\\s]", "\\s"));
static_assert(equal_to("[\\w]", "\\w"));
static_assert(test("[\\D]")); /* there's no good way of verifying these 3 */
static_assert(test("[\\S]"));
static_assert(test("[\\W]"));

/* other character class tests */
static_assert(parse("[ [:graph:]]", "[ -~]"));
static_assert(equal_to("[[:alpha:][:digit:]]", "[[:alnum:]]"));
static_assert(equal_to("[\\d[:upper:]a-z]", "[[:alnum:]]"));

/* alt-to-character-class merging tests */
static_assert(alt_to_cc("a|c", "[ac]"));
static_assert(alt_to_cc("a|b", "[a-b]"));
static_assert(alt_to_cc("a|b|c|d", "[a-d]"));
static_assert(alt_to_cc("ab|c|e", "ab|[ce]"));
static_assert(alt_to_cc("ce|c|e", "ce|[ce]"));
static_assert(alt_to_cc("a|c|ac", "[ac]|ac"));
static_assert(alt_to_cc("a|g|[ce]", "[aceg]"));
static_assert(alt_to_cc("a|f|[c-e]", "[ac-f]"));
static_assert(alt_to_cc(" |[[:graph:]]", "[ -~]"));
static_assert(alt_to_cc("[[:alpha:]]|[[:digit:]]", "[0-9A-Za-z]"));

/* other test cases */
static_assert(parse("(?# comment )", ""));
static_assert(parse("(?:aaa)", "aaa"));
static_assert(parse("\\Q...\\E", "..."));
static_assert(test("(v*)*|j*"));