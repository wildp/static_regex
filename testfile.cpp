#include <print>
#include <meta>

#include <rx/regex.hpp>
#include "tests/headers/printable_tree.hpp"
#include "tests/headers/printable_tdfa.hpp"
#include "tests/headers/tdfa_matcher.hpp"
#include "tests/headers/tnfa_matcher.hpp"
#include "tests/headers/tree_matcher.hpp"

// template<typename CharT>
// struct std::formatter<rx::detail::char_class_impl<CharT>> {
//     constexpr auto parse(std::format_parse_context& ctx) {
//         return ctx.begin();
//     }

//     auto format(const rx::detail::char_class_impl<CharT>& obj, std::format_context& ctx) const {
//         return std::format_to(ctx.out(), "[cc]");
//     }
// };


// template<typename T>
// std::string print_struct(const T& value)
// {
//     std::string result{ "{" };

//     template for (constexpr auto mem : std::define_static_array(std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked())))
//     {
//         if constexpr (std::is_enum_v<typename [:std::meta::type_of(mem):]>)
//         {
//             result += std::format(" .{}={},", std::define_static_string(std::meta::display_string_of(mem)), rx::detail::enum_to_string(value.[:mem:]));
//         }
//         else
//         {
//             result += std::format(" .{}={},", std::define_static_string(std::meta::display_string_of(mem)), value.[:mem:]);
//         }
//     }

//     // if constexpr (std::define_static_array(std::meta::nonstatic_data_members_of(^^T, ctx)).size() > 0)
//     //     result.pop_back();
//     result += " }";
//     return result;
// }

// template<typename... Ts>
// std::string variant_contents(const std::variant<Ts...>& value)
// {
//     template for (constexpr auto e : { (^^Ts)... })
//     {
//         if (std::holds_alternative<typename [:e:]>(value))
//         {
//             return std::format("{}: {}", std::define_static_string(std::meta::display_string_of(e)), print_struct(std::get<typename [:e:]>(value)));
//         }
//     }

//     return "<error>";
// }

namespace
{
    using namespace rx::testing;

    [[maybe_unused]] void t1()
    {
        using namespace rx::literals;
        auto matcher = "Hello World"_rx;
        std::println("{}", matcher.match("Hello World").has_value());
    }

    [[maybe_unused]] void t2(std::string_view pat, const std::vector<std::string_view>& test)
    {
        printable<rx::detail::expr_tree<char>> tmp{ pat };
        std::println("Pattern: {}", tmp.to_pattern());

        // const auto& ci{ tmp.get_capture_info() };
        // const auto cc{ ci.capture_count() };
        // for (std::size_t i{ 0 }; i < cc; ++i)
        // {
        //     auto [beg, end]{ ci.lookup(i) };
        //     for (const auto& e : std::vector(beg, end))
        //         std::println("{}: {}+{}, {}+{}", i, e.first.tag_number, e.first.offset, e.second.tag_number, e.second.offset);
        // }

        rx::testing::tnfa_matcher<char> tnfa{ tmp };

        for (auto sv : test)
        {
            auto result{ tnfa.match(sv) };

            if (result.has_value())
                std::println("'{}': Match: {}", sv, *result);
            else
                std::println("'{}': No Match", sv);
        }
    }

    [[maybe_unused]] void t3(std::string_view pat, const std::vector<std::string_view>& test)
    {
        rx::detail::parser_flags f{ .enable_possessive=true, .enable_backrefs=true, .enable_branchreset=true };
        printable<tree_matcher<char>> tree{ pat, f };

        std::println("Pattern: {}", tree.to_pattern());

        for (auto sv : test)
        {
            auto result{ tree.match(sv) };

            if (result.has_value())
                std::println("'{}': Match: {}", sv, *result);
            else
                std::println("'{}': No Match", sv);
        }
    }

    [[maybe_unused]] void t4(std::string_view pat, const std::vector<std::string_view>& test)
    {
        printable<rx::detail::expr_tree<char>> tmp{ pat };
        std::println("Pattern: {}", tmp.to_pattern());
        
        rx::testing::tnfa_matcher<char> tnfa{ tmp };
        rx::testing::tdfa_matcher<char> tdfa{ tnfa };

        for (auto sv : test)
        {
            auto result{ tdfa.match(sv) };

            if (result.has_value())
                std::println("'{}': Match: {}", sv, *result);
            else
                std::println("'{}': No Match", sv);
        }
    }

