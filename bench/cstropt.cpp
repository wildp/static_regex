#include <benchmark/benchmark.h>
#include <rx/regex.hpp>


static const char8_t data[] = {
#embed "pg3200.txt"
, '\0' /* null terminator */
};

static const std::string_view input{ reinterpret_cast<const char*>(data) };

struct non_optimised_cstr_sentinel_t
{
    template<typename CharT>
    friend constexpr bool operator==(const CharT* c, non_optimised_cstr_sentinel_t)
    {
        return *c == CharT{};
    }
};

inline constexpr non_optimised_cstr_sentinel_t non_optimised_cstr_sentinel;

static void BM_cstring_opt(benchmark::State& state, auto m)
{
    const char* cstr{ input.data() };
    for (auto _ : state)
    {
        auto range = m.range(cstr);
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

static void BM_cstring_std(benchmark::State& state, auto m)
{
    const char* cstr{ input.data() };
    for (auto _ : state)
    {
        auto range = m.range(cstr, non_optimised_cstr_sentinel);
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

static void BM_string_view(benchmark::State& state, auto m)
{
    for (auto _ : state)
    {
        auto range = m.range(input);
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

using namespace rx::literals;

#define TEST(PATTERN)                                \
BENCHMARK_CAPTURE(BM_cstring_opt, PATTERN, PATTERN); \
BENCHMARK_CAPTURE(BM_cstring_std, PATTERN, PATTERN); \
BENCHMARK_CAPTURE(BM_string_view, PATTERN, PATTERN);

TEST(R"(Twain)"_rx);
TEST(R"((?i)Twain)"_rx);
TEST(R"([a-z]shing)"_rx);
TEST(R"(Huck[a-zA-Z]+|Saw[a-zA-Z]+)"_rx);
TEST(R"(\b\w+nn\b)"_rx);
TEST(R"([a-q][^u-z]{13}x)"_rx);
TEST(R"(Tom|Sawyer|Huckleberry|Finn)"_rx);
TEST(R"((?i)(?:Tom|Sawyer|Huckleberry|Finn))"_rx);
TEST(R"(.{0,2}(?:Tom|Sawyer|Huckleberry|Finn))"_rx);
TEST(R"(.{2,4}(?:Tom|Sawyer|Huckleberry|Finn))"_rx);
TEST(R"(Tom.{10,25}river|river.{10,25}Tom)"_rx);
TEST(R"([a-zA-Z]+ing)"_rx);
TEST(R"(\s[a-zA-Z]{0,12}ing\s)"_rx);
TEST(R"(([A-Za-z]awyer|[A-Za-z]inn)\s)"_rx);
TEST(R"(["'][^"']{0,30}[?!\.][\"'])"_rx);

BENCHMARK_MAIN();