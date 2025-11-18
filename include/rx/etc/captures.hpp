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
        using capture_number_t  = std::uint_least16_t;
    
        struct pair_entry
        {
            tag_number_t    tag_number;
            int             offset;         /* note: offset should normally be negative */
        };

        using tag_pair_t = std::pair<pair_entry, pair_entry>;
        using tag_remapper_t = std::flat_map<tag_number_t, tag_number_t>;

        using key_type = capture_number_t;
        using value_type = tag_pair_t;

        constexpr capture_info()
        {
            keys_.emplace_back(0);
            values_.emplace_back(pair_entry{ .tag_number = start_of_input_tag, .offset = 0 }, pair_entry{ .tag_number = end_of_input_tag, .offset = 0 });
        }

        constexpr void insert(capture_number_t cap, tag_number_t lhs, tag_number_t rhs)
        {
            if (cap == 0)
                throw std::invalid_argument("Cannot insert capture with number 0");

            const auto key_it{ std::ranges::upper_bound(keys_, cap) };
            const auto value_it{ std::ranges::begin(values_) + std::distance(std::ranges::begin(keys_), key_it) };

            keys_.emplace(key_it, cap);
            values_.emplace(value_it, pair_entry{ .tag_number = lhs, .offset = 0 }, pair_entry{ .tag_number = rhs, .offset = 0 });
        }

        constexpr void set_match_start_tag(tag_number_t value)
        {
            values_.at(0).first.tag_number = value;
        }

        [[nodiscard]] constexpr capture_number_t capture_count() const
        {
            auto key_copy{ keys_ };
            auto [last, _]{ std::ranges::unique(key_copy) };
            return std::saturate_cast<capture_number_t>(std::ranges::distance(std::ranges::begin(key_copy), last));
        }

        [[nodiscard]] constexpr std::pair<bool, bool> capture_side(tag_number_t tag) const
        {
            static constexpr auto compose = [](const auto& g, const auto& f) {
                return [=]<typename T>(T&& arg) { 
                    return std::invoke(g, std::invoke(f, std::forward<T>(arg)));
                };
            };

            /* for some reason, this doesn't seem to work? */
            return { std::ranges::contains(values_, tag, compose(&pair_entry::tag_number, &tag_pair_t::first)),
                     std::ranges::contains(values_, tag, compose(&pair_entry::tag_number, &tag_pair_t::second)) };
        }

        [[nodiscard]] constexpr auto lookup(capture_number_t cap) const
        {
            auto [key_beg, key_end]{ std::ranges::equal_range(keys_, cap) };

            return std::ranges::subrange{ std::ranges::begin(values_) + std::distance(std::ranges::begin(keys_), key_beg),
                                          std::ranges::begin(values_) + std::distance(std::ranges::begin(keys_), key_end) };
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
                if (auto it{ map.find(val.first.tag_number) }; it != map.end())
                {
                    val.first.offset += it->second.offset;
                    val.first.tag_number = it->second.tag_number;
                }

                if (auto it{ map.find(val.second.tag_number) }; it != map.end())
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
            auto [tmp_beg, tmp_end]{ std::ranges::unique(set) };
            set.erase(tmp_beg, tmp_end);
            std::erase_if(set, [](tag_number_t n) { return n < 0; });

            for (tag_number_t i{ 0 }; std::cmp_less(i, set.size()); ++i)
                remapper[set.at(i)] = i;

            for (auto& val : values_)
            {
                if (auto it{ remapper.find(val.first.tag_number) }; it != remapper.end())
                    val.first.tag_number = it->second;

                if (auto it{ remapper.find(val.second.tag_number) }; it != remapper.end())
                    val.second.tag_number = it->second;
            }

            return remapper;
        }

    private:
        std::vector<capture_number_t> keys_;
        std::vector<tag_pair_t> values_;
    };
}