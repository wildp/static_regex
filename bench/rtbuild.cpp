#include <benchmark/benchmark.h>
#include <rx/regex.hpp>


static void test(benchmark::State& state, const std::string_view pattern)
{
    using namespace rx::detail;

    for (auto _ : state)
    {
        expr_tree ast{ pattern };
        ast.insert_search_prefix();
        ast.optimise_tags();

        tagged_nfa nfa{ ast, rx::detail::default_fsm_flags::search_all };
        nfa.rewrite_assertions();

        tagged_dfa dfa{ nfa };
        dfa.optimise_registers();
        // dfa.minimise_states();
        dfa.minimise_transition_edges();

        benchmark::DoNotOptimize(dfa);
    }
}


#define TEST(PATTERN) BENCHMARK_CAPTURE(test, PATTERN, PATTERN);


TEST(R"(Twain)");
TEST(R"((?i)Twain)");
TEST(R"([a-z]shing)");
TEST(R"(Huck[a-zA-Z]+|Saw[a-zA-Z]+)");
TEST(R"(\b\w+nn\b)");
// TEST(R"([a-q][^u-z]{13}x)");
TEST(R"(Tom|Sawyer|Huckleberry|Finn)");
TEST(R"((?i)Tom|Sawyer|Huckleberry|Finn)");
TEST(R"(.{0,2}(Tom|Sawyer|Huckleberry|Finn))");
TEST(R"(.{2,4}(Tom|Sawyer|Huckleberry|Finn))");
// TEST(R"(Tom.{10,25}river|river.{10,25}Tom)");
TEST(R"([a-zA-Z]+ing)");
TEST(R"(\s[a-zA-Z]{0,12}ing\s)");
TEST(R"(([A-Za-z]awyer|[A-Za-z]inn)\s)");
TEST(R"(["'][^"']{0,30}[?!\.][\"'])");


BENCHMARK_MAIN();