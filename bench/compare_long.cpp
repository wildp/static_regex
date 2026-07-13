#include <benchmark/benchmark.h>
#include <ctre.hpp>
#include <srx/regex.hpp>
#include <regex>
#include <re2/re2.h>
#include <boost/regex.hpp>
#include <boost/xpressive/xpressive_static.hpp>
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>


static const char8_t data[] = {
#embed "pg3200.txt"
, '\0' /* null terminator */
};

static const std::string_view input{ reinterpret_cast<const char*>(data) };


namespace {

template<srx::string_literal Pattern>
struct xpr_pattern {};

template<srx::string_literal Pattern>
consteval xpr_pattern<Pattern> operator ""_xpr() { return {}; }

template<typename Matcher>
std::size_t matchcount_ctre(Matcher re)
{
    auto view = re.multiline_range(input);
    return std::ranges::distance(view.begin(), view.end());
}

template<typename Matcher>
std::size_t matchcount_srx(Matcher re)
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

template<srx::string_literal Pattern>
std::size_t matchcount_xpr(const xpr_pattern<Pattern>)
{
    auto re = [] -> boost::xpressive::cregex {
        using namespace boost::xpressive;
        if constexpr (Pattern.view() == R"(Twain)")
            return as_xpr("Twain");
        else if constexpr (Pattern.view() == R"((?i)Twain)")
            return icase("Twain");
        else if constexpr (Pattern.view() == R"([a-z]shing)")
            return range('a', 'z') >> "shing";
        else if constexpr (Pattern.view() == R"(Huck[a-zA-Z]+|Saw[a-zA-Z]+)")
            return ("Huck" >> +set[range('a', 'z') | range('A', 'Z')]) | ("Saw" >> +set[range('a', 'z') | range('A', 'Z')]);
        else if constexpr (Pattern.view() == R"(\b\w+nn\b)")
            return _b >> +_w >> "nn" >> _b;
        else if constexpr (Pattern.view() == R"([a-q][^u-z]{13}x)")
            return range('a', 'q') >> repeat<13, 13>(~range('u', 'z')) >> "x";
        else if constexpr (Pattern.view() == R"(Tom|Sawyer|Huckleberry|Finn)")
            return as_xpr("Tom") | "Sawyer" | "Huckleberry" | "Finn";
        else if constexpr (Pattern.view() == R"((?i)(?:Tom|Sawyer|Huckleberry|Finn))")
            return icase(as_xpr("Tom") | "Sawyer" | "Huckleberry" | "Finn");
        else if constexpr (Pattern.view() == R"(.{0,2}(?:Tom|Sawyer|Huckleberry|Finn))")
            return repeat<0, 2>(~(set = '\n')) >> (as_xpr("Tom")| "Sawyer" | "Huckleberry" | "Finn");
        else if constexpr (Pattern.view() == R"(.{2,4}(?:Tom|Sawyer|Huckleberry|Finn))")
            return repeat<2, 4>(~(set = '\n')) >> (as_xpr("Tom") | "Sawyer" | "Huckleberry" | "Finn");
        else if constexpr (Pattern.view() == R"(Tom.{10,25}river|river.{10,25}Tom)")
            return ("Tom" >> repeat<10, 25>(~(set = '\n')) >> "river") | ("river" >> repeat<10, 25>(~(set = '\n')) >> "Tom");
        else if constexpr (Pattern.view() == R"([a-zA-Z]+ing)")
            return +set[range('a', 'z') | range('A', 'Z')] >> "ing";
        else if constexpr (Pattern.view() == R"(\s[a-zA-Z]{0,12}ing\s)")
            return _s >> repeat<0, 12>(set[range('a', 'z') | range('A', 'Z')]) >> "ing" >> _s;
        else if constexpr (Pattern.view() == R"((?:[A-Za-z]awyer|[A-Za-z]inn)\s)")
            return ((set[range('A', 'Z') | range('a', 'z')] >> "awyer") | (set[range('A', 'Z') | range('a', 'z')] >> "inn")) >> _s;
        else if constexpr (Pattern.view() == R"(["'][^"']{0,30}[?!\.][\"'])")
            return (set = '\"', '\'') >> repeat<0,30>(~(set = '\"', '\'')) >> (set= '?','!','.') >> (set = '\"', '\'');
        else
            static_assert(false, "Unknown Pattern");
    }();

    boost::xpressive::cregex_iterator it{ input.begin(), input.end(), re };
    boost::xpressive::cregex_iterator end{};
    return std::ranges::distance(it, end);
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

} // namespace


template<typename Matcher>
static void BM_ctre(benchmark::State& state, Matcher re, const std::size_t count)
{
    if (matchcount_ctre(re) != count)
        return;

    for (auto _ : state)
    {
        auto tmp = matchcount_ctre(re);
        benchmark::DoNotOptimize(tmp);
    }
}

template<typename Matcher>
static void BM_srx(benchmark::State& state, Matcher re, const std::size_t count)
{
    if (matchcount_srx(re) != count)
        return;

    for (auto _ : state)
    {
        auto tmp = matchcount_srx(re);
        benchmark::DoNotOptimize(tmp);
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
        auto tmp = matchcount_std(pattern, flags);
        benchmark::DoNotOptimize(tmp);
    }
}

static void BM_boost(benchmark::State& state, std::string_view pattern, const std::size_t count)
{
    if (matchcount_boost(pattern) != count)
        return;

    for (auto _ : state)
    {
        auto tmp = matchcount_boost(pattern);
        benchmark::DoNotOptimize(tmp);
    }
}

static void BM_re2(benchmark::State& state, const std::string_view pattern, const std::size_t count)
{
    if (auto tmp = matchcount_re2(pattern); tmp != count)
        return;

    for (auto _ : state)
    {
        auto tmp = matchcount_re2(pattern);
        benchmark::DoNotOptimize(tmp);
    }
}

template<typename Matcher>
static void BM_xpr(benchmark::State& state, Matcher re, const std::size_t count)
{
    if (matchcount_xpr(re) != count)
        return;

    for (auto _ : state)
    {
        auto tmp = matchcount_xpr(re);
        benchmark::DoNotOptimize(tmp);
    }
}

static void BM_pcre2(benchmark::State& state, const std::string_view pattern, const std::size_t count)
{
    if (matchcount_pcre2(pattern) != count)
        return;

    for (auto _ : state)
    {
        auto tmp = matchcount_pcre2(pattern);
        benchmark::DoNotOptimize(tmp);
    }
}

static void BM_pcre2jit(benchmark::State& state, const std::string_view pattern, const std::size_t count)
{
    if (matchcount_pcre2jit(pattern) != count)
        return;

    for (auto _ : state)
    {
        auto tmp = matchcount_pcre2jit(pattern);
        benchmark::DoNotOptimize(tmp);
    }
}

using namespace ctre::literals;
using namespace srx::literals;

#define TEST(PATTERN, COUNT)                                 \
BENCHMARK_CAPTURE(BM_srx, PATTERN, PATTERN ## _srx, COUNT);    \
BENCHMARK_CAPTURE(BM_ctre, PATTERN, PATTERN ## _ctre, COUNT);\
BENCHMARK_CAPTURE(BM_re2, PATTERN, PATTERN, COUNT);          \
BENCHMARK_CAPTURE(BM_xpr, PATTERN, PATTERN ## _xpr, COUNT);  \
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
TEST(R"((?:[A-Za-z]awyer|[A-Za-z]inn)\s)", 209);
TEST(R"(["'][^"']{0,30}[?!\.][\"'])", 8886);

BENCHMARK_MAIN();