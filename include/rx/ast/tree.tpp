#pragma once

#include "rx/etc/captures.hpp"
#include "rx/etc/error.hpp"
#include <limits>
#include <numeric>
#include <rx/ast/tree.hpp>

#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>
#include <variant>


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
                    const auto& cat{ std::get<concat>(entry) };

                    if (cat.idxs.empty())
                    {
                        stack.pop_back();
                    }
                    else if (pos == cat.idxs.size())
                    {
                        auto& vec{ tag_vec.at(idx) };

                        for (std::size_t i : cat.idxs)
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
                }
                break;

            case ast_index<alt>:
                {
                    const auto& atl{ std::get<alt>(entry) };

                    if (pos == atl.idxs.size())
                    {
                        auto& vec{ tag_vec.at(idx) };

                        for (std::size_t i : atl.idxs)
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
                }
                break;

            case ast_index<repeat>:
                {
                    const auto& rep{ std::get<repeat>(entry) };

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
                }
                break;

            case ast_index<tag>:
                {
                    const auto& tag_entry{ std::get<tag>(entry) };

                    auto& vec{ tag_vec.at(idx) };
                    vec.emplace_back(tag_entry.number);
                    stack.pop_back();
                }
                break;

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
                const_len.at(idx) = std::get<char_str>(entry).data.size();
                stack.pop_back();
                break;

            case ast_index<char_class>:
                const_len.at(idx) = 1;
                stack.pop_back();
                break;
            
            case ast_index<backref>:
                stack.pop_back();
                break;

            case ast_index<concat>:
                {
                    const auto& cat{ std::get<concat>(entry) };

                    if (pos == cat.idxs.size())
                    {
                        auto tmp{ cat.idxs | std::views::transform([&](std::size_t i){ return const_len.at(i); })
                                           | std::ranges::to<std::vector>() };

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
                }
                break;

            case ast_index<alt>:
                {
                    const auto& atl{ std::get<alt>(entry) };

                    if (pos == atl.idxs.size())
                    {
                        auto tmp{ atl.idxs | std::views::transform([&](std::size_t i){ return const_len.at(i); })
                                           | std::ranges::to<std::vector>() };

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
                }
                break;

            case ast_index<repeat>:
                {
                    const auto& rep{ std::get<repeat>(entry) };

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
                }
                break;

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

        for (std::size_t i{ 0 }; i < expressions_.size(); ++i)
        {
            if (not std::holds_alternative<concat>(expressions_.at(i)))
                continue;

            std::optional<capture_info::pair_entry> current;
            auto& target{ std::get<concat>(expressions_.at(i)).idxs };

            if (i == root_idx())
                current = { .tag_number = end_of_input_tag, .offset = 0 };

            // if not consteval {
            //     if (i == root_idx())
            //         std::println("root idx {}: {}", i, target);
            //     else
            //         std::println("idx {}: {}", i, target);
            // }
            
            for (std::size_t j{ target.size() }; j > 0; --j)
            {
                const std::size_t idx{ target.at(j - 1) };

                if (auto* tn{ std::get_if<tag>(&expressions_.at(idx)) }; tn != nullptr)
                {
                    if (current.has_value())
                    {
                        // if not consteval { if (i == root_idx()) std::println("remap {}", tn->number); }

                        /* remap tag */

                        auto [_, success]{ tag_remap.try_emplace(tn->number, *current) };
                        if (not success)
                            throw tree_error("Tag appears more than once in AST");

                        tn->number = -1; /* set to an invalid value */
                        target.erase(target.begin() + j - 1);
                    }
                    else
                    {
                        // if not consteval { if (i == root_idx()) std::println("setcurrent {}", tn->number); }

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

            // if not consteval { std::println("end idx"); }
        
        }

        // if not consteval
        // {
        //     std::println("Remaps");
        //     for (auto [k, v] : tag_remap)
        //         std::println("{}: {}+{}", k, v.tag_number, v.offset);
        //     std::println("End");
        // }

        /* re-number map and re-number tags in capture_info */

        const auto remapper{ capture_info_.remap_tags(tag_remap) };

        /* re-number tags in ast */

        for (auto& expr : expressions_)
        {
            if (auto* tn{ std::get_if<tag>(&expr) }; tn != nullptr)
            {
                if (auto it{ remapper.find(tn->number) }; it != remapper.end())
                    tn->number = it->second;
            }
        }

        if (remapper.size() > std::numeric_limits<tag_number_t>::max())
            throw tree_error("Tag number exceeded");

        tag_count_ = std::saturate_cast<tag_number_t>(remapper.size());
    }
}