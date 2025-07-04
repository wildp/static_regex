module;

#include <algorithm>
#include <concepts>
#include <vector>
#include <utility>

export module rx.util:partition;

// TODO: implement partitioning with contents?
// TODO: make paritioning even more generic
// TODO: rewrite to only erase once at end of function


export namespace rx::detail
{
    template<std::integral T>
    constexpr void partition(std::vector<std::pair<T, T>>& vec)
    {
        using interval_t = std::pair<T, T>;
        using vec_t = std::vector<interval_t>;

        constexpr auto re_sort_lookahead = [](vec_t& vec, std::size_t current_idx)
        {
            if (auto it{ std::ranges::begin(vec) + current_idx + 1 }; it + 1 != std::ranges::end(vec))
            {
                if (*it >= *(it + 1))
                {
                    /* re-sort lookahead */

                    auto pos{ std::ranges::lower_bound(it + 1, std::ranges::end(vec), *it) };

                    if (pos == std::ranges::end(vec) or *pos != *it)
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
            if (auto it{ std::ranges::begin(vec) + current_idx + 1 }; it + 1 != std::ranges::end(vec))
            {
                if (*it >= *(it + 1))
                {
                    /* re-sort lookahead */
                    auto pos{ std::ranges::lower_bound(it + 1, std::ranges::end(vec), *it) };
                    if (pos == std::ranges::end(vec) or *pos != *it)
                    {
                        auto rot{ std::ranges::rotate(it, it + 1, pos) };

                        /* insert new pair */
                        auto pos2{ std::ranges::upper_bound(std::ranges::begin(rot) + 1, std::ranges::end(vec), to_insert) };
                        if (pos2 == std::ranges::end(vec) or *pos2 != to_insert)
                            vec.insert(pos2, to_insert);
                    }
                    else
                    {
                        /* remove lookahead as a duplicate */
                        
                        if (to_insert >= *(it + 1))
                        { 
                            auto pos2{ std::ranges::lower_bound(it + 1, std::ranges::end(vec), to_insert) };
                            if (pos2 == std::ranges::end(vec) or *pos2 != to_insert)
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
                    }
                }
                else
                {
                    auto pos2{ std::ranges::lower_bound(it + 1, std::ranges::end(vec), to_insert) };
                    if (pos2 == std::ranges::end(vec) or *pos2 != to_insert)
                        vec.insert(pos2, to_insert);
                }
            }
            else
            {
                /* lookahead is last element in vec; append new pair*/
                if (to_insert > vec.back())
                {
                    vec.emplace(std::ranges::end(vec), to_insert);
                }
                else if (to_insert < vec.back())
                {
                    vec.emplace(std::ranges::end(vec) - 1, to_insert);
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
}