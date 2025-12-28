#pragma once

#include <algorithm>
#include <cstddef>
#include <flat_map>
#include <iterator>
#include <limits>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>


namespace rx::detail
{
    template<typename T, typename CharT>
    concept charset_interval_range = std::ranges::contiguous_range<T>
                                     and std::same_as<std::ranges::range_value_t<T>, std::pair<CharT, CharT>>;

    template<typename CharT>
    class charset
    {
    public:
        using char_type = CharT;
        using char_interval = std::pair<char_type, char_type>;
        
        consteval charset() noexcept = default;


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

        [[nodiscard]] constexpr const std::vector<char_interval>& get_intervals() const noexcept
        {
            return data_;
        }


        /* modifiers */

        constexpr void clear() noexcept
        {
            data_.clear();
        }

        constexpr void negate()
        {
            data_ = std::move(make_absolute_complement(data_));
        }

        constexpr void insert(char_type c)
        {
            this->insert_single(c);
        }

        constexpr void insert(char_type first, char_type last)
        {
            this->merge_into(this->insert_single(first), last);
        }


        /* operators */

        constexpr charset& operator&=(const charset& other)
        {
            if (this != &other)
                data_ = std::move(make_intersection(data_, other.data_));
            return *this;
        }

        constexpr charset& operator|=(const charset& other)
        {
            if (this != &other)
                data_ = std::move(make_union(data_, other.data_));
            return *this;
        }

        constexpr charset& operator^=(const charset& other)
        {
            if (this != &other)
                data_ = std::move(make_symmetric_difference(data_, other.data_));
            else
                data_.clear();
            return *this;
        }

        constexpr charset& operator-=(const charset& other)
        {
            if (this != &other)
                data_ = std::move(make_relative_complement(data_, other.data_));
            else
                data_.clear();
            return *this;
        }

        friend constexpr charset operator&(const charset& lhs, const charset& rhs)
        {
            if (&lhs == &rhs)
                return lhs;
            return charset{ make_intersection(lhs.data_, rhs.data_) };
        }

        friend constexpr charset operator|(const charset& lhs, const charset& rhs)
        {
            if (&lhs == &rhs)
                return lhs;
            return charset{ make_union(lhs.data_, rhs.data_) };
        }

        friend constexpr charset operator^(const charset& lhs, const charset& rhs)
        {
            if (&lhs == &rhs)
                return lhs;
            return charset{ make_symmetric_difference(lhs.data_, rhs.data_) };
        }

        friend constexpr charset operator-(const charset& lhs, const charset& rhs)
        {
            if (&lhs == &rhs)
                return charset{};
            return charset{ make_relative_complement(lhs.data_, rhs.data_) };
        }

        friend constexpr charset operator~(const charset& cs)
        {
            return charset{ make_absolute_complement(cs.data_) };
        }

        friend constexpr bool operator==(const charset&, const charset&) = default;
        friend constexpr auto operator<=>(const charset&, const charset&) = default; /* largely meaningless but necessary */


        /* partition functions and type aliases */

        using ref = std::reference_wrapper<const charset>;

        template<typename T>
        using ref_pair = std::pair<ref, T>;

        using partition_result = std::vector<charset>;

        template<typename T>
        using partition_pair_result = std::vector<std::pair<charset, std::vector<T>>>;

        template<typename T>
        using partition_contents_result = std::vector<std::vector<T>>;
     
        [[nodiscard]] static constexpr auto partition(const std::vector<ref>& input) -> partition_result;

        template<typename T>
        [[nodiscard]] static constexpr auto partition_ext(const std::vector<ref_pair<T>>& input) -> partition_pair_result<T>;

        template<typename T>
        [[nodiscard]] static constexpr auto partition_contents(const std::vector<ref_pair<T>>& input) -> partition_contents_result<T>;


        /* friends :) */

        template<typename CT>
        friend class static_charset;

    private:
        using underlying_t = std::vector<char_interval>;
        using partition_entry = std::pair<char_interval, std::vector<bool>>;
        using partitioned_intervals = std::vector<partition_entry>;

        constexpr explicit charset(underlying_t&& data) : data_{ std::move(data) } {}

        constexpr underlying_t::iterator insert_single(char_type c);
        constexpr underlying_t::iterator merge_into(underlying_t::iterator inserted, char_type c);