    [[maybe_unused]] void t5(std::string_view pat, const std::vector<std::string_view>& test)
    {
        printable<rx::detail::expr_tree<char>> tmp{ pat };
        std::println("Pattern: {}", tmp.to_pattern());

        const auto& ci{ tmp.get_capture_info() };
        for (std::size_t i{ 0 }; i < ci.capture_count(); ++i)
            for (const auto& elem: ci.lookup(i))
                std::println("{}: {}+{}, {}+{}", i, elem.first.tag_number, elem.first.offset, elem.second.tag_number, elem.second.offset);

        tmp.optimise_tags();

        // const auto& ci{ tmp.get_capture_info() };
        for (std::size_t i{ 0 }; i < ci.capture_count(); ++i)
            for (const auto& elem: ci.lookup(i))
                std::println("{}: {}+{}, {}+{}", i, elem.first.tag_number, elem.first.offset, elem.second.tag_number, elem.second.offset);

        rx::detail::tagged_nfa<char> tnfa{ tmp };
        printable<rx::testing::tdfa_matcher<char>> tdfa{ tnfa };
        std::println("Pattern ok\n");

        tdfa.print();
        std::println("\n");

        for (auto sv : test)
        {
            auto result{ tdfa.match(sv) };

            if (result.has_value())
                std::println("'{}': Match: {}", sv, *result);
            else
                std::println("'{}': No Match", sv);
        }
    }

    [[maybe_unused]] void t6(std::string_view pat, const std::vector<std::string_view>& test)
    {
        printable<rx::detail::expr_tree<char>> tmp{ pat };
        std::println("Pattern: {}", tmp.to_pattern());
        tmp.optimise_tags();
        rx::detail::tagged_nfa<char> tnfa{ tmp };
        printable<rx::testing::tdfa_matcher<char>> tdfa{ tnfa };
        std::println("Pattern ok\n");

        tdfa.optimise_registers();
        std::println("Opt ok\n");

        tdfa.print();
        std::println("\n");

        for (auto sv : test)
        {
            auto result{ tdfa.match(sv) };

            if (result.has_value())
                std::println("'{}': Match: {}", sv, *result);
            else
                std::println("'{}': No Match", sv);
        }
    }

    [[maybe_unused]] void t7(std::string_view pat, const std::vector<std::string_view>& test)
    {
        printable<rx::detail::expr_tree<char>> tmp{ pat };
        tmp.insert_search_prefix();
        std::println("Pattern: {}", tmp.to_pattern());
        tmp.optimise_tags();

        rx::detail::tagged_nfa<char> tnfa{ tmp };
        printable<rx::testing::tdfa_matcher<char>> tdfa{ tnfa };
        std::println("Pattern ok\n");

        tdfa.print();
        tdfa.optimise_registers();
        std::println("Opt ok\n");

        tdfa.print();
        std::println("\n");

        for (auto sv : test)
        {
            auto result{ tdfa.match(sv) };

            if (result.has_value())
                std::println("{:?}: Match: {}", sv, *result);
            else
                std::println("{:?}: No Match", sv);
        }
    }

    template<rx::detail::string_literal S>
    [[maybe_unused]] void t8m(rx::compile_time_regex<S> m, const std::vector<std::string_view>& test)
    {
        for (auto sv : test)
        {
            if (auto res = m.match(sv))
                std::println("{:?}: Match: {::?}", sv, res);
            else
                std::println("{:?}: No Match", sv);
        }
    }

    template<rx::detail::string_literal S>
    [[maybe_unused]] void t8w(rx::compile_time_regex<S> m, const std::vector<std::string_view>& test)
    {
        for (auto sv : test)
        {
            if (auto res = m.starts_with(sv))
                std::println("{:?}: Partial Match: {::?}", sv, res);
            else
                std::println("{:?}: No Match", sv);
        }
    }

    template<rx::detail::string_literal S>
    [[maybe_unused]] void t8s(rx::compile_time_regex<S> m, const std::vector<std::string_view>& test)
    {
        for (auto sv : test)
        {
            if (auto res = m.search(sv))
                std::println("{:?}: Found Match: {::?}", sv, res);
            else
                std::println("{:?}: No Match", sv);
        }
    }
    
}


