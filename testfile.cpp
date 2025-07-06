#include <print>
#include <meta>

#include <rx/regex.hpp>
#include "tests/headers/printable_tree.hpp"
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

//     template for (constexpr auto mem: std::define_static_array(std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked())))
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
    [[maybe_unused]] void t1()
    {
        using namespace rx::literals;
        auto matcher = "Hello World"_rx;
        std::println("{}", matcher.test());
    }

    [[maybe_unused]] void t2(std::string_view pat, const std::vector<std::string_view>& test)
    {
        rx::testing::printable_expr_tree<char> tmp{ pat };
        std::println("Pattern: {}", tmp.to_pattern());

        // const auto& ci{ tmp.get_capture_info() };
        // const auto cc{ ci.capture_count() };
        // for (std::size_t i{ 0 }; i < cc; ++i)
        // {
        //     auto [beg, end]{ ci.lookup(i) };
        //     for (const auto& e: std::vector(beg, end))
        //         std::println("{}: {}+{}, {}+{}", i, e.first.tag_number, e.first.offset, e.second.tag_number, e.second.offset);
        // }

        rx::testing::tnfa_matcher<char> tnfa{ tmp };

        for (auto sv : test)
        {
            auto result{ tnfa.submatches(sv) };

            if (result.has_value())
                std::println("'{}': Match: {}", sv, *result);
            else
                std::println("'{}': No Match", sv);
        }
    }

    [[maybe_unused]] void t3(std::string_view pat, const std::vector<std::string_view>& test)
    {
        rx::detail::parser_flags f{ .enable_possessive=true, .enable_backrefs=true, .enable_branchreset=true };
        rx::testing::tree_matcher<char> tree{ pat, f };

        rx::testing::printable_expr_tree<char> tmp{ pat, f };
        std::println("Pattern: {}", tmp.to_pattern());

        for (auto sv : test)
        {
            auto result{ tree.submatches(sv) };

            if (result.has_value())
                std::println("'{}': Match: {}", sv, *result);
            else
                std::println("'{}': No Match", sv);
        }
    }

    [[maybe_unused]] void t4(std::string_view pat, const std::vector<std::string_view>& test)
    {
        rx::testing::printable_expr_tree<char> tmp{ pat };
        std::println("Pattern: {}", tmp.to_pattern());
        
        rx::testing::tnfa_matcher<char> tnfa{ tmp };
        rx::testing::tdfa_matcher<char> tdfa{ tnfa };

        for (auto sv : test)
        {
            auto result{ tdfa.submatches(sv) };

            if (result.has_value())
                std::println("'{}': Match: {}", sv, *result);
            else
                std::println("'{}': No Match", sv);
        }
    }

    [[maybe_unused]] void t5(std::string_view pat, const std::vector<std::string_view>& test)
    {
        rx::testing::printable_expr_tree<char> tmp{ pat };
        std::println("Pattern: {}", tmp.to_pattern());

        const auto& ci{ tmp.get_capture_info() };
        for (std::size_t i{ 0 }; i < ci.capture_count(); ++i)
        {
            const auto [beg, end]{ ci.lookup(i) };
            for (auto it{ beg }; it != end; ++it)
                std::println("{}: {}+{}, {}+{}", i, it->first.tag_number, it->first.offset, it->second.tag_number, it->second.offset);
        }

        tmp.optimise_tags();

        // const auto& ci{ tmp.get_capture_info() };
        for (std::size_t i{ 0 }; i < ci.capture_count(); ++i)
        {
            const auto [beg, end]{ ci.lookup(i) };
            for (auto it{ beg }; it != end; ++it)
                std::println("{}: {}+{}, {}+{}", i, it->first.tag_number, it->first.offset, it->second.tag_number, it->second.offset);
        }

        rx::testing::tnfa_matcher<char> tnfa{ tmp };
        rx::testing::tdfa_matcher<char> tdfa{ tnfa };
        std::println("Pattern ok\n");

        tdfa.dump();
        std::println("\n");

        for (auto sv : test)
        {
            auto result{ tdfa.submatches(sv) };

            if (result.has_value())
                std::println("'{}': Match: {}", sv, *result);
            else
                std::println("'{}': No Match", sv);
        }
    }
}


int main()
{
 
    using namespace rx::detail;

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
    // t5("(a)*", {});
    // t5("(ab)*", {});
    // t5("(a)", { "a" });

    // t5("(a)+(b)*", {"ab", "abb", "aaabbb" });
    // t5("(a)+b*|c*", { "ab", "aab" });

    t5("(?:()a())*()(?:a|()b)()b*", { "ab" });


    return 0;
}