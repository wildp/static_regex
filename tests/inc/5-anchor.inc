// -*- C++ -*- tests for simple assertions

/* sof+eof anchor tests */
static_assert(match("a$"_test, "a"));
static_assert(match("^a"_test, "a"));
static_assert(match("^a$"_test, "a"));
static_assert(match("^$"_test, ""));
static_assert(prefix_match("a$"_test, "a"));
static_assert(prefix_match("^a"_test, "a"));
static_assert(prefix_match("^a$"_test, "a"));
static_assert(prefix_match("^$"_test, ""));
static_assert(not search("^$"_test, "a"));
static_assert(search("^$"_test, "", { 0, 0 }));
static_assert(search("$^"_test, "", { 0, 0 }));
static_assert(search("a$"_test, "a", { 0, 1 }));
static_assert(search("^a"_test, "a", { 0, 1 }));
static_assert(search("^a$"_test, "a", { 0, 1 }));
static_assert(prefix_match("(a)+?$$"_test, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?$$"_test, "aa", { 0, 2, 1, 2 }));
static_assert(prefix_match("(a)+?$"_test, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?$"_test, "aa", { 0, 2, 1, 2 }));
static_assert(prefix_match("(a)+?"_test, "a", { 0, 1, 0, 1 }));
static_assert(prefix_match("(a)+?"_test, "aa", { 0, 1, 0, 1 }));
static_assert(not search("^^ab"_test, "bab"));
static_assert(search("^^ab"_test, "aba", { 0, 2 }));
static_assert(not search("^ab"_test, "bab"));
static_assert(search("^ab"_test, "aba", { 0, 2 }));
static_assert(search("ab"_test, "bab", { 1, 3 }));
static_assert(search("ab"_test, "aba", { 0, 2 }));
static_assert(search("(abc)|(^abc)"_test, "abc", { 0, 3, 0, 3, no_tag, no_tag }));

/* sol+eol anchor tests */
static_assert(prefix_match("(?m:a$)"_test, "a", { 0, 1 }));
static_assert(prefix_match("(?m:a$)"_test, "a\na", { 0, 1 }));
static_assert(prefix_match("(?m:a$\na)"_test, "a\na", { 0, 3 }));
static_assert(prefix_match("(?m:a$\na)"_test, "a\na", { 0, 3 }));
static_assert(prefix_match("(?m)($\na)+"_test, "\na", { 0, 2, 0, 2 }));
static_assert(prefix_match("(?m)($\na)+"_test, "\na\na", { 0, 4, 2, 4 }));
static_assert(prefix_match("(?m)(\n$)+"_test, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(prefix_match("(?m)(a$)"_test, "a\na", { 0, 1, 0, 1 }));
static_assert(search("(?m:^a)"_test, "a", { 0, 1 }));
static_assert(search("(?m:^a)"_test, "\na", { 1, 2 }));
static_assert(search("(?ms)(^a.?)*"_test, "a\na", { 0, 3, 2, 3 }));
static_assert(search("(?m:^\n*$)"_test, "\n\n\n\n", { 0, 4 }));
static_assert(search("(?m:^\n*?$)"_test, "\n\n\n\n", { 0, 0 }));
static_assert(prefix_match("(?m)(\n$)+"_test, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)(^\n$)+"_test, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)($\n^)+"_test, "\n\n\n\n", { 0, 4, 3, 4 }));
static_assert(search("(?m)(^a\n)+"_test, "a\na\na", { 0, 4, 2, 4 }));
static_assert(search_all("(?m)^a"_test, "a\na\na", { { 0, 1 }, { 2, 3 }, { 4, 5 } }));

/* word boundary tests */
static_assert(not search(R"(\b)"_test, ""));
static_assert(search(R"(\B)"_test, "", { 0, 0 }));
static_assert(search_all(R"(\b)"_test, "ab+-cd", { { 0, 0 }, { 2, 2 }, { 4, 4 }, { 6, 6 } }));
static_assert(search_all(R"(\B)"_test, "ab<>cd", { { 1, 1 }, { 3, 3 }, { 5, 5 } }));
static_assert(search(R"(\B)"_test, "a_", { 1, 1 }));
static_assert(search(R"(a\b)"_test, "a+", { 0, 1 }));
static_assert(search(R"(a\b)"_test, "a.", { 0, 1 }));
static_assert(not search(R"(a\b)"_test, "a_"));
static_assert(search_all(R"(\b|abc)"_test, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
static_assert(search_all(R"(\b|^abc)"_test, "abc", { { 0, 0 }, { 0, 3 }, { 3, 3 } }));
