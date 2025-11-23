#include <rx/etc/charset.hpp>


namespace 
{
    consteval auto make_cs(std::string_view sv)
    {
        rx::detail::charset<char> result;
        for (std::size_t i{ 0 }; i + 1 < sv.size(); i += 2)
            result.insert(sv[i], sv[i + 1]);
        return result;
    }

    consteval auto make_inverted_cs(std::string_view sv)
    {
        rx::detail::charset<char> result;
        char tmp{ std::numeric_limits<char>::min() };
        for (std::size_t i{ 0 }; i + 1 < sv.size(); i += 2)
        {
            result.insert(tmp, sv[i]);
            tmp = sv[i + 1];
        }
        result.insert(tmp, std::numeric_limits<char>::max());
        return result;
    }

    consteval auto make_cs_vec(const std::vector<const char*>& vec)
    {
        std::vector<rx::detail::charset<char>> result;
        result.reserve(vec.size());
        for (const char* cstr : vec)
            result.emplace_back(make_cs(cstr));
        return result;
    }


    consteval bool test_ident(const char* arg, const char* result)
    {
        return make_cs(arg) == make_cs(result);
    }

    consteval bool test_compl_ext(const char* arg, const char* result)
    {
        return (~make_cs(arg)) == make_cs(result);
    }

    consteval bool test_compl(const char* arg, const char* result)
    {
        return (~make_cs(arg)) == make_inverted_cs(result);
    }

    consteval bool test_union(const char* lhs, const char* rhs, const char* result)
    {
        return (make_cs(lhs) | make_cs(rhs)) == make_cs(result);
    }

    consteval bool test_inter(const char* lhs, const char* rhs, const char* result)
    {
        return (make_cs(lhs) & make_cs(rhs)) == make_cs(result);
    }

    consteval bool test_symdif(const char* lhs, const char* rhs, const char* result)
    {
        return (make_cs(lhs) ^ make_cs(rhs)) == make_cs(result);
    }

    consteval bool test_relcomp(const char* lhs, const char* rhs, const char* result)
    {
        return (make_cs(lhs) - make_cs(rhs)) == make_cs(result);
    }

    consteval bool contains(const char* arg, char c)
    {
        return make_cs(arg).contains(c);
    }

    consteval bool test_part(const std::vector<const char*>& arg, const std::vector<const char*>& result)
    {
        const auto input{ make_cs_vec(arg) };
        std::vector refs{ std::from_range, input | std::views::transform([](const auto& b){ return std::cref(b); }) };
        return rx::detail::charset<char>::partition(refs) == make_cs_vec(result);
    }
}

/* insertion tests */
static_assert(test_ident("", ""));
static_assert(test_ident("abac", "ac"));
static_assert(test_ident("acab", "ac"));
static_assert(test_ident("bcab", "ac"));
static_assert(test_ident("bcac", "ac"));
static_assert(test_ident("acac", "ac"));
static_assert(test_ident("adce", "ae"));
static_assert(test_ident("abcd", "ad"));
static_assert(test_ident("abde", "abde"));
static_assert(test_ident("deab", "abde"));

/* complement tests (implementation defined) */
static_assert(test_compl_ext("", "\x80\x7f"));
static_assert(test_compl_ext("\x80\x7f", ""));

/* complement tests */
static_assert(test_compl("", ""));
static_assert(test_compl("be", "af"));
static_assert(test_compl("bcfg", "adeh"));
static_assert(test_compl("bcef", "addg"));
static_assert(test_compl("begi", "affj"));

/* union tests */
static_assert(test_union("", "", ""));
static_assert(test_union("aa", "aa", "aa"));
static_assert(test_union("aa", "bb", "ab"));
static_assert(test_union("ab", "bc", "ac"));
static_assert(test_union("aa", "cc", "aacc"));
static_assert(test_union("ab", "dg", "abdg"));
static_assert(test_union("abde", "bc", "ae"));

/* intersection tests */
static_assert(test_inter("", "", ""));
static_assert(test_inter("aa", "aa", "aa"));
static_assert(test_inter("aa", "bb", ""));
static_assert(test_inter("ab", "bc", "bb"));
static_assert(test_inter("bc", "ab", "bb"));
static_assert(test_inter("ae", "cg", "ce"));
static_assert(test_inter("aadd", "aadd", "aadd"));
static_assert(test_inter("aacd", "aadd", "aadd"));
static_assert(test_inter("aceg", "cegl", "cceegg"));
static_assert(test_inter("abde", "bd", "bbdd"));

/* symmetric difference tests */
static_assert(test_symdif("", "", ""));
static_assert(test_symdif("aa", "aa", ""));
static_assert(test_symdif("ac", "", "ac"));
static_assert(test_symdif("", "ac", "ac"));
static_assert(test_symdif("ab", "bc", "aacc"));
static_assert(test_symdif("aa", "bb", "ab"));
static_assert(test_symdif("af", "bbdd", "aaccef"));
static_assert(test_symdif("aceg", "ce", "abddfg"));
static_assert(test_symdif("abde", "ab", "de"));

/* relative complement tests */
static_assert(test_relcomp("", "", ""));
static_assert(test_relcomp("aa", "aa", ""));
static_assert(test_relcomp("ac", "", "ac"));
static_assert(test_relcomp("", "ac", ""));
static_assert(test_relcomp("ac", "bc", "aa"));
static_assert(test_relcomp("bc", "ac", ""));

/* contains tests */
static_assert(contains("bb", 'b'));
static_assert(not contains("bb", 'a'));
static_assert(not contains("bb", 'c'));
static_assert(contains("Azaz", 'z'));
static_assert(contains("Azaz", 'A'));
static_assert(contains("Azaz", 'f'));
static_assert(contains("ac", 'c'));
static_assert(contains("acef", 'c'));
static_assert(not contains("ac", 'd'));
static_assert(not contains("acef", 'd'));

/* partition tests */
static_assert(test_part({}, {}));
static_assert(test_part({"ac"}, {"ac"}));
static_assert(test_part({"ac","ac"}, {"ac"}));
static_assert(test_part({"ac","ae"}, {"de","ac"}));
static_assert(test_part({"ae","ac"}, {"de","ac"}));
static_assert(test_part({"ae","bc"}, {"aade","bc"}));
static_assert(test_part({"bc","ae"}, {"aade","bc"}));
static_assert(test_part({"aj","bdfg"}, {"aaeehj","bdfg"}));
static_assert(test_part({"aj","em"}, {"ad","km", "ej"}));
static_assert(test_part({"aj","em"}, {"ad","km", "ej"}));
static_assert(test_part({"em","aj"}, {"km","ad", "ej"}));
static_assert(test_part({"adgh","dfhj"}, {"acgg","efij", "ddhh"}));
static_assert(test_part({"adgh","dfhj", "dh"}, {"ac","ij", "gg", "ef", "ddhh"}));