#pragma once

#include <optional>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <variant>
#include <vector>

#include <rx/etc/captures.hpp>
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
        [[nodiscard]] constexpr std::optional<tag_result> match(I first, I last) const
        {
            return match_implementation(first, last, false);
        }

        template<std::ranges::random_access_range R>
        requires (std::convertible_to<std::ranges::range_value_t<R>, CharT>)
        [[nodiscard]] constexpr std::optional<tag_result> match(R&& r) const
        {
            return match(std::ranges::begin(r), std::ranges::end(r));
        }

        [[nodiscard]] constexpr std::optional<tag_result> match(const CharT* cstr) const
        {
            return match(std::basic_string_view{ cstr });
        }

        template<std::random_access_iterator I>
        requires (std::convertible_to<std::iter_value_t<I>, CharT>)
        [[nodiscard]] constexpr std::optional<tag_result> partial_match(I first, I last) const
        {
            return match_implementation(first, last, true);
        }

        template<std::ranges::random_access_range R>
        requires (std::convertible_to<std::ranges::range_value_t<R>, CharT>)
        [[nodiscard]] constexpr std::optional<tag_result> partial_match(R&& r) const
        {
            return partial_match(std::ranges::begin(r), std::ranges::end(r));
        }

        [[nodiscard]] constexpr std::optional<tag_result> partial_match(const CharT* cstr) const
        {
            return partial_match(std::basic_string_view{ cstr });
        }

    private:
        static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };

        template<std::random_access_iterator I>
        requires (std::convertible_to<std::iter_value_t<I>, CharT>)
        [[nodiscard]] constexpr std::optional<tag_result> match_implementation(I first, I last, bool enable_fallback) const;

        template<std::random_access_iterator I>
        requires (std::convertible_to<std::iter_value_t<I>, CharT>)
        constexpr void regops_implementation(I it, std::size_t op_index, std::vector<I>& registers, std::vector<bool>& registers_enabled) const;
        
    };

    template<typename CharT>
    tdfa_matcher(const detail::tagged_nfa<CharT>&) -> tdfa_matcher<CharT>;
    
    
    /* tagged dfa simulation */

    template<typename CharT>
    template<std::random_access_iterator I>
    requires (std::convertible_to<std::iter_value_t<I>, CharT>)
    constexpr auto tdfa_matcher<CharT>::match_implementation(const I first, const I last, const bool enable_fallback) const -> std::optional<tag_result>
    {
        using namespace rx::detail;

        std::vector<I> registers(this->reg_count());
        std::vector<bool> registers_enabled(this->reg_count(), false);

        std::size_t next_state{ this->match_start };
        std::size_t fallback_state{ fallback_disabled };

        I it{ first };
        I fallback_it{ last };
        
        while (true)
        {
            if (enable_fallback and this->fallback_nodes().contains(next_state))
            {
                fallback_state = next_state;
                fallback_it = it;
            }

            if (it == last)
            {
                if (this->final_nodes().contains(next_state))
                {
                    regops_implementation(it, this->final_nodes().at(next_state).op_index, registers, registers_enabled);
                    it -= this->final_nodes().at(next_state).final_offset;
                    break;  /* outer */
                }
            }
            else
            {
                bool success{ false };

                for (const auto& t : this->get_node(next_state).tr)
                {
                    if (t.cs.contains(*it))
                    {
                        next_state = t.next;
                        regops_implementation(it, t.op_index, registers, registers_enabled);
                        ++it;
                        success = true;
                        break; /* inner */
                    }
                }

                if (success)
                    continue; /* outer */
            }

            if (not enable_fallback or fallback_state == fallback_disabled)
                return {}; /* skip converting tag registers to captures */
            
            it = fallback_it;
            regops_implementation(it, this->fallback_nodes().at(fallback_state).op_index, registers, registers_enabled);
            it -= this->final_nodes().at(fallback_state).final_offset;
            break; /* outer */
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
                               : ((p.first.tag_number == start_of_input_tag) ? first : it), p.first.offset),
                     std::next((p.second.tag_number >= 0) ? registers.at(final_reg.at(p.second.tag_number))
                               : ((p.second.tag_number == start_of_input_tag) ? first : it), p.second.offset) };
        };

        for (std::size_t i{ 0 }; i < ci.capture_count(); ++i)
        {
            auto rng{ ci.lookup(i) | std::views::filter(f)
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

    template<typename CharT>
    template<std::random_access_iterator I>
    requires (std::convertible_to<std::iter_value_t<I>, CharT>)
    constexpr void tdfa_matcher<CharT>::regops_implementation(I it, std::size_t op_index, std::vector<I>& registers, std::vector<bool>& registers_enabled) const
    {
        for (const auto& op : this->get_regops(op_index))
        {
            if (auto* set{ std::get_if<detail::tdfa::regop::set>(&op.op) }; set != nullptr)
            {
                if (set->val) registers.at(op.dst) = it;
                registers_enabled.at(op.dst) = set->val;
            }
            else if (auto* copy{ std::get_if<detail::tdfa::regop::copy>(&op.op) }; copy != nullptr)
            {
                registers.at(op.dst) = registers.at(copy->src);
                registers_enabled.at(op.dst) = registers_enabled.at(copy->src);
            }
            else
            {
                throw std::runtime_error("Unknown error");
            }
        }
    }

}