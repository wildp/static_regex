#pragma once

#include <optional>
#include <ranges>
#include <stdexcept>
#include <variant>
#include <vector>

#include "rx/etc/captures.hpp"
#include <rx/etc/cdarray.hpp>
#include <rx/etc/util.hpp>
#include <rx/fsm/tdfa.hpp>


namespace rx::testing
{
    template<typename CharT>
    class tdfa_matcher : public detail::tagged_dfa<CharT>
    {
    public:
        using tag_result = std::vector<std::size_t>;
        using detail::tagged_dfa<CharT>::tagged_dfa;

        template<std::random_access_iterator I>
        requires (std::convertible_to<std::iter_value_t<I>, CharT>)
        [[nodiscard]] constexpr std::optional<tag_result> submatches(I first, I last) const;

        template<std::ranges::random_access_range R>
        requires (std::convertible_to<std::ranges::range_value_t<R>, CharT>)
        [[nodiscard]] constexpr std::optional<tag_result> submatches(R&& r) const
        {
            return submatches(std::ranges::begin(r), std::ranges::end(r));
        }

        template<std::random_access_iterator I>
        requires (std::convertible_to<std::iter_value_t<I>, CharT>)
        [[nodiscard]] constexpr bool match(I first, I last) const
        {
            return submatches(first, last).has_value();
        }

        template<std::ranges::random_access_range R>
        requires (std::convertible_to<std::ranges::range_value_t<R>, CharT>)
        [[nodiscard]] constexpr bool match(R&& r) const
        {
            return match(std::ranges::begin(r), std::ranges::end(r));
        }
    };
    
    /* tagged dfa simulation */

    template<typename CharT>
    template<std::random_access_iterator I>
    requires (std::convertible_to<std::iter_value_t<I>, CharT>)
    constexpr auto tdfa_matcher<CharT>::submatches(const I first, const I last) const -> std::optional<tag_result>
    {
        using namespace rx::detail;

        std::vector<I> registers(this->reg_count());
        std::vector<bool> registers_enabled(this->reg_count(), false);

        std::size_t next_state{ this->match_start };

        I it{ first };
        
        while (it != last)
        {
            const auto& state{ this->get_node(next_state) };
            bool fail{ true };

            for (const auto& t : state.tr)
            {
                if (t.lower <= *it and *it <= t.upper)
                {
                    next_state = t.next;

                    for (const auto& op : this->get_regops(t.op_index))
                    {
                        if (auto* set{ std::get_if<tdfa::regop::set>(&op.op) }; set != nullptr)
                        {
                            if (set->val) registers.at(op.dst) = it;
                            registers_enabled.at(op.dst) = set->val;
                        }
                        else if (auto* copy{ std::get_if<tdfa::regop::copy>(&op.op) }; copy != nullptr)
                        {
                            registers.at(op.dst) = registers.at(copy->src);
                            registers_enabled.at(op.dst) = registers_enabled.at(copy->src);
                        }
                        else
                        {
                            throw std::runtime_error("Unknown error");
                        }
                    }

                    ++it;

                    fail = false;
                    break;
                }
            }

            if (fail)
                return {}; /* no transition with symbol */
        }

        const auto& fsi{ this->final_nodes() };

        if (not fsi.contains(next_state))
            return {}; /* state is not an accepting state */

        for (const auto& op : this->get_regops(fsi.at(next_state)))
        {
            if (auto* set{ std::get_if<tdfa::regop::set>(&op.op) }; set != nullptr)
            {
                if (set->val) registers.at(op.dst) = it;
                registers_enabled.at(op.dst) = set->val;
            }
            else if (auto* copy{ std::get_if<tdfa::regop::copy>(&op.op) }; copy != nullptr)
            {
                registers.at(op.dst) = registers.at(copy->src);
                registers_enabled.at(op.dst) = registers_enabled.at(copy->src);
            }
            else
            {
                throw std::runtime_error("Unknown error");
            }
        }
        
        /* convert from tag registers to captures */

        std::vector<std::size_t> res;
        const capture_info& ci{ this->get_capture_info() };
        const auto& final_reg{ this->final_registers() };

        using namespace rx::detail;
    
        auto f = [&](const capture_info::tag_pair_t& p) -> bool {
            return not ((p.first.tag_number >= 0 and not registers_enabled.at(final_reg.at(p.first.tag_number)))
                            or (p.second.tag_number >= 0 and not registers_enabled.at(final_reg.at(p.second.tag_number))));
        };

        auto t = [&](const capture_info::tag_pair_t& p) -> std::pair<I, I> {
            return { std::next((p.first.tag_number >= 0) ? registers.at(final_reg.at(p.first.tag_number))
                                : ((p.first.tag_number == start_of_input_tag) ? first : last), p.first.offset),
                     std::next((p.second.tag_number >= 0) ? registers.at(final_reg.at(p.second.tag_number))
                                : ((p.second.tag_number == start_of_input_tag) ? first : last), p.second.offset) };
        };

        for (std::size_t i{ 0 }; i < ci.capture_count(); ++i)
        {
            const auto [beg, end]{ ci.lookup(i) };

            auto rng{ std::ranges::subrange(beg, end) | std::views::filter(f)
                                                      | std::views::transform(t)
                                                      | std::ranges::to<std::vector>() };
                    
            if (std::ranges::size(rng) == 0)
            {
                res.insert(res.end(), { detail::no_tag, detail::no_tag });
                continue;
            }

            auto max_elem{ std::ranges::max_element(rng, std::ranges::less{}, &std::pair<I, I>::first) };
            auto [bit, blast]{ *max_elem };

            res.push_back(std::distance(first, bit));
            res.push_back(std::distance(first, blast));
        }

        return res;
    }
}