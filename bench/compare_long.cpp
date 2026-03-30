#include <benchmark/benchmark.h>
#include <ctre.hpp>
#include <rx/regex.hpp>
#include <regex>
#include <re2/re2.h>
#include <boost/regex.hpp>
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>


static const char8_t data[] = {
#embed "pg3200.txt"
, '\0' /* null terminator */
};

static const std::string_view input{ reinterpret_cast<const char*>(data) };


namespace
{
    template<typename Matcher>
    std::size_t matchcount_ctre(Matcher re)
    {
        auto view = re.multiline_range(input);
        return std::ranges::distance(view.begin(), view.end());
    }

    template<typename Matcher>
    std::size_t matchcount_rx(Matcher re)
    {
        auto view = re.range(input);
        return std::ranges::distance(view.begin(), view.end());
    }

    std::size_t matchcount_std(const std::string_view pattern, auto flags)
    {
        std::basic_regex re{ std::string{ pattern }, flags };
        std::cregex_iterator it{ input.begin(), input.end(), re };
        std::cregex_iterator end{};
        return std::ranges::distance(it, end);
    }

    std::size_t matchcount_boost(const std::string_view pattern)
    {
        boost::basic_regex re{ std::string{ pattern } };
        boost::cregex_iterator it{ input.begin(), input.end(), re, boost::regex_constants::match_not_dot_newline };
        boost::cregex_iterator end{};
        return std::ranges::distance(it, end);
    }

    std::size_t matchcount_re2(const std::string_view pattern)
    {
        re2::RE2 re(pattern);
        std::string_view sv(input);
        std::size_t count{ 0 };
        while (RE2::FindAndConsume(&sv, re))
            ++count;

        return count;
    }

    std::size_t matchcount_pcre2(const std::string_view pattern)
    {
        std::size_t count{ 0 };
        int err_num{};
        PCRE2_SIZE err_off{};

        auto* re = pcre2_compile(reinterpret_cast<PCRE2_SPTR>(pattern.data()), pattern.size(), 0, &err_num, &err_off, nullptr);

        if (not re)
            return -1;

        auto* md = pcre2_match_data_create_from_pattern(re, nullptr);

        PCRE2_SIZE offset{ 0 };

        while ((pcre2_match(re, reinterpret_cast<PCRE2_SPTR>(input.data()), input.size(), offset, 0, md, nullptr)) >= 0)
        {
            ++count;
            auto* ovec = pcre2_get_ovector_pointer(md);
            offset = ovec[1];
        }

        pcre2_match_data_free(md);
        pcre2_code_free(re);

        return count;
    }

    std::size_t matchcount_pcre2jit(const std::string_view pattern)
    {
        std::size_t count{ 0 };
        int err_num{};
        PCRE2_SIZE err_off{};

        auto* re = pcre2_compile(reinterpret_cast<PCRE2_SPTR>(pattern.data()), pattern.size(), 0, &err_num, &err_off, nullptr);

        if (not re)
            return -1;

        if (pcre2_jit_compile(re, PCRE2_JIT_COMPLETE) != 0)
            return -1;

        auto* md = pcre2_match_data_create_from_pattern(re, nullptr);

        PCRE2_SIZE offset{ 0 };

        while ((pcre2_jit_match(re, reinterpret_cast<PCRE2_SPTR>(input.data()), input.size(), offset, 0, md, nullptr)) >= 0)
        {
            ++count;
            auto* ovec = pcre2_get_ovector_pointer(md);
            offset = ovec[1];
        }

        pcre2_match_data_free(md);
        pcre2_code_free(re);

        return count;
    }
}


