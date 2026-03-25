// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <cstdint>
#include <flat_map>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <vector>


namespace rx::detail
{
    using tag_number_t = int; /* note: tags must be non-negative */
    inline constexpr tag_number_t start_of_input_tag{ std::numeric_limits<tag_number_t>::min() };
    inline constexpr tag_number_t end_of_input_tag{ std::numeric_limits<tag_number_t>::min() + 1 };

    class capture_info
    {
    public:
        using capture_number_t = std::uint_least16_t;

        struct pair_entry
        {
            tag_number_t tag_number;
            int          offset;     /* note: offset should normally be negative */
        };

        using tag_pair_t     = std::pair<pair_entry, pair_entry>;
        using tag_remapper_t = std::flat_map<tag_number_t, tag_number_t>;
        using staging_info_t = std::flat_map<tag_number_t, tag_number_t>;

        using key_type   = capture_number_t;
        using value_type = tag_pair_t;

        constexpr capture_info()
        {
            keys_.emplace_back(0);
            values_.emplace_back(pair_entry{ .tag_number = start_of_input_tag, .offset = 0 }, pair_entry{ .tag_number = end_of_input_tag, .offset = 0 });
        }

        constexpr void insert(capture_number_t cap, tag_number_t lhs, tag_number_t rhs)
        {
            if (cap == 0)
                throw std::invalid_argument("capture_info::insert: cannot insert capture with number 0");

            const auto key_it = std::ranges::upper_bound(keys_, cap);
            const auto value_it = values_.begin() + (key_it - keys_.begin());

            keys_.emplace(key_it, cap);
            values_.emplace(value_it, pair_entry{ .tag_number = lhs, .offset = 0 }, pair_entry{ .tag_number = rhs, .offset = 0 });
        }

        constexpr void set_match_start_tag(tag_number_t value)
        {
            values_.at(0).first.tag_number = value;
        }

        [[nodiscard]] constexpr capture_number_t capture_count() const
        {
            auto key_copy = keys_;
            auto [last, _] = std::ranges::unique(key_copy);
            return std::saturate_cast<capture_number_t>(last - key_copy.begin());
        }

        [[nodiscard]] constexpr std::pair<bool, bool> capture_side(tag_number_t tag) const
        {
            static constexpr auto compose = [](const auto& g, const auto& f) {
                return [=]<typename T>(T&& arg) {
                    return std::invoke(g, std::invoke(f, std::forward<T>(arg)));
                };
            };

            /* for some reason, this doesn't seem to work? */
            return {
                std::ranges::contains(values_, tag, compose(&pair_entry::tag_number, &tag_pair_t::first)),
                std::ranges::contains(values_, tag, compose(&pair_entry::tag_number, &tag_pair_t::second))
            };
        }

        [[nodiscard]] constexpr auto lookup(capture_number_t cap) const
        {
            auto [key_beg, key_end] = std::ranges::equal_range(keys_, cap);

            return std::ranges::subrange{
                values_.begin() + (key_beg - keys_.begin()),
                values_.begin() + (key_end - keys_.begin())
            };
        }

        [[nodiscard]] constexpr tag_remapper_t remap_tags(const std::flat_map<tag_number_t, pair_entry>& map)
        {
            static constexpr auto compose = [](const auto& g, const auto& f) {
                return [=]<typename T>(T&& arg) {
                    return std::invoke(g, std::invoke(f, std::forward<T>(arg)));
                };
            };

            for (auto& val : values_)
            {
                if (auto it = map.find(val.first.tag_number); it != map.end())
                {
                    val.first.offset += it->second.offset;
                    val.first.tag_number = it->second.tag_number;
                }

                if (auto it = map.find(val.second.tag_number); it != map.end())
                {
                    val.second.offset += it->second.offset;
                    val.second.tag_number = it->second.tag_number;
                }
            }

            tag_remapper_t remapper;

            std::vector<tag_number_t> set;
            set.append_range(values_ | std::views::transform(compose(&pair_entry::tag_number, &tag_pair_t::first)));
            set.append_range(values_ | std::views::transform(compose(&pair_entry::tag_number, &tag_pair_t::second)));
            std::ranges::sort(set);
            auto [tmp_beg, tmp_end] = std::ranges::unique(set);
            set.erase(tmp_beg, tmp_end);
            std::erase_if(set, [](tag_number_t n) { return n < 0; });

            for (tag_number_t i{ 0 }; std::cmp_less(i, set.size()); ++i)
                remapper[set.at(i)] = i;

            for (auto& val : values_)
            {
                if (auto it = remapper.find(val.first.tag_number); it != remapper.end())
                    val.first.tag_number = it->second;

                if (auto it = remapper.find(val.second.tag_number); it != remapper.end())
                    val.second.tag_number = it->second;
            }

            return remapper;
        }

        [[nodiscard]] constexpr tag_remapper_t eliminate_branch_reset()
        {
            tag_remapper_t result;

            for (std::size_t i{ 1 }, i_end{ keys_.size() }; i < i_end; ++i)
            {
                const auto capnum = keys_.at(i - 1);

                if (keys_.at(i) != capnum)
                    continue;

                if (values_[i - 1].first.offset != 0 or values_[i - 1].first.offset != 0)
                    throw std::logic_error("capture_info::get_multitags: tags already optimised");

                const auto first_target = values_[i - 1].first.tag_number;
                const auto second_target = values_[i - 1].second.tag_number;

                result.emplace(values_[i].first.tag_number, first_target);
                result.emplace(values_[i].second.tag_number, second_target);

                while (i < i_end and keys_.at(i) == capnum)
                {
                    result.emplace(values_[i].first.tag_number, first_target);
                    result.emplace(values_[i].second.tag_number, second_target);

                    if (values_[i].first.offset != 0 or values_[i].first.offset != 0)
                        throw std::logic_error("capture_info::get_multitags: tags already optimised");

                    values_[i].first.tag_number = first_target;
                    values_[i].second.tag_number = second_target;

                    ++i;
                };
            }

            auto zv = std::views::zip(keys_, values_);
            auto [it, _] = std::ranges::unique(zv, {}, [](const auto& v) -> decltype(auto) { return get<0>(v); });
            auto dist = std::ranges::distance(std::ranges::begin(zv), it);
            keys_.erase(keys_.begin() + dist, keys_.end());
            values_.erase(values_.begin() + dist, values_.end());

            return result;
        }

        [[nodiscard]] constexpr staging_info_t get_staged_tags() const
        {
            staging_info_t result;

            for (const auto& [lhs , rhs] : values_)
                if (lhs.offset == 0 and lhs.tag_number != start_of_input_tag)
                    result.emplace(lhs.tag_number, rhs.tag_number);

            return result;
        }

    private:
        std::vector<capture_number_t> keys_;
        std::vector<tag_pair_t> values_;
    };
}
