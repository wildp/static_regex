#include <print>

// import std;
import rx;
import rx.ast;
import rx.fsm;
import rx.tests;

namespace
{
    [[maybe_unused]] void t1()
    {
        using namespace rx::literals;
        auto matcher = "Hello World"_rx;
        std::println("{}", matcher.test());
    }

    [[maybe_unused]] void t2()
    {
        rx::testing::printable_expr_tree<char> tmp("a(aa)b|(aa)ac");
        // rx::testing::printable_expr_tree<char> tmp("ab|cd");
        std::println("Pattern: {}", tmp.to_pattern());
        
        rx::testing::tnfa_matcher<char> tnfa{ tmp };

        for (auto sv : std::vector<std::string_view>{"aaab", "aaac" })
        {
            auto result{ tnfa.submatches(sv) };

            if (result.has_value())
                std::println("Match: {}", *result);
            else
                std::println("No Match");
        }
    }

    [[maybe_unused]] void t3()
    {
        rx::testing::tree_matcher<char> tree("a(bb|b)b");
        // rx::testing::tree_matcher<char> tree("(?|(a)|(b)(c))(d)\\3", rx::detail::parser_flags{ .enable_possessive=true, .enable_backrefs=true, .enable_branchreset=true } );

        for (auto sv : std::vector<std::string_view>{"abb", "abbb" })
        {
            auto result{ tree.match(sv) };

            if (result)
                std::println("Match: {}", sv);
            else
                std::println("No Match: {}", sv);
        }
    }
}


int main()
{
 
    using namespace rx::detail;

    // t1();
    // t2();
    t3();

    return 0;
}