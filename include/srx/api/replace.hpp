// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>

#include "srx/api/static_regex.hpp"
#include "srx/etc/string_literal.hpp"
#include "srx/etc/util.hpp"
#include "srx/gen/result.hpp"


namespace srx {
namespace detail {

template<std::bidirectional_iterator I, std::sentinel_for<I> S, typename Regex>
class stashing_regex_iterator;

template<std::bidirectional_iterator I, std::sentinel_for<I> S, string_literal Pattern, mode Mode>
class stashing_regex_iterator<I, S, static_regex<Pattern, Mode>>
{
    using matcher_type  = [: detail::get_matcher_refl(Mode, true) :]<Pattern, detail::default_fsm_flags::search_all>;
    using result_type   = matcher_type::template iterated_result<I>;

public:
    using iterator_concept  = std::input_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using value_type        = matcher_type::template result<I>;;
    using difference_type   = std::ptrdiff_t;

    stashing_regex_iterator() requires std::default_initializable<I> and std::default_initializable<S> = default;

    constexpr explicit stashing_regex_iterator(I first, S last)
        : result_{ first, last }, first_{ std::move(first) }, last_{ std::move(last) } {}

    constexpr const value_type& operator*() const noexcept
    {
        return result_.res;
    }

    constexpr const value_type* operator->() const noexcept
    {
        return &result_.res;
    }

    constexpr stashing_regex_iterator& operator++()
    {
        if (not result_.res)
            return *this;

        if constexpr (result_type::needs_begin)
            result_.advance(first_, last_);
        else
            result_.advance(last_);

        return *this;
    }

    constexpr void operator++(int)
    {
        ++*this;
    }

    friend constexpr bool operator==(const stashing_regex_iterator& x, std::default_sentinel_t)
    {
        return not x.result_.res.has_value();
    }

    template<std::ranges::input_range W, int...>
        requires std::ranges::view<W>
    friend class submatches_view;

private:
    using maybe_it = maybe_type_t<result_type::needs_begin, I>;

    result_type result_;
    [[no_unique_address]] maybe_it first_;
    [[no_unique_address]] S last_;
    [[no_unique_address]] matcher_type matcher_;
};

template<typename CharT>
consteval auto replace_fmt_pattern()
{
    if constexpr (std::same_as<CharT, char>)
        return string_literal{ R"(\$(?:[&`'+$]|\d+))" };
    else if constexpr (std::same_as<CharT, wchar_t>)
        return string_literal{ LR"(\$(?:[&`'+$]|\d+))" };
    else if constexpr (std::same_as<CharT, char8_t>)
        return string_literal{ u8R"(\$(?:[&`'+$]|\d+))" };
    else if constexpr (std::same_as<CharT, char16_t>)
        return string_literal{ uR"(\$(?:[&`'+$]|\d+))" };
    else if constexpr (std::same_as<CharT, char32_t>)
        return string_literal{ UR"(\$(?:[&`'+$]|\d+))" };
    else
        return;
}

template<std::bidirectional_iterator I, std::sentinel_for<I> S>
constexpr std::size_t parse_unsigned(I first, const S last, const std::size_t submatch_count)
{
    static constexpr std::size_t base{ 10 };
    std::size_t result{ 0 };

    while (first != last)
    {
        result = std::saturating_add(std::saturating_mul(result, base), static_cast<std::size_t>(*first - '0'));
        ++first;
    }

    if (result >= submatch_count)
        throw regex_error("replace_fmt: invalid replacement");

    return result;
}

namespace replace_constants {

enum replace_constant : std::size_t
{
    match       =  0uz,
    prematch    = -1uz,
    postmatch   = -2uz,
    skip        = -3uz,
};

} // namespace replace_constants

template<std::bidirectional_iterator I, std::sentinel_for<I> S>
constexpr std::size_t parse_fmt_replace(I first, const S last, const std::size_t submatch_count)
{
    if (first == last)
        throw regex_error("replace_fmt: invalid replacement");

    if (*first == '&')
        return replace_constants::match;
    else if (*first == '`')
        return replace_constants::prematch;
    else if (*first == '\'')
        return replace_constants::postmatch;
    else if (*first == '+')
        return submatch_count - 1;
    else if (*first == '$')
        return replace_constants::skip;
    else
        return parse_unsigned(first, last, submatch_count);
}


template<std::bidirectional_iterator I>
class replace_fmt
{
public:
    using char_type = std::iter_value_t<I>;
    using subrange_type = std::ranges::subrange<I>;

