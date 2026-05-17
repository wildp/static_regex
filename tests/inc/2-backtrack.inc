// -*- C++ -*- backtracking-specific tests

/* backtracking tests */
static_assert(match("a(b|bb)b"_test, "abb"));
static_assert(match("a(b|bb)b"_test, "abbb"));
static_assert(match("a(bb|b)b"_test, "abb"));
static_assert(match("a(bb|b)b"_test, "abbb"));
static_assert(match("(a*b)+"_test, "aaababaaaab"));
static_assert(match("(a+){4}"_test, "aaaaaaaaaaaa"));
static_assert(match("(aa|aaa)*a"_test, "aaaaaa"));

/* atomic matching tests */
static_assert(match("(a)++"_test, "aaaa", { 3, 4 }));
static_assert(match("(a)++[ab]"_test, "aaab", { 2, 3 }));
static_assert(not match("(a)++[ab]"_test, "aaaa"));
static_assert(match("(?:(a)++[ab])|(a*)"_test, "aaab", { 2, 3, no_tag, no_tag }));
static_assert(match("(?:(a)++[ab])|(a*)"_test, "aaaa", { no_tag, no_tag, 0, 4 }));
static_assert(match("a{1,2}+"_test, "a"));
static_assert(match("a{1,2}+"_test, "aa"));
static_assert(not match("a{1,2}+"_test, "aaa"));
static_assert(not match("a{1,2}+"_test, "aaa"));
static_assert(match("a{1,2}+[ab]"_test, "ab"));
static_assert(not match("a{1,2}+[ab]"_test, "aa"));


/* backreference tests */
static_assert(match("(abc)\\1"_test, "abcabc"));
static_assert(match("(a|b|c)\\g1"_test, "bb"));
static_assert(match("(a|\1){2}"_test, "aa"));
static_assert(match("(a)(b)\\g2\\g1"_test, "abba"));
static_assert(match("(ab(cd))\\1\\2"_test, "abcdabcdcd"));
static_assert(not match("(ab(cd))\\1\\2"_test, "abcdcdabcd"));
static_assert(match("((ab|e\\3)(c*)d){2}"_test, "abccdeccd"));
static_assert(match("((a)(b)\\3\\2){2}"_test, "abbaabba"));
static_assert(not match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\10"_test, "abcdefghijkj"));
static_assert(not match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g10"_test, "abcdefghijkj"));
static_assert(match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g10"_test, "abcdefghijka0"));
static_assert(match("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)\\g{10}"_test, "abcdefghijkj"));
static_assert(match("(.*)\\1"_test, "abcdeabcde"));
static_assert(not match("\\1(abc)\\1"_test, "abc"));
static_assert(match(".?|(..+?)\\1+"_test, "a"));
static_assert(not match(".?|(..+?)\\1+"_test, "aa"));
static_assert(not match(".?|(..+?)\\1+"_test, "aaa"));
static_assert(match(".?|(..+?)\\1+"_test, "aaaa"));
static_assert(not match(".?|(..+?)\\1+"_test, "aaaaa"));
static_assert(match(".?|(..+?)\\1+"_test, "aaaaaaaaaa"));
static_assert(not match(".?|(..+?)\\1+"_test, "aaaaaaaaaaa"));

#if DISABLE_OVERLAPPING_BACKREF_TESTS == 0
/* overlapping backreference tests */
static_assert(match("(abc|a\\1c)+"_test, "abc"));
static_assert(match("(abc|a\\1c)+"_test, "abcaabcc"));
static_assert(match("(abc|a\\1c)+"_test, "abcaabccaaabccc"));
#endif