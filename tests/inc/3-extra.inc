// -*- C++ -*- extra feature tests for matching

/* capture features */
static_assert(match("(a)"_test, "a"));
static_assert(match("(a)(b)"_test, "ab"));
static_assert(match("(a(b))"_test, "ab"));
static_assert(match("(?:a)"_test, "a"));
static_assert(match("(?#comment)"_test, ""));
static_assert(match("(?:c)(a)(b)\\2"_test, "cabb"));
static_assert(not match("(?:c)(a)(b)\\2"_test, "caba"));
static_assert(match("(?|(a)|(b))\\1"_test, "aa"));
static_assert(match("(?|(a)|(b))\\1"_test, "bb"));
static_assert(match("(?|(a)|(b))(c)\\1\\2"_test, "acac"));
static_assert(match("(?|(a)|(b))(c)\\1\\2"_test, "bcbc"));
static_assert(match("(?|(a)|(b)(c))(d)\\3"_test, "add"));
static_assert(match("(?|(a)|(b)(c))(d)\\1\\2"_test, "bcdbc"));
static_assert(match("(?|(a)|(b)(c)|(d)(e))(f)\\1\\2"_test, "bcfbc"));
static_assert(match("(?|(a)|(b)(c)|(d)(e))(f)\\1\\2"_test, "defde"));

/* caseless flag */
static_assert(match("(?i)abc"_test, "ABC"));
static_assert(match("(?i)abc"_test, "Abc"));
static_assert(match("(?i)abc"_test, "aBc"));
static_assert(match("(?i)a[be]"_test, "aB"));
static_assert(match("(?i)a[Z-a]"_test, "az"));
static_assert(match("a(?i:b)c"_test, "abc"));
static_assert(match("a(?i:b)c"_test, "aBc"));
static_assert(not match("a(?i:b)c"_test, "abC"));

/* dotall flag */
static_assert(match("(?s)."_test, "\n"));
static_assert(match("a.b"_test, "a-b"));
static_assert(not match("a.b"_test, "a\nb"));
static_assert(match("(?s)a.b"_test, "a\nb"));
static_assert(not match("((?s))a.b"_test, "a\nb"));
static_assert(not match("(?:(?s))a.b"_test, "a\nb"));

/* ungreedy flag */
static_assert(match("(?U)(a)?a*"_test, "aa", { no_tag, no_tag }));
static_assert(match("(?U)(a)??a*"_test, "aa", { 0, 1 }));
static_assert(match("(?U:)(a)?a*"_test, "aa", { 0, 1 }));
static_assert(match("(?U:)(a)??a*"_test, "aa", { no_tag, no_tag }));
static_assert(match("(?U)(aa)+a*"_test, "aaaaa", { 0, 2 }));
static_assert(match("(?U)(aa)+?a*"_test, "aaaaa", { 2, 4 }));
static_assert(match("(?U-U)(a)?a*"_test, "aa", { 0, 1 }));
static_assert(match("(?U-U)(a)??a*"_test, "aa", { no_tag, no_tag }));
static_assert(match("(?U)(a)?a*(?-U)(b)?b*"_test, "aabb", { no_tag, no_tag, 2, 3 }));
static_assert(match("(?U)(a)??a*(?-U)(b)??b*"_test, "aabb", { 0, 1, no_tag, no_tag }));

/* additional parser tests */
static_assert(match("\\Q...\\E"_test, "..."));
static_assert(not match("\\Q...\\E"_test, "aaa"));
static_assert(match("\\Qabc\\E+"_test, "abc"));
static_assert(match("\\Qabc\\E+"_test, "abccc"));
static_assert(not match("\\Qabc\\E+"_test, "abcabc"));