template<typename Matcher>
static void BM_ctre(benchmark::State& state, Matcher re, const std::size_t count)
{
    if (auto tmp = matchcount_ctre(re); tmp != count)
        return;

    for (auto _ : state)
    {
        auto range = re.multiline_range(input);
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

template<typename Matcher>
static void BM_rx(benchmark::State& state, Matcher re, const std::size_t count)
{
    if (matchcount_rx(re) != count)
        return;

    for (auto _ : state)
    {
        auto range = re.range(input);
        for (auto result : range)
            benchmark::DoNotOptimize(result);
    }
}

static void BM_std(benchmark::State& state, std::string_view pattern, const std::size_t count)
{
    auto flags = std::regex_constants::ECMAScript;

    if (pattern.starts_with("(?i)"))
    {
        pattern.remove_prefix(4);
        flags |= std::regex_constants::icase;
    }

    if (matchcount_std(pattern, flags) != count)
        return;

    for (auto _ : state)
    {
        std::basic_regex re{ std::string{ pattern }, flags };
        std::cregex_iterator it{ input.begin(), input.end(), re };
        for (const std::cregex_iterator end{}; it != end; ++it)
        {
            auto result = *it;
            benchmark::DoNotOptimize(result);
        }
    }
}

static void BM_boost(benchmark::State& state, std::string_view pattern, const std::size_t count)
{
    if (matchcount_boost(pattern) != count)
        return;

    for (auto _ : state)
    {
        boost::basic_regex re{ std::string{ pattern } };
        boost::cregex_iterator it{ input.begin(), input.end(), re, boost::regex_constants::match_not_dot_newline };
        for (const boost::cregex_iterator end{}; it != end; ++it)
        {
            auto result = *it;
            benchmark::DoNotOptimize(result);
        }
    }
}

static void BM_re2(benchmark::State& state, const std::string_view pattern, const std::size_t count)
{
    if (auto tmp = matchcount_re2(pattern); tmp != count)
        return;

    for (auto _ : state)
    {
        re2::RE2 re(pattern);
        std::string_view result;
        std::string_view sv(input);

        while (RE2::FindAndConsume(&sv, re, &result))
        {
            std::string_view tmp(result);
            benchmark::DoNotOptimize(tmp);
        }
    }
}

static void BM_pcre2(benchmark::State& state, const std::string_view pattern, const std::size_t count)
{
    if (matchcount_pcre2(pattern) != count)
        return;

    for (auto _ : state)
    {
        int err_num{};
        PCRE2_SIZE err_off{};

        auto* re = pcre2_compile(reinterpret_cast<PCRE2_SPTR>(pattern.data()), pattern.size(), 0, &err_num, &err_off, nullptr);

        if (not re)
            return;

        auto* md = pcre2_match_data_create_from_pattern(re, nullptr);

        PCRE2_SIZE offset{ 0 };
        while ((pcre2_match(re, reinterpret_cast<PCRE2_SPTR>(input.data()), input.size(), offset, 0, md, nullptr)) >= 0)
        {
            auto* ovec = pcre2_get_ovector_pointer(md);
            offset = ovec[1];

            benchmark::DoNotOptimize(ovec[0]);
            benchmark::DoNotOptimize(ovec[1]);
        }

        pcre2_match_data_free(md);
        pcre2_code_free(re);
    }
}

static void BM_pcre2jit(benchmark::State& state, const std::string_view pattern, const std::size_t count)
{
    if (matchcount_pcre2jit(pattern) != count)
        return;

    for (auto _ : state)
    {
        int err_num;
        PCRE2_SIZE err_off;

        auto* re = pcre2_compile(reinterpret_cast<PCRE2_SPTR>(pattern.data()), pattern.size(), 0, &err_num, &err_off, nullptr);

        if (not re)
            return;

        if (pcre2_jit_compile(re, PCRE2_JIT_COMPLETE) != 0)
            return;

        auto* md = pcre2_match_data_create_from_pattern(re, nullptr);

        PCRE2_SIZE offset{ 0 };
        while ((pcre2_jit_match(re, reinterpret_cast<PCRE2_SPTR>(input.data()), input.size(), offset, 0, md, nullptr)) >= 0)
        {
            auto* ovec = pcre2_get_ovector_pointer(md);
            offset = ovec[1];

            benchmark::DoNotOptimize(ovec[0]);
            benchmark::DoNotOptimize(ovec[1]);
        }

        pcre2_match_data_free(md);
        pcre2_code_free(re);
    }
}

using namespace ctre::literals;
using namespace rx::literals;

#define TEST(PATTERN, COUNT)                                 \
BENCHMARK_CAPTURE(BM_rx, PATTERN, PATTERN ## _rx, COUNT);    \
BENCHMARK_CAPTURE(BM_ctre, PATTERN, PATTERN ## _ctre, COUNT);\
BENCHMARK_CAPTURE(BM_re2, PATTERN, PATTERN, COUNT);          \
BENCHMARK_CAPTURE(BM_boost, PATTERN, PATTERN, COUNT);        \
BENCHMARK_CAPTURE(BM_pcre2, PATTERN, PATTERN, COUNT);        \
BENCHMARK_CAPTURE(BM_pcre2jit, PATTERN, PATTERN, COUNT);     \
BENCHMARK_CAPTURE(BM_std, PATTERN, PATTERN, COUNT);

TEST(R"(Twain)", 771);
TEST(R"((?i)Twain)", 925);
TEST(R"([a-z]shing)", 1540);
TEST(R"(Huck[a-zA-Z]+|Saw[a-zA-Z]+)", 259);
TEST(R"(\b\w+nn\b)", 261);
TEST(R"([a-q][^u-z]{13}x)", 4093);
TEST(R"(Tom|Sawyer|Huckleberry|Finn)", 2592);
TEST(R"((?i)(?:Tom|Sawyer|Huckleberry|Finn))", 4146);
TEST(R"(.{0,2}(?:Tom|Sawyer|Huckleberry|Finn))", 2592);
TEST(R"(.{2,4}(?:Tom|Sawyer|Huckleberry|Finn))", 1973);
TEST(R"(Tom.{10,25}river|river.{10,25}Tom)", 2);
TEST(R"([a-zA-Z]+ing)", 78422);
TEST(R"(\s[a-zA-Z]{0,12}ing\s)", 55248);
TEST(R"(([A-Za-z]awyer|[A-Za-z]inn)\s)", 209);
TEST(R"(["'][^"']{0,30}[?!\.][\"'])", 8886);

BENCHMARK_MAIN();