#pragma once

#include <rx/fsm/tnfa.hpp>

#include <algorithm>
#include <iterator>
#include <limits>
#include <ranges>
#include <utility>


namespace rx::detail
{
    /* modified thompson's algorithm */

    template<typename CharT>
    constexpr std::size_t tagged_nfa<CharT>::node_create()
    {
        auto tmp{ nodes_.size() };
        nodes_.emplace_back();
        return tmp;
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::epsilon(const std::size_t q0, const std::size_t qf, const int priority, const int tag)
    {
        nodes_.at(q0).tr.emplace_back(std::in_place_type<epsilon_tr>, qf, priority, tag);
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_ntags(std::size_t q0, const std::size_t qf, const std::vector<int>& ntags)
    {
        if (not ntags.empty())
        {
            /* note: in tree_expr tags start at 0, whereas here they start at 1 */
            for (const int tag : ntags | std::views::take(ntags.size() - 1))
            {
                auto qi{ node_create() };
                epsilon(q0, qi, 0, -(tag + 1));
                q0 = qi;
            }

            epsilon(q0, qf, 0, -(ntags.back() + 1));
        }
        else 
        {     
            epsilon(q0, qf);
        }
    }

    
    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_wildcard(const std::size_t q0, const std::size_t qf)
    {
        if constexpr (char_is_utf8<CharT>)
        {
            // TODO: implement multibyte chars;
            throw tnfa_error("UTF8 wildcards are unimplemented");
        }
        else if constexpr (char_is_utf16<CharT>)
        {
            // TODO: implement multibyte chars;
            throw tnfa_error("UTF16 wildcards are unimplemented");
        }
        else 
        {
            nodes_.at(q0).tr.emplace_back(std::in_place_type<n_tr<CharT>>, qf, std::numeric_limits<CharT>::lowest(), std::numeric_limits<CharT>::max());
        }
    }

    template<typename CharT>
    constexpr tagged_nfa<CharT>::tagged_nfa(const expr_tree<CharT>& ast) : 
        capture_info_{ ast.get_capture_info() }, tag_count_{ ast.tag_count() }
    {
        std::vector<std::vector<int>> tag_vec{};
        if (tag_count_ > 1) ast.make_tag_vec(tag_vec);

        std::vector<stack_elem> stack;
        stack.emplace_back(match_start, end, ast.root_idx());

        while (not stack.empty())
        {
            auto [q0, qf, idx]{ stack.back() };
            stack.pop_back();

            const auto& entry{ ast.get_expr(idx) };

            switch (entry.index())
            {
            case ast_index<typename ast_t::char_str>:
                if (const auto& str{ std::get<typename ast_t::char_str>(entry) }; not str.data.empty())
                {
                    for (const auto c : str.data | std::views::take(str.data.size() - 1))
                    {
                        auto qi{ node_create() };
                        nodes_.at(q0).tr.emplace_back(std::in_place_type<n_tr<CharT>>, qi, c, c);
                        q0 = qi;
                    }
                    
                    nodes_.at(q0).tr.emplace_back(std::in_place_type<n_tr<CharT>>, qf, str.data.back(), str.data.back());
                }
                else
                {
                    epsilon(q0, qf);
                }
                break;

            case ast_index<typename ast_t::char_class>:
                {
                    const auto& cla{ std::get<typename ast_t::char_class>(entry) };
                    // using uct = decltype(cla)::underlying_char_type;
                    using char_range = decltype(cla.data)::char_range;

                    if constexpr (char_is_utf8<CharT>)
                    {
                        // TODO: implement multibyte chars;
                        throw tnfa_error("UTF8 character classes are unimplemented");
                    }
                    else if constexpr (char_is_utf16<CharT>)
                    {
                        // TODO: implement multibyte chars;
                        throw tnfa_error("UTF16 character classes are unimplemented");
                    }
                    else
                    {
                        std::vector<char_range> negated;

                        if (cla.data.is_negated())
                            cla.data.make_negated(negated);

                        const std::vector<char_range>& ref{ (cla.data.is_negated()) ? negated : cla.data.get() };

                        for (const auto& [lower, upper] : ref)
                            nodes_.at(q0).tr.emplace_back(std::in_place_type<n_tr<CharT>>, qf, lower, upper);                    
                    }
                }
                break;

            case ast_index<typename ast_t::concat>:
                if (const auto& cat{ std::get<typename ast_t::concat>(entry) }; not cat.idxs.empty())
                {
                    for (const auto i : cat.idxs | std::views::take(cat.idxs.size() - 1))
                    {
                        auto qi{ node_create() };
                        stack.emplace_back(q0, qi, i);
                        q0 = qi;
                    }
            
                    stack.emplace_back(q0, qf, cat.idxs.back());
                }
                else
                {
                    epsilon(q0, qf);
                }
                break;

            case ast_index<typename ast_t::alt>:
                if (const auto& alt{ std::get<typename ast_t::alt>(entry) }; not alt.idxs.empty())
                {
                    if (tag_vec.empty() or tag_vec.at(idx).empty())
                    {
                        /* generate naive tag-free nfa */
                        for (std::size_t i{ 0 }; i < alt.idxs.size(); ++i)
                        {
                            auto qi{ node_create() };
                            epsilon(q0, qi, i);
                            stack.emplace_back(qi, qf, alt.idxs.at(i));
                        }
                    }
                    else
                    {
                        /* generate tag-aware nfa */
                        for (std::size_t i{ 0 }; i < alt.idxs.size(); ++i)
                        {
                            if (i + 1 == alt.idxs.size())
                            {
                                stack.emplace_back(q0, qf, alt.idxs.at(i));
                            }
                            else 
                            {
                                auto q1{ node_create() };
                                auto p2{ node_create() };
                                auto p1{ node_create() };
                                auto q2{ node_create() };
                
                                epsilon(q0, q1, 0);
                                epsilon(q0, p1, 1);
                                
                                stack.emplace_back(q1, p2, alt.idxs.at(i));
                                make_ntags(p1, q2, tag_vec.at(alt.idxs.at(i)));
    
                                std::vector<int> remaining_ntags;
                                for (std::size_t j{ i + 1 }; j < alt.idxs.size(); ++j)
                                    std::ranges::copy(tag_vec.at(alt.idxs.at(j)), std::back_inserter(remaining_ntags));
    
                                std::ranges::sort(remaining_ntags);
                                auto [_, last]{ std::ranges::unique(remaining_ntags) };
                                remaining_ntags.erase(last, remaining_ntags.end());
    
                                make_ntags(p2, qf, remaining_ntags);
                
                                q0 = q2;
                            }
                        }
                    }
                }
                else
                {
                    epsilon(q0, qf);
                }
                break;

            case ast_index<typename ast_t::repeat>:
                {
                    const auto& rep{ std::get<typename ast_t::repeat>(entry) };

                    if (rep.mode == repeater_mode::possessive)
                        throw tnfa_error("Possessive repetition is unimplemented");

                    /* reminder: max < min denotes infinity */
                    int max{ (rep.max < rep.min) ? rep.min - 1 : rep.max };
                    int min{ rep.min };
                    const bool lazy{ rep.mode == repeater_mode::lazy };

                    while (true)
                    {
                        if (min > 1)
                        {
                            auto q1{ node_create() };

                            stack.emplace_back(q0, q1, rep.idx);
            
                            min -= 1;
                            max -= 1;
                            q0 = q1;
                        }
                        else if (min == 1 and max == 1)
                        {
                            stack.emplace_back(q0, qf, rep.idx);

                            break;
                        }
                        else if (min == 1 and max > 1)
                        {
                            auto q1{ node_create() };
                            auto q2{ node_create() };

                            stack.emplace_back(q0, q1, rep.idx);
                            epsilon(q1, qf, lazy);
                            epsilon(q1, q2, not lazy);
            
                            max -= 1;
                            q0 = q2;
                        }
                        else if (min == 0)
                        {
                            auto q1{ node_create() };

                            epsilon(q0, q1, lazy);

                            if (tag_vec.empty() or tag_vec.at(idx).empty())
                            {
                                /* generate tag-free nfa */
                                epsilon(q0, qf, not lazy);
                            }
                            else
                            {
                                /* generate tag-aware nfa */
                                auto p1{ node_create() };
                                epsilon(q0, p1, not lazy);
                                make_ntags(p1, qf, tag_vec.at(rep.idx));
                            }

                            min = 1;
                            q0 = q1;
                        }
                        else if (min == 1 and max < min)
                        {
                            auto q1{ node_create() };

                            stack.emplace_back(q0, q1, rep.idx);
                            epsilon(q1, q0, lazy);
                            epsilon(q1, qf, not lazy);
            
                            break;
                        }
                        // else if (min == 0 and max == 0)
                        // {
                        //     epsilon(q0, qf);
                        //     break;
                        // }
                        else
                        {
                            throw tnfa_error("Invalid repeater in tree");
                        }
                    }
                }
                break;

            case ast_index<typename ast_t::tag>:
                {
                    /* in tree_expr tags start at 0, whereas here they start at 1 */
                    const auto& tag_entry{ std::get<typename ast_t::tag>(entry) };
                    epsilon(q0, qf, 0, tag_entry.number + 1);
                }
                break;

            case ast_index<typename ast_t::assertion>:
                throw tnfa_error("Assertions are unimplemented");

            case ast_index<typename ast_t::backref>:
                throw tnfa_error("Backreferences are unimplemented");

            default:
                throw tree_error("Invalid tree");

            }
        }
        

        // TODO: maybe add this in the ast instead of at this stage? 

        /* join search_start to match_start with ".*?t", where t is the start_tag */

        // auto q_tmp{ node_create() };
        // make_wildcard(substr_start, q_tmp);
        // epsilon(q_tmp, match_start, 0, tag_count_++);
        // epsilon(q_tmp, substr_start, 1);
    }

}