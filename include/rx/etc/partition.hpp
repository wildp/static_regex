#pragma once

#include <algorithm>
#include <concepts>
#include <utility>
#include <vector>


// TODO: implement partitioning with contents?
// TODO: make paritioning even more generic
// TODO: rewrite to only erase once at end of function

namespace rx::detail
{
    template<std::integral T>
    constexpr void partition_v1(std::vector<std::pair<T, T>>& vec)
    {
        using interval_t = std::pair<T, T>;
        using vec_t = std::vector<interval_t>;

        constexpr auto re_sort_lookahead = [](vec_t& vec, std::size_t current_idx)
        {
            if (auto it{ vec.begin() + current_idx + 1 }; it + 1 != vec.end())
            {
                if (*it >= *(it + 1))
                {
                    /* re-sort lookahead */

                    auto pos{ std::ranges::lower_bound(it + 1, vec.end(), *it) };

                    if (pos == vec.end() or *pos != *it)
                    {
                        std::ranges::rotate(it, it + 1, pos);
                    }
                    else
                    {
                        /* remove lookahead as a duplicate */
                        vec.erase(it);
                    }
                }         
            }
        };

        constexpr auto re_sort_lookahead_and_insert = [](vec_t& vec, std::size_t current_idx, interval_t to_insert)
        {
            if (auto it{ vec.begin() + current_idx + 1 }; it + 1 != vec.end())
            {
                if (*it >= *(it + 1))
                {
                    /* re-sort lookahead */
                    auto pos{ std::ranges::lower_bound(it + 1, vec.end(), *it) };
                    if (pos == vec.end() or *pos != *it)
                    {
                        auto rot{ std::ranges::rotate(it, it + 1, pos) };

                        /* insert new pair */
                        auto pos2{ std::ranges::upper_bound(std::ranges::begin(rot) + 1, vec.end(), to_insert) };
                        if (pos2 == vec.end() or *pos2 != to_insert)
                            vec.insert(pos2, to_insert);
                    }
                    else
                    {
                        /* remove lookahead as a duplicate */
                        
                        if (to_insert >= *(it + 1))
                        { 
                            auto pos2{ std::ranges::lower_bound(it + 1, vec.end(), to_insert) };
                            if (pos2 == vec.end() or *pos2 != to_insert)
                            {
                                *it = to_insert;
                                std::ranges::rotate(it, it + 1, pos2);
                            }
                            else
                            {
                                /* remove to_insert as a duplicate */
                                vec.erase(it);
                            }
                        }
                        else
                        {
                            *it = to_insert;
                        }
                    }
                }
                else
                {
                    auto pos2{ std::ranges::lower_bound(it + 1, vec.end(), to_insert) };
                    if (pos2 == vec.end() or *pos2 != to_insert)
                        vec.insert(pos2, to_insert);
                }
            }
            else
            {
                /* lookahead is last element in vec; append new pair*/
                if (to_insert > vec.back())
                {
                    vec.emplace(vec.end(), to_insert);
                }
                else if (to_insert < vec.back())
                {
                    vec.emplace(vec.end() - 1, to_insert);
                }
                else
                {
                    /* lookahead is same as element to insert */
                }
            }
        };

        std::ranges::sort(vec);
        auto to_erase{ std::ranges::unique(vec) };
        vec.erase(to_erase.begin(), to_erase.end());

        for (std::size_t i{ 0 }; i + 1 < vec.size();)
        {
            auto& [current_lower, current_upper]{ vec.at(i) };
            auto& [lookahead_lower, lookahead_upper]{ vec.at(i + 1) };

            if (current_lower == lookahead_lower)
            {
                /* current_upper < lookahead_upper
                 * because vec is sorted and has unique elements */

                /* -----
                 * ---------
                 */

                lookahead_lower = current_upper + 1;

                re_sort_lookahead(vec, i);
            }
            else if (current_upper >= lookahead_lower)
            {
                /* current and lookahead overlap
                 * and current_lower < lookahead_lower */

                if (current_upper == lookahead_upper)
                {
                    /* -----------
                     *      ------
                     */

                    current_upper = lookahead_lower - 1; 

                    re_sort_lookahead(vec, i);
                }
                else if (current_upper < lookahead_upper)
                {
                    /* ------
                     *    ---------
                     */

                    interval_t to_insert{ current_upper + 1, lookahead_upper };
                    lookahead_upper = current_upper;
                    current_upper = lookahead_lower - 1;

                    re_sort_lookahead_and_insert(vec, i, to_insert);
                }
                else
                {
                    /* --------------
                     *    --------
                     */

                    interval_t to_insert{ lookahead_upper + 1, current_upper };
                    current_upper = lookahead_lower - 1;
                    
                    re_sort_lookahead_and_insert(vec, i, to_insert);
                }
            }
            else
            {
                ++i;
            }
        }
    }

