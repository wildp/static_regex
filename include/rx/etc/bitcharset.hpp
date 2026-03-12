// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <ranges>
#include <utility>
#include <vector>

#include "rx/etc/vec_bool_adaptor.hpp"


namespace rx::detail
{
    template<typename CharT>
    class bitcharset
    {
        using integer_type = std::uint64_t;
        static constexpr std::size_t byte_bits{ std::numeric_limits<unsigned char>::digits };
        static constexpr std::size_t integer_bits{ std::numeric_limits<integer_type>::digits };
        static constexpr std::size_t total_size{ (0b1uz << (sizeof(CharT) * byte_bits)) };
        static constexpr std::size_t array_size{ total_size / integer_bits };
        static constexpr std::size_t min_offset{ (std::is_signed_v<CharT>) ? (array_size / 2) : 0z };
        static constexpr auto offset_max{ static_cast<int>(integer_bits) };

        static constexpr std::size_t acceptable_numbers_of_bits_in_a_byte{ 8 };
        static_assert(byte_bits == acceptable_numbers_of_bits_in_a_byte);
        static_assert(sizeof(CharT) < sizeof(int));

    public:
        using char_type = CharT;
        using char_interval = std::pair<char_type, char_type>;

        consteval bitcharset() noexcept = default;

        template<typename... Args>
        requires (sizeof...(Args) >= 1) and ((std::convertible_to<Args, char_type> or std::convertible_to<Args, char_interval>) and ...)
        constexpr explicit bitcharset(Args... args)
        {
            template for (constexpr std::size_t i : std::views::iota(0uz, sizeof...(Args)))
            {
                if constexpr (std::convertible_to<Args...[i], char_type>)
                    insert(args...[i]);
                else if constexpr (std::convertible_to<Args...[i], char_interval>)
                    insert(args...[i].first, args...[i].second);
            }
        }


        /* observers */

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            integer_type result{ 0uz };
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result |= data_[i];
            return (result == 0);
        }

        [[nodiscard]] constexpr bool full() const noexcept
        {
            std::size_t result{ 0uz };
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result |= ~data_[i];
            return (result == 0);
        }

        [[nodiscard]] constexpr std::size_t count() const noexcept
        {
            std::size_t result{ 0 };
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result += std::popcount(data_[i]);
            return result;
        }

        [[nodiscard]] constexpr std::size_t interval_count() const noexcept
        {
            std::size_t interval_count{ 0 };
            bool carry{ false };

            for (std::size_t i{ 0 }; i < array_size; ++i)
            {
                integer_type tmp{ data_[(i + min_offset) % array_size] };
                int offset{ 0 };

                while (true)
                {
                    const int zeros{ std::countr_zero(tmp) };

                    offset += zeros;

                    if (offset >= offset_max)
                    {
                        carry = false;
                        break;
                    }

                    tmp >>= zeros;

                    const int ones{ std::countr_one(tmp) }; /* note: ones >= 1 is always true */

                    if (not (zeros == 0 and carry))
                        ++interval_count;

                    carry = false;
                    offset += ones;

                    if (offset >= offset_max)
                    {
                        carry = true;
                        break;
                    }

                    tmp >>= ones;
                }
            }

            return interval_count;
        }

        [[nodiscard]] constexpr int score_intervals() const noexcept
        {
            int score{ 0 };
            bool carry{ false };

            for (std::size_t i{ 0 }; i < array_size; ++i)
            {
                integer_type tmp{ data_[(i + min_offset) % array_size] };
                int offset{ 0 };

                while (true)
                {
                    const int zeros{ std::countr_zero(tmp) };

                    offset += zeros;

                    if (offset >= offset_max)
                    {
                        carry = false;
                        break;
                    }

                    tmp >>= zeros;

                    const int ones{ std::countr_one(tmp) }; /* note: ones >= 1 is always true */

                    if (not (zeros == 0 and carry))
                    {
                        if (ones == 1)
                            score += 1;
                        else
                            score += 2;
                    }

                    carry = false;
                    offset += ones;

                    if (offset >= offset_max)
                    {
                        carry = true;
                        break;
                    }

                    tmp >>= ones;
                }
            }

            if (contains(std::numeric_limits<char_type>::min()) and contains(std::numeric_limits<char_type>::min() + 1))
                --score;

            if (contains(std::numeric_limits<char_type>::max()) and contains(std::numeric_limits<char_type>::max() - 1))
                --score;

            return score;
        }

