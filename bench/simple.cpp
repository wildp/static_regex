#include <benchmark/benchmark.h>
#include <ctre.hpp>
#include <rx/regex.hpp>


template<typename Matcher>
static void BM_ctre_match(benchmark::State& state, Matcher m, const std::string_view input)
{
   for (auto _ : state)
   {
      auto result{ m.match(input) };
      benchmark::DoNotOptimize(result);
   }
}

template<typename Matcher>
static void BM_rx_match(benchmark::State& state, Matcher m, const std::string_view input)
{
   for (auto _ : state)
   {
      auto result{ m.match(input) };
      benchmark::DoNotOptimize(result);
   }
}

using namespace rx::literals;
using namespace ctre::literals;

#define TEST(NAME, PATTERN, INPUT)                             \
BENCHMARK_CAPTURE(BM_rx_match, NAME, PATTERN ## _rx, INPUT); \
BENCHMARK_CAPTURE(BM_ctre_match, NAME, PATTERN ## _ctre, INPUT);

TEST(email, R"([a-zA-Z0-9._%+\x2D]+@[a-zA-Z0-9.\x2D]+\.[a-zA-Z]{2,})", "john.doe@example.com");
TEST(ipv4, R"((?:(?:25[0-5]|(?:2[0-4]|1\d|[1-9]|)\d)\.?\b){4})", "127.0.0.1");
TEST(ipv6, R"((?:[a-f0-9:]+:+)+[a-f0-9]*)", "2001:0db8:85a3:0000:0000:8a2e:0370:7334");
TEST(url, R"((?:[a-z\-]+)?:?(?:\/\/.*\.[^\/]+))", "https://example.com/path/resource.txt");

BENCHMARK_MAIN();