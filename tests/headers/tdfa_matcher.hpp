// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <vector>

#include <srx/etc/captures.hpp>
#include <srx/etc/util.hpp>
#include <srx/fsm/tdfa.hpp>


namespace srx {
namespace testing {

template<typename CharT>
class tdfa_matcher : public detail::tagged_dfa<CharT>
{
public:
    using tag_result = std::vector<std::size_t>;
    using detail::tagged_dfa<CharT>::tagged_dfa;

    template<std::random_access_iterator I>
        requires std::convertible_to<std::iter_value_t<I>, CharT>
    [[nodiscard]] constexpr std::optional<tag_result> match(I first, I last) const
    {
        if (this->is_onepass())
            return match_implementation(first, last, false, this->match_start).first;
        else
            return multipass_implementation(first, last, false, this->match_start).first;
    }

    template<std::ranges::random_access_range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, CharT>
    [[nodiscard]] constexpr std::optional<tag_result> match(R&& r) const
    {
        return match(std::ranges::begin(r), std::ranges::end(r));
    }

    [[nodiscard]] constexpr std::optional<tag_result> match(const CharT* cstr) const
    {
        return match(std::basic_string_view{ cstr });
    }

    template<std::random_access_iterator I>
        requires std::convertible_to<std::iter_value_t<I>, CharT>
    [[nodiscard]] constexpr std::optional<tag_result> partial_match(I first, I last) const
    {
        if (this->is_onepass())
            return match_implementation(first, last, true, this->match_start).first;
        else
            return multipass_implementation(first, last, true, this->match_start).first;
    }

    template<std::ranges::random_access_range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, CharT>
    [[nodiscard]] constexpr std::optional<tag_result> partial_match(R&& r) const
    {
        return partial_match(std::ranges::begin(r), std::ranges::end(r));
    }

    [[nodiscard]] constexpr std::optional<tag_result> partial_match(const CharT* cstr) const
    {
        return partial_match(std::basic_string_view{ cstr });
    }

    template<std::random_access_iterator I>
        requires std::convertible_to<std::iter_value_t<I>, CharT>
    [[nodiscard]] constexpr std::vector<tag_result> match_all(I first, I last) const
    {
        std::vector<tag_result> result;
        auto it = first;
        auto prev_it = first;

        auto ret = (this->is_onepass())
                    ? match_implementation(it, last, true, this->match_start)
                    : multipass_implementation(it, last, true, this->match_start);

        while (ret.first.has_value())
        {
            const auto mfirst = ret.first->at(0);
            const auto mlast = ret.first->at(1);
            std::ranges::advance(it, mlast);

            result.emplace_back(std::move(*ret.first));
            std::ranges::for_each(result.back(), [x = std::ranges::distance(first, prev_it)](auto& v){ v += x; });

            if (ret.second == detail::tdfa::no_continue)
                break;

            detail::tdfa::state_t next_start{};

            if (mfirst != mlast)
                next_start = this->continue_nodes().at(ret.second);
            else if (this->additional_continue_nodes().empty())
                throw std::logic_error("additional_continue_nodes is empty");
            else if (mlast == 0)
                next_start = this->additional_continue_nodes().back();
            else
                next_start = this->additional_continue_nodes().at(ret.second);

            if (this->is_onepass())
                ret = match_implementation(it, last, true, next_start);
            else
                ret = multipass_implementation(it, last, true, next_start);

            prev_it = it;
        }

        return result;
    }

    template<std::ranges::random_access_range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, CharT>
    [[nodiscard]] constexpr std::vector<tag_result> match_all(R&& r) const
    {
        return match_all(std::ranges::begin(r), std::ranges::end(r));
    }

    [[nodiscard]] constexpr std::vector<tag_result> match_all(const CharT* cstr) const
    {
        return match_all(std::basic_string_view{ cstr });
    }

private:
    static constexpr std::size_t fallback_disabled{ std::numeric_limits<std::size_t>::max() };

    using impl_ret_type = std::pair<std::optional<tag_result>, detail::tdfa::continue_at_t>;

    template<std::random_access_iterator I>
        requires std::convertible_to<std::iter_value_t<I>, CharT>
    [[nodiscard]] constexpr impl_ret_type match_implementation(I first, I last, bool enable_fallback, std::size_t start) const;

    template<std::random_access_iterator I>
        requires std::convertible_to<std::iter_value_t<I>, CharT>
    constexpr void regops_implementation(I it, std::size_t op_index, std::vector<I>& registers, std::vector<bool>& registers_enabled) const;