        static constexpr underlying_t make_intersection(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs);
        static constexpr underlying_t make_union(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs);
        static constexpr underlying_t make_symmetric_difference(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs);
        static constexpr underlying_t make_relative_complement(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs);
        static constexpr underlying_t make_absolute_complement(const charset_interval_range<char_type> auto& und);

        static constexpr void part_sort_lookahead(partitioned_intervals& part, std::size_t current_idx);
        static constexpr void part_sort_lookahead_and_insert(partitioned_intervals& part, std::size_t current_idx, partition_entry&& to_insert);
        static constexpr void part_sort_and_dedup(partitioned_intervals& sorted_part);
        static constexpr void part_merge_intervals(partitioned_intervals& sorted_part);
        static constexpr auto part_make_map(const partitioned_intervals& part) -> std::flat_map<std::vector<bool>, charset>;

        underlying_t data_;
    };


    template<typename CharT>
    constexpr auto charset<CharT>::insert_single(const char_type c) -> underlying_t::iterator
    {
        if (data_.empty())
            return data_.emplace(data_.end(), c, c);

        /* form a closed interval */
        auto lower_bound{ data_.begin() };
        auto upper_bound{ std::ranges::prev(data_.end()) };

        while (true)
        {
            auto midpoint{ lower_bound + (std::ranges::distance(lower_bound, upper_bound) / 2)};

            if (midpoint->first != std::numeric_limits<char_type>::min() and c + 1 == midpoint->first)
            {
                /* extend range of pair by 1 */
                midpoint->first -= 1;

                /* attempt to merge with (mid - 1) */
                if (midpoint != data_.begin())
                {
                    if (auto prev{ std::ranges::prev(midpoint) }; midpoint->first - prev->second <= 1)
                    {
                        midpoint->first = prev->second;
                        midpoint = data_.erase(prev);
                    }
                }

                return midpoint;
            }
            else if (midpoint->second != std::numeric_limits<char_type>::max() and c == midpoint->second + 1)
            {
                /* extend range of pair by 1 */
                midpoint->second += 1;

                /* attempt to merge with (mid + 1) */
                if (auto next{ std::ranges::next(midpoint) }; next != data_.end() and next->first - midpoint->second <= 1)
                {
                    midpoint->second = next->second;
                    data_.erase(next);
                }
                
                return midpoint;
            }
            else if (c < midpoint->first)
            {
                if (midpoint == lower_bound)
                {
                    /* insert pair before mid */
                    return data_.emplace(midpoint, c, c);
                }
                else
                {
                    /* continue search */
                    upper_bound = std::ranges::prev(midpoint);
                }

            }
            else if (c > midpoint->second)
            {
                if (midpoint == upper_bound)
                {
                    /* insert pair after mid */
                    return data_.emplace(std::ranges::next(midpoint), c, c);
                }
                else
                {
                    /* continue search */
                    lower_bound = std::ranges::next(midpoint);
                }
            }
            else /* (lower <= c and c <= upper) */
            {
                /* c is already in char class */
                return midpoint;
            }
        }
    }

    template<typename CharT>
    constexpr auto charset<CharT>::merge_into(underlying_t::iterator inserted, const char_type c) -> underlying_t::iterator
    {
        /* assume inserted != data_.end() */

        if (inserted->second >= c)
            return inserted; /* no need to merge */
        
        inserted->second = c;
        
        /* attempt to merge */

        while (true)
        {
            auto next{ std::ranges::next(inserted) };

            if (next == data_.end())
                break; /* inserted is at end of data_: can't merge */

            if (c >= next->second)
            {
                /* range of (inserted) is a superset of (next): erase (next) */
                data_.erase(next);
            }
            else if (c + 1 >= next->first)
            {
                /* we don't need to worry about signed integer overflow here, since if
                * `last == numeric_limits<CharT>::max()`, then `next->first == next->second`
                * is true and so the previous branch (`last >= next->second`) is taken instead */

                /* ranges of (inserted) and (next) partially overlap or are adjacent: merge */
                inserted->second = next->second;
                data_.erase(next);
                break;
            }
            else
            {
                /* ranges of (inserted) and (next) do not overlap and are not adjacent: can't merge */
                break;
            }
        }

        return inserted;
    }