    template<std::integral T, typename U>
    struct partition_entry
    {
        std::pair<T, T> range;
        std::vector<U>  data;

        constexpr explicit partition_entry(T lower, T upper) : range{ lower, upper } {}
    };

    template<std::integral T, typename U>
    constexpr void partition_v2(std::vector<partition_entry<T, U>>& vec)
    {
        using vec_t = std::vector<partition_entry<T, U>>;

        constexpr auto re_sort_lookahead = [](vec_t& vec, std::size_t current_idx)
        {
            if (auto it{ vec.begin() + current_idx + 1 }; it + 1 != vec.end())
            {
                if (it->range >= (it + 1)->range)
                {
                    /* re-sort lookahead */

                    auto pos{ std::ranges::lower_bound(it + 1, vec.end(), it->range, std::ranges::less{}, &partition_entry<T,U>::range) };

                    if (pos == vec.end() or pos->range != it->range)
                    {
                        std::ranges::rotate(it, it + 1, pos);
                    }
                    else
                    {
                        /* transfer contents and remove lookahead as a duplicate */
                        it->data.append_range(pos->data);
                        std::swap(it->data, pos->data);
                        vec.erase(it);
                    }
                }         
            }
        };

        constexpr auto re_sort_lookahead_and_insert = [](vec_t& vec, std::size_t current_idx, partition_entry<T, U> to_insert)
        {
            if (auto it{ vec.begin() + current_idx + 1 }; it + 1 != vec.end())
            {
                if (it->range >= (it + 1)->range)
                {
                    /* re-sort lookahead */
                    auto pos{ std::ranges::lower_bound(it + 1, vec.end(), it->range, std::ranges::less{}, &partition_entry<T,U>::range) };
                    if (pos == vec.end() or pos->range != it->range)
                    {
                        auto rot{ std::ranges::rotate(it, it + 1, pos) };

                        /* insert new pair */
                        auto pos2{ std::ranges::upper_bound(std::ranges::begin(rot) + 1, vec.end(), to_insert.range, std::ranges::less{}, &partition_entry<T,U>::range) };

                        if (pos2 == vec.end() or pos2->range != to_insert.range)
                        {
                            vec.insert(pos2, std::move(to_insert));
                        }
                        else
                        {
                            /* merge to_insert with pos */
                            pos2->data.insert(pos2->data.begin(), std::make_move_iterator(to_insert.data.begin()), std::make_move_iterator(to_insert.data.end()));
                        }
                    }
                    else
                    {
                        /* remove lookahead as a duplicate */
                        pos->data.insert(pos->data.begin(), std::make_move_iterator(it->data.begin()), std::make_move_iterator(it->data.end()));

                        if (to_insert.range >= (it + 1)->range)
                        { 
                            auto pos2{ std::ranges::lower_bound(it + 1, vec.end(), to_insert.range, std::ranges::less{}, &partition_entry<T, U>::range) };
                            if (pos2 == vec.end() or pos2->range != to_insert.range)
                            {
                                *it = std::move(to_insert);
                                std::ranges::rotate(it, it + 1, pos2);
                            }
                            else
                            {
                                /* remove to_insert as a duplicate */
                                pos2->data.insert(pos2->data.begin(), std::make_move_iterator(to_insert.data.begin()), std::make_move_iterator(to_insert.data.end()));
                                vec.erase(it);
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
                    /* lookahead is correctly ordered */

                    auto pos2{ std::ranges::lower_bound(it + 1, vec.end(), to_insert.range, std::ranges::less{}, &partition_entry<T, U>::range) };
                    if (pos2 == vec.end() or pos2->range != to_insert.range)
                    {
                        vec.insert(pos2, to_insert);
                    }
                    else
                    {
                        /* merge lookahead into existing entry*/
                        pos2->data.insert(pos2->data.begin(), std::make_move_iterator(to_insert.data.begin()), std::make_move_iterator(to_insert.data.end()));
                    }
                }
            }
            else
            {
                /* lookahead is last element in vec; append new entry */
                if (to_insert.range > vec.back().range)
                {
                    vec.emplace(vec.end(), std::move(to_insert));
                }
                else if (to_insert.range < vec.back().range)
                {
                    vec.emplace(vec.end() - 1, std::move(to_insert));
                }
                else
                {
                    /* lookahead is same as element to insert; merge */
                    vec.back().data.insert(vec.back().data.begin(), std::make_move_iterator(to_insert.data.begin()), std::make_move_iterator(to_insert.data.end()));
                }
            }
        };

        std::ranges::stable_sort(vec, std::ranges::less{}, &partition_entry<T,U>::range);

        for (auto it{ vec.begin() }; it != vec.end(); ++it)
        {
            const auto duplicate_begin{ std::next(it) };
            auto duplicate_it{ duplicate_begin };

            for (; duplicate_it != vec.end() and duplicate_it->range == it->range; ++duplicate_it)
                it->data.insert(it->data.end(), std::make_move_iterator(duplicate_it->data.begin()), std::make_move_iterator(duplicate_it->data.end()));

            if (duplicate_it != duplicate_begin)
                vec.erase(duplicate_begin, duplicate_it);
        }

        // TODO: implement the part below

        for (std::size_t i{ 0 }; i + 1 < vec.size();)
        {
            auto& [current_lower, current_upper]{ vec.at(i).range };
            auto& [lookahead_lower, lookahead_upper]{ vec.at(i + 1).range };

            if (current_lower == lookahead_lower)
            {
                /* current_upper < lookahead_upper
                 * because vec is sorted and has unique elements */

                /* -----
                 * ---------
                 */

                 lookahead_lower = current_upper + 1;
                 vec.at(i).data.append_range(vec.at(i + 1).data);

                re_sort_lookahead(vec, i);
            }
            else if (current_upper >= lookahead_lower)
            {
                /* current and lookahead overlap
                 * and current_lower < lookahead_lower */

                if (current_upper == lookahead_upper)
                {
                    /* -----------
                     *      ------
                     */

                    current_upper = lookahead_lower - 1;
                    vec.at(i + 1).data.insert_range(vec.at(i + 1).data.begin(), vec.at(i).data);

                    re_sort_lookahead(vec, i);
                }
                else if (current_upper < lookahead_upper)
                {
                    /* ------
                     *    ---------
                     */

                    partition_entry<T, U> to_insert{ current_upper, lookahead_upper };
                    to_insert.range.first += 1;
                    lookahead_upper = current_upper;
                    current_upper = lookahead_lower - 1;
                    to_insert.data.assign_range(vec.at(i + 1).data);
                    vec.at(i + 1).data.insert_range(vec.at(i + 1).data.begin(), vec.at(i).data);


                    re_sort_lookahead_and_insert(vec, i, to_insert);
                }
                else
                {
                    /* --------------
                     *    --------
                     */

                    partition_entry<T, U> to_insert{ lookahead_upper, current_upper };
                    to_insert.range.first += 1;
                    current_upper = lookahead_lower - 1;
                    to_insert.data.assign_range(vec.at(i).data);
                    vec.at(i + 1).data.insert_range(vec.at(i + 1).data.begin(), vec.at(i).data);
                    
                    re_sort_lookahead_and_insert(vec, i, to_insert);
                }
            }
            else
            {
                ++i;
            }
        }
    }
}