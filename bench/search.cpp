#ifndef SRX_BENCH_SEARCH_MODE
#error "SRX_BENCH_SEARCH_MODE is not defined"
#endif

#include <benchmark/benchmark.h>
#include <ctre.hpp>
#include <srx/regex.hpp>

#if SRX_BENCH_SEARCH_MODE == 2
#include <deque>
#elif SRX_BENCH_SEARCH_MODE == 3
#include <list>
#endif

static const char8_t data[] = {
#embed "pg3200.txt"
, '\0' /* null terminator */
};

#if SRX_BENCH_SEARCH_MODE == 0
static const std::string_view input{ reinterpret_cast<const char*>(data) };
#elif SRX_BENCH_SEARCH_MODE == 1
static const char* input{ reinterpret_cast<const char*>(data) };
#elif SRX_BENCH_SEARCH_MODE == 2
static const std::deque input(std::from_range, std::ranges::subrange(reinterpret_cast<const char*>(data), srx::detail::cstr_sentinel));
#elif SRX_BENCH_SEARCH_MODE == 3
static const std::list input(std::from_range, std::ranges::subrange(reinterpret_cast<const char*>(data), srx::detail::cstr_sentinel));
#endif

template<typename Matcher>
static void BM_ctre(benchmark::State& state, Matcher m)
{
    for (auto _ : state)
    {
        auto range = m.multiline_range(input);
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

template<typename Matcher>
static void BM_srx(benchmark::State& state, Matcher m)
{
    for (auto _ : state)
    {
        auto range = m.range(input);
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

using namespace ctre::literals;
using namespace srx::literals;

template<srx::string_literal Pattern>
consteval srx::static_regex<Pattern, srx::mode::linear> operator ""_srxl() { return {}; }

template<srx::string_literal Pattern>
consteval srx::static_regex<Pattern, srx::mode::table_dfa> operator ""_srxt() { return {}; }

template<srx::string_literal Pattern>
consteval srx::static_regex<Pattern, srx::mode::backtrack> operator ""_srxb() { return {}; }

#define TEST(PATTERN)                                   \
BENCHMARK_CAPTURE(BM_srx, q/PATTERN, PATTERN ## _srx);  \
BENCHMARK_CAPTURE(BM_srx, l/PATTERN, PATTERN ## _srxl); \
BENCHMARK_CAPTURE(BM_srx, t/PATTERN, PATTERN ## _srxt); \
BENCHMARK_CAPTURE(BM_srx, b/PATTERN, PATTERN ## _srxb); \
BENCHMARK_CAPTURE(BM_ctre, /PATTERN, PATTERN ## _ctre);

#define TEST_NO_FAST(PATTERN)                           \
BENCHMARK_CAPTURE(BM_srx, q/PATTERN, PATTERN ## _srx);  \
BENCHMARK_CAPTURE(BM_srx, b/PATTERN, PATTERN ## _srxb); \
BENCHMARK_CAPTURE(BM_ctre, /PATTERN, PATTERN ## _ctre);

TEST(R"(Twain)");
TEST(R"((?i)Twain)");
TEST(R"([a-z]shing)");
TEST(R"(Huck[a-zA-Z]+|Saw[a-zA-Z]+)");
TEST(R"(\b\w+nn\b)");
TEST_NO_FAST(R"([a-q][^u-z]{13}x)");
TEST(R"(Tom|Sawyer|Huckleberry|Finn)");
TEST(R"((?i)(?:Tom|Sawyer|Huckleberry|Finn))");
TEST(R"(.{0,2}(?:Tom|Sawyer|Huckleberry|Finn))");
TEST(R"(.{2,4}(?:Tom|Sawyer|Huckleberry|Finn))");
TEST_NO_FAST(R"(Tom.{10,25}river|river.{10,25}Tom)");
TEST(R"([a-zA-Z]+ing)");
TEST(R"(\s[a-zA-Z]{0,12}ing\s)");
TEST(R"((?:[A-Za-z]awyer|[A-Za-z]inn)\s)");
TEST(R"(["'][^"']{0,30}[?!\.][\"'])");

BENCHMARK_MAIN();