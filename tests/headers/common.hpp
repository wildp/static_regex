// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <srx/regex.hpp>


template<typename MatchResult, std::bidirectional_iterator Iter>
consteval bool submatch_check(const MatchResult& mr, const std::vector<std::size_t>& captures, const Iter start, std::size_t init = 0)
{
    using srx::detail::no_tag;

    if (((mr.size() - init) * 2) != captures.size())
        return false;

    for (std::size_t i{ init }, i_end{ mr.size() }; i < i_end; ++i)
    {
        const auto submatch = mr.at(i);
        auto lhs = captures.at((i - init) * 2);
        auto rhs = captures.at(((i - init) * 2) + 1);

        if (lhs == no_tag and rhs == no_tag)
        {
            if (submatch.matched())
                return false;
        }
        else if (lhs != no_tag or rhs != no_tag)
        {
            if (not submatch.matched())
                return false;

            if (not (submatch.begin() == std::ranges::next(start, lhs) and submatch.end() == std::ranges::next(start, rhs)))
                return false;
        }
        else
        {
            return false;
        }
    }

    return true;
}

#if COMMON_HPP_INPUT_MODE == 0 /* contiguous sized range */

template<srx::string_literal S, srx::mode M>
consteval bool match(srx::static_regex<S, M> pattern, std::string_view str)
{
    return pattern.is_match(str);
}

template<srx::string_literal S, srx::mode M>
consteval bool match(srx::static_regex<S, M> pattern, std::string_view str, const std::vector<std::size_t>& captures)
{
    return submatch_check(pattern.match(str), captures, str.begin(), 1);
}

template<srx::string_literal S, srx::mode M>
consteval bool prefix_match(srx::static_regex<S, M> pattern, std::string_view str)
{
    return pattern.starts_with_match(str);
}

template<srx::string_literal S, srx::mode M>
consteval bool prefix_match(srx::static_regex<S, M> pattern, std::string_view str, const std::vector<std::size_t>& captures)
{
    return submatch_check(pattern.prefix_match(str), captures, str.begin());
}

template<srx::string_literal S, srx::mode M>
consteval bool search(srx::static_regex<S, M> pattern, std::string_view str)
{
    return pattern.contains_match(str);
}

template<srx::string_literal S, srx::mode M>
consteval bool search(srx::static_regex<S, M> pattern, std::string_view str, const std::vector<std::size_t>& captures)
{
    return submatch_check(pattern.search(str), captures, str.begin());
}

template<srx::string_literal S, srx::mode M>
consteval bool search_all(srx::static_regex<S, M> pattern, std::string_view str, const std::vector<std::vector<std::size_t>>& captures)
{
    for (const auto& [match, caps] : std::views::zip(pattern.range(str), captures))
    {
        if (not submatch_check(match, caps, str.begin()))
            return false;
    }
    return true;
}

#elif COMMON_HPP_INPUT_MODE == 1 /* contiguous non-sized range */

template<srx::string_literal S, srx::mode M>
consteval bool match(srx::static_regex<S, M> pattern, const char* cstr)
{
    return pattern.is_match(cstr);
}

template<srx::string_literal S, srx::mode M>
consteval bool match(srx::static_regex<S, M> pattern, const char* cstr, const std::vector<std::size_t>& captures)
{
    std::string_view sv{ cstr };
    return submatch_check(pattern.match(cstr), captures, sv.begin(), 1);
}

template<srx::string_literal S, srx::mode M>
consteval bool prefix_match(srx::static_regex<S, M> pattern, const char* cstr)
{
    return pattern.starts_with_match(cstr);
}

template<srx::string_literal S, srx::mode M>
consteval bool prefix_match(srx::static_regex<S, M> pattern, const char* cstr, const std::vector<std::size_t>& captures)
{
    std::string_view sv{ cstr };
    return submatch_check(pattern.prefix_match(cstr), captures, sv.begin());
}

template<srx::string_literal S, srx::mode M>
consteval bool search(srx::static_regex<S, M> pattern, const char* cstr)
{
    return pattern.contains_match(cstr);
}

template<srx::string_literal S, srx::mode M>
consteval bool search(srx::static_regex<S, M> pattern, const char* cstr, const std::vector<std::size_t>& captures)
{
    std::string_view sv{ cstr };
    return submatch_check(pattern.search(cstr), captures, sv.begin());
}

template<srx::string_literal S, srx::mode M>
consteval bool search_all(srx::static_regex<S, M> pattern, const char* cstr, const std::vector<std::vector<std::size_t>>& captures)
{
    std::string_view sv{ cstr };
    for (const auto& [match, caps] : std::views::zip(pattern.range(cstr), captures))
    {
        if (not submatch_check(match, caps, sv.begin()))
            return false;
    }
    return true;
}

#endif