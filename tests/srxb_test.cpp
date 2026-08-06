// static_regex tests: string_view

#include "headers/common.hpp"

using srx::detail::no_tag;

template<srx::string_literal Pattern>
consteval srx::static_regex<Pattern, srx::mode::linear_twopass> operator ""_test() { return {}; }

#include "inc/1-basic.inc"
#include "inc/4-partial.inc"
#include "inc/5-anchor.inc"
