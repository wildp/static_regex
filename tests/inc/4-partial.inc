// -*- C++ -*- tests for fallback and search

/* fallback tests */
static_assert(prefix_match("abc"_test, "abcdef", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_test, "abc", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_test, "abcdef", { 0, 6 }));
static_assert(prefix_match("abc|abcdef"_test, "abcdef", { 0, 3 }));
static_assert(prefix_match("abcdef|abc"_test, "abcde", { 0, 3 }));

/* lazy prefix matching tests */
static_assert(prefix_match("(abc)+"_test, "abcabc", { 0, 6, 3, 6 }));
static_assert(prefix_match("(abc)+?"_test, "abcabc", { 0, 3, 0, 3 }));
static_assert(prefix_match("(abc)+?a"_test, "abcabc", { 0, 4, 0, 3 }));
static_assert(prefix_match("(abc)+?a"_test, "abcabca", { 0, 4, 0, 3 }));

/* additional capture location tests */
static_assert(match("(ab+c)+?(ab+c|.*d)"_test, "abcabbcacd", { 0, 3, 3, 10 }));
static_assert(prefix_match("(ab+c)+?(ab+c|.*d)"_test, "abcabbcacd", { 0, 7, 0, 3, 3, 7 }));
static_assert(prefix_match("(ab+c)+(ab+c|.*d)"_test, "abcabbcacd", { 0, 10, 3, 7, 7, 10 }));
static_assert(search("([ad]b+c)+?([ad])"_test, "aaabacabcdbbcacd", { 6, 10, 6, 9, 9, 10 }));
static_assert(search("([ad]b+c)+([ad])"_test, "aaabacabcdbbcacd", { 6, 14, 9, 13, 13, 14 }));

/* search tests */
static_assert(search("a"_test, "abcd", { 0, 1 }));
static_assert(search("b"_test, "abcd", { 1, 2 }));
static_assert(search("d"_test, "abcd", { 3, 4 }));
static_assert(not search("e"_test, "abcd"));
static_assert(search("aa"_test, "abaab", { 2, 4 }));
static_assert(search("ab"_test, "abaab", { 0, 2 }));
static_assert(search("bc"_test, "abcd", { 1, 3 }));
static_assert(search_all("a"_test, "a", { { 0, 1 } }));
static_assert(search_all("a"_test, "aaa", { { 0, 1 }, { 1, 2 }, { 2, 3 } }));
static_assert(search_all("ab"_test, "abab", { { 0, 2 }, { 2, 4 } }));
static_assert(search_all("ab"_test, "abaab", { { 0, 2 }, { 3, 5 } }));
static_assert(search_all("(?:)|abc"_test, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
static_assert(search_all("(?:)|abc"_test, "abcabc", { { 0, 0 }, { 0, 3 }, { 3, 3 }, { 3, 6 }, { 6, 6 } }));
// TODO: add more tests

/* additional search tests (for simd epilogues: also run at run-time) */
static_assert(search_all("aa"_test, "aabbaa", { { 0, 2 }, { 4, 6 } }));
static_assert(search_all("aa"_test, "aabbaab", { { 0, 2 }, { 4, 6 } }));
static_assert(search_all("aa"_test, "aabbaabb", { { 0, 2 }, { 4, 6 } }));
