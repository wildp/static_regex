// -*- C++ -*- basic matching tests for all implementations

/* basic tests */
static_assert(match(""_test, ""));
static_assert(not match(""_test, "a"));
static_assert(not match("a"_test, ""));
static_assert(match("a"_test, "a"));
static_assert(not match("a"_test, "b"));

/* grouping tests */
static_assert(not match("a|b"_test, ""));
static_assert(match("a|b"_test, "a"));
static_assert(match("a|b"_test, "b"));
static_assert(not match("a|b"_test, "c"));
static_assert(match("a|b|c"_test, "c"));
static_assert(match("a|b|c"_test, "b"));
static_assert(match("a|b|c"_test, "a"));
static_assert(not match("aa"_test, ""));
static_assert(not match("aa"_test, "a"));
static_assert(match("aa"_test, "aa"));
static_assert(not match("aa"_test, "aaa"));
static_assert(not match("ab"_test, ""));
static_assert(not match("ab"_test, "a"));
static_assert(not match("ab"_test, "b"));
static_assert(match("ab"_test, "ab"));
static_assert(not match("ab"_test, "ba"));
static_assert(not match("ab"_test, "abb"));
static_assert(not match("ab"_test, "aab"));
static_assert(not match("ab"_test, "bb"));

/* character class tests */
static_assert(match("[a-c]"_test, "a"));
static_assert(match("[a-c]"_test, "b"));
static_assert(match("[a-c]"_test, "c"));
static_assert(not match("[a-c]"_test, "d"));

/* repeater tests */
static_assert(match("a*"_test, ""));
static_assert(match("a*"_test, "a"));
static_assert(match("a*"_test, "aa"));
static_assert(match("a*"_test, "aaaaa"));
static_assert(not match("a+"_test, ""));
static_assert(match("a+"_test, "a"));
static_assert(match("a+"_test, "aa"));
static_assert(match("a+"_test, "aaaaa"));
static_assert(match("a?"_test, ""));
static_assert(match("a?"_test, "a"));
static_assert(not match("a?"_test, "aa"));
static_assert(not match("a?"_test, "aaaaa"));
static_assert(not match("a{1}"_test, ""));
static_assert(match("a{1}"_test, "a"));
static_assert(not match("a{1}"_test, "aa"));
static_assert(not match("a{2,5}"_test, "a"));
static_assert(match("a{2,5}"_test, "aa"));
static_assert(match("a{2,5}"_test, "aaa"));
static_assert(match("a{2,5}"_test, "aaaa"));
static_assert(match("a{2,5}"_test, "aaaaa"));
static_assert(not match("a{2,5}"_test, "aaaaaaa"));
static_assert(not match("a{2,}"_test, "a"));
static_assert(match("a{2,}"_test, "aa"));
static_assert(match("a{2,}"_test, "aaa"));
static_assert(match("a{2,}"_test, "aaaa"));
static_assert(match("a{2,}"_test, "aaaaa"));
static_assert(not match("a{2,}?"_test, "a"));
static_assert(match("a{2,}?"_test, "aa"));
static_assert(match("a{2,}?"_test, "aaa"));
static_assert(match("a{2,}?"_test, "aaaa"));
static_assert(match("a{2,}?"_test, "aaaaa"));
static_assert(not match("a{3},"_test, "aa"));
static_assert(match("a{3}"_test, "aaa"));
static_assert(not match("a{3}"_test, "aaaa"));
static_assert(match("a{0}"_test, ""));
static_assert(not match("a{0}"_test, "a"));
static_assert(match("a{0,1}"_test, ""));
static_assert(match("a{0,1}"_test, "a"));
static_assert(not match("a{0,1}"_test, "aa"));
static_assert(not match("a{0,1}"_test, "aaa"));
static_assert(not match("a{1,2}"_test, ""));
static_assert(match("a{1,2}"_test, "a"));
static_assert(match("a{1,2}"_test, "aa"));
static_assert(not match("a{1,2}"_test, "aaa"));
static_assert(not match("a{1,2}"_test, "aaa"));

/* laziness tests */
static_assert(match("(a)?a*"_test, "", { no_tag, no_tag }));
static_assert(match("(a)?a*"_test, "a", { 0, 1 }));
static_assert(match("(a)?a*"_test, "aa", { 0, 1 }));
static_assert(match("(a)??a*"_test, "aa", { no_tag, no_tag }));
static_assert(match("(a)+?a?"_test, "aa", { 0, 1 }));
static_assert(match("(a)+a?"_test, "aa", { 1, 2 }));
static_assert(match("(a){2,3}?a?"_test, "aaa", { 1, 2 }));
static_assert(match("(a){2,3}a?"_test, "aaa", { 2, 3 }));
static_assert(match("(ab+c)+?(ab+c|.*d)"_test, "abcabbcacd", { 0, 3, 3, 10 }));
static_assert(match("(ab+c)+(ab+c|.*d)"_test, "abcabbcacd", { 3, 7, 7, 10 }));

/* submatch disambiguation tests */
static_assert(match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_test, "abcdefg", { 6, 7 }));     /* [perl]: a bcdef g */
static_assert(not match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_test, "abcdefg", { 4, 7 })); /* [posix]: ab c d efg */
static_assert(not match("(a|bcdef|g|ab|c|d|e|efg|fg)*"_test, "abcdefg", { 5, 7 })); /* [incorrect]: ab c d e fg */

/* capture location tests */
static_assert(match("(a)"_test, "a", { 0, 1 }));
static_assert(match("a(a)"_test, "aa", { 1, 2 }));
static_assert(match("a(a)a"_test, "aaa", { 1, 2 }));
static_assert(match("(a)*"_test, "a", { 0, 1 }));
static_assert(match("(a)*"_test, "aa", { 1, 2 }));
static_assert(match("(?:(a)|(d))c"_test, "ac", { 0, 1, no_tag, no_tag }));
static_assert(match("(?:(a)|(d))c"_test, "dc", { no_tag, no_tag, 0, 1 }));
static_assert(match("(a)+(b)*"_test, "ab", { 0, 1, 1, 2 }));
static_assert(match("(a)+(b)*"_test, "aab", { 1, 2, 2, 3 }));
static_assert(match("(aa)+a*"_test, "aaaaa", { 2, 4 }));
static_assert(match("(aa)+?a*"_test, "aaaaa", { 0, 2 }));
static_assert(match("(a((a)))(a)c"_test, "aaac", { 0, 2, 1, 2, 1, 2, 2, 3 }));
static_assert(match("a(aa)b|(aa)ac"_test, "aaab", { 1, 3, no_tag, no_tag }));
static_assert(match("a(aa)b|(aa)ac"_test, "aaac", { no_tag, no_tag, 0, 2 }));

/* wildcard tests */
static_assert(not match("."_test, ""));
static_assert(match("."_test, "0"));
static_assert(match("."_test, "@"));
static_assert(match("."_test, "$"));
static_assert(match("."_test, "z"));
static_assert(match("."_test, "A"));
static_assert(match("."_test, "."));
static_assert(not match("."_test, "\n"));
static_assert(match("(?s)."_test, "\n"));
// TODO: add test cases for non ascii chars