    template<typename CharT>
    constexpr auto charset<CharT>::make_intersection(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs) -> underlying_t
    {
        underlying_t result;

        auto lit{ std::ranges::cbegin(lhs) };
        auto rit{ std::ranges::cbegin(rhs) };
        const auto lend{ std::ranges::cend(lhs) };
        const auto rend{ std::ranges::cend(rhs) };

        while (lit != lend and rit != rend)
        {
            if (lit->second < rit->first)
            {
                ++lit;
            }
            else if (rit->second < lit->first)
            {
                ++rit;
            }
            else
            {
                const char_type first{ std::max(lit->first, rit->first) };

                if (lit->second == rit->second)
                {
                    result.emplace_back(first, lit->second);
                    ++lit, ++rit;
                }
                else if (lit->second < rit->second)
                {
                    result.emplace_back(first, lit->second);
                    ++lit;
                }
                else /* (lit->second > rit->second) */
                {
                    result.emplace_back(first, rit->second);
                    ++rit;
                }
            }
        }

        return result;
    }

    template<typename CharT>
    constexpr auto charset<CharT>::make_union(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs) -> underlying_t
    {
        underlying_t result;

        std::ranges::merge(lhs, rhs, std::back_inserter(result), {}, &char_interval::first, &char_interval::first);

        for (auto it{ result.begin() }; it != result.end();)
        {
            const auto erase_begin{ std::ranges::next(it) };
            auto next{ erase_begin };

            for (; next != result.end(); ++next)
            {
                if (it->second != std::numeric_limits<char_type>::max() and it->second + 1 < next->first)
                    break; /* no overlap */ 
                
                if (it->second < next->second)
                    it->second = next->second;
            }

            it = result.erase(erase_begin, next);
        }

        return result;
    }

    template<typename CharT>
    constexpr auto charset<CharT>::make_symmetric_difference(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs) -> underlying_t
    {
        if (lhs.empty())
            return underlying_t{ std::from_range, rhs };
        if (rhs.empty())
            return underlying_t{ std::from_range, lhs };

        underlying_t result;

        auto lit{ std::ranges::cbegin(lhs) };
        auto rit{ std::ranges::cbegin(rhs) };
        const auto lend{ std::ranges::cend(lhs) };
        const auto rend{ std::ranges::cend(rhs) };

        std::optional<char_type> tmp;

        while (lit != lend and rit != rend)
        {
            const auto [min_first, max_first]{ std::minmax(lit->first, rit->first) };
            const auto min_first_or_tmp{ tmp.value_or(min_first) }; 

            const auto [min_second, max_second]{ std::minmax(lit->second, rit->second) };
            auto& smaller_it{ (lit->second < rit->second) ? lit : rit };

            if (min_second < max_first)
            {

                if (not result.empty() and result.back().second + 1 == min_first_or_tmp)
                    result.back().second = min_second;
                else
                    result.emplace_back(min_first_or_tmp, min_second);

                tmp.reset();
                std::ranges::advance(smaller_it, 1);
            }
            else 
            {
                /* lit and rit overlap */

                if (min_first_or_tmp < max_first)
                {   
                    if (not result.empty() and result.back().second + 1 == min_first_or_tmp)
                        result.back().second = max_first - 1;
                    else
                        result.emplace_back(min_first_or_tmp, max_first - 1);
                }

                if (min_second < max_second)
                {
                    tmp = min_second + 1; /* tmp <= max_second on next iteration */
                    std::ranges::advance(smaller_it, 1);
                }
                else /* (min_second == max_second) */
                {
                    tmp.reset();
                    std::ranges::advance(lit, 1);
                    std::ranges::advance(rit, 1);
                }
            }
        }

        if (lit != lend or rit != rend)
        {
            auto it{ (lit != lend) ? lit : rit };
            const auto end{ (lit != lend) ? lend : rend };
            const auto min_first_or_tmp{ tmp.value_or(it->first) };

            if (not result.empty() and result.back().second + 1 == min_first_or_tmp)
                result.back().second = it->second;
            else
                result.emplace_back(min_first_or_tmp, it->second);

            std::ranges::advance(it, 1);

            while (it != end)
            {
                result.emplace_back(it->first, it->second);
                std::ranges::advance(it, 1);
            }
        }

        return result;
    }

