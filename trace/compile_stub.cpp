#include <rx/regex.hpp>

#ifndef REGEX_PATTERN
#error "REGEX_PATTERN is not defined"
#endif

#ifndef FUNCTION_NAME
#error "FUNCTION_NAME is not defined"
#endif

auto FUNCTION_NAME(std::string_view sv)
{
    return rx::detail::p1306_matcher<REGEX_PATTERN, rx::detail::default_fsm_flags::search_all>::operator()(sv.begin(), sv.end(), 0);
}

