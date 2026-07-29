#include <benchmark/benchmark.h>
#include <srx/regex.hpp>

static const char8_t data[] = {
#embed "../bench/pg3200.txt"
, '\0' /* null terminator */
};

static const std::string_view input{ reinterpret_cast<const char*>(data) };

int main()
{
    using namespace srx::detail;

    constexpr int iterations{ 100 };
    // const auto re = srx::static_regex<R"(.{2,4}(Tom|Sawyer|Huckleberry|Finn))", srx::mode::standard>{};
    const auto re = srx::static_regex<R"(.{2,4}(Tom|Sawyer|Huckleberry|Finn))", srx::mode::linear>{};

    for (int i{ 0 }; i < iterations; ++i)
    {
        auto view = re.range(input);
        auto tmp = std::ranges::distance(view.begin(), view.end());
        benchmark::DoNotOptimize(tmp);
    }
}
