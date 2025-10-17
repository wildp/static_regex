#include <print>
#include <meta>

#include <rx/regex.hpp>

#include "tests/headers/printable_tree.hpp"
#include "tests/headers/printable_tdfa.hpp"
#include "tests/headers/tdfa_matcher.hpp"
#include "tests/headers/tnfa_matcher.hpp"
#include "tests/headers/tree_matcher.hpp"


namespace
{
    using namespace rx::testing;
    namespace dff = rx::detail::default_fsm_flags;

    struct tester_info
    {
        int stage{ 0 };[[maybe_unused]] 
        rx::detail::parser_flags parse{};
        rx::detail::fsm_flags fsm{ dff::full_match };
        bool dbgok : 1 { false };
        bool dbghc : 1 { false };
        bool otags : 1 { false };
        bool ptree : 1 { true };
        bool ptags : 1 { false };
        bool ptdfa : 1 { false };
        bool pdfao : 1 { false };
        bool pdfam : 1 { false };
    };

    template<typename M>
    [[maybe_unused]] void print_match_results(const M& m, const std::vector<std::string_view>& tests)
    {
        for (auto sv : tests)
        {
            auto result{ m.match(sv) };

            if (result.has_value())
                std::println("'{}': Match: {}", sv, *result);
            else
                std::println("'{}': No Match", sv);
        }
    }

    [[maybe_unused]] void print_capture_info(const rx::detail::capture_info& ci)
    {
        for (std::size_t i{ 0 }; i < ci.capture_count(); ++i)
            for (const auto& elem: ci.lookup(i))
                std::println("{}: {}+{}, {}+{}", i, elem.first.tag_number, elem.first.offset, elem.second.tag_number, elem.second.offset);
    }


    template<tester_info Flags = {}>
    struct tester
    {
        using test_type = void;

        using tree_base = std::conditional_t<(Flags.stage == 0), tree_matcher<char>, rx::detail::expr_tree<char>>;
        using tnfa_base = std::conditional_t<(Flags.stage == 1), tnfa_matcher<char>, rx::detail::tagged_nfa<char>>;
        using tdfa_base = std::conditional_t<(Flags.stage >= 2), tdfa_matcher<char>, rx::detail::tagged_dfa<char>>;
        using tree_type = std::conditional_t<(Flags.ptree), printable<tree_base>, tree_base>;
        using tnfa_type = tnfa_base;
        using tdfa_type = std::conditional_t<(Flags.ptdfa or Flags.pdfao or Flags.pdfam), printable<tdfa_base>, tdfa_base>;

        void operator()(std::string_view pat, const std::vector<std::string_view>& test) const
        {
            namespace rdt = rx::detail::tdfa;

            tree_type tree{ pat, Flags.parse };
            
            if constexpr (Flags.fsm.is_search) tree.insert_search_prefix();

            if constexpr (Flags.ptree) std::println("Pattern: {}", tree.to_pattern());

            if constexpr (Flags.ptags) print_capture_info(tree.get_capture_info());

            if constexpr (Flags.otags)
            {
                tree.optimise_tags();
                if constexpr (Flags.ptags) print_capture_info(tree.get_capture_info());
            }

            if constexpr (Flags.stage >= 1)
            {
                tnfa_type tnfa{ tree, Flags.fsm };

                if constexpr (Flags.stage >= 2)
                {
                    tdfa_type tdfa{ tnfa };

                    if constexpr (Flags.dbgok) std::println("Pattern ok\n"); 
                    if constexpr (Flags.ptdfa) { tdfa.print(); std::println("\n"); } 

                    if constexpr (Flags.stage >= 3)
                    {
                        tdfa.optimise_registers();

                        if constexpr (Flags.dbgok) std::println("Opt ok\n");
                        if constexpr (Flags.pdfao) { tdfa.print(); std::println("\n"); } 

                        if constexpr (Flags.stage == 4)
                        {
                            rdt::min<char>::compact_regop_blocks(tdfa);

                            if constexpr (Flags.dbgok) std::println("Regcompact ok\n");
                            if constexpr (Flags.pdfam) { tdfa.print(); std::println("\n"); }

                            if constexpr (Flags.dbghc) { std::println("{}", rdt::min<char>::dry_run(tdfa)); }
                        }
                        else if constexpr (Flags.stage >= 5)
                        {
                            tdfa.minimise_states();
                            
                            if constexpr (Flags.dbgok) std::println("Min ok\n");
                            if constexpr (Flags.pdfam) { tdfa.print(); std::println("\n"); }
                        }
                    }

                    print_match_results(tdfa, test);
                }
                else
                {
                    print_match_results(tnfa, test);
                }
            }
            else
            {
                print_match_results(tree, test);
            }
        }
    };

    [[maybe_unused]] constexpr auto t0 = tester<{ .stage=1 }>{};
    [[maybe_unused]] constexpr auto t1 = tester<{ .stage=0, .parse{ .enable_possessive=true, .enable_backrefs=true, .enable_branchreset=true } }>{};
    [[maybe_unused]] constexpr auto t2 = tester<{ .stage=2 }>{};
    [[maybe_unused]] constexpr auto t3 = tester<{ .stage=2, .dbgok=true, .otags=true, .ptags=true, .ptdfa=true }>{};
    [[maybe_unused]] constexpr auto t4 = tester<{ .stage=3, .dbgok=true, .otags=true, .pdfao=true }>{};
    [[maybe_unused]] constexpr auto t5 = tester<{ .stage=3, .fsm=dff::search_single, .dbgok=true, .otags=true, .ptdfa=true, .pdfao=true }>{};
    [[maybe_unused]] constexpr auto t6 = tester<{ .stage=4, .fsm=dff::partial_match, .dbgok=true, .otags=true, .ptdfa=true, .pdfam=true }>{};
    [[maybe_unused]] constexpr auto t7 = tester<{ .stage=4, .dbghc=true, .otags = true, .pdfam=true }>{};
    [[maybe_unused]] constexpr auto t8 = tester<{ .stage=5, .dbgok=true, .otags = true, .pdfao=true, .pdfam=true }>{};
    [[maybe_unused]] constexpr auto t9 = tester<{ .stage=5, .fsm=dff::partial_match, .dbgok =true, .otags=true, .pdfao=true, .pdfam=true }>{};

