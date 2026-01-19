// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <format>
#include <iosfwd>
#include <iterator>
#include <numeric>
#include <string>
#include <string_view>

#include "rx/etc/util.hpp"


namespace rx
{
    namespace detail
    {
        template<std::bidirectional_iterator Iter>
        class submatch_factory;
    }


    template<std::bidirectional_iterator I>
    class submatch
    {
    public:
        using iterator               = I;
        using reverse_iterator       = std::reverse_iterator<iterator>;
#if __cpp_lib_ranges_as_const >= 202311L
        using const_iterator         = std::const_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
#endif
        using value_type             = std::iter_value_t<iterator>;
        using size_type              = std::size_t;
        using string_type            = std::basic_string<value_type>;
        using view_type              = std::basic_string_view<value_type>;

        constexpr submatch() = default;

        /* observers */

        [[nodiscard]] constexpr bool matched() const noexcept
        {
            if constexpr (std::contiguous_iterator<I>)
                return std::to_address(first_) != std::to_address(I{});
            else
                return match_;
        }

        [[nodiscard]] constexpr explicit(false) operator bool() const noexcept
        {
            return this->matched();
        }

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            return first_ == last_;
        }

        [[nodiscard]] constexpr size_type size() const
        {
            return std::saturate_cast<size_type>(std::ranges::distance(first_, last_));
        }

        [[nodiscard]] constexpr size_type length() const
        {
            return this->size();
        }

        /* iterators */

        [[nodiscard]] constexpr iterator begin() const noexcept
        {
            return first_;
        }

        [[nodiscard]] constexpr iterator end() const noexcept
        {
            return last_;
        }

        [[nodiscard]] constexpr reverse_iterator rbegin() const noexcept
        {
            return std::make_reverse_iterator(this->end());
        }

        [[nodiscard]] constexpr reverse_iterator rend() const noexcept
        {
            return std::make_reverse_iterator(this->begin());
        }

#if __cpp_lib_ranges_as_const >= 202311L
        [[nodiscard]] constexpr const_iterator cbegin() const noexcept
        requires std::bidirectional_iterator<const_iterator>
        {
            return std::make_const_iterator(this->begin());
        }

        [[nodiscard]] constexpr const_iterator cend() const noexcept
        requires std::bidirectional_iterator<const_iterator>>
        {
            return std::make_const_iterator(this->end());
        }

        [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
        requires std::bidirectional_iterator<const_iterator>
        {
            return std::make_reverse_iterator(this->cend());
        }

        [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
        requires std::bidirectional_iterator<const_iterator>
        {
            return std::make_reverse_iterator(this->cbegin());
        }
#endif

        /* structured binding support */

        template<std::size_t N>
        requires (N < 2)
        [[nodiscard]] constexpr const auto& get() const &
        {
            if constexpr (N == 0)
                return first_;
            if constexpr (N == 1)
                return last_;
        }

        template<std::size_t N>
        requires (N < 2)
        [[nodiscard]] constexpr auto&& get() &&
        {
            if constexpr (N == 0)
                return std::move(first_);
            if constexpr (N == 1)
                return std::move(last_);
        }

        /* conversion */

        [[nodiscard]] constexpr string_type str() const
        {
            return { first_, last_ };
        }

        [[nodiscard]] constexpr view_type view() const
        requires std::contiguous_iterator<I>
        {
            return { first_, last_ };
        }

        [[nodiscard]] constexpr explicit(false) operator string_type() const
        {
            return this->str();
        }

        [[nodiscard]] constexpr explicit(false) operator view_type() const
        requires std::contiguous_iterator<I>
        {
            return this->view();
        }

#if __cpp_lib_ranges_as_const >= 202311L
        [[nodiscard]] constexpr explicit(false) operator submatch<const_iterator>() const &
        requires (not std::same_as<const_iterator, iterator>)
        {
            return { first_, last_ };
        }

        [[nodiscard]] constexpr explicit(false) operator submatch<const_iterator>() &&
        requires (not std::same_as<const_iterator, iterator>)
        {
            return { std::move(first_), std::move(last_) };
        }
#endif

        /* operators */

        [[nodiscard]] friend constexpr bool operator==(const submatch& lhs, const submatch& rhs)
        {
            return (lhs and rhs)
                   ? std::ranges::equal(lhs, rhs)
                   : lhs.matched() == rhs.matched();
        }

        [[nodiscard]] friend constexpr bool operator==(const submatch& sub, const view_type view)
        {
            return sub
                   ? std::ranges::equal(sub, view)
                   : false;
        }

        [[nodiscard]] friend constexpr auto operator<=>(const submatch& lhs, const submatch& rhs)
        {
            return (lhs and rhs)
                   ? std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())
                   : lhs.matched() <=> rhs.matched();
        }

        [[nodiscard]] friend constexpr auto operator<=>(const submatch& sub, const view_type view)
        {
            return sub
                   ? std::lexicographical_compare_three_way(sub.begin(), sub.end(), view.begin(), view.end())
                   : std::strong_ordering::less;
        }

        template<typename CharT, typename Traits>
        [[nodiscard]] friend std::basic_ostream<CharT, Traits> operator<<(std::basic_ostream<CharT, Traits>& os, const submatch& sub)
        {
            if constexpr (std::contiguous_iterator<I>)
                return os << sub.view();
            else
                return os << sub.str();
        }

        /* misc */

        void swap(submatch& other) noexcept(std::is_nothrow_swappable_v<I>)
        {
            std::swap(first_, other.first_);
            std::swap(last_, other.last_);
        }

    private:
        static constexpr bool use_bool{ not std::contiguous_iterator<I> };  
        using maybe_bool = detail::maybe_type_t<use_bool, bool>;

        constexpr submatch(I first, I last)
            : first_{ std::move(first) }, last_{ std::move(last) }, match_{ true } {}

        friend class detail::submatch_factory<I>;

        I first_{};
        I last_{};

        [[no_unique_address]] maybe_bool match_{ false };
    };
}


/* structured binding support for submatch */

template<std::bidirectional_iterator I>
struct std::tuple_size<rx::submatch<I>> : integral_constant<std::size_t, 2> {};

template<std::size_t N, std::bidirectional_iterator I>
requires (N < 2)
struct std::tuple_element<N, rx::submatch<I>>
{
    using type = rx::submatch<I>::const_iterator;
};


/* formatting support for submatch */

template<std::bidirectional_iterator I>
inline constexpr auto std::format_kind<rx::submatch<I>> = std::range_format::string;

static_assert(std::formattable<rx::submatch<const char*>, char>);


/* submatch factory implementation */

namespace rx::detail
{
    template<std::bidirectional_iterator I>
    class submatch_factory
    {
    public:
        [[nodiscard]] static constexpr submatch<I> make_submatch(I first, I last)
        {
            return { first, last };
        }
    };
}
