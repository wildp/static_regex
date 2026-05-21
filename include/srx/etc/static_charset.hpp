// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <concepts>
#include <limits>
#include <meta>
#include <ranges>
#include <type_traits>

#include "srx/etc/bitcharset.hpp"
#include "srx/etc/charset.hpp"
#include "srx/etc/static_span.hpp"
#include "srx/etc/util.hpp"


namespace srx::detail
{
    template<typename CharT>
    class static_charset;

    template<typename T, typename CharT>
    concept charset_like = one_of<T, charset<CharT>, static_charset<CharT>>;

    template<typename T>
    concept unconstrained_charset_like = template_instantiation_of<T, charset> or template_instantiation_of<T, static_charset>;

    template<typename CharT>
    class static_charset
    {
    public:
        using charset_type = charset<CharT>;
        using char_type = charset_type::char_type;
        using char_interval = charset_type::char_interval;

        consteval static_charset() noexcept = default;

        consteval explicit static_charset(const charset_type& cs)
            : data_{ cs.data_ } {}

        consteval explicit static_charset(const bitcharset<CharT>& bcs)
            requires (sizeof(CharT) == 1)
            : data_{ bcs.get_intervals() } {}

        template<typename... Args>
            requires (sizeof...(Args) >= 1) and ((std::convertible_to<Args, char_type> or std::convertible_to<Args, char_interval>) and ...)
        constexpr explicit static_charset(Args... args)
        {
            charset_type tmp;

            template for (constexpr std::size_t i : std::views::iota(0uz, sizeof...(Args)))
            {
                if constexpr (std::convertible_to<Args...[i], char_type>)
                    tmp.insert(args...[i]);
                else if constexpr (std::convertible_to<Args...[i], char_interval>)
                    tmp.insert(args...[i].first, args...[i].second);
            }

            data_ = static_span{ tmp.data_ };
        }

        constexpr auto make_unsigned() const noexcept requires std::signed_integral<CharT>
        {
            using uchar_type = std::make_unsigned_t<char_type>;
            charset<uchar_type> result;

            static constexpr uchar_type umin{ std::numeric_limits<uchar_type>::min() };
            static constexpr uchar_type umax{ std::numeric_limits<uchar_type>::max() };
            static constexpr char_type threshold{ static_cast<char_type>(umin) };

            for (const auto& [first, last] : data_)
            {
                if (first == last)
                {
                    result.insert(static_cast<uchar_type>(first));
                }
                else if (first < threshold and last >= threshold)
                {
                    result.insert(static_cast<uchar_type>(first), umax);
                    result.insert(umin, static_cast<uchar_type>(last));
                }
                else
                {
                    result.insert(static_cast<uchar_type>(first), static_cast<uchar_type>(last));
                }
            }

            return result;
        }


        /* observers */

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            return data_.empty();
        }

        [[nodiscard]] constexpr bool full() const noexcept
        {
            return data_.size() == 1
                   and data_[0].first == std::numeric_limits<CharT>::min()
                   and data_[0].second == std::numeric_limits<CharT>::max();
        }

        [[nodiscard]] constexpr std::size_t count() const noexcept
        {
            std::size_t result{ 0 };
            for (const auto& [first, second] : data_)
                result += (second + 1 - first);
            return result;
        }

        [[nodiscard]] constexpr std::size_t interval_count() const noexcept
        {
            return data_.size();
        }

        [[nodiscard]] constexpr int score_intervals() const noexcept
        {
            int score{ 0 };

            for (const auto& [beg, end] : data_)
            {
                if (beg == end)
                    score += 1;
                else
                    score += 2;
            }

            if (not data_.empty())
            {
                if (const auto& [beg, end] = data_.front(); beg == std::numeric_limits<char_type>::min() and end != std::numeric_limits<char_type>::min())
                    --score;
                if (const auto& [beg, end] = data_.back(); end == std::numeric_limits<char_type>::max() and beg != std::numeric_limits<char_type>::max())
                    --score;
            }

            return score;
        }

