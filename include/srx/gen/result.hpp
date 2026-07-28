// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <cstddef>
#include <iterator>
#include <meta>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

#include "srx/api/submatch.hpp"
#include "srx/etc/string_literal.hpp"
#include "srx/etc/util.hpp"
#include "srx/gen/compile.hpp"


namespace srx {
namespace detail {

template<std::meta::info Info>
struct p1306dfa;

template<srx::string_literal Pattern>
struct naive_matcher;

} // namespace detail


template<std::bidirectional_iterator I, srx::detail::static_match_result_info Captures>
class static_match_results
{
    using factory = detail::submatch_factory<I>;

    template<bool Const>
    class proxy_iterator;

public:
    using size_type              = std::size_t;
    using char_type              = std::remove_cv_t<std::iter_value_t<I>>;
    using submatch_type          = submatch<I>;
    using iterator               = proxy_iterator<std::same_as<I, std::const_iterator<I>>>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_iterator         = proxy_iterator<true>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type submatch_count{ Captures.fci.capture_count() };

    constexpr static_match_results() noexcept(std::is_nothrow_default_constructible_v<I>)
    {
        if constexpr (has_registers and not has_enabled)
            reg_.fill(I{});
    }

    /* observers */

    [[nodiscard]] constexpr bool has_value() const noexcept
    {
        if constexpr (std::contiguous_iterator<I>)
            return std::to_address(match_end_) != std::to_address(I{});
        else
            return match_success_;
    }

    [[nodiscard]] constexpr explicit(false) operator bool() const noexcept
    {
        return this->has_value();
    }

    [[nodiscard]] constexpr size_type size() const noexcept
    {
        return (this->has_value()) ? submatch_count : 0;
    }

    /* array-like access */

    [[nodiscard]] constexpr submatch_type operator[](size_type n) const noexcept
    {
        template for (constexpr size_type N : std::views::iota(0uz, submatch_count))
            if (n == N)
                return get<N>(*this);
        std::unreachable();
    }

    [[nodiscard]] constexpr submatch_type at(size_type i) const
    {
        this->range_check(i);
        return this->operator[](i);
    }

    /* iterator support */

    [[nodiscard]] constexpr iterator begin() const noexcept
    {
        return { this, 0 };
    }

    [[nodiscard]] constexpr iterator end() const noexcept
    {
        return { this, this->size() };
    }

    [[nodiscard]] constexpr reverse_iterator rbegin() const noexcept
    {
        return std::make_reverse_iterator(this->end());
    }

