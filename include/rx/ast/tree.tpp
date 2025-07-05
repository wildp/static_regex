#pragma once

#include <rx/ast/tree.hpp>

#include <algorithm>
#include <iterator>
#include <utility>


// TODO: Add fixed tag optimisation (delete unnecessary tags)

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

                    if (pos == cat.idxs.size())
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
    constexpr void expr_tree<CharT>::optimise_tags()
    {
        // TODO: perform tag optimisation
    }
}