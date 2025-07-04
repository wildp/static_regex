module;

#include <flat_map>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <variant>
#include <vector>

export module rx.tests:tdfa_matcher;

// import std;
import rx.fsm;
import rx.util;

namespace rx::testing
{
    export template<typename CharT>
    class tdfa_matcher : public detail::tagged_dfa<CharT>
    {
    public:
        using tag_result = std::vector<std::size_t>;
        using detail::tagged_dfa<CharT>::tagged_dfa;

        template<std::input_iterator I, std::sentinel_for<I> S>
        requires (std::convertible_to<std::iter_value_t<I>, CharT>)
        [[nodiscard]] constexpr std::optional<tag_result> submatches(I first, S last) const;

        template<std::ranges::input_range R>
        requires (std::convertible_to<std::ranges::range_value_t<R>, CharT>)
        [[nodiscard]] constexpr std::optional<tag_result> submatches(R&& r) const
        {
            return submatches(std::ranges::begin(r), std::ranges::end(r));
        }

        template<std::input_iterator I, std::sentinel_for<I> S>
        requires (std::convertible_to<std::iter_value_t<I>, CharT>)
        [[nodiscard]] constexpr bool match(I first, S last) const
        {
            return submatches(first, last).has_value();
        }

        template<std::ranges::input_range R>
        requires (std::convertible_to<std::ranges::range_value_t<R>, CharT>)
        [[nodiscard]] constexpr bool match(R&& r) const
        {
            return match(std::ranges::begin(r), std::ranges::end(r));
        }
    };
    
    /* tagged dfa simulation */

    template<typename CharT>
    template<std::input_iterator I, std::sentinel_for<I> S>
    requires (std::convertible_to<std::iter_value_t<I>, CharT>)
    constexpr auto tdfa_matcher<CharT>::submatches(const I first, const S last) const -> std::optional<tag_result>
    {
        using namespace rx::detail;

        std::vector<I> registers(this->reg_count());
        std::vector<bool> registers_enabled(this->reg_count(), false);

        /* while tags and captures are still linked,
         * assign the first tag to the start of the input */
        registers.at(0) = first;
        registers_enabled.at(0) = true;
        /* end */

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

                    for (const auto& op : t.ops)
                    {
                        if (auto* set{ std::get_if<regop::set>(&op.op) }; set != nullptr)
                        {
                            if (set->val) registers.at(op.dst) = it;
                            registers_enabled.at(op.dst) = set->val;
                        }
                        else if (auto* copy{ std::get_if<regop::copy>(&op.op) }; copy != nullptr)
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

        for (const auto& op : fsi.at(next_state))
        {
            if (auto* set{ std::get_if<regop::set>(&op.op) }; set != nullptr)
            {
                if (set->val) registers.at(op.dst) = it;
                registers_enabled.at(op.dst) = set->val;
            }
            else if (auto* copy{ std::get_if<regop::copy>(&op.op) }; copy != nullptr)
            {
                registers.at(op.dst) = registers.at(copy->src);
                registers_enabled.at(op.dst) = registers_enabled.at(copy->src);
            }
            else
            {
                throw std::runtime_error("Unknown error");
            }
        }
        
        /* convert from vector of it to vector of size_t */

        std::vector<std::size_t> result(this->tag_count(), no_tag);
        
        for (std::size_t i{ 0 }; i < this->tag_count(); ++i)
            if (registers_enabled.at(i + this->tag_count()))
                result.at(i) = std::ranges::distance(first, registers.at(i + this->tag_count()));

        return result;
    }
}