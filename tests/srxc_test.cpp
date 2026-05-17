// static_regex tests: const char*

#define COMMON_HPP_INPUT_MODE 1 /* contiguous non-sized range */
#include "headers/common.hpp"

/* these tests exist due to the implementation divergence between
 * the std::string_view and const char* matcher implementations. */

using srx::detail::no_tag;

template<srx::string_literal Pattern>
consteval srx::static_regex<Pattern> operator ""_test() { return {}; }

#include "inc/1-basic.inc"
#include "inc/4-partial.inc"
#include "inc/5-anchor.inc"