        [[nodiscard]] constexpr bool contains(char_type c) const noexcept
        {
            /* widen to accommodate signed chars */
            const int input{ static_cast<std::make_unsigned_t<char_type>>(c) };

            return data_[input / integer_bits] & (0b1uz << (input % integer_bits));
        }

        [[nodiscard]] constexpr std::vector<char_interval> get_intervals() const
        {
            std::vector<char_interval> result;

            /* widen to allow position to equal std::numeric_limits<char_type>::max() + 1 */
            int position{ std::numeric_limits<char_type>::min() };

            for (std::size_t i{ 0 }; i < array_size; ++i)
            {
                integer_type tmp{ data_[(i + min_offset) % array_size] };
                int offset{ 0 };

                while (true)
                {
                    const int zeros{ std::countr_zero(tmp) };

                    position += std::min(zeros, offset_max - offset);
                    offset += zeros;

                    if (offset >= offset_max)
                        break;

                    tmp >>= zeros;

                    const int ones{ std::countr_one(tmp) }; /* note: ones >= 1 is always true */
                    const auto prev_pos{ position };

                    position += ones;
                    offset += ones;

                    if (not result.empty() and result.back().second == static_cast<char_type>(prev_pos - 1))
                        result.back().second = static_cast<char_type>(position - 1);
                    else
                        result.emplace_back(prev_pos, static_cast<char_type>(position - 1));

                    if (offset >= offset_max)
                        break;

                    tmp >>= ones;
                }
            }

            return result;
        }


        /* modifiers */

