// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "tree.hpp"

#include <algorithm>
#include <functional>
#include <iterator>
#include <limits>
#include <numeric>
#include <utility>
#include <variant>

#include "rx/api/regex_error.hpp"
#include "rx/etc/captures.hpp"


namespace rx::detail
{
    /* helper for tagged nfa conversion */

    template<typename CharT>
    constexpr void expr_tree<CharT>::make_tag_vec(std::vector<std::vector<int>>& tag_vec) const
    {
        tag_vec.clear();
        tag_vec.resize(expressions_.size());

        using stack_elem_t = std::pair<std::size_t, std::size_t>;

        std::vector<stack_elem_t> stack;
        stack.emplace_back(root_idx(), false);

        while (not stack.empty())
        {
            auto& [idx, pos]{ stack.back() };
            const auto& entry{ expressions_.at(idx) };

            switch (entry.index())
            {
            case ast_index<assertion>:
            case ast_index<char_str>:
            case ast_index<char_class>:
            case ast_index<backref>:
                stack.pop_back();
                break;

            case ast_index<concat>:
            {
                const auto& cat{ get<concat>(entry) };

                if (cat.idxs.empty())
                {
                    stack.pop_back();
                }
                else if (pos == cat.idxs.size())
                {
                    auto& vec{ tag_vec.at(idx) };

                    for (const std::size_t i : cat.idxs)
                        std::ranges::copy(tag_vec.at(i), std::back_inserter(vec));

                    std::ranges::sort(vec);
                    auto [_, last]{ std::ranges::unique(vec) };
                    vec.erase(last, vec.end());

                    stack.pop_back();
                }
                else
                {
                    pos += 1;
                    stack.emplace_back(cat.idxs.at(pos - 1), 0);
                }
                break;
            }

            case ast_index<alt>:
            {
                const auto& atl{ get<alt>(entry) };

                if (pos == atl.idxs.size())
                {
                    auto& vec{ tag_vec.at(idx) };

                    for (const std::size_t i : atl.idxs)
                        std::ranges::copy(tag_vec.at(i), std::back_inserter(vec));

                    std::ranges::sort(vec);
                    auto [_, last]{ std::ranges::unique(vec) };
                    vec.erase(last, vec.end());

                    stack.pop_back();
                }
                else
                {
                    pos += 1;
                    stack.emplace_back(atl.idxs.at(pos - 1), 0);
                }
                break;
            }

            case ast_index<repeat>:
            {
                const auto& rep{ get<repeat>(entry) };

                if (pos == 1)
                {
                    auto& vec{ tag_vec.at(idx) };
                    std::ranges::copy(tag_vec.at(rep.idx), std::back_inserter(vec));

                    stack.pop_back();
                }
                else
                {
                    pos += 1;
                    stack.emplace_back(rep.idx, 0);
                }
                break;
            }

            case ast_index<tag>:
            {
                const auto& tag_entry{ get<tag>(entry) };

                auto& vec{ tag_vec.at(idx) };
                vec.emplace_back(tag_entry.number);
                stack.pop_back();
                break;
            }

            default:
                throw tree_error("Invalid tree");
            }
        }
    }


    /* fixed tag optimisation (delete unnecessary tags) */

