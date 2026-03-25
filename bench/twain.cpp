#include <benchmark/benchmark.h>
#include <ctre.hpp>
#include <rx/regex.hpp>


static const char8_t data[] = {
#embed "pg3200.txt"
, '\0' /* null terminator */
};

static const std::string_view input{ reinterpret_cast<const char*>(data) };

template<typename Matcher>
static void BM_ctre(benchmark::State& state, Matcher m)
{
    const char* cstr{ input.data() };
    for (auto _ : state)
    {
        auto range = m.range(cstr);
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

template<rx::string_literal Pattern>
static void BM_rx(benchmark::State& state, rx::static_regex<Pattern> m)
{
    const char* cstr{ input.data() };
    for (auto _ : state)
    {
        auto range = m.range(cstr);
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

template<rx::string_literal Pattern>
static void BM_rxf(benchmark::State& state, rx::static_regex<Pattern, rx::mode::fast> m)
{
    const char* cstr{ input.data() };
    for (auto _ : state)
    {
        auto range = m.range(cstr);
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

template<rx::string_literal Pattern>
static void BM_rxn(benchmark::State& state, rx::static_regex<Pattern, rx::mode::naive> m)
{
    const char* cstr{ input.data() };
    for (auto _ : state)
    {
        auto range = m.range(cstr);
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

using namespace ctre::literals;
using namespace rx::literals;

#define TEST(PATTERN)                                  \
BENCHMARK_CAPTURE(BM_rx, PATTERN, PATTERN ## _rx);     \
BENCHMARK_CAPTURE(BM_rxf, PATTERN, PATTERN ## _rxf);   \
BENCHMARK_CAPTURE(BM_rxn, PATTERN, PATTERN ## _rxn);   \
BENCHMARK_CAPTURE(BM_ctre, PATTERN, PATTERN ## _ctre);

#define TEST_NO_FAST(PATTERN)                          \
BENCHMARK_CAPTURE(BM_rx, PATTERN, PATTERN ## _rx);     \
BENCHMARK_CAPTURE(BM_rxn, PATTERN, PATTERN ## _rxn);   \
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
TEST(R"(([A-Za-z]awyer|[A-Za-z]inn)\s)");
TEST(R"(["'][^"']{0,30}[?!\.][\"'])");

BENCHMARK_MAIN();