// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "headers/tree_matcher.hpp"


namespace {

constexpr srx::detail::parser_flags expo_test_flags{ .enable_possessive = true, .enable_backrefs = true, .enable_branchreset = true };

template<typename CharT>
consteval bool match(const CharT* pattern, const CharT* str, const std::vector<std::size_t>& captures = {})
{
    const srx::testing::tree_matcher ast{ pattern, expo_test_flags };
    const auto match_result = ast.match(str);

    if (captures.empty())
        return match_result.has_value();
    else if (match_result.has_value())
        return std::ranges::equal(*match_result | std::views::drop(2), captures);
    else
        return false;
}

} // namespace

using srx::detail::no_tag;

constexpr const char* operator ""_test(const char* cstr, std::size_t) { return cstr; }

#define DISABLE_OVERLAPPING_BACKREF_TESTS 1

#include "inc/1-basic.inc"
#include "inc/2-backtrack.inc"
#include "inc/3-extra.inc"