    constexpr replace_fmt() requires std::default_initializable<I>
        : subranges_{ subrange_type{} }, captures_{} {}

    template<std::sentinel_for<I> S>
    constexpr replace_fmt(I first, S last, const std::size_t submatch_count)
    {
        using matcher_type = [: detail::get_matcher_refl(mode::standard, true) :]<detail::replace_fmt_pattern<char_type>(), detail::default_fsm_flags::search_all>;
        using result_type = matcher_type::template iterated_result<I>;

        result_type delim_matcher_result{ first, last };

        while (delim_matcher_result.res)
        {
            const auto& [mfirst, mlast] = get<0>(delim_matcher_result.res);
            const auto& cap = captures_.emplace_back(detail::parse_fmt_replace(std::ranges::next(mfirst), mlast, submatch_count));

            if (cap == replace_constants::skip) /* treat $$ in format as single $ */
                subranges_.emplace_back(first, std::ranges::next(mfirst));
            else
                subranges_.emplace_back(first, mfirst);

            first = mlast;
            delim_matcher_result.advance(last);
        }

        if constexpr (std::same_as<I, S>)
            subranges_.emplace_back(first, last);
        else
            subranges_.emplace_back(first, std::ranges::next(first, last));
    }

    constexpr auto zipped() const
    {
        return std::views::zip(subranges_, captures_);
    }

    constexpr const auto& trailing() const
    {
        return subranges_.back();
    }

    constexpr const auto& subranges() const noexcept
    {
        return subranges_;
    }

    constexpr const auto& captures() const noexcept
    {
        return captures_;
    }

private:
    std::vector<subrange_type> subranges_;
    std::vector<std::size_t> captures_;
};


template<typename CharT>
class static_replace_fmt
{
public:
    using char_type = CharT;
    using subrange_type = detail::static_span<char_type>;

    explicit consteval static_replace_fmt(std::basic_string_view<CharT> sv, const std::size_t match_count)
        : static_replace_fmt(replace_fmt{ sv.begin(), sv.end(), match_count }) {}

    constexpr auto zipped() const
    {
        return std::views::zip(subranges_, captures_);
    }

    constexpr const auto& trailing() const
    {
        return subranges_.back();
    }

    constexpr const auto& subranges() const noexcept
    {
        return subranges_;
    }

    constexpr const auto& captures() const noexcept
    {
        return captures_;
    }

private:
    explicit consteval static_replace_fmt(const replace_fmt<const char_type*>& fmt)
        : subranges_{ fmt.subranges() | std::views::transform(make_subrange) }
        , captures_{ fmt.captures() } {}

    static consteval subrange_type make_subrange(const std::ranges::subrange<const char_type*>& s)
    {
        return s;
    }

    detail::static_span<subrange_type> subranges_;
    detail::static_span<std::size_t> captures_;
};

} // namespace detail


template<string_literal Fmt>
struct fmt_t {};

template<string_literal Fmt>
inline constexpr fmt_t<Fmt> fmt;


template<class I, class O>
using regex_replace_result = std::ranges::in_out_result<I, O>;


namespace detail {

struct replace_impl
{
private:
    template<typename CharT, std::bidirectional_iterator I, std::sentinel_for<I> S, std::output_iterator<CharT> O,
             string_literal Pattern, mode Mode, std::bidirectional_iterator F, std::sentinel_for<F> FmtS>
    static constexpr regex_replace_result<I, O>
    impl(I first, const S last, O result, static_regex<Pattern, Mode> /* regex */, F fmt_first, FmtS fmt_last)
    {
        using iterator_type = stashing_regex_iterator<I, S, static_regex<Pattern, Mode>>;
        using sentinel_type = std::default_sentinel_t;
        using result_type = iterator_type::value_type;

