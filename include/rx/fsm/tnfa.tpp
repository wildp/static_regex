#pragma once

#include <rx/fsm/tnfa.hpp>

#include <algorithm>
#include <iterator>
#include <limits>
#include <ranges>
#include <utility>

#include <rx/api/regex_error.hpp>


namespace rx::detail
{
    struct stack_elem
    {
        tnfa::state_t q0, qf;
        std::size_t idx;
    };


    /* modified thompson's algorithm */

    template<typename CharT>
    constexpr auto tagged_nfa<CharT>::node_create() -> state_t
    {
        const state_t tmp{ nodes_.size() };
        nodes_.emplace_back();
        return tmp;
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_epsilon(const state_t q0, const state_t qf, const int priority, const int tag)
    {
        nodes_.at(q0).tr.emplace_back(qf, std::in_place_type<tnfa::epsilon_tr>, priority, tag);
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_transition(const state_t q0, const state_t qf, CharT c)
    {
        nodes_.at(q0).tr.emplace_back(qf, std::in_place_type<tnfa::normal_tr<CharT>>, c, c);
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_transition(const state_t q0, const state_t qf, CharT lower, CharT upper)
    {
        nodes_.at(q0).tr.emplace_back(qf, std::in_place_type<tnfa::normal_tr<CharT>>, lower, upper);
    }

    template<typename CharT>
    template<tnfa::assert_category V>
    constexpr void tagged_nfa<CharT>::make_assert(const state_t q0, const state_t qf, tnfa::ac<V> /* category */)
    {
        using type = std::conditional_t<V == acat_t::eof, tnfa::eof_anchor_tr,
                                        std::conditional_t<V == acat_t::sof, tnfa::sof_anchor_tr, void>>;

        nodes_.at(q0).tr.emplace_back(qf, std::in_place_type<type>);
    }

    template<typename CharT>
    template<tnfa::assert_category V>
    constexpr void tagged_nfa<CharT>::make_assert(const state_t q0, const state_t qf, const tnfa::char_set<CharT> cs, tnfa::ac<V> /* category */)
    {
        using type = std::conditional_t<V == acat_t::lookahead, tnfa::lookahead_1_tr<CharT>,
                                std::conditional_t<V == acat_t::lookbehind, tnfa::lookbehind_1_tr<CharT>, void>>;

        nodes_.at(q0).tr.emplace_back(qf, std::in_place_type<type>, cs);
    }
    
    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_assert(const state_t q0, const state_t qf, const state_t p0, const state_t pf)
    {
        nodes_.at(q0).tr.emplace_back(qf, std::in_place_type<tnfa::lookahead_assert_tr>, p0, pf);
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_copy(const state_t q0, const state_t qf, const transition_info& type)
    {
        nodes_.at(q0).tr.emplace_back(qf, type);
    }


    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_ntags(state_t q0, const state_t qf, const std::vector<int>& ntags)
    {
        if (not ntags.empty())
        {
            /* note: in tree_expr tags start at 0, whereas here they start at 1 */
            for (const int tag : ntags | std::views::take(ntags.size() - 1))
            {
                const state_t qi{ node_create() };
                make_epsilon(q0, qi, 0, -(tag + 1));
                q0 = qi;
            }

            make_epsilon(q0, qf, 0, -(ntags.back() + 1));
        }
        else 
        {     
            make_epsilon(q0, qf);
        }
    }

    
    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_wildcard(const state_t q0, const state_t qf)
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
            make_transition(q0, qf, std::numeric_limits<CharT>::lowest(), std::numeric_limits<CharT>::max());
        }
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::thompson(const expr_tree<CharT>& ast)
    {
        std::vector<std::vector<int>> tag_vec{};
        if (tag_count_ > 1) ast.make_tag_vec(tag_vec);

        std::vector<stack_elem> stack;
        stack.emplace_back(start_node_, default_final_node, ast.root_idx());

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
                        make_transition(q0, qi, c);
                        q0 = qi;
                    }
                    
                    make_transition(q0, qf, str.data.back());
                }
                else
                {
                    make_epsilon(q0, qf);
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
                            make_transition(q0, qf, lower, upper);             
                    }
                }
                break;

            case ast_index<typename ast_t::concat>:
                if (const auto& cat{ std::get<typename ast_t::concat>(entry) }; not cat.idxs.empty())
                {
                    for (const auto i : cat.idxs | std::views::take(cat.idxs.size() - 1))
                    {
                        const state_t qi{ node_create() };
                        stack.emplace_back(q0, qi, i);
                        q0 = qi;
                    }
            
                    stack.emplace_back(q0, qf, cat.idxs.back());
                }
                else
                {
                    make_epsilon(q0, qf);
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
                            const state_t qi{ node_create() };
                            make_epsilon(q0, qi, i);
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
                                const state_t q1{ node_create() };
                                const state_t p2{ node_create() };
                                const state_t p1{ node_create() };
                                const state_t q2{ node_create() };
                
                                make_epsilon(q0, q1, 0);
                                make_epsilon(q0, p1, 1);
                                
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
                    make_epsilon(q0, qf);
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
                            const state_t q1{ node_create() };

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
                            const state_t q1{ node_create() };
                            const state_t q2{ node_create() };

                            stack.emplace_back(q0, q1, rep.idx);
                            make_epsilon(q1, qf, not lazy);
                            make_epsilon(q1, q2, lazy);
            
                            max -= 1;
                            q0 = q2;
                        }
                        else if (min == 0)
                        {
                            const state_t q1{ node_create() };

                            make_epsilon(q0, q1, lazy);

                            if (tag_vec.empty() or tag_vec.at(idx).empty())
                            {
                                /* generate tag-free nfa */
                                make_epsilon(q0, qf, not lazy);
                            }
                            else
                            {
                                /* generate tag-aware nfa */
                                const state_t p1{ node_create() };
                                make_epsilon(q0, p1, not lazy);
                                make_ntags(p1, qf, tag_vec.at(rep.idx));
                            }

                            min = 1;
                            q0 = q1;
                        }
                        else if (min == 1 and max < min)
                        {
                            const state_t q1{ node_create() };

                            stack.emplace_back(q0, q1, rep.idx);
                            make_epsilon(q1, q0, lazy);
                            make_epsilon(q1, qf, not lazy);
            
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
                    make_epsilon(q0, qf, 0, tag_entry.number + 1);
                }
                break;

            case ast_index<typename ast_t::assertion>:
                {
                    const typename ast_t::assertion& assertion{ std::get<typename ast_t::assertion>(entry) };

                    switch (assertion.type)
                    {
                    case assert_type::line_start:
                        throw tnfa_error("Non-trivial assertions are unimplemented");
                        // TODO: implement
                        // has_lookbehind_1_ = true;
                        // [[fallthrough]];
                    
                    case assert_type::text_start:
                        make_assert(q0, qf, tnfa::ac<acat_t::sof>{});
                        has_sof_anchor_ = true;
                        break;
                    
                    case assert_type::line_end:
                        throw tnfa_error("Non-trivial assertions are unimplemented");
                        // TODO: implement
                        // has_lookahead_1_ = true;
                        // [[fallthrough]];
                    
                    case assert_type::text_end:
                        make_assert(q0, qf, tnfa::ac<acat_t::eof>{});
                        has_eof_anchor_ = true;
                        break;

                    default:
                        throw tnfa_error("Non-trivial assertions are unimplemented");
                        break;
                    }
                }
                break;

            case ast_index<typename ast_t::backref>:
                throw tnfa_error("Backreferences are unimplemented");

            default:
                throw tree_error("Invalid tree");

            }
        }
    }

    
    /* observers */

    template<typename CharT>
    constexpr std::vector<std::size_t> tagged_nfa<CharT>::strict_e_closure(const std::size_t q) const
    {
        std::vector<std::size_t> result;
        std::vector to_visit{ q };

        std::vector<bool> visited(node_count(), false);

        while (not to_visit.empty())
        {
            const std::size_t current{ to_visit.back() };
            to_visit.pop_back();

            result.emplace_back(current);
            
            const auto& current_node{ get_node(current) };
            
            for (const auto& tr : current_node.tr | std::views::reverse)
            {
                if (not std::holds_alternative<tnfa::epsilon_tr>(tr.type))
                    continue;

                if (visited.at(tr.next))
                    continue;

                visited[tr.next] = true;
                to_visit.emplace_back(tr.next);
            }
        }

        return result;
    }

    template<typename CharT>
    constexpr std::vector<std::size_t> tagged_nfa<CharT>::relaxed_e_closure(const std::size_t q) const
    {
        std::vector<std::size_t> result;
        std::vector to_visit{ q };

        std::vector<bool> visited(node_count(), false);

        while (not to_visit.empty())
        {
            const std::size_t current{ to_visit.back() };
            to_visit.pop_back();

            result.emplace_back(current);
            
            const auto& current_node{ get_node(current) };
            
            for (const auto& tr : current_node.tr | std::views::reverse)
            {
                if (std::holds_alternative<tnfa::normal_tr<CharT>>(tr.type))
                    continue;

                if (visited.at(tr.next))
                    continue;

                visited[tr.next] = true;
                to_visit.emplace_back(tr.next);
            }
        }

        return result;
    }


    /* dead state elimination */

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::remove_dead_states()
    {
        // TODO: Implement
    }

    /* assertion elimination */

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_sof_anchors() 
    {
        const auto ec{ relaxed_e_closure(start_node()) };

        /* create a copy of the start node's e-closure */

        std::flat_map<std::size_t, std::size_t> mapped_states;
        for (const std::size_t q : ec)
            mapped_states[q] = node_create();

        /* duplicate transitions */

        for (const auto [q, p] : mapped_states)
        {
            for (const auto& tr: nodes_.at(q).tr)
            {
                if (const auto* ntr{ std::get_if<tnfa::normal_tr<CharT>>(&tr.type)}; ntr != nullptr)
                {
                    /* transition from copied e-closure to main graph */
                    make_transition(p, tr.next, ntr->lower, ntr->upper);
                }
                else if (std::holds_alternative<tnfa::sof_anchor_tr>(tr.type))
                {
                    /* transition from copied e-closure to main graph */
                    make_epsilon(p, tr.next);
                    /* also add e-transition in copied graph in case of ^^ */
                    make_epsilon(p, mapped_states.at(tr.next));
                }
                else
                {
                    /* transition within copied e-closure */
                    make_copy(p, mapped_states.at(tr.next), tr.type);
                }
            }
        }

        /* remove all sof anchors */

        constexpr auto pred = [](const tnfa::transition<CharT>& tr)
        {
            return (std::holds_alternative<tnfa::sof_anchor_tr>(tr.type));
        };

        for (auto& node : nodes_)
            std::erase_if(node.tr, pred);

        /* reassign start node */

        start_node_ = mapped_states.at(start_node());
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_eof_anchors() 
    {
        std::vector<std::size_t> to_revisit;
        std::vector<std::size_t> ec;

        for (std::size_t q{ 0 }; q < node_count(); ++q)
        {
            for (const auto& tr: get_node(q).tr)
            {
                if (std::holds_alternative<tnfa::eof_anchor_tr>(tr.type))
                {
                    to_revisit.emplace_back(q);
                    ec.append_range(relaxed_e_closure(tr.next));
                }
            }
        }

        std::ranges::sort(ec);
        const auto [ret, last]{ std::ranges::unique(ec) };
        ec.erase(ret, last);

        /* create copy of e-closures  */

        std::flat_map<state_t, state_t> mapped_states;
        std::vector<std::pair<state_t, tnfa::final_node_info>> final_node_duplication_info;

        for (const std::size_t q : ec)
        {
            if (const auto it{ final_nodes_.find(q) }; it != final_nodes_.end()) 
            {  
                if (it->second.is_fallback)
                {
                    const state_t p{ node_create() };
                    mapped_states[q] = p;
                    final_node_duplication_info.emplace_back(p, it->second);
                }
                else
                {
                    /* avoid creating duplicate non-fallback final nodes */
                    mapped_states[q] = q;
                }
            }
            else
            {
                mapped_states[q] = node_create();
            }
        }

        /* duplicate transitions */

        for (const auto [q, p] : mapped_states)
        {
            if (q == p) continue;

            for (const auto& tr: nodes_.at(q).tr)
            {
                if (std::holds_alternative<tnfa::normal_tr<CharT>>(tr.type))
                {
                    /* do not insert transition */
                }
                else if (std::holds_alternative<tnfa::eof_anchor_tr>(tr.type))
                {
                    /* replace eof anchor with e-transition in copied subgraph in case of $$ */
                    make_epsilon(p, mapped_states.at(tr.next));
                }
                else
                {
                    /* transition within copied subgraph */
                    make_copy(p, mapped_states.at(tr.next), tr.type);
                }

            }
        }

        /* replace all eof anchors with transitions into copied subgraph */

        constexpr auto pred = [](const tnfa::transition<CharT>& tr)
        {
            return std::holds_alternative<tnfa::eof_anchor_tr>(tr.type);
        };

        for (const std::size_t q : to_revisit)
        {
            auto& trs{ nodes_.at(q).tr };
            for (auto it{ trs.begin() }; (it = std::ranges::find_if(it, trs.end(), pred)) != trs.end(); ++it)
            {
                it->next = mapped_states.at(it->next);
                it->type.template emplace<tnfa::epsilon_tr>(0, 0);
            }
        }

        /* duplicate final nodes */

        for (const auto& [mapped_state, fni] : final_node_duplication_info)
        {
            final_nodes_.emplace(mapped_state, tnfa::final_node_info{
                .continuation_index = fni.continuation_index,
                .is_fallback = false
            });
        }
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_assertions() 
    {
        if (has_sof_anchor_) rewrite_sof_anchors();
        if (has_eof_anchor_) rewrite_eof_anchors();


        if (has_sof_anchor_ or has_eof_anchor_ or has_lookahead_1_ or has_lookbehind_1_ or has_lookahead_n_) remove_dead_states();
    }

    
    /* constructor */

    template<typename CharT>
    constexpr tagged_nfa<CharT>::tagged_nfa(const expr_tree<CharT>& ast, fsm_flags flags) : 
        capture_info_{ ast.get_capture_info() }, tag_count_{ ast.tag_count() }, flags_{ flags }
    {
        if (flags_.is_iterator)
            cont_info_.emplace_back(start_node_);

        final_nodes_.emplace(default_final_node, tnfa::final_node_info{
            .continuation_index = start_node_,
            .is_fallback = (flags.enable_fallback and not flags_.longest_match)
        });

        thompson(ast);
        rewrite_assertions();
    }
}