    template<typename CharT>
    constexpr auto charset<CharT>::make_relative_complement(const charset_interval_range<char_type> auto& lhs, const charset_interval_range<char_type> auto& rhs) -> underlying_t
    {
        if (rhs.empty())
            return underlying_t{ std::from_range, lhs };

        underlying_t result;

        auto lit{ lhs.cbegin() };
        auto rit{ rhs.cbegin() };
        const auto lend{ lhs.cend() };
        const auto rend{ rhs.cend() };

        char_type rhs_lower{ std::numeric_limits<char_type>::min() };

        while (lit != lend)
        {
            char_type rhs_upper{};

            if (rit == rend)
            {
                if (rhs_lower == std::numeric_limits<char_type>::min())
                    break; /* reached end of complement of rhs */
                else
                    rhs_upper = std::numeric_limits<char_type>::max();
            }
            else if (rhs_lower >= rit->first)
            {
                ++rit;
                continue;
            }
            else
            {
                rhs_upper = rit->first - 1;
            }

            bool advance_rit{ false };

            if (lit->second < rhs_lower)
            {
                ++lit;
            }
            else if (rhs_upper < lit->first)
            {
                advance_rit = true;
            }
            else
            {
                const char_type first{ std::max(lit->first, rhs_lower) };

                if (lit->second == rhs_upper)
                {
                    result.emplace_back(first, lit->second);
                    ++lit;
                    advance_rit = true;
                }
                else if (lit->second < rhs_upper)
                {
                    result.emplace_back(first, lit->second);
                    ++lit;
                }
                else /* (lit->second > rhs_upper) */
                {
                    result.emplace_back(first, rhs_upper);
                    advance_rit = true;
                }
            }

            if (advance_rit)
            {
                if (rit == rend)
                    break; /* reached end of complement of rhs */

                rhs_lower = static_cast<char_type>(static_cast<std::make_unsigned_t<char_type>>(rit->second) + 1);
                ++rit;
            }
        }

        return result;
    }

    template<typename CharT>
    constexpr auto charset<CharT>::make_absolute_complement(const charset_interval_range<char_type> auto& und) -> underlying_t
    {
        underlying_t result;
        char_type tmp{ std::numeric_limits<char_type>::min() };

        for (const auto& [lower, upper] : und)
        {
            if (tmp < lower)
                result.emplace_back(tmp, lower - 1);

            /* avoid UB by casting to unsigned for addition */
            tmp = static_cast<char_type>(static_cast<std::make_unsigned_t<char_type>>(upper) + 1);
        }

        if (tmp != std::numeric_limits<char_type>::min() or std::ranges::empty(und))
            result.emplace_back(tmp, std::numeric_limits<char_type>::max());

        return result;
    }

    template<typename CharT>
    constexpr void charset<CharT>::part_sort_lookahead(partitioned_intervals& part, std::size_t current_idx)
    {
        if (auto it{ part.begin() + current_idx + 1 }; it + 1 != part.end())
        {
            if (*it >= *(it + 1))
            {
                /* re-sort lookahead */
                const auto pos{ std::ranges::lower_bound(it + 1, part.end(), it->first, {}, &partition_entry::first) };
                if (pos == part.end() or *pos != *it)
                {
                    std::ranges::rotate(it, it + 1, pos);
                }
                else
                {
                    /* remove lookahead as a duplicate */
                    part.erase(it);
                }
            }         
        }
    }