    [[nodiscard]] constexpr reverse_iterator rend() const noexcept
    {
        return std::make_reverse_iterator(this->begin());
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return this->begin();
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return this->end();
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
    {
        return std::make_reverse_iterator(this->cend());
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
    {
        return std::make_reverse_iterator(this->cbegin());
    }

    /* tuple support */

    template<size_type N>
        requires (N < submatch_count)
    [[nodiscard]] friend constexpr submatch_type get(const static_match_results& r) noexcept
    {
        if (r.has_value())
            return r.template force_get<N>();
        return {};
    }

    template<std::meta::info Info>
    friend struct detail::p1306dfa;

    template<srx::string_literal Pattern>
    friend struct detail::naive_matcher;

private:
    /* implementation helpers */

    static constexpr bool has_registers{ Captures.register_count != 0 };
    static constexpr bool has_success{ not std::contiguous_iterator<I> };
    static constexpr bool has_enabled{ has_registers and has_success };
    static constexpr bool has_match_start{ Captures.fci.has_match_start() };

    constexpr explicit static_match_results(I start)
        noexcept(std::is_nothrow_default_constructible_v<I> and std::is_nothrow_move_constructible_v<I>)
        : match_start_{ std::move(start) }
    {
        if constexpr (has_registers and not has_enabled)
            reg_.fill(I{});
    }

    constexpr void clear_match() noexcept(std::is_nothrow_default_constructible_v<I>)
    {
        if constexpr (std::contiguous_iterator<I>)
            match_end_ = I{};
        else
            match_success_ = false;
    }

    constexpr void reset(I new_start)
    {
        clear_match();
        if constexpr (has_match_start)
            match_start_ = std::move(new_start);
        if constexpr (has_registers and not has_enabled)
            reg_.fill(I{});
        if constexpr (has_enabled)
            enabled_.fill(false);
    }

    template<detail::tag_number_t N>
    [[nodiscard]] constexpr bool tag_enabled() const noexcept
    {
        if constexpr (N == detail::start_of_input_tag or N == detail::end_of_input_tag)
            return true;
        else if constexpr (std::contiguous_iterator<I>)
            return std::to_address(reg_[Captures.final_registers[N]]) != std::to_address(I{});
        else
            return enabled_[Captures.final_registers[N]];
    }

    template<detail::tag_number_t N>
    [[nodiscard]] constexpr I get_tag() const noexcept
    {
        if constexpr (N == detail::start_of_input_tag)
            return match_start_;
        else if constexpr (N == detail::end_of_input_tag)
            return match_end_;
        else
            return reg_[Captures.final_registers[N]];
    }

    template<size_type N>
        requires (N < submatch_count)
    [[nodiscard]] constexpr submatch_type force_get() const noexcept
    {
        static constexpr auto current = Captures.fci.captures[N];

        if constexpr (current.first.tag_number == current.second.tag_number)
        {
            if (tag_enabled<current.first.tag_number>())
            {
                return factory::make_submatch(
                    std::ranges::next(get_tag<current.first.tag_number>(), current.first.offset),
                    std::ranges::next(get_tag<current.second.tag_number>(), current.second.offset)
                );
            }
        }
        else
        {
            if (tag_enabled<current.first.tag_number>() and tag_enabled<current.second.tag_number>())
            {
                return factory::make_submatch(
                    std::ranges::next(get_tag<current.first.tag_number>(), current.first.offset),
                    std::ranges::next(get_tag<current.second.tag_number>(), current.second.offset)
                );
            }
        }

        return {};
    }

    constexpr void range_check(size_type n) const
    {
        if (n >= this->size())
            throw std::out_of_range("static_match_results::range_check: n >= this->size()");
    }


    /* data members and protected trivial accessors */

    using registers_type   = detail::maybe_type_t<has_registers, std::array<I, Captures.register_count>>;
    using enabled_type     = detail::maybe_type_t<has_enabled, std::array<bool, Captures.register_count>>;
    using match_start_type = detail::maybe_type_t<has_match_start, I>;
    using success_type     = detail::maybe_type_t<has_success, bool>;

    [[no_unique_address]] registers_type reg_;
    [[no_unique_address]] match_start_type match_start_{};
    I match_end_{};
    [[no_unique_address]] enabled_type enabled_{};
    [[no_unique_address]] success_type match_success_{ false };
};


/* iterator implementation */

template<std::bidirectional_iterator I, srx::detail::static_match_result_info Captures>
template<bool Const>
class static_match_results<I, Captures>::proxy_iterator
{
public:
    using iterator_concept  = std::random_access_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using value_type        = submatch<std::conditional_t<Const, std::const_iterator<I>, I>>;
    using difference_type   = std::ptrdiff_t;

    proxy_iterator() = default;

    constexpr proxy_iterator(const static_match_results* ptr, size_type pos) noexcept
        : ptr_{ ptr }, pos_{ pos } {}

    constexpr explicit(false) proxy_iterator(proxy_iterator<not Const> i) noexcept requires Const
        : ptr_{ i.ptr_ }, pos_{ i.pos_ } {}

    constexpr value_type operator*() const noexcept
    {
        return (*ptr_)[pos_];
    }

    constexpr value_type operator[](difference_type n) const noexcept
    {
        return (*ptr_)[pos_ + n];
    }

    constexpr proxy_iterator& operator++() noexcept
    {
        ++pos_;
        return *this;
    }

    constexpr proxy_iterator operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    constexpr proxy_iterator& operator--() noexcept
    {
        --pos_;
        return *this;
    }

    constexpr proxy_iterator operator--(int) noexcept
    {
        auto tmp = *this;
        --*this;
        return tmp;
    }

    constexpr proxy_iterator& operator+=(difference_type n) noexcept
    {
        pos_ += n;
        return *this;
    }

    constexpr proxy_iterator& operator-=(difference_type n) noexcept
    {
        pos_ -= n;
        return *this;
    }

    friend constexpr bool operator==(const proxy_iterator&, const proxy_iterator&) = default;

    friend constexpr auto operator<=>(const proxy_iterator&, const proxy_iterator&) = default;

    friend constexpr proxy_iterator operator+(const proxy_iterator& i, difference_type n) noexcept
    {
        return { i.ptr_, i.pos_ + n };
    }

    friend constexpr proxy_iterator operator+(difference_type n, const proxy_iterator& i) noexcept
    {
        return { i.ptr_, n + i.pos_ };
    }

    friend constexpr proxy_iterator operator-(const proxy_iterator& i, difference_type n) noexcept
    {
        return { i.ptr_, i.pos_ - n };
    }

    friend constexpr difference_type operator-(const proxy_iterator& x, const proxy_iterator& y) noexcept
    {
        return x.pos_ - y.pos_;
    }

    friend class proxy_iterator<not Const>;

private:
    const static_match_results* ptr_{ nullptr };
    size_type pos_{ 0 };
};

} // namespace srx


/* structured binding support for static_match_results */

template<std::bidirectional_iterator Iter, srx::detail::static_match_result_info Captures>
struct std::tuple_size<srx::static_match_results<Iter, Captures>>
    : integral_constant<std::size_t, srx::static_match_results<Iter, Captures>::submatch_count> {};

template<std::size_t N, std::bidirectional_iterator Iter, srx::detail::static_match_result_info Captures>
    requires (N < srx::static_match_results<Iter, Captures>::submatch_count)
struct std::tuple_element<N, srx::static_match_results<Iter, Captures>>
{
    using type = srx::static_match_results<Iter, Captures>::submatch_type;
};