int main()
{
    using namespace rx::literals;

    // t1();

    // t2("()", { "", "a" });
    // t2("(a)", { "", "a" });
    // t2("a(aa)b|(aa)ac", { "aaab", "aaac" });

    // t3("a(aa)b|(aa)ac", { "aaab", "aaac" });
    // t3("(a*b)+", { "aaababaaaab" });
    // t3("a(bb|b)b", { "abb", "abbb" });
    // t3("(?|(a)|(b)(c))(d)\\3", { "abb", "abbb" });

    // t5("a(aa)b|(aa)ac", { "aaab", "aaac" });
    // t5("(a)*(a|b)b*", {});
    // t5("(a)*(b)*", {});
    // t5("(a|b)*b", {});
    // t5("(a)*", { "", "a", "aa" });
    // t5("(ab)*", {});
    // t5("(a)", { "a" });

    // t5("(a)+(b)*", {"ab", "abb", "aaabbb" });
    // t5("(a)+b*|c*", { "ab", "aab" });

    // t6("(?:()a())*()(?:a|()b)()b*", { "ab" });

    // t7("Wor(ld)+", { "Hello World", "WWWorldld" });

    // t7("(abc)*", {});
    // t7("(abc)*|(bcd)+", {});
    // t7("(abc|g.)+", {});

    // t7(".abc", {});

    // t8m("(?:()a())*()(?:a|()b)()b*"_rx, { "a", "b", "ab", "aaa", "aaabb", "abba", "bbaa" });
    // t8m("(?i)a(aa)b|(aa)ac"_rx, { "aAaB", "aAaC" } );
    // t8s("Wor(ld)"_rx, { "World World World", "Hello World", "World" });
    // t8m("a(bc)d"_rx, { "abcd" });

    // t8m("(ab+c)+?(ab+c)?"_rx, { "abcabbc" });
    // t8m("(ab+c)+(ab+c)?"_rx, { "abcabbc" });

    // t8s("(ab+c)+?(ab+c)?"_rx, { "abcabbc" });
    // t8s("(ab+c)+(ab+c)?"_rx, { "abcabbc" });
    
    // t8w("(ab+c)+?(ab+c|.*d)"_rx, { "abcabbcacd__" });
    // t8w("(ab+c)+(ab+c|.*d)"_rx, { "abcabbcacd__" });

    // t8s("(ab+c)+?(ab+c)?"_rx, { "aaabacabcabbcac" });
    // t8s("(ab+c)+(ab+c)?"_rx, { "aaabacabcabbcac" });

    // auto w3c_email_regex = R"([[:alnum:].+/=?^_-]+@[[:alnum:]](?:[[:alnum:]-]{0,61}[[:alnum:]])?(?:\.[[:alnum:]](?:[[:alnum:]-]{0,61}[[:alnum:]])?)+)"_rx;
    // auto simple_email_regex = R"([\w\-\.]+@([\w-]+\.)+[\w-]{2,})"_rx;
    // t8m(w3c_email_regex, { "Hello@example.com", "test@example@example", "@example.example" });
    // t8m(simple_email_regex, { "Hello@example.com", "test@example@example", "@example.example" });
    // t8s(w3c_email_regex, { "Hello@example.com", "test@example@example", "@example.example" }); /* RIP compile times */

    // t4("[ac]est|best", { "best", "cest" });

    // t3("(a)+a?", { "aa" });
    // t4("(ab)+?(?:ab)?", { "abab" });
    // t4("(a)+a?", { "aa" });

    // t3("(ab+c)+?(ab+c|.*d)", { "abcabbcacd" });
    // t4("(?s)(ab+c)+?(ab+c|.*d)", { "abcabbcacd" });

    // t2("(a|bcdef|g|ab|c|d|e|efg|fg)*", { "abcdefg" });
    // t3("(a|bcdef|g|ab|c|d|e|efg|fg)*", { "abcdefg" });
    // t4("(a|bcdef|g|ab|c|d|e|efg|fg)*", { "abcdefg" });
    // t6("(a|bcdef|g|ab|c|d|e|efg|fg)(?:(a|bcdef|g|ab|c|d|e|efg|fg)(?:(a|bcdef|g|ab|c|d|e|efg|fg)(a|bcdef|g|ab|c|d|e|efg|fg)*)?)?", { "abcdefg", "bcdefg" });

    // t8w("(abc)+?a"_rx, { "abcabc", "abcabca" });
    // t8w("<!--.*?-->"_rx, { "<!-- Hello -->", "<!-- Hello --> -->" });

    // t4("a{2,5}?", {});
    t4("a{2,5}?", { "a", "aa", "aaa", "aaaa", "aaaaa" });

    return 0;
}