    template<typename CharT>
    constexpr void charset<CharT>::part_sort_lookahead_and_insert(partitioned_intervals& part, std::size_t current_idx, partition_entry&& to_insert)
    {
        if (auto it{ part.begin() + current_idx + 1 }; it + 1 != part.end())
        {
            if (*it >= *(it + 1))
            {
                /* re-sort lookahead */
                const auto pos{ std::ranges::lower_bound(it + 1, part.end(), it->first, {}, &partition_entry::first) };
                if (pos == part.end() or *pos != *it)
                {
                    const auto rot{ std::ranges::rotate(it, it + 1, pos) };

                    /* insert new pair */
                    const auto pos2{ std::ranges::upper_bound(std::ranges::begin(rot) + 1, part.end(), to_insert) };
                    if (pos2 == part.end() or *pos2 != to_insert)
                        part.emplace(pos2, std::move(to_insert));
                }
                else
                {
                    /* remove lookahead as a duplicate */
                    if (to_insert >= *(it + 1))
                    { 
                        const auto pos2{ std::ranges::lower_bound(it + 1, part.end(), to_insert.first, {}, &partition_entry::first) };
                        if (pos2 == part.end() or pos2->first != to_insert.first)
                        {
                            *it = std::move(to_insert);
                            std::ranges::rotate(it, it + 1, pos2);
                        }
                        else
                        {
                            /* remove to_insert as a duplicate */
                            part.erase(it);
                        }
                    }
                    else
                    {
                        *it = std::move(to_insert);
                    }
                }
            }
            else
            {
                const auto pos2{ std::ranges::lower_bound(it + 1, part.end(), to_insert.first, {},  &partition_entry::first) };
                if (pos2 == part.end() or pos2->first != to_insert.first)
                    part.emplace(pos2, std::move(to_insert));
            }
        }
        else
        {
            /* lookahead is last element in vec; append new pair*/
            if (to_insert.first > part.back().first)
            {
                part.emplace(part.end(), std::move(to_insert));
            }
            else if (to_insert.first < part.back().first)
            {
                part.emplace(part.end() - 1, std::move(to_insert));
            }
            else
            {
                /* lookahead is same as element to insert */
            }
        }
    };

    template<typename CharT>
    constexpr void charset<CharT>::part_sort_and_dedup(partitioned_intervals& part)
    {
        std::ranges::sort(part, {}, &partition_entry::first);
        
        for (auto it{ part.begin() }; it != part.end(); ++it)
        {
            const auto duplicate_begin{ std::next(it) };
            auto duplicate_it{ duplicate_begin };

            for (; duplicate_it != part.end() and duplicate_it->first == it->first; ++duplicate_it)
                for (std::size_t j{ 0 }, j_max{ std::min(it->second.size(), duplicate_it->second.size()) }; j < j_max; ++j)
                    it->second[j] = it->second[j] or duplicate_it->second[j];

            if (duplicate_it != duplicate_begin)
                part.erase(duplicate_begin, duplicate_it);
        }
    }

    template<typename CharT>
    constexpr void charset<CharT>::part_merge_intervals(partitioned_intervals& part)
    {
        for (std::size_t i{ 0 }; i + 1 < part.size();)
        {
            auto& [current, current_mask]{ part[i] };
            auto& [lookahead, lookahead_mask]{ part[i + 1] };

            if (current.first == lookahead.first)
            {
                /* current.second <= lookahead.second
                 * because vec is sorted and has unique elements */

                if (current.second < lookahead.second)
                {
                    /* -----
                     * ---------
                     */

                    lookahead.first = current.second + 1;

                    for (std::size_t j{ 0 }, j_max{ std::min(current_mask.size(), lookahead_mask.size()) }; j < j_max; ++j)
                        current_mask[j] = current_mask[j] or lookahead_mask[j];

                    part_sort_lookahead(part, i);
                }
                else
                {
                    /* ---------
                     * ---------
                     */

                    for (std::size_t j{ 0 }, j_max{ std::min(current_mask.size(), lookahead_mask.size()) }; j < j_max; ++j)
                        current_mask[j] = current_mask[j] or lookahead_mask[j];

                    part.erase(part.begin() + i + 1); /* dedup */
                }
            }
            else if (current.second >= lookahead.first)
            {
                /* current and lookahead overlap
                 * and current.first < lookahead.first */

                if (current.second == lookahead.second)
                {
                    /* -----------
                     *      ------
                     */

                    current.second = lookahead.first - 1; 

                    for (std::size_t j{ 0 }, j_max{ std::min(current_mask.size(), lookahead_mask.size()) }; j < j_max; ++j)
                        lookahead_mask[j] = lookahead_mask[j] or current_mask[j];

                    part_sort_lookahead(part, i);
                }
                else if (current.second < lookahead.second)
                {
                    /* ------
                     *    ---------
                     */

                    partition_entry to_insert{ { current.second + 1, lookahead.second }, lookahead_mask };
                    lookahead.second = current.second;
                    current.second = lookahead.first - 1;

                    for (std::size_t j{ 0 }, j_max{ std::min(current_mask.size(), lookahead_mask.size()) }; j < j_max; ++j)
                        lookahead_mask[j] = lookahead_mask[j] or current_mask[j];

                    part_sort_lookahead_and_insert(part, i, std::move(to_insert));
                }
                else
                {
                    /* --------------
                     *    --------
                     */

                    partition_entry to_insert{ { lookahead.second + 1, current.second }, current_mask };
                    current.second = lookahead.first - 1;

                    for (std::size_t j{ 0 }, j_max{ std::min(current_mask.size(), lookahead_mask.size()) }; j < j_max; ++j)
                        lookahead_mask[j] = lookahead_mask[j] or current_mask[j];

                    part_sort_lookahead_and_insert(part, i, std::move(to_insert));
                }
            }
            else
            {
                ++i;
            }
        }
    }

