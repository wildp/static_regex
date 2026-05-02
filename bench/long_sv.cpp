#include <benchmark/benchmark.h>
#include <ctre.hpp>
#include <srx/regex.hpp>


static const char8_t data[] = {
#embed "pg3200.txt"
, '\0' /* null terminator */
};

static const std::string_view input{ reinterpret_cast<const char*>(data) };

template<typename Matcher>
static void BM_ctre(benchmark::State& state, Matcher m)
{
    for (auto _ : state)
    {
        auto range = m.range(input);
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

template<srx::string_literal Pattern>
static void BM_srx(benchmark::State& state, srx::static_regex<Pattern> m)
{
    for (auto _ : state)
    {
        auto range = m.range(input);
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

template<srx::string_literal Pattern>
static void BM_srxf(benchmark::State& state, srx::static_regex<Pattern, srx::mode::linear> m)
{
    for (auto _ : state)
    {
        auto range = m.range(input);
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

template<srx::string_literal Pattern>
static void BM_srxn(benchmark::State& state, srx::static_regex<Pattern, srx::mode::backtrack> m)
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
consteval srx::static_regex<Pattern, srx::mode::linear> operator ""_srxf() { return {}; }

template<srx::string_literal Pattern>
consteval srx::static_regex<Pattern, srx::mode::backtrack> operator ""_srxn() { return {}; }

#define TEST(PATTERN)                                  \
BENCHMARK_CAPTURE(BM_srx, PATTERN, PATTERN ## _srx);     \
BENCHMARK_CAPTURE(BM_srxf, PATTERN, PATTERN ## _srxf);   \
BENCHMARK_CAPTURE(BM_srxn, PATTERN, PATTERN ## _srxn);   \
BENCHMARK_CAPTURE(BM_ctre, PATTERN, PATTERN ## _ctre);

#define TEST_NO_FAST(PATTERN)                          \
BENCHMARK_CAPTURE(BM_srx, PATTERN, PATTERN ## _srx);     \
BENCHMARK_CAPTURE(BM_srxn, PATTERN, PATTERN ## _srxn);   \
BENCHMARK_CAPTURE(BM_ctre, PATTERN, PATTERN ## _ctre);

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