        constexpr void clear() noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] = 0;
        }

        constexpr void negate() noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] = ~data_[i];
        }

        constexpr void insert(char_type c) noexcept
        {
            /* widen to accommodate signed chars */
            const int input{ static_cast<std::make_unsigned_t<char_type>>(c) };

            data_[input / integer_bits] |= 0b1uz << (input % integer_bits);
        }

        constexpr void insert(char_type first, char_type last) noexcept /* preconditions: first <= last */
        {
            /* widen to accommodate signed chars and to ensure last + 1 > last */
            const std::size_t beg{ static_cast<std::make_unsigned_t<char>>(first) };
            const std::size_t end{ static_cast<std::make_unsigned_t<char>>(last) + 1u };

            const std::size_t select1{ beg / integer_bits };
            const std::size_t select2{ end / integer_bits };

            std::array<std::size_t, array_size> mask1{};
            mask1[select1] = (0b1uz << (beg % integer_bits)) - 1;
            std::ranges::fill_n(mask1.begin(), select1, ~0uz);

            if constexpr (std::is_signed_v<char_type>)
            {
                if (std::signbit(first) != std::signbit(last))
                    for (std::size_t i{ 0 }; i < array_size; ++i)
                        mask1[i] = ~mask1[i];
            }

            std::array<std::size_t, array_size> mask2{};
            mask2[select2] ^= (0b1uz << (end % integer_bits)) - 1;
            std::ranges::fill_n(mask2.begin(), select2, ~0uz);

            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] |= mask1[i] ^ mask2[i];
        }


        /* operators */

        constexpr bitcharset& operator&=(const bitcharset& other) noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] &= other.data_[i];
            return *this;
        }

        constexpr bitcharset& operator|=(const bitcharset& other) noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] |= other.data_[i];
            return *this;
        }

        constexpr bitcharset& operator^=(const bitcharset& other) noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] ^= other.data_[i];
            return *this;
        }

        constexpr bitcharset& operator-=(const bitcharset& other) noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] &= ~other.data_[i];
            return *this;
        }

        friend constexpr bitcharset operator&(const bitcharset& lhs, const bitcharset& rhs) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = lhs.data_[i] & rhs.data_[i];
            return result;
        }

        friend constexpr bitcharset operator|(const bitcharset& lhs, const bitcharset& rhs) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = lhs.data_[i] | rhs.data_[i];
            return result;
        }

        friend constexpr bitcharset operator^(const bitcharset& lhs, const bitcharset& rhs) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = lhs.data_[i] ^ rhs.data_[i];
            return result;
        }

        friend constexpr bitcharset operator-(const bitcharset& lhs, const bitcharset& rhs) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = lhs.data_[i] & ~rhs.data_[i];
            return result;
        }

        friend constexpr bitcharset operator~(const bitcharset& arg) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = ~arg.data_[i];
            return result;
        }

        friend constexpr bool operator==(const bitcharset&, const bitcharset&) noexcept = default;
        friend constexpr auto operator<=>(const bitcharset&, const bitcharset&) noexcept = default;


        /* additional member functions */

        constexpr void make_ascii_case_insensitive() noexcept requires std::same_as<char_type, char>
        {
            static constexpr int uppercase_beg{ static_cast<std::make_unsigned_t<char_type>>('a') };
            static constexpr int uppercase_end{ static_cast<int>(static_cast<std::make_unsigned_t<char_type>>('z')) + 1 };
            static constexpr int lowercase_beg{ static_cast<std::make_unsigned_t<char_type>>('A') };
            static constexpr int lowercase_end{ static_cast<int>(static_cast<std::make_unsigned_t<char_type>>('Z')) + 1 };

            /* ensure that A-Za-z lies in the same integer_t in bitcharset<char> */
            static constexpr auto index{ uppercase_beg / integer_bits };
            static_assert(uppercase_beg / integer_bits == index);
            static_assert(uppercase_end / integer_bits == index);
            static_assert(lowercase_beg / integer_bits == index);
            static_assert(lowercase_end / integer_bits == index);

            static constexpr integer_type uppercase_mask{ ((0b1uz << (uppercase_beg % integer_bits)) - 1) ^ ((0b1uz << (uppercase_end % integer_bits)) - 1) };
            static constexpr integer_type lowercase_mask{ ((0b1uz << (lowercase_beg % integer_bits)) - 1) ^ ((0b1uz << (lowercase_end % integer_bits)) - 1) };
            static constexpr int uppercase_offset{ std::countr_zero(uppercase_mask) };
            static constexpr int lowercase_offset{ std::countr_zero(lowercase_mask) };

            const integer_type mask1{ (data_[index] & lowercase_mask) >> lowercase_offset };
            const integer_type mask2{ (data_[index] & uppercase_mask) >> uppercase_offset };

            data_[index] |= (mask1 << uppercase_offset);
            data_[index] |= (mask2 << lowercase_offset);
        }


        /* partition functions and type aliases */

        using ref = std::reference_wrapper<const bitcharset>;

        template<typename T>
        using ref_pair = std::pair<ref, T>;

        using partition_result = std::vector<bitcharset>;

        template<typename T>
        using partition_pair_result = std::vector<std::pair<bitcharset, std::vector<T>>>;

        template<typename T>
        using partition_contents_result = std::vector<std::vector<T>>;

        [[nodiscard]] static constexpr auto partition(const std::vector<ref>& input) -> partition_result;

        template<typename T>
        [[nodiscard]] static constexpr auto partition_ext(const std::vector<ref_pair<T>>& input) -> partition_pair_result<T>;

        template<typename T>
        [[nodiscard]] static constexpr auto partition_contents(const std::vector<ref_pair<T>>& input) -> partition_contents_result<T>;

    private:
        std::array<integer_type, array_size> data_{};
    };

    template<typename CharT>
    constexpr auto bitcharset<CharT>::partition(const std::vector<ref>& input) -> partition_result
    {
        if (input.empty())
            return {};
        else if (input.size() == 1)
            return { input.back().get() };

        /* note: partitions.size() >= 1 is always true, since for each iteration we insert
         *       at least one element into next_gen per element in partitions, since at
         *       most one of partitions[i] & val and partitions[i] & ~val will be empty */
        std::vector<bitcharset> partitions(1);
        partitions.back().negate();

        for (const bitcharset& val : input)
        {
            std::vector<bitcharset> next_gen;
            const auto partition_size{ partitions.size() };
            next_gen.reserve(partition_size * 2);

            for (std::size_t i{ 0 }; i < partition_size; ++i)
                if (auto cs{ partitions[i] & val }; not cs.empty())
                    next_gen.emplace_back(cs);

            const auto complement{ ~val };

            for (std::size_t i{ 0 }; i + 1 < partition_size; ++i)
                if (auto cs{ partitions[i] & complement }; not cs.empty())
                    next_gen.emplace_back(cs);

            /* always insert intersection of all complements as last element */
            next_gen.emplace_back(partitions.back() & complement);

            partitions = std::move(next_gen);
        }

        /* remove last element, which corresponds to intersection of all complements */
        partitions.pop_back();

        std::ranges::reverse(partitions); // TEMPORARY: TODO: Remove later

        return partitions;
    }

    template<typename CharT>
    template<typename T>
    constexpr auto bitcharset<CharT>::partition_ext(const std::vector<ref_pair<T>>& input) -> partition_pair_result<T>
    {
        using part_pair = std::pair<bitcharset, bitset_t>;

        if (input.empty())
            return {};
        else if (input.size() == 1)
            return { { input.back().first, { input.back().second } } };

        std::vector<part_pair> partitions(1);
        partitions.back().first.negate();

        for (const auto& [val, _] : input)
        {
            std::vector<part_pair> next_gen;
            next_gen.reserve(partitions.size() * 2);

            const auto complement{ ~val.get() };

            for (const auto& [v, from] : partitions)
            {
                if (auto cs{ v & complement }; not cs.empty())
                {
                    next_gen.emplace_back(cs, from);
                    next_gen.back().second.push_back(false);
                }
            }

            for (const auto& [v, from] : partitions)
            {
                if (auto cs{ v & val.get() }; not cs.empty())
                {
                    next_gen.emplace_back(cs, from);
                    next_gen.back().second.push_back(true);
                }
            }

            partitions = std::move(next_gen);
        }

        partition_pair_result<T> result;
        const bitset_t empty(input.size(), false);

        for (const auto& [v, from] : partitions)
        {
            if (from == empty)
                continue;

            result.emplace_back(v, std::vector<T>{});
            for (std::size_t i{ 0 }, i_end{ input.size() }; i < i_end; ++i)
                if (from.at(i))
                    result.back().second.emplace_back(input[i].second);
        }

        return result;
    }

    template<typename CharT>
    template<typename T>
    constexpr auto bitcharset<CharT>::partition_contents(const std::vector<ref_pair<T>>& input) -> partition_contents_result<T>
    {
        using part_pair = std::pair<bitcharset, bitset_t>;

        if (input.empty())
            return {};
        else if (input.size() == 1)
            return { std::vector<T>{ input.back().second } };

        std::vector<part_pair> partitions(1);
        partitions.back().first.negate();

        for (const auto& [val, _] : input)
        {
            std::vector<part_pair> next_gen;
            next_gen.reserve(partitions.size() * 2);

            const auto complement{ ~val.get() };

            for (const auto& [v, from] : partitions)
            {
                if (auto cs{ v & complement }; not cs.empty())
                {
                    next_gen.emplace_back(cs, from);
                    next_gen.back().second.push_back(false);
                }
            }

            for (const auto& [v, from] : partitions)
            {
                if (auto cs{ v & val.get() }; not cs.empty())
                {
                    next_gen.emplace_back(cs, from);
                    next_gen.back().second.push_back(true);
                }
            }

            partitions = std::move(next_gen);
        }

        std::vector<std::vector<T>> result;
        const bitset_t empty(input.size(), false);

        for (const auto& [v, from] : partitions)
        {
            if (from == empty)
                continue;

            result.emplace_back();
            for (std::size_t i{ 0 }, i_end{ input.size() }; i < i_end; ++i)
                if (from.at(i))
                    result.back().emplace_back(input[i].second);
        }

        return result;
    }
}