    template<typename CharT>
    constexpr auto charset<CharT>::part_make_map(const partitioned_intervals& part) -> std::flat_map<std::vector<bool>, charset>
    {
        std::flat_map<std::vector<bool>, charset> map;

        for (const auto& [interval, mask] : part)
        {
            /* since part is already sorted, we can directly insert
             * instead of calling charset::insert() */
            map[mask].data_.emplace_back(interval);
        }

        return map;
    }

    template<typename CharT>
    constexpr auto charset<CharT>::partition(const std::vector<ref>& input) -> partition_result
    {
        if (input.empty())
            return {};
        else if (input.size() == 1)
            return { input.back().get() };

        partitioned_intervals part;

        for (std::size_t i{ 0 }; i < input.size(); ++i)
        {
            std::vector<bool> mask(input.size(), false);
            mask[input.size() - i - 1] = true;
            for (const auto& pair: input[i].get().data_)
                part.emplace_back(pair, mask);
        }

        part_sort_and_dedup(part);
        part_merge_intervals(part);
        const auto map{ part_make_map(part) };

        return map.values();
    }

    template<typename CharT>
    template<typename T>
    constexpr auto charset<CharT>::partition_ext(const std::vector<ref_pair<T>>& input) -> partition_pair_result<T>
    {
        if (input.empty())
            return {};
        else if (input.size() == 1)
            return { { input.back().first, { input.back().second } } };

        partitioned_intervals part;

        for (std::size_t i{ 0 }; i < input.size(); ++i)
        {
            std::vector<bool> mask(input.size(), false);
            mask[input.size() - i - 1] = true;
            for (const auto& pair: input[i].first.get().data_)
                part.emplace_back(pair, mask);
        }

        part_sort_and_dedup(part);
        part_merge_intervals(part);
        auto map{ part_make_map(part) };

        partition_pair_result<T> result;

        for (auto it{ map.begin() }, end{ map.end() }; it != end; ++it)
        {
            result.emplace_back(std::move(it->second), std::vector<T>{});
            for (std::size_t i{ 0 }; i < input.size(); ++i)
                if (it->first.at(input.size() - i - 1))
                    result.back().second.emplace_back(input[i].second);
        }

        return result;
    }


    template<typename CharT>
    template<typename T>
    constexpr auto charset<CharT>::partition_contents(const std::vector<ref_pair<T>>& input) -> partition_contents_result<T>
    {
        if (input.empty())
            return {};
        else if (input.size() == 1)
            return { { input.back().second } };

        partitioned_intervals part;

        for (std::size_t i{ 0 }; i < input.size(); ++i)
        {
            std::vector<bool> mask(input.size(), false);
            mask[input.size() - i - 1] = true;
            for (const auto& pair: input[i].first.get().data_)
                part.emplace_back(pair, mask);
        }

        part_sort_and_dedup(part);
        part_merge_intervals(part);
        const auto map{ part_make_map(part) };

        partition_contents_result<T> result;

        for (auto it{ map.cbegin() }, end{ map.cend() }; it != end; ++it)
        {
            result.emplace_back();
            for (std::size_t i{ 0 }; i < input.size(); ++i)
                if (it->first.at(input.size() - i - 1))
                    result.back().emplace_back(input[i].second);
        }

        return result;
    }
    
}