        const replace_fmt fmt{ fmt_first, fmt_last, result_type::submatch_count };

        iterator_type it{ first, last };
        sentinel_type end{};

        for (; it != end; ++it)
        {
            auto [mfirst, mlast] = get<0>(*it);

            result = std::ranges::copy(first, mfirst, result).out;

            for (const auto& [substr, idx] : fmt.zipped())
            {
                result = std::ranges::copy(substr, result).out;

                if (idx == replace_constants::match)
                    result = std::ranges::copy(mfirst, mlast, result).out;
                else if (idx == replace_constants::prematch)
                    result = std::ranges::copy(first, mfirst, result).out;
                else if (idx == replace_constants::postmatch)
                    result = std::ranges::copy(mlast, last, result).out;
                else if (idx != replace_constants::skip)
                    result = std::ranges::copy(it->at(idx), result).out;
            }

            result = std::ranges::copy(fmt.trailing(), result).out;
            first = mlast;
        }

        return std::ranges::copy(first, last, result);
    }

    template<typename CharT, std::bidirectional_iterator I, std::sentinel_for<I> S, std::output_iterator<CharT> O,
             string_literal Pattern, mode Mode, string_literal Fmt>
    static constexpr regex_replace_result<I, O>
    impl(I first, const S last, O result, static_regex<Pattern, Mode> /* regex */, fmt_t<Fmt>)
    {
        using iterator_type = stashing_regex_iterator<I, S, static_regex<Pattern, Mode>>;
        using sentinel_type = std::default_sentinel_t;
        using result_type = iterator_type::value_type;

        static constexpr static_replace_fmt fmt{ Fmt.view(), result_type::submatch_count };

        iterator_type it{ first, last };
        sentinel_type end{};

        for(; it != end; ++it)
        {
            auto [mfirst, mlast] = get<0>(*it);
            result = std::ranges::copy(first, mfirst, result).out;

            // TODO: change to use constexpr structured binding when supported
            template for (constexpr auto pair : fmt.zipped())
            {
                constexpr std::size_t idx{ get<1>(pair) };
                result = std::ranges::copy(get<0>(pair), result).out;

                if constexpr (idx == replace_constants::match)
                    result = std::ranges::copy(mfirst, mlast, result).out;
                else if constexpr (idx == replace_constants::prematch)
                    result = std::ranges::copy(first, mfirst, result).out;
                else if constexpr (idx == replace_constants::postmatch)
                    result = std::ranges::copy(mlast, last, result).out;
                else if constexpr (idx != replace_constants::skip)
                    result = std::ranges::copy(get<idx>(*it), result).out;
            }

            result = std::ranges::copy(fmt.trailing(), result).out;
            first = mlast;
        }

        return std::ranges::copy(first, last, result);
    }

public:
    template<std::bidirectional_iterator I, std::sentinel_for<I> S, std::output_iterator<std::iter_value_t<I>> O, regex_like Regex,
             std::bidirectional_iterator F, std::sentinel_for<F> FmtS, std::same_as<std::iter_value_t<I>> CharT = Regex::char_type>
        requires std::same_as<std::iter_value_t<F>, CharT>
    static constexpr regex_replace_result<I, O>
    operator()(I first, S last, O result, Regex pattern, F fmt, FmtS fmt_last)
    {
        return impl<CharT>(first, last, result, pattern, fmt, fmt_last);
    }