    template<rx::detail::string_literal S>
    [[maybe_unused]] void match(rx::compile_time_regex<S> m, const std::vector<std::string_view>& test)
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
    [[maybe_unused]] void starts_with(rx::compile_time_regex<S> m, const std::vector<std::string_view>& test)
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
    [[maybe_unused]] void search(rx::compile_time_regex<S> m, const std::vector<std::string_view>& test)
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

    // t0("()", { "", "a" });
    // t0("(a)", { "", "a" });
    // t0("a(aa)b|(aa)ac", { "aaab", "aaac" });

    // t1("a(aa)b|(aa)ac", { "aaab", "aaac" });
    // t1("(a*b)+", { "aaababaaaab" });
    // t1("a(bb|b)b", { "abb", "abbb" });
    // t1("(?|(a)|(b)(c))(d)\\3", { "abb", "abbb" });

    // t3("a(aa)b|(aa)ac", { "aaab", "aaac" });
    // t3("(a)*(a|b)b*", {});
    // t3("(a)*(b)*", {});
    // t3("(a|b)*b", {});
    // t3("(a)*", { "", "a", "aa" });
    // t3("(ab)*", {});
    // t3("(a)", { "a" });

    // t3("(a)+(b)*", {"ab", "abb", "aaabbb" });
    // t3("(a)+b*|c*", { "ab", "aab" });

    // t4("(?:()a())*()(?:a|()b)()b*", { "ab" });

    // t5("Wor(ld)+", { "Hello World", "WWWorldld" });

    // t5("(abc)*", {});
    // t5("(abc)*|(bcd)+", {});
    // t5("(abc|g.)+", {});

    // t5(".abc", {});

    // match("(?:()a())*()(?:a|()b)()b*"_rx, { "a", "b", "ab", "aaa", "aaabb", "abba", "bbaa" });
    // match("(?i)a(aa)b|(aa)ac"_rx, { "aAaB", "aAaC" } );
    // search("Wor(ld)"_rx, { "World World World", "Hello World", "World" });
    // match("a(bc)d"_rx, { "abcd" });

    // match("(ab+c)+?(ab+c)?"_rx, { "abcabbc" });
    // match("(ab+c)+(ab+c)?"_rx, { "abcabbc" });

    // search("(ab+c)+?(ab+c)?"_rx, { "abcabbc" });
    // search("(ab+c)+(ab+c)?"_rx, { "abcabbc" });
    
    // starts_with("(ab+c)+?(ab+c|.*d)"_rx, { "abcabbcacd__" });
    // starts_with("(ab+c)+(ab+c|.*d)"_rx, { "abcabbcacd__" });

    // search("(ab+c)+?(ab+c)?"_rx, { "aaabacabcabbcac" });
    // search("(ab+c)+(ab+c)?"_rx, { "aaabacabcabbcac" });

    // auto w3c_email_regex = R"([[:alnum:].+/=?^_-]+@[[:alnum:]](?:[[:alnum:]-]{0,61}[[:alnum:]])?(?:\.[[:alnum:]](?:[[:alnum:]-]{0,61}[[:alnum:]])?)+)"_rx;
    // auto simple_email_regex = R"([\w\-\.]+@([\w-]+\.)+[\w-]{2,})"_rx;
    // match(w3c_email_regex, { "Hello@example.com", "test@example@example", "@example.example" });
    // match(simple_email_regex, { "Hello@example.com", "test@example@example", "@example.example" });
    // search(w3c_email_regex, { "Hello@example.com", "test@example@example", "@example.example" }); /* RIP compile times */

    // t2("[ac]est|best", { "best", "cest" });

    // t1("(a)+a?", { "aa" });
    // t2("(ab)+?(?:ab)?", { "abab" });
    // t2("(a)+a?", { "aa" });

    // t1("(ab+c)+?(ab+c|.*d)", { "abcabbcacd" });
    // t2("(?s)(ab+c)+?(ab+c|.*d)", { "abcabbcacd" });

    // t0("(a|bcdef|g|ab|c|d|e|efg|fg)*", { "abcdefg" });
    // t1("(a|bcdef|g|ab|c|d|e|efg|fg)*", { "abcdefg" });
    // t2("(a|bcdef|g|ab|c|d|e|efg|fg)*", { "abcdefg" });
    // t4("(a|bcdef|g|ab|c|d|e|efg|fg)(?:(a|bcdef|g|ab|c|d|e|efg|fg)(?:(a|bcdef|g|ab|c|d|e|efg|fg)(a|bcdef|g|ab|c|d|e|efg|fg)*)?)?", { "abcdefg", "bcdefg" });

    // starts_with("(abc)+?a"_rx, { "abcabc", "abcabca" });
    // starts_with("<!--.*?-->"_rx, { "<!-- Hello -->", "<!-- Hello --> -->" });

    // t2("a{2,5}?", {});
    // t2("a{2,5}?", { "a", "aa", "aaa", "aaaa", "aaaaa" });

    // t5("abB|AB", { "aaabB", "bbbbbABBB" });

    // t7("abB|AB", {});
    // t8("abB|AB", {});

    return 0;
}