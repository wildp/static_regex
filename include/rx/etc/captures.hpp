#pragma once

#include <cstdint>
#include <flat_map>
#include <numeric>
#include <ranges>
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
            int             offset;         /* note: offset should be positive */
        };

        using tag_pair_t = std::pair<pair_entry, pair_entry>;

        // We use a temporary workaround since constexpr flat_map has been implemented while constexpr flat_map hasn't
#if __cpp_lib_constexpr_flat_map >= 202502L
        using capture_map_t = std::flat_multimap<std::size_t, tag_pair_t>;
#else
        using capture_map_t = std::flat_map<std::size_t, std::vector<tag_pair_t>>;
#endif

        constexpr void insert(capture_number_t cap, tag_number_t lhs, tag_number_t rhs)
        {
#if __cpp_lib_constexpr_flat_map >= 202502L
            data_.emplace(cap, std::make_pair(pair_entry{ .tag_number = lhs, .offset = 0 },
                                              pair_entry{ .tag_number = rhs, .offset = 0 }));
#else
            data_[cap].emplace_back(std::make_pair(pair_entry{ .tag_number = lhs, .offset = 0 },
                                                   pair_entry{ .tag_number = rhs, .offset = 0 }));
#endif
        }

        [[nodiscard]] constexpr capture_number_t capture_count() const
        {
#if __cpp_lib_constexpr_flat_map >= 202502L
            auto tmp{ data_.keys() };
            auto [last, _]{ std::ranges::unique(tmp) };
            return std::saturate_cast<capture_number_t>(std::ranges::distance(std::ranges::begin(tmp), last));
#else
            return std::saturate_cast<capture_number_t>(data_.keys().size());
#endif
        }

        [[nodiscard]] constexpr std::pair<bool, bool> capture_side(tag_number_t tag) const
        {
            constexpr auto compose = [](const auto& g, const auto& f) {
                return [=]<typename T>(T&& arg) { 
                    return std::invoke(g, std::invoke(f, std::forward<T>(arg)));
                };
            };

#if __cpp_lib_constexpr_flat_map >= 202502L
            const auto& ref{ data_.values() };
#else
            const auto ref{ data_.values() | std::views::join };
#endif
            /* for some reason, this doesn't seem to work? */
            // return { std::ranges::contains(ref, tag, compose(&pair_entry::tag_number, &tag_pair_t::first)),
            //          std::ranges::contains(ref, tag, compose(&pair_entry::tag_number, &tag_pair_t::second)) };

            bool is_lhs{ false };
            bool is_rhs{ false };

            for (const auto& val : ref)
            {
                if (std::invoke(compose(&pair_entry::tag_number, &tag_pair_t::first), val) == tag)
                    is_lhs = true;
                if (std::invoke(compose(&pair_entry::tag_number, &tag_pair_t::second), val) == tag)
                    is_rhs = true;
            }

            return std::make_pair(is_lhs, is_rhs);
        }

        [[nodiscard]] constexpr auto lookup(capture_number_t cap) const
        {
#if __cpp_lib_constexpr_flat_map >= 202502L
            return data_.equal_range(cap);
#else
            // temporary but semantically different implementation
            const auto& tmp{ data_.at(cap) };
            return std::make_pair(std::ranges::begin(tmp), std::ranges::end(tmp));
#endif
        }

    private:
        capture_map_t data_;
    };
}