    template<typename CharT>
    constexpr std::vector<std::optional<std::size_t>> expr_tree<CharT>::make_const_len_vec()
    {
        using opt_t = std::optional<std::size_t>;
        std::vector<opt_t> const_len(expressions_.size());

        using stack_elem_t = std::pair<std::size_t, std::size_t>;

        std::vector<stack_elem_t> stack;
        stack.emplace_back(root_idx(), false);

        while (not stack.empty())
        {
            auto& [idx, pos]{ stack.back() };
            const auto& entry{ expressions_.at(idx) };

            switch (entry.index())
            {
            case ast_index<assertion>:
            case ast_index<tag>:
                const_len.at(idx) = 0;
                stack.pop_back();
                break;

            case ast_index<char_str>:
                const_len.at(idx) = get<char_str>(entry).data.size();
                stack.pop_back();
                break;

            case ast_index<char_class>:
                const_len.at(idx) = 1; // TODO: change this for multibyte char classes
                stack.pop_back();
                break;

            case ast_index<backref>:
                stack.pop_back();
                break;

            case ast_index<concat>:
            {
                const auto& cat{ get<concat>(entry) };

                if (pos == cat.idxs.size())
                {
                    auto tmp{
                        cat.idxs | std::views::transform([&](std::size_t i) { return const_len.at(i); })
                        | std::ranges::to<std::vector>()
                    };

                    if (std::ranges::all_of(tmp, [](const opt_t& o) { return o.has_value(); }))
                        const_len.at(idx) = std::ranges::fold_left(tmp | std::views::transform([](const opt_t& o) { return *o; }),
                                                                   0, std::plus{});

                    stack.pop_back();
                }
                else
                {
                    pos += 1;
                    stack.emplace_back(cat.idxs.at(pos - 1), 0);
                }
                break;
            }

            case ast_index<alt>:
            {
                const auto& atl{ get<alt>(entry) };

                if (pos == atl.idxs.size())
                {
                    auto tmp{
                        atl.idxs | std::views::transform([&](std::size_t i) { return const_len.at(i); })
                        | std::ranges::to<std::vector>()
                    };

                    auto first{ *std::ranges::begin(tmp) };

                    if (std::ranges::size(tmp) > 0 and std::ranges::all_of(tmp, [&](const opt_t& o) { return o == first; }))
                        const_len.at(idx) = first;

                    stack.pop_back();
                }
                else
                {
                    pos += 1;
                    stack.emplace_back(atl.idxs.at(pos - 1), 0);
                }
                break;
            }

            case ast_index<repeat>:
            {
                const auto& rep{ get<repeat>(entry) };

                if (pos == 1)
                {
                    if (rep.min == rep.max and const_len.at(rep.idx))
                        const_len.at(idx) = *const_len.at(rep.idx) * rep.min;

                    stack.pop_back();
                }
                else
                {
                    pos += 1;
                    stack.emplace_back(rep.idx, 0);
                }
                break;
            }

            default:
                throw tree_error("Invalid tree");
            }
        }

        return const_len;
    }

    template<typename CharT>
    constexpr void expr_tree<CharT>::optimise_tags()
    {
        const auto const_len{ make_const_len_vec() };

        std::flat_map<tag_number_t, capture_info::pair_entry> tag_remap;

        for (std::size_t i{ 0 }, i_end{ expressions_.size() }; i < i_end; ++i)
        {
            if (not holds_alternative<concat>(expressions_.at(i)))
                continue;

            std::optional<capture_info::pair_entry> current;
            auto& target{ get<concat>(expressions_.at(i)).idxs };

            if (i == root_idx())
                current = { .tag_number = end_of_input_tag, .offset = 0 };

            for (std::size_t j{ target.size() }; j > 0; --j)
            {
                const std::size_t idx{ target.at(j - 1) };

                if (auto* tn{ get_if<tag>(&expressions_.at(idx)) }; tn != nullptr)
                {
                    if (current.has_value())
                    {
                        /* remap tag */

                        auto [_, success]{ tag_remap.try_emplace(tn->number, *current) };
                        if (not success)
                            throw tree_error("Tag appears more than once in AST");

                        tn->number = -1; /* set to an invalid value */
                        target.erase(target.begin() + j - 1);
                    }
                    else
                    {
                        current = { .tag_number = tn->number, .offset = 0 };
                    }
                }
                else if (current.has_value())
                {
                    if (const_len.at(idx))
                        current->offset -= *const_len.at(idx);
                    else
                        current = {};
                }
            }
        }

        /* re-map start of input tag if possible */

        if (const auto& opt{ const_len.at(root_idx_) }; opt.has_value())
            tag_remap.try_emplace(start_of_input_tag, capture_info::pair_entry{ .tag_number = end_of_input_tag, .offset = -(static_cast<int>(*opt)) });

        /* re-number map and re-number tags in capture_info */

        const auto remapper{ capture_info_.remap_tags(tag_remap) };

        /* re-number tags in ast */

        for (auto& expr : expressions_)
        {
            if (auto* tn{ get_if<tag>(&expr) }; tn != nullptr)
                if (auto it{ remapper.find(tn->number) }; it != remapper.end())
                    tn->number = it->second;
        }

        if (remapper.size() > std::numeric_limits<tag_number_t>::max())
            throw tree_error("Tag number exceeded");

        tag_count_ = std::saturate_cast<tag_number_t>(remapper.size());
    }


