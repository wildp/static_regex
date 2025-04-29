#include <rx/tree.hpp>

#include "util/printable_tree.hpp"

namespace 
{
    template<typename CharT>
    consteval bool parse(const CharT* str)
    {
        const rx::testing::printable_expr_tree<CharT> ast{ str };
        return (ast.to_pattern() == str);
    }

    template<typename CharT>
    consteval bool parse(const CharT* str, const CharT* result)
    {
        const rx::testing::printable_expr_tree<CharT> ast{ str };
        return (ast.to_pattern() == result);
    }

    template<typename CharT>
    consteval bool parse_error(const CharT* str)
    {
        try
        {
            const rx::detail::expr_tree<CharT> ast{ str };
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
#endif // __cpp_constexpr_exceptions >= 202411L

/* repeat de-generalisation in pretty printer */
static_assert(parse("a{0,}", "a*"));
static_assert(parse("a{1,}", "a+"));
static_assert(parse("a{0,1}", "a?"));
static_assert(parse("a{0,}?", "a*?"));
static_assert(parse("a{1,}?", "a+?"));
static_assert(parse("a{0,1}?", "a??"));
static_assert(test("a{1,512}"));

/* other test cases */
static_assert(test("(v*)*|j*"));