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
        rx::detail::partition_v1(input_vec);
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


namespace
{
    using partition_entry_t = rx::detail::partition_entry<char, int>;

    struct pe : partition_entry_t
    {
        template<typename... Ints>
        constexpr pe(std::string_view sv, Ints... values) : partition_entry_t(sv.at(0), sv.at(1))
        {
            this->data.assign({ values... });
        }
    };

    consteval std::vector<partition_entry_t> make_entries(std::vector<pe>& input)
    {
        std::vector<partition_entry_t> result;
        result.reserve(input.size());
        for (auto&& i: input)
            result.emplace_back(static_cast<partition_entry_t&&>(std::move(i)));
        return result;
    }

    consteval bool test2(std::vector<pe> input, std::vector<pe> target)
    {
        auto input_vec{ make_entries(input) };
        const auto target_vec{ make_entries(target) };
        rx::detail::partition_v2(input_vec);
        return std::ranges::equal(input_vec, target_vec);
    }
}

static_assert(test2({{"aa",0},{"aa",1}}, {{"aa",0,1}}));
static_assert(test2({{"aa",1},{"aa",0}}, {{"aa",1,0}}));
static_assert(test2({{"aa",0},{"aa",0}}, {{"aa",0,0}}));
static_assert(test2({{"aa",0},{"aa",1},{"aa",0}}, {{"aa",0,1,0}}));
static_assert(test2({{"aa",0,2},{"aa",1,3}}, {{"aa",0,2,1,3}}));

static_assert(test2({{"ac",0},{"ac",1}}, {{"ac",0,1}}));
static_assert(test2({{"ac",0},{"de",1}}, {{"ac",0},{"de",1}}));
// static_assert(test2({{"ae",0},{"ac",1}}, {{"ac",0,1},{"de",0}})); // fails
static_assert(test2({{"ad",0},{"af",1}}, {{"ad",0,1},{"ef",1}}));
static_assert(test2({{"ac",0},{"cd",1}}, {{"ab",0},{"cc",0,1},{"dd",1}}));
static_assert(test2({{"ad",0},{"bc",1}}, {{"aa",0},{"bc",0,1},{"dd",0}}));
static_assert(test2({{"ad",0},{"bd",1}}, {{"aa",0},{"bd",0,1}}));

static_assert(test2({{"ac",0},{"ac",1},{"xz",2}}, {{"ac",0,1},{"xz",2}}));
static_assert(test2({{"ac",0},{"de",1},{"xz",2}}, {{"ac",0},{"de",1},{"xz",2}}));
// static_assert(test2({{"ae",0},{"ac",1},{"xz",2}}, {{"ac",0,1},{"de",0},{"xz",2}})); // fails
static_assert(test2({{"ac",0},{"cd",1},{"xz",2}}, {{"ab",0},{"cc",0,1},{"dd",1},{"xz",2}}));
static_assert(test2({{"ad",0},{"bc",1},{"xz",2}}, {{"aa",0},{"bc",0,1},{"dd",0},{"xz",2}}));
static_assert(test2({{"ad",0},{"bd",1},{"xz",2}}, {{"aa",0},{"bd",0,1},{"xz",2}}));

// static_assert(test2({{"ag",0},{"ad",1},{"eg",2}}, {{"ad",0,1},{"eg",0,2}})); // fails
// static_assert(test2({{"eg",0},{"ag",1},{"ad",2}}, {{"ad",1,2},{"eg",0,1}})); // fails
// static_assert(test2({{"eg",0},{"ad",1},{"ag",2}}, {{"ad",1,2},{"eg",0,2}})); // fails

static_assert(test2({{"ac",0},{"af",1},{"az",2}}, {{"ac",0,1,2},{"df",1,2},{"gz",2}}));
// static_assert(test2({{"af",0},{"ac",1},{"az",2}}, {{"ac",0,1,2},{"df",0,2},{"gz",2}})); // fails
static_assert(test2({{"ad",0},{"af",1},{"dj",2}}, {{"ac",0,1},{"dd",0,1,2},{"ef",1,2},{"gj",2}}));
static_assert(test2({{"ad",0},{"af",1},{"dj",2},{"xz",3}}, {{"ac",0,1},{"dd",0,1,2},{"ef",1,2},{"gj",2},{"xz",3}}));
// static_assert(test2({{"af",0},{"dj",1},{"ad",2}}, {{"ac",0,2},{"dd",0,1,2},{"ef",0,1},{"gj",1}})); // fails
// static_assert(test2({{"af",0},{"ac",1},{"az",2},{"xz",3}}, {{"ac",0,1,2},{"df",0,2},{"gw",2},{"xz",2,3}})); // fails
// static_assert(test2({{"ad",0},{"bc",1},{"bd",2},{"xz",3}}, {{"aa",0},{"bc",0,1,2},{"dd",0,2},{"xz",3}})); // fails

// static_assert(test2({{"ap",0},{"ad",1},{"ep",2}}, {{"ad",0,1},{"ep",0,2}})); // fails
// static_assert(test2({{"ap",0},{"ad",1},{"en",2}}, {{"ad",0,1},{"en",0,2},{"op",0}})); // fails
// static_assert(test2({{"ad",0},{"ag",1},{"ef",2}}, {{"ad",0,1},{"ef",1,2},{"gg",1}})); // fails
static_assert(test2({{"ad",0},{"ag",1},{"ez",2}}, {{"ad",0,1},{"eg",1,2},{"hz",2}}));