    template<std::random_access_iterator I>
        requires std::convertible_to<std::iter_value_t<I>, CharT>
    [[nodiscard]] constexpr impl_ret_type multipass_implementation(I first, I last, bool enable_fallback, std::size_t start) const;
};

template<typename CharT>
tdfa_matcher(const detail::tagged_nfa<CharT>&) -> tdfa_matcher<CharT>;


/* tagged dfa simulation */

template<typename CharT>
template<std::random_access_iterator I>
    requires std::convertible_to<std::iter_value_t<I>, CharT>
constexpr auto tdfa_matcher<CharT>::match_implementation(const I first, const I last, const bool enable_fallback, const std::size_t start) const -> impl_ret_type
{
    using namespace srx::detail;

    std::vector<I> registers(this->reg_count());
    std::vector<bool> registers_enabled(this->reg_count(), false);

    std::size_t next_state{ start };
    std::size_t fallback_state{ fallback_disabled };

    I it{ first };
    I fallback_it{ last };

    auto continue_at = tdfa::no_continue;

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
                const auto& fni = this->final_nodes().at(next_state);
                regops_implementation(it, fni.op_index, registers, registers_enabled);
                it -= fni.offset;
                if (this->fallback_nodes().contains(next_state))
                    continue_at = this->fallback_nodes().at(next_state).continue_at;
                break; /* outer */
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

            if (const auto& dt = this->get_node(next_state).default_tr; dt.has_value())
            {
                next_state = dt->next;
                regops_implementation(it, dt->op_index, registers, registers_enabled);
                ++it;
                continue; /* outer */
            }
        }

        if (not enable_fallback or fallback_state == fallback_disabled)
            return { std::nullopt, continue_at }; /* skip converting tag registers to captures */

        const auto& fni = this->final_nodes().at(fallback_state);
        const auto& fbni = this->fallback_nodes().at(fallback_state);

        it = fallback_it;
        regops_implementation(it, fbni.op_index, registers, registers_enabled);
        continue_at = fbni.continue_at;
        it -= fni.offset;
        break; /* outer */
    }

    /* convert from tag registers to captures */

    tag_result res;
    const capture_info& ci{ this->get_capture_info() };
    const auto& final_reg = this->final_registers();

    auto f = [&](const capture_info::tag_pair_t& p) -> bool {
        return not ((p.first.tag_number >= 0 and not registers_enabled.at(final_reg.at(p.first.tag_number)))
                    or (p.second.tag_number >= 0 and not registers_enabled.at(final_reg.at(p.second.tag_number))));
    };

    auto t = [&](const capture_info::tag_pair_t& p) -> std::pair<I, I> {
        return {
            std::ranges::next((p.first.tag_number >= 0)
                                ? registers.at(final_reg.at(p.first.tag_number))
                                : ((p.first.tag_number == start_of_input_tag) ? first : it), p.first.offset),
            std::ranges::next((p.second.tag_number >= 0)
                                ? registers.at(final_reg.at(p.second.tag_number))
                                : ((p.second.tag_number == start_of_input_tag) ? first : it), p.second.offset)
        };
    };

    for (std::size_t i{ 0 }, i_end{ ci.capture_count() }; i < i_end; ++i)
    {
        auto rng = ci.lookup(i)
                    | std::views::filter(f)
                    | std::views::transform(t)
                    | std::ranges::to<std::vector>();

        if (std::ranges::size(rng) == 0)
        {
            res.insert(res.end(), { no_tag, no_tag });
            continue;
        }

        auto max_elem = std::ranges::max_element(rng, std::ranges::less{}, &std::pair<I, I>::first);
        auto [bit, blast] = *max_elem;

        res.push_back(std::ranges::distance(first, bit));
        res.push_back(std::ranges::distance(first, blast));
    }

    return { res, continue_at };
}

template<typename CharT>
template<std::random_access_iterator I>
    requires std::convertible_to<std::iter_value_t<I>, CharT>
