#include <rx/etc/partition.hpp>


namespace
{
    using test_t = std::vector<std::pair<char, char>>;

    consteval test_t sv_to_test(std::string_view sv)
    {
        test_t result;
        for (std::size_t i{ 0 }; i + 1 < sv.size(); i += 2)
            result.emplace_back(sv.at(i), sv.at(i + 1));
        return result;
    }

    consteval bool test(const char* input, const char* target)
    {
        auto input_vec{ sv_to_test(input) };
        const auto target_vec{ sv_to_test(target) };
        rx::detail::partition(input_vec);
        return std::ranges::equal(input_vec, target_vec);
    }
}

static_assert(test("acac", "ac"));
static_assert(test("acde", "acde"));
static_assert(test("aeac", "acde"));
static_assert(test("adaf", "adef"));
static_assert(test("accd", "abccdd"));
static_assert(test("adbc", "aabcdd"));
static_assert(test("adbd", "aabd"));

static_assert(test("acacxz", "acxz"));
static_assert(test("acdexz", "acdexz"));
static_assert(test("aeacxz", "acdexz"));
static_assert(test("accdxz", "abccddxz"));
static_assert(test("adbcxz", "aabcddxz"));
static_assert(test("adbdxz", "aabdxz"));

static_assert(test("agadeg", "adeg"));
static_assert(test("egagad", "adeg"));
static_assert(test("egadag", "adeg"));

static_assert(test("acafaz", "acdfgz"));
static_assert(test("afacaz", "acdfgz"));
static_assert(test("adafdj", "acddefgj"));
static_assert(test("adafdjxz", "acddefgjxz"));
static_assert(test("afdjad", "acddefgj"));
static_assert(test("afacazxz", "acdfgwxz"));
static_assert(test("adbcbdxz", "aabcddxz"));

static_assert(test("apadep", "adep"));
static_assert(test("apaden", "adenop"));
static_assert(test("adagef", "adefgg"));
static_assert(test("adagez", "adeghz"));