        [[nodiscard]] constexpr bool should_invert() const noexcept
        {
            /* an extremely simple heuristic (this could probably be improved?) */
            return data_.size() > 0
                   and data_.front().first == std::numeric_limits<CharT>::min()
                   and data_.back().second == std::numeric_limits<CharT>::max();
        }

        [[nodiscard]] constexpr bool contains(char_type c) const
        {
            const auto it = std::ranges::lower_bound(data_, c, {}, &char_interval::second);
            if (it == data_.end())
                return false;
            return c >= it->first;
        }

        [[nodiscard]] constexpr const static_span<char_interval>& get_intervals() const noexcept
        {
            return data_;
        }


        /* operators */

        constexpr explicit(false) operator charset_type() const
        {
            return charset_type{ typename charset_type::underlying_t(std::from_range, data_) };
        }

        constexpr charset_type operator~() const
        {
            return charset_type{ charset_type::make_absolute_complement(data_) };
        }

        friend constexpr charset_type operator&(const charset_like<char_type> auto& lhs, const charset_like<char_type> auto& rhs)
        {
            return charset_type{ charset_type::make_intersection(lhs.data_, rhs.data_) };
        }

        friend constexpr charset_type operator|(const charset_like<char_type> auto& lhs, const charset_like<char_type> auto& rhs)
        {
            return charset_type{ charset_type::make_union(lhs.data_, rhs.data_) };
        }

        friend constexpr charset_type operator^(const charset_like<char_type> auto& lhs, const charset_like<char_type> auto& rhs)
        {
            return charset_type{ charset_type::make_symmetric_difference(lhs.data_, rhs.data_) };
        }

        friend constexpr charset_type operator-(const charset_like<char_type> auto& lhs, const charset_like<char_type> auto& rhs)
        {
            return charset_type{ charset_type::make_relative_complement(lhs.data_, rhs.data_) };
        }

        friend constexpr bool operator==(const charset_like<char_type> auto& lhs, const charset_like<char_type> auto& rhs)
        {
            return std::ranges::equal(lhs.data_, rhs.data_);
        };

        friend constexpr auto operator<=>(const charset_like<CharT> auto& lhs, const charset_like<CharT> auto& rhs)
        {
            return std::lexicographical_compare_three_way(lhs.data_.begin(), lhs.data_.end(), rhs.data_.begin(), rhs.data_.end());
        }


        /* additional operators for charset */

        friend constexpr charset_type& operator&=(charset_type& lhs, const static_charset& rhs)
        {
            lhs.data_ = std::move(charset_type::make_intersection(lhs.data_, rhs.data_));
            return lhs;
        }

        friend constexpr charset_type& operator|=(charset_type& lhs, const static_charset& rhs)
        {
            lhs.data_ = std::move(charset_type::make_union(lhs.data_, rhs.data_));
            return lhs;
        }

        friend constexpr charset_type& operator^=(charset_type& lhs, const static_charset& rhs)
        {
            lhs.data_ = std::move(charset_type::make_symmetric_difference(lhs.data_, rhs.data_));
            return lhs;
        }

        friend constexpr charset_type& operator-=(charset_type& lhs, const static_charset& rhs)
        {
            lhs.data_ = std::move(charset_type::make_relative_complement(lhs.data_, rhs.data_));
            return lhs;
        }

        static_span<char_interval> data_;
    };


    template<typename T>
    struct nontransient_constexpr_version_of {};

    template<typename CharT>
    struct nontransient_constexpr_version_of<bitcharset<CharT>>
    {
        using type = bitcharset<CharT>;
    };

    template<typename CharT>
    struct nontransient_constexpr_version_of<charset<CharT>>
    {
        using type = static_charset<CharT>;
    };

    template<typename CharT>
    struct nontransient_constexpr_version_of<static_charset<CharT>>
    {
        using type = static_charset<CharT>;
    };

    template<typename T>
    using nontransient_constexpr_version_of_t = nontransient_constexpr_version_of<T>::type;
}