constexpr void tdfa_matcher<CharT>::regops_implementation(I it, std::size_t op_index, std::vector<I>& registers, std::vector<bool>& registers_enabled) const
{
    for (const auto& op : this->get_regops(op_index))
    {
        if (auto* set = get_if<detail::tdfa::regop::set>(&op.op); set != nullptr)
        {
            if (set->val) registers.at(op.dst) = it;
            registers_enabled.at(op.dst) = set->val;
        }
        else if (auto* copy = get_if<detail::tdfa::regop::copy>(&op.op); copy != nullptr)
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

template<typename CharT>
template<std::random_access_iterator I>
    requires std::convertible_to<std::iter_value_t<I>, CharT>
constexpr auto tdfa_matcher<CharT>::multipass_implementation(const I first, const I last, const bool enable_fallback, const std::size_t start) const -> impl_ret_type
{
    using namespace srx::detail;

    std::vector<tdfa::blkidx_t> backlink_indices;

    std::size_t next_state{ start };
    std::size_t fallback_state{ fallback_disabled };

    std::ptrdiff_t fallback_offset{ 0 };

    I it{ first };
    I final_it{ first };
    std::optional<const tdfa::backlink&> backlink;

    auto continue_at = tdfa::no_continue;

    while (true)
    {
        if (enable_fallback)
        {
            if (this->fallback_nodes().contains(next_state))
            {
                fallback_state = next_state;
                fallback_offset = 0;
            }
            else
            {
                ++fallback_offset;
            }
        }

        if (it == last)
        {
            if (this->final_nodes().contains(next_state))
            {
                const auto& fni = this->final_nodes().at(next_state);
                backlink = this->get_backlinks(fni.op_index).front();
                final_it = std::ranges::prev(it, fni.offset);
                if (this->fallback_nodes().contains(next_state))
                    continue_at = this->fallback_nodes().at(next_state).continue_at;
                break; /* outer */
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
                    backlink_indices.emplace_back(t.op_index);
                    ++it;
                    success = true;
                    break; /* inner */
                }
            }

            if (success)
                continue; /* outer */

            if (const auto& dt = this->get_node(next_state).default_tr; dt.has_value())
            {
                next_state = dt->next;
                backlink_indices.emplace_back(dt->op_index);
                ++it;
                continue; /* outer */
            }
        }

        if (not enable_fallback or fallback_state == fallback_disabled)
            return { std::nullopt, continue_at }; /* skip converting tag registers to captures */

        std::ranges::advance(it, -fallback_offset);
        while (fallback_offset-- > 0)
            backlink_indices.pop_back();

        const auto& fni = this->final_nodes().at(fallback_state);
        final_it = std::ranges::prev(it, fni.offset);
        backlink = this->get_backlinks(fni.op_index).front();
        continue_at = this->fallback_nodes().at(fallback_state).continue_at;
        break; /* outer */
    }

    /* backwards pass to get tags */

    std::vector<std::size_t> tag_vec(this->reg_count());
    std::vector<bool> tags_assigned(this->reg_count(), false);

    while (not std::ranges::all_of(tags_assigned, std::identity{}))
    {
        for (const auto tag : backlink.value().tags_seq | std::views::reverse)
        {
            if (tag > 0 and not tags_assigned.at(tag - 1))
            {
                tags_assigned.at(tag - 1) = true;
                tag_vec.at(tag - 1) = std::ranges::distance(first, it);
            }
            else if (tag < 0 and not tags_assigned.at((-tag) - 1))
            {
                tags_assigned.at((-tag) - 1) = true;
                tag_vec.at((-tag) - 1) = no_tag;
            }
        }

        if (it == first)
            break;

        backlink = this->get_backlinks(backlink_indices.back()).at(backlink->prev_index);
        backlink_indices.pop_back();
        --it;
    }

    /* convert from tags to captures */

    tag_result res;
    const capture_info& ci{ this->get_capture_info() };
    const std::size_t final_size{ static_cast<std::size_t>(std::ranges::distance(first, final_it)) };

    auto f = [&](const capture_info::tag_pair_t& p) -> bool {
        return not ((p.first.tag_number >= 0 and tag_vec.at(p.first.tag_number) == no_tag)
                    or (p.second.tag_number >= 0 and tag_vec.at(p.second.tag_number) == no_tag));
    };

    auto t = [&](const capture_info::tag_pair_t& p) -> std::pair<std::size_t, std::size_t> {
        return {
            (p.first.tag_number >= 0)
            ? tag_vec.at(p.first.tag_number)
            : ((p.first.tag_number == start_of_input_tag) ? 0 : final_size) + p.first.offset,
            (p.second.tag_number >= 0)
            ? tag_vec.at(p.second.tag_number)
            : ((p.second.tag_number == start_of_input_tag) ? 0 : final_size) + p.second.offset
        };
    };

    for (std::size_t i{ 0 }, i_end{ ci.capture_count() }; i < i_end; ++i)
    {
        auto rng = ci.lookup(i)
                    | std::views::filter(f)
                    | std::views::transform(t)
                    | std::ranges::to<std::vector>();

        if (std::ranges::size(rng) == 0)
        {
            res.insert(res.end(), { no_tag, no_tag });
            continue;
        }

        auto max_elem = std::ranges::max_element(rng, std::ranges::less{}, &std::pair<std::size_t, std::size_t>::first);

        res.push_back(max_elem->first);
        res.push_back(max_elem->second);
    }

    return { res, continue_at };
}

} // namespace testing
} // namespace srx
