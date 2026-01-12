// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <concepts>
#include <meta>
#include <ranges>

#include <rx/etc/bitcharset.hpp>
#include <rx/etc/charset.hpp>
#include <rx/etc/static_span.hpp>
#include <rx/etc/util.hpp>


namespace rx::detail
{
    template<typename CharT>
    class static_charset;

    template<typename T, typename CharT>
    concept charset_like = one_of<T, charset<CharT>, static_charset<CharT>>;
    
    template<typename CharT>
    class static_charset
    {
    public:
        using charset = charset<CharT>;
        using char_type = charset::char_type;
        using char_interval = charset::char_interval;

        consteval static_charset() noexcept = default;

        consteval explicit static_charset(const charset& cs)
            : data_{ std::define_static_array(cs.data_) } {}

        template<typename... Args>
        requires (sizeof...(Args) >= 1) and ((std::convertible_to<Args, char_type> or std::convertible_to<Args, char_interval>) and ...)
        constexpr explicit static_charset(Args... args)
        {
            charset tmp;

            template for (constexpr std::size_t i : std::views::iota(0uz, sizeof...(Args)))
            {
                if constexpr (std::convertible_to<Args...[i], char_type>)
                    tmp.insert(args...[i]);
                else if constexpr (std::convertible_to<Args...[i], char_interval>)
                    tmp.insert(args...[i].first, args...[i].second);
            }

            data_ = std::define_static_array(tmp.data_);
        }


        /* observers */

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            return data_.empty();
        }

        [[nodiscard]] constexpr std::size_t count() const noexcept
        {
            std::size_t result{ 0 };
            for (const auto [first, second] : data_)
                result += (first + 1 - second);
            return result;
        }

        [[nodiscard]] constexpr bool contains(char_type c) const
        {
            const auto it{ std::ranges::lower_bound(data_, c, {}, &char_interval::second) };
            if (it == data_.end())
                return false;
            return c >= it->first;
        }

        [[nodiscard]] constexpr const static_span<const char_interval>& get_intervals() const noexcept
        {
            return data_;
        }


        /* operators */

        constexpr explicit operator charset() const
        {
            return charset{ typename charset::underlying_t{ std::from_range, data_ } };
        }

        constexpr charset operator~()
        {
            return charset{ charset::make_absolute_complement(data_) };
        }

        friend constexpr charset operator&(const charset_like<char_type> auto& lhs, const charset_like<char_type> auto& rhs)
        {
            return charset{ charset::make_intersection(lhs.data_, rhs.data_) };
        }

        friend constexpr charset operator|(const charset_like<char_type> auto& lhs, const charset_like<char_type> auto& rhs)
        {
            return charset{ charset::make_union(lhs.data_, rhs.data_) };
        }

        friend constexpr charset operator^(const charset_like<char_type> auto& lhs, const charset_like<char_type> auto& rhs)
        {
            return charset{ charset::make_symmetric_difference(lhs.data_, rhs.data_) };
        }

        friend constexpr charset operator-(const charset_like<char_type> auto& lhs, const charset_like<char_type> auto& rhs)
        {
            return charset{ charset::make_relative_complement(lhs.data_, rhs.data_) };
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

        friend constexpr charset& operator&=(charset& lhs, const static_charset& rhs)
        {
            lhs.data_ = std::move(charset::make_intersection(lhs.data_, rhs.data_));
            return lhs;
        }

        friend constexpr charset& operator|=(charset& lhs, const static_charset& rhs)
        {
            lhs.data_ = std::move(charset::make_union(lhs.data_, rhs.data_));
            return lhs;
        }

        friend constexpr charset& operator^=(charset& lhs, const static_charset& rhs)
        {
            lhs.data_ = std::move(charset::make_symmetric_difference(lhs.data_, rhs.data_));
            return lhs;
        }

        friend constexpr charset& operator-=(charset& lhs, const static_charset& rhs)
        {
            lhs.data_ = std::move(charset::make_relative_complement(lhs.data_, rhs.data_));
            return lhs;
        }

    private:
        static_span<const char_interval> data_;
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
