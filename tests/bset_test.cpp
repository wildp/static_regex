#include <rx/etc/bitcharset.hpp>


namespace 
{
    consteval auto make_bs(std::string_view sv)
    {
        rx::detail::bitcharset<char> result;
        for (std::size_t i{ 0 }; i + 1 < sv.size(); i += 2)
            result.insert(sv[i], sv[i + 1]);
        return result;
    }

    consteval auto make_inverted_bs(std::string_view sv)
    {
        rx::detail::bitcharset<char> result;
        char tmp{ std::numeric_limits<char>::min() };
        for (std::size_t i{ 0 }; i + 1 < sv.size(); i += 2)
        {
            result.insert(tmp, sv[i]);
            tmp = sv[i + 1];
        }
        result.insert(tmp, std::numeric_limits<char>::max());
        return result;
    }

    consteval auto make_pairvec(std::string_view sv)
    {
        std::vector<rx::detail::bitcharset<char>::char_interval> result;
        for (std::size_t i{ 0 }; i + 1 < sv.size(); i += 2)
            result.emplace_back(sv[i], sv[i + 1]);
        return result;
    }

    consteval bool test_ident(const char* arg, const char* result)
    {
        return make_bs(arg) == make_bs(result);
    }

    consteval bool test_compl_ext(const char* arg, const char* result)
    {
        return (~make_bs(arg)) == make_bs(result);
    }

    consteval bool test_compl(const char* arg, const char* result)
    {
        return (~make_bs(arg)) == make_inverted_bs(result);
    }

    consteval bool test_union(const char* lhs, const char* rhs, const char* result)
    {
        return (make_bs(lhs) | make_bs(rhs)) == make_bs(result);
    }

    consteval bool test_inter(const char* lhs, const char* rhs, const char* result)
    {
        return (make_bs(lhs) & make_bs(rhs)) == make_bs(result);
    }

    consteval bool test_symdif(const char* lhs, const char* rhs, const char* result)
    {
        return (make_bs(lhs) ^ make_bs(rhs)) == make_bs(result);
    }

    consteval bool test_relcomp(const char* lhs, const char* rhs, const char* result)
    {
        return (make_bs(lhs) - make_bs(rhs)) == make_bs(result);
    }

    consteval bool test_intervals(const char* arg, const char* result)
    {
        return (make_bs(arg).get_intervals() == make_pairvec(result));
    }

    consteval bool test_caseless(const char* arg, const char* result)
    {
        auto bs{ make_bs(arg) };
        bs.make_ascii_case_insensitive();
        return (bs == make_bs(result));
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

/* bitcharset-specific tests (conversion to intervals) */
static_assert(test_intervals("ac", "ac"));
static_assert(test_intervals("0a9z", "0z"));
static_assert(test_intervals("accd", "ad"));
static_assert(test_intervals("acef", "acef"));
static_assert(test_intervals("0Aaz", "0Aaz"));

/* bitcharset-specific tests (case insensitivity) */
static_assert(test_caseless("09", "09"));
static_assert(test_caseless("ac", "ACac"));
static_assert(test_caseless("AC", "ACac"));
static_assert(test_caseless("acXZ", "ACXZacxz"));
static_assert(test_caseless("09ac", "09ACac"));