    /* convert pattern for use in regex search */

    template<typename CharT>
    constexpr void expr_tree<CharT>::insert_search_prefix()
    {
        /* make true wildcard */
        const std::size_t wildcard_idx{ expressions_.size() };
        char_class cc{ true };
        cc.data.normalise();
        expressions_.emplace_back(std::in_place_type<char_class>, std::move(cc));

        /* make lazy repeater of wildcard */
        const std::size_t repeater_idx{ expressions_.size() };
        expressions_.emplace_back(std::in_place_type<repeat>, wildcard_idx, 0, -1, repeater_mode::lazy);

        /* conditionally make start tag */
        const std::size_t start_tag_idx{ expressions_.size() };
        if (flags_.enable_start_tag)
        {
            const tag_number_t start_tag{ tag_count_++ };
            if (start_tag < 0)
                throw tree_error("Capture limit exceed");
            capture_info_.set_match_start_tag(start_tag);
            expressions_.emplace_back(std::in_place_type<tag>, start_tag);
        }

        /* insert into pattern */
        if (type& ast{ expressions_.at(root_idx_) }; holds_alternative<concat>(ast))
        {
            /* root idx is already concat, so we can avoid creating a new concat as root */
            auto& target{ get<concat>(ast).idxs };
            if (flags_.enable_start_tag)
                target.insert(target.begin(), { repeater_idx, start_tag_idx });
            else
                target.insert(target.begin(), { repeater_idx });
        }
        else
        {
            /* make new root node */
            const std::size_t new_root_idx{ expressions_.size() };
            if (flags_.enable_start_tag)
                expressions_.emplace_back(std::in_place_type<concat>, std::vector{ repeater_idx, start_tag_idx, root_idx_ });
            else
                expressions_.emplace_back(std::in_place_type<concat>, std::vector{ repeater_idx, root_idx_ });
            root_idx_ = new_root_idx;
        }
    }


    /* convert pattern for use in naive matcher */

    template<typename CharT>
    constexpr void expr_tree<CharT>::simplify_counted_repeat()
    {
        for (std::size_t i{ 0 }, i_end{ expressions_.size() }; i < i_end; ++i)
        {
            if (not holds_alternative<repeat>(expressions_[i]))
                continue;

            repeat rep{ get<repeat>(expressions_[i]) };

            /* here we simplify `a{n,m}` into 3 primitives: `a{n}`, `a*`, and `a?` */

            if (rep.min == 0)
            {
                if (rep.max > 1)
                {
                    std::size_t cat{ rep.idx };

                    for (auto i{ rep.min + 1 }; i < rep.max; ++i)
                    {
                        const std::size_t quest{ expressions_.size() };
                        expressions_.emplace_back(std::in_place_type<repeat>, cat, 0, 1, rep.mode);

                        cat = expressions_.size();
                        expressions_.emplace_back(std::in_place_type<concat>, std::vector{ rep.idx, quest });
                    }

                    expressions_[i].template emplace<repeat>(cat, 0, 1, rep.mode);
                }
            }
            else if (rep.min < rep.max)
            {
                /* simplify `a{n,m}`, where 0 < n < m */

                const std::size_t fixed_rep{ expressions_.size() };
                expressions_.emplace_back(std::in_place_type<repeat>, rep.idx, rep.min, rep.min, rep.mode);

                std::size_t quest{ expressions_.size() };
                expressions_.emplace_back(std::in_place_type<repeat>, rep.idx, 0, 1, rep.mode);

                for (auto i{ rep.min + 1 }; i < rep.max; ++i)
                {
                    const std::size_t cat{ expressions_.size() };
                    expressions_.emplace_back(std::in_place_type<concat>, std::vector{ rep.idx, quest });

                    quest = expressions_.size();
                    expressions_.emplace_back(std::in_place_type<repeat>, cat, 0, 1, rep.mode);
                }

                expressions_[i].template emplace<concat>(std::vector{ fixed_rep, quest });
            }
            else if (rep.min > rep.max)
            {
                /* simplify `a{n,}` and `a+`, where n > 0 */

                const std::size_t fixed_rep{ expressions_.size() };
                expressions_.emplace_back(std::in_place_type<repeat>, rep.idx, rep.min, rep.min, rep.mode);

                const std::size_t star{ expressions_.size() };
                expressions_.emplace_back(std::in_place_type<repeat>, rep.idx, 0, -1, rep.mode);

                expressions_[i].template emplace<concat>(std::vector{ fixed_rep, star });
            }
        }
    }

