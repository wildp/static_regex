#include <benchmark/benchmark.h>
#include <rx/regex.hpp>


int main()
{
    using namespace rx::detail;

    constexpr int iterations{ 10 };
    const std::string_view pattern{ R"(.{2,4}(Tom|Sawyer|Huckleberry|Finn))" };

    for (int i{ 0 }; i < iterations; ++i)
    {
        expr_tree ast{ pattern };
        ast.insert_search_prefix();
        ast.optimise_tags();

        tagged_nfa nfa{ ast, rx::detail::default_fsm_flags::search_all };
        nfa.rewrite_assertions();

        tagged_dfa dfa{ nfa };
        dfa.optimise_registers();
        dfa.minimise_states();
        dfa.minimise_transition_edges();

        benchmark::DoNotOptimize(dfa);
    }
}