    template<std::ranges::bidirectional_range R, std::output_iterator<std::ranges::range_value_t<R>> O,
             regex_like Regex, std::ranges::bidirectional_range FmtR, std::same_as<std::ranges::range_value_t<R>> CharT = Regex::char_type>
        requires std::same_as<std::ranges::range_value_t<FmtR>, CharT> and (not std::convertible_to<FmtR, std::basic_string_view<CharT>>)
    static constexpr regex_replace_result<std::ranges::borrowed_iterator_t<R>, O>
    operator()(R&& r, O result, Regex pattern, FmtR&& fmt)
    {
        return operator()(std::ranges::begin(r), std::ranges::end(r), result, pattern, std::ranges::begin(fmt), std::ranges::end(fmt));
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, std::output_iterator<std::iter_value_t<I>> O,
             regex_like Regex, std::same_as<std::iter_value_t<I>> CharT>
    static constexpr regex_replace_result<I, O>
    operator()(I first, S last, O result, Regex pattern, std::basic_string_view<CharT> fmt)
    {
        return operator()(first, last, result, pattern, fmt.begin(), fmt.end());
    }

    template<std::ranges::bidirectional_range R, std::output_iterator<std::ranges::range_value_t<R>> O,
             regex_like Regex, std::same_as<std::ranges::range_value_t<R>> CharT>
    static constexpr regex_replace_result<std::ranges::borrowed_iterator_t<R>, O>
    operator()(R&& r, O result, Regex pattern, std::basic_string_view<CharT> fmt)
    {
        return operator()(std::ranges::begin(r), std::ranges::end(r), result, pattern, fmt.begin(), fmt.end());
    }

    template<std::ranges::contiguous_range CR, regex_like Regex,
             std::same_as<std::ranges::range_value_t<CR>> CharT = Regex::char_type>
    static constexpr std::basic_string<CharT>
    operator()(CR&& cr, Regex pattern, std::basic_string_view<std::type_identity_t<CharT>> fmt)
    {
        std::basic_string<CharT> result;
        operator()(std::ranges::begin(cr), std::ranges::end(cr), std::back_inserter(result), pattern, fmt.begin(), fmt.end());
        return result;
    }

    template<typename CharT, typed_regex_like<CharT> Regex>
    static constexpr std::basic_string<CharT>
    operator()(const CharT* cstr, Regex pattern, std::basic_string_view<std::type_identity_t<CharT>> fmt)
    {
        std::basic_string<CharT> result;
        operator()(cstr, cstr_sentinel, std::back_inserter(result), pattern, fmt.begin(), fmt.end());
        return result;
    }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, std::output_iterator<std::iter_value_t<I>> O,
             static_regex_like Regex, string_literal Fmt, std::same_as<std::iter_value_t<I>> CharT = Regex::char_type>
        requires std::same_as<typename decltype(Fmt)::value_type, CharT>
    static constexpr regex_replace_result<I, O>
    operator()(I first, S last, O result, Regex pattern, fmt_t<Fmt>)
    {
        return impl<CharT>(first, last, result, pattern, fmt<Fmt>);
    }

    template<std::ranges::bidirectional_range R, std::output_iterator<std::ranges::range_value_t<R>> O,
             static_regex_like Regex, string_literal Fmt, std::same_as<std::ranges::range_value_t<R>> CharT = Regex::char_type>
        requires std::same_as<typename decltype(Fmt)::value_type, CharT>
    static constexpr regex_replace_result<std::ranges::borrowed_iterator_t<R>, O>
    operator()(R&& r, O result, Regex pattern, fmt_t<Fmt>)
    {
        return operator()(std::ranges::begin(r), std::ranges::end(r), result, pattern, fmt<Fmt>);
    }

    template<std::ranges::contiguous_range CR, static_regex_like Regex, string_literal Fmt,
             std::same_as<std::ranges::range_value_t<CR>> CharT = Regex::char_type>
        requires std::same_as<typename decltype(Fmt)::value_type, CharT>
    static constexpr std::basic_string<CharT>
    operator()(CR&& cr, Regex pattern, fmt_t<Fmt>)
    {
        std::basic_string<CharT> result;
        operator()(std::ranges::begin(cr), std::ranges::end(cr), std::back_inserter(result), pattern, fmt<Fmt>);
        return result;
    }

    template<typename CharT, typed_static_regex_like<CharT> Regex, string_literal Fmt>
        requires std::same_as<typename decltype(Fmt)::value_type, CharT>
    static constexpr std::basic_string<CharT>
    operator()(const CharT* cstr, Regex pattern, fmt_t<Fmt>)
    {
        std::basic_string<CharT> result;
        operator()(cstr, cstr_sentinel, std::back_inserter(result), pattern, fmt<Fmt>);
        return result;
    }
};

} // namespace detail

inline constexpr detail::replace_impl regex_replace;

} // namespace srx