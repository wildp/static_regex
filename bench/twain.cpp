#include <benchmark/benchmark.h>
#include <ctre.hpp>
#include <rx/regex.hpp>


static const char8_t data[] = {
#embed "pg3200.txt"
, '\0' /* null terminator */
};

template<typename Matcher>
static void BM_ctre(benchmark::State& state, Matcher m)
{
    const auto input{ std::ranges::subrange(data, rx::detail::cstr_sentinel) };
    for (auto _ : state)
    {
        auto range{ m.range(input) };
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

template<typename Matcher>
static void BM_rx(benchmark::State& state, Matcher m)
{
    const auto input{ std::ranges::subrange(data, rx::detail::cstr_sentinel) };
    for (auto _ : state)
    {
        auto range{ input | rx::views::regex_match(m) };
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

using namespace ctre::literals;
using namespace rx::literals;

#define TEST(PATTERN)                                \
BENCHMARK_CAPTURE(BM_rx, PATTERN, PATTERN ## _rx);   \
BENCHMARK_CAPTURE(BM_ctre, PATTERN, PATTERN ## _ctre);

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