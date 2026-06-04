// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "headers/tnfa_matcher.hpp"


namespace {

template<typename CharT>
consteval bool match(const CharT* pattern, const CharT* str, const std::vector<std::size_t>& captures = {})
{
    using namespace srx::detail;
    const expr_tree ast{ pattern };

    const srx::testing::tnfa_matcher nfa{ ast, default_fsm_flags::full_match };
    const auto match_result = nfa.match(str);

    if (captures.empty())
        return match_result.has_value();
    else if (not match_result.has_value())
        return false;
    else
        return std::ranges::equal(match_result.value() | std::views::drop(2), captures);
}

template<typename CharT>
consteval bool ends_with(const CharT* pattern, const CharT* str, std::size_t start_pos)
{
    using namespace srx::detail;
    expr_tree ast{ pattern };
    ast.insert_search_prefix();

    const srx::testing::tnfa_matcher nfa{ ast, default_fsm_flags::full_match };
    const auto match_result = nfa.match(str);

    if (not match_result.has_value())
        return false;
    else
        return match_result.value().at(0) == start_pos;
}

} // namespace

using srx::detail::no_tag;

constexpr const char* operator ""_test(const char* cstr, std::size_t) { return cstr; }

#include "inc/1-basic.inc"

/* search prefix tests */
static_assert(ends_with("ab"_test, "ab", 0));
static_assert(ends_with("aa"_test, "aaa", 1));
static_assert(ends_with("cde"_test, "abcde", 2));
static_assert(ends_with("cde"_test, "cdecde", 3));
static_assert(ends_with("bcd"_test, "abcdbcd", 4));