    template<typename CharT>
    constexpr void expr_tree<CharT>::optimise_exact_repeat()
    {
        for (std::size_t i{ 0 }, i_end{ expressions_.size() }; i < i_end; ++i)
        {
            if (not holds_alternative<repeat>(expressions_[i]))
                continue;

            repeat rep{ get<repeat>(expressions_[i]) };

            if (rep.min != rep.max or not holds_alternative<char_str>(expressions_.at(rep.idx)))
                continue;

            auto& cs{ expressions_[i].template emplace<char_str>() };

            const auto& old{ get<char_str>(expressions_[rep.idx]) };
            cs.data.reserve(rep.min * old.data.size());
            for (int j{ 0 }; j < rep.min; ++j)
                cs.data += old.data;
        }
    }

    template<typename CharT>
    constexpr auto expr_tree<CharT>::tag_to_register()
    {
        const auto branch_remapper{ capture_info_.eliminate_branch_reset() };

        for (auto& expr : expressions_)
        {
            if (auto* tn{ get_if<tag>(&expr) }; tn != nullptr)
                if (auto it{ branch_remapper.find(tn->number) }; it != branch_remapper.end())
                    tn->number = it->second;
        }

        std::vector<bool> ignore(tag_count_, false);

        /* the section below has been copied from optimise_tags, except:
         * - branch reset tags are ignored,
         * - the start of input is not remapped
         */

        for (const auto tag : branch_remapper.values())
            ignore.at(tag) = true;

        const auto const_len{ make_const_len_vec() };

        std::flat_map<tag_number_t, capture_info::pair_entry> tag_remap;

        for (std::size_t i{ 0 }, i_end{ expressions_.size() }; i < i_end; ++i)
        {
            if (not holds_alternative<concat>(expressions_.at(i)))
                continue;

            std::optional<capture_info::pair_entry> current;
            auto& target{ get<concat>(expressions_.at(i)).idxs };

            if (i == root_idx())
                current = { .tag_number = end_of_input_tag, .offset = 0 };

            for (std::size_t j{ target.size() }; j > 0; --j)
            {
                const std::size_t idx{ target.at(j - 1) };

                if (auto* tn{ get_if<tag>(&expressions_.at(idx)) }; tn != nullptr and not ignore.at(tn->number))
                {
                    if (current.has_value())
                    {
                        /* remap tag */

                        auto [_, success]{ tag_remap.try_emplace(tn->number, *current) };
                        if (not success)
                            throw tree_error("Tag appears more than once in AST");

                        tn->number = -1; /* set to an invalid value */
                        target.erase(target.begin() + j - 1);
                    }
                    else
                    {
                        current = { .tag_number = tn->number, .offset = 0 };
                    }
                }
                else if (current.has_value())
                {
                    if (const_len.at(idx))
                        current->offset -= *const_len.at(idx);
                    else
                        current = {};
                }
            }
        }

        /* re-number map and re-number tags in capture_info */

        const auto remapper{ capture_info_.remap_tags(tag_remap) };

        /* re-number tags in ast */

        for (auto& expr : expressions_)
        {
            if (auto* tn{ get_if<tag>(&expr) }; tn != nullptr)
                if (auto it{ remapper.find(tn->number) }; it != remapper.end())
                    tn->number = it->second;
        }

        if (remapper.size() > std::numeric_limits<tag_number_t>::max())
            throw tree_error("Tag number exceeded");

        tag_count_ = std::saturate_cast<tag_number_t>(remapper.size());


        /* --- end copied section ---  */

        /* this returns an over-approximation of the tags which need staging;
         * only lhs tag for capturing groups which contain a
         * backreference to themselves need to be staged                      */
        return capture_info_.get_staged_tags();
    }
}
