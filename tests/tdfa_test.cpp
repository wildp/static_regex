// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "headers/tdfa_matcher.hpp"


namespace {

template<typename CharT>
consteval bool match(const CharT* pattern, const CharT* str, const std::vector<std::size_t>& captures = {})
{
    using namespace srx::detail;
    const expr_tree ast{ pattern };
    tagged_nfa nfa{ ast, default_fsm_flags::full_match };
    nfa.rewrite_assertions();

    const srx::testing::tdfa_matcher dfa{ nfa };
    const auto match_result = dfa.match(str);

    if (captures.empty())
        return match_result.has_value();
    else if (not match_result.has_value())
        return false;
    else
        return std::ranges::equal(match_result.value() | std::views::drop(2), captures);
}

template<typename CharT>
consteval bool prefix_match(const CharT* pattern, const CharT* str, const std::vector<std::size_t>& captures = {})
{
    using namespace srx::detail;
    const expr_tree ast{ pattern };
    tagged_nfa nfa{ ast, default_fsm_flags::partial_match };
    nfa.rewrite_assertions();

    const srx::testing::tdfa_matcher dfa{ nfa };
    const auto match_result = dfa.partial_match(str);

    if (captures.empty())
        return match_result.has_value();
    else if (not match_result.has_value())
        return false;
    else
        return std::ranges::equal(match_result.value(), captures);
}

template<typename CharT>
consteval bool search(const CharT* pattern, const CharT* str, const std::vector<std::size_t>& captures = {})
{
    using namespace srx::detail;
    expr_tree ast{ pattern };
    ast.insert_search_prefix();
    tagged_nfa nfa{ ast, default_fsm_flags::search_single };
    nfa.rewrite_assertions();

    const srx::testing::tdfa_matcher dfa{ nfa };
    const auto match_result = dfa.partial_match(str);

    if (captures.empty())
        return match_result.has_value();
    else if (not match_result.has_value())
        return false;
    else
        return std::ranges::equal(match_result.value(), captures);
}

template<typename CharT>
consteval bool search_all(const CharT* pattern, const CharT* str, const std::vector<std::vector<std::size_t>>& captures = {})
{
    using namespace srx::detail;
    expr_tree ast{ pattern };
    ast.insert_search_prefix();
    tagged_nfa nfa{ ast, default_fsm_flags::search_all };
    nfa.rewrite_assertions();
    if (ast.empty_match_possible())
        nfa.add_non_empty_match_pathway();

    const srx::testing::tdfa_matcher dfa{ nfa };
    const auto match_result = dfa.match_all(str);

    if (captures.empty())
        return not match_result.empty();
    else if (match_result.empty())
        return false;
    else
        return std::ranges::equal(match_result, captures);
}

} // namespace

using srx::detail::no_tag;

constexpr const char* operator ""_test(const char* cstr, std::size_t) { return cstr; }

#include "inc/1-basic.inc"
#include "inc/4-partial.inc"
#include "inc/5-anchor.inc"