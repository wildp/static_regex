// static_regex (linear) tests: string_view

#include "headers/common.hpp"

using srx::detail::no_tag;

template<srx::string_literal Pattern>
consteval srx::static_regex<Pattern, srx::mode::linear> operator ""_test() { return {}; }

/* note: tests from inc/1-basic.inc are entirely covered by srxc_test */
#include "inc/4-partial.inc"
#include "inc/5-anchor.inc"
