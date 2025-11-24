#pragma once

#include <rx/fsm/tnfa.hpp>

#include <algorithm>
#include <concepts>
#include <iterator>
#include <limits>
#include <ranges>
#include <type_traits>
#include <utility>

#include <rx/api/regex_error.hpp>


namespace rx::detail
{
    struct stack_elem
    {
        tnfa::state_t q0, qf;
        std::size_t idx;
    };


    /* graph helper functions */

    template<typename CharT>
    constexpr auto tagged_nfa<CharT>::node_create() -> state_t
    {
        const state_t tmp{ nodes_.size() };
        nodes_.emplace_back();
        return tmp;
    }

    template<typename CharT>
    constexpr auto tagged_nfa<CharT>::node_copy(const state_t q) -> state_t
    {
        const state_t tmp{ nodes_.size() };
        nodes_.emplace_back();

        const auto& n{ nodes_.at(q) };
        nodes_[tmp].is_final = n.is_final;
        nodes_[tmp].is_fallback = n.is_fallback ;
        nodes_[tmp].continue_at = n.continue_at;
        nodes_[tmp].final_offset = n.final_offset;

        return tmp;
    }

    
    /* transition creation helper functions */

    template<typename CharT>
    template<typename... Args>
    constexpr void tagged_nfa<CharT>::transition_create(const state_t q0, const state_t qf, Args&&... args)
    {
        const std::size_t transition_index{ transitions_.size() };
        transitions_.emplace_back(q0, qf, std::forward<Args>(args)...);
        nodes_.at(q0).out_tr.emplace_back(transition_index);
        nodes_.at(qf).in_tr.emplace_back(transition_index);
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_epsilon(const state_t q0, const state_t qf, const int priority, const int tag)
    {
        transition_create(q0, qf, std::in_place_type<tnfa::epsilon_tr>, priority, tag);
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_transition(const state_t q0, const state_t qf, char_type c)
    {
        transition_create(q0, qf, std::in_place_type<tnfa::normal_tr<char_type>>, tnfa::charset_t<char_type>{ c });
    }

    template<typename CharT>
    template<typename CharSet>
    requires std::convertible_to<std::remove_cvref_t<CharSet>, tnfa::charset_t<CharT>>
    constexpr void tagged_nfa<CharT>::make_transition(const state_t q0, const state_t qf, CharSet&& cs)
    {
        transition_create(q0, qf, std::in_place_type<tnfa::normal_tr<char_type>>, std::forward<CharSet>(cs));
    }

    template<typename CharT>
    template<tnfa::assert_category V>
    constexpr void tagged_nfa<CharT>::make_assert(const state_t q0, const state_t qf, tnfa::ac<V> /* category */)
    {
        using type = std::conditional_t<V == acat_t::eof, tnfa::eof_anchor_tr,
                                        std::conditional_t<V == acat_t::sof, tnfa::sof_anchor_tr, void>>;

        transition_create(q0, qf, std::in_place_type<type>);
    }

    template<typename CharT>
    template<typename CharSet, tnfa::assert_category V>
    requires std::convertible_to<std::remove_cvref_t<CharSet>, tnfa::charset_t<CharT>>
    constexpr void tagged_nfa<CharT>::make_assert(const state_t q0, const state_t qf, CharSet&& cs, tnfa::ac<V> /* category */)
    {
        using type = std::conditional_t<V == acat_t::lookahead, tnfa::lookahead_1_tr<char_type>,
                                std::conditional_t<V == acat_t::lookbehind, tnfa::lookbehind_1_tr<char_type>, void>>;

        transition_create(q0, qf, std::in_place_type<type>, std::forward<CharSet>(cs));
    }

    // template<typename CharT>
    // template<tnfa::assert_category V>
    // constexpr void tagged_nfa<CharT>::make_assert(const state_t q0, const state_t qf, const state_t p0, const state_t pf, tnfa::ac<V> /* category */)
    // {
    //     using type = std::conditional_t<V == acat_t::lookahead, tnfa::lookahead_tr,
    //                             std::conditional_t<V == acat_t::lookbehind, tnfa::lookbehind_tr, void>>;

    //     transition_create(q0, qf, std::in_place_type<type>, p0, pf);
    // }
    
    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_copy(const state_t q0, const state_t qf, const transition_info& type)
    {
        transition_create(q0, qf, type);
    }


    /* modified thompson's algorithm */

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
    constexpr void tagged_nfa<CharT>::thompson(const expr_tree<char_type>& ast)
    {
        std::vector<std::vector<int>> tag_vec{};
        if (tag_count_ > 1) ast.make_tag_vec(tag_vec);

        std::vector<stack_elem> stack;
        stack.emplace_back(default_start_node, default_final_node, ast.root_idx());

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

                    if constexpr (char_is_utf8<char_type>)
                    {
                        // TODO: implement multibyte chars;
                        throw tnfa_error("UTF8 character classes are unimplemented");
                    }
                    else if constexpr (char_is_utf16<char_type>)
                    {
                        // TODO: implement multibyte chars;
                        throw tnfa_error("UTF16 character classes are unimplemented");
                    }
                    else if constexpr (std::same_as<tnfa::charset_t<char_type>, typename ast_t::char_class::impl_type::underlying_type>)
                    {
                        make_transition(q0, qf, cla.data.get());
                    }
                    else
                    {
                        // Should be unreachable?
                        throw tnfa_error("Cannot convert character class to representation used by tnfa");
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

                    using p = typename char_set_type::char_interval;

                    /* note: these are not Unicode-aware; TODO: FIX */
                    static constexpr char_set_type newline_cs{ '\n' };
                    static constexpr char_set_type word_cs{ p{ '0', '9'}, p{ 'A', 'Z' }, p{ 'a', 'z' }, '_' };
                    [[maybe_unused]] static constexpr char_set_type not_word_cs{ ~word_cs };

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
                        make_assert(q0, qf, newline_cs, tnfa::ac<acat_t::lookahead>{});
                        has_lookahead_1_ = true;
                        [[fallthrough]];
                    
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
    template<tnfa::ec_mode Mode, bool RetBitVec>
    constexpr auto tagged_nfa<CharT>::epsilon_closure(std::vector<state_t>&& qs) const -> ec_result<RetBitVec>
    {
        using result_t = std::conditional_t<RetBitVec, std::monostate, ec_result<RetBitVec>>;

        std::vector to_visit{ std::move(qs) };
        std::ranges::reverse(to_visit);
    
        std::vector<bool> visited(node_count(), false);
        result_t result;

        for (const auto q : to_visit)
            visited.at(q) = true;  

        while (not to_visit.empty())
        {
            const state_t current{ to_visit.back() };
            to_visit.pop_back();

            if constexpr (not RetBitVec)
                result.emplace_back(current);
            
            for (const std::size_t tr_idx : get_node(current).out_tr | std::views::reverse)
            {
                const auto& tr{ get_tr(tr_idx) };

                if constexpr (Mode == tnfa::ec_mode::ec)
                {
                    /* only count e-transitions */
                    if (not std::holds_alternative<tnfa::epsilon_tr>(tr.type))
                        continue;
                }
                else if constexpr (Mode == tnfa::ec_mode::aec)
                {
                    /* allow assertions as e-transitions */
                    if (std::holds_alternative<tnfa::normal_tr<char_type>>(tr.type))
                        continue;
                }

                if (visited.at(tr.dst))
                    continue;

                visited[tr.dst] = true;
                to_visit.emplace_back(tr.dst);
            }
        }

        if constexpr (RetBitVec)
            return visited;
        else
            return result;
    }

    template<typename CharT>
    template<tnfa::ec_mode Mode, bool RetBitVec>
    constexpr auto tagged_nfa<CharT>::backwards_epsilon_closure(std::vector<state_t>&& qs) const -> ec_result<RetBitVec>
    {
        using result_t = std::conditional_t<RetBitVec, std::monostate, ec_result<RetBitVec>>;

        std::vector to_visit{ std::move(qs) };
        std::ranges::reverse(to_visit);
        
        std::vector<bool> visited(node_count(), false);
        result_t result;

        for (const auto q : to_visit)
            visited.at(q) = true;        

        while (not to_visit.empty())
        {
            const state_t current{ to_visit.back() };
            to_visit.pop_back();

            if constexpr (not RetBitVec)
                result.emplace_back(current);
            
            for (const std::size_t tr_idx : get_node(current).in_tr | std::views::reverse)
            {
                const auto& tr{ get_tr(tr_idx) };

                if constexpr (Mode == tnfa::ec_mode::ec)
                {
                    /* only count e-transitions */
                    if (not std::holds_alternative<tnfa::epsilon_tr>(tr.type))
                        continue;
                }
                else if constexpr (Mode == tnfa::ec_mode::aec)
                {
                    /* allow assertions as e-transitions */
                    if (std::holds_alternative<tnfa::normal_tr<char_type>>(tr.type))
                        continue;
                }

                if (visited.at(tr.src))
                    continue;

                visited[tr.src] = true;
                to_visit.emplace_back(tr.src);
            }
        }

        if constexpr (RetBitVec)
            return visited;
        else
            return result;
    }


    /* dead state elimination */

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::remove_dead_and_unreachable_states()
    {
        /* determine reachable states */

        std::vector<state_t> initial_nodes{ start_node_ };
    
        for (const auto& ci :cont_info_)
            if (ci.value != start_node_)
                initial_nodes.emplace_back(ci.value);

        const auto reachable_nodes{ epsilon_closure<tnfa::ec_mode::reach, true>(std::move(initial_nodes)) };

        /* determine live states */

        std::vector<state_t> final_nodes{};
    
        for (state_t q{ 0 }; q < nodes_.size(); ++q)
            if (nodes_[q].is_final)
                final_nodes.emplace_back(q);

        const auto live_nodes{ backwards_epsilon_closure<tnfa::ec_mode::reach, true>(std::move(final_nodes)) };

        /* remove transitions containing dead and unreachable nodes */

        std::vector<bool> live_and_reachable(nodes_.size(), false);
        std::ranges::transform(live_nodes, reachable_nodes, live_and_reachable.begin(), std::logical_and{});

        std::vector<bool> removed_transitions(transitions_.size(), false);

        for (std::size_t i{ 0 }; i < transitions_.size(); ++i)
        {
            auto& tr{ transitions_[i] };

            if (tr.src == std::numeric_limits<state_t>::max() or tr.dst == std::numeric_limits<state_t>::max())
                continue;

            if (live_and_reachable.at(tr.src) and live_and_reachable.at(tr.dst))
                continue;

            removed_transitions[i] = true;
            tr.unset();
        }

        /* remove dead and unreachable nodes and transitions from nodes */

        const auto pred = [&](const std::size_t i){ return removed_transitions.at(i); };

        for (state_t q{ 0 }; q < nodes_.size(); ++q)
        {
            tnfa::node& node{ nodes_[q] };

            if (live_nodes[q])
            {
                std::erase_if(node.in_tr, pred);
                std::erase_if(node.out_tr, pred);
            }
            else
            {
                /* reset from, to, and is_final */
                node = tnfa::node{};
            }
        }
    }

    /* assertion elimination */

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_sof_anchors() 
    {
        const auto ec{ epsilon_closure<tnfa::ec_mode::aec>({ start_node() }) };

        /* create a copy of the start node's e-closure */

        std::flat_map<std::size_t, std::size_t> mapped_states;
        
        for (const state_t q : ec)
        {
            if (const auto& node{ get_node(q) }; node.is_final) 
                mapped_states[q] = q; /* do not create duplicate final nodes */
            else
                mapped_states[q] = node_copy(q);
        }

        /* duplicate transitions */

        for (const auto [q, p] : mapped_states)
        {
            /* q == p is only possible when there are no transitions from q */
            for (const std::size_t tr_idx : nodes_.at(q).out_tr)
            {
                /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
                const auto& tr{ get_tr(tr_idx) }; 

                if (std::holds_alternative<tnfa::normal_tr<char_type>>(tr.type))
                {
                    /* transition from copied e-closure to main graph */
                    make_copy(p, tr.dst, tr.type);
                }
                else if (std::holds_alternative<tnfa::sof_anchor_tr>(tr.type))
                {
                     /* replace sof anchor with e-transition in copied subgraph */
                    make_epsilon(p, mapped_states.at(tr.dst));
                }
                else
                {
                    /* transition within copied e-closure */
                    make_copy(p, mapped_states.at(tr.dst), tr.type);
                }
            }
        }

        /* remove all sof anchors */

        for (std::size_t tr_idx{ 0 }; tr_idx < transitions_.size(); ++tr_idx)
        {
            auto& tr{ transitions_[tr_idx] };

            if (not std::holds_alternative<tnfa::sof_anchor_tr>(tr.type))
                continue;

            /* delete transition */
            std::erase(nodes_.at(tr.src).out_tr, tr_idx);
            std::erase(nodes_.at(tr.dst).in_tr, tr_idx);
            tr.unset();
        }
            

        /* reassign start node */

        start_node_ = mapped_states.at(start_node());
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_eof_anchors() 
    {
        /* NOTE: this function must be called before any other function which adds final nodes */
        std::vector<bool> bec{ backwards_epsilon_closure<tnfa::ec_mode::aec, true>({ default_final_node }) };

        std::vector<state_t> initial;
        std::vector<std::size_t> to_revisit;

        for (std::size_t tr_idx{ 0 }; tr_idx < transitions_.size(); ++tr_idx)
        {
            const auto& tr{ transitions_[tr_idx] };

            if (not std::holds_alternative<tnfa::eof_anchor_tr>(tr.type))
                continue;

            to_revisit.emplace_back(tr_idx);

            if (bec.at(tr.dst))
                initial.emplace_back(tr.dst);
        }

        const auto ec{ epsilon_closure<tnfa::ec_mode::aec>(std::move(initial)) };

        /* create copy of the e-closures  */

        std::flat_map<state_t, state_t> mapped_states;

        for (const state_t q : ec)
        {
            if (const auto& node{ get_node(q) }; node.is_final) 
            {  
                if (node.is_fallback)
                {
                    /* create duplicate non-fallback final node */
                    const state_t p{ node_copy(q) };
                    nodes_[p].is_fallback = false;
                    mapped_states[q] = p;
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
            /* q == p is only possible when there are no transitions from q */
            for (const std::size_t tr_idx : nodes_.at(q).out_tr)
            {
                /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
                const auto& tr{ get_tr(tr_idx) }; 

                if (std::holds_alternative<tnfa::normal_tr<char_type>>(tr.type))
                {
                    /* do not insert transition */
                }
                else if (std::holds_alternative<tnfa::eof_anchor_tr>(tr.type))
                {
                    /* replace eof anchor with e-transition in copied subgraph */
                    make_epsilon(p, mapped_states.at(tr.dst));
                }
                else
                {
                    /* transition within copied subgraph */
                    make_copy(p, mapped_states.at(tr.dst), tr.type);
                }
            }
        }

        /* replace all eof anchors with transitions into copied subgraph, or remove if not possible */

        for (const std::size_t tr_idx : to_revisit)
        {
            auto& tr{ transitions_[tr_idx] };

            if (auto it{ mapped_states.find(tr.dst) }; it != mapped_states.end())
            {
                /* remap rhs and replace with e-transition */
                const auto [q, p]{ *it };
                tr.type.template emplace<tnfa::epsilon_tr>(0, 0);

                if (q != p)
                {
                    std::erase(nodes_.at(q).in_tr, tr_idx);
                    nodes_.at(p).in_tr.emplace_back(tr_idx);
                    tr.dst = p;
                }
            }
            else
            {
                /* delete transition */
                std::erase(nodes_.at(tr.src).out_tr, tr_idx);
                std::erase(nodes_.at(tr.dst).in_tr, tr_idx);
                tr.unset();
            }
        }
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_sc_lookahead()
    {
        using transition_map_t = std::flat_map<tnfa::charset_t<char_type>, std::vector<std::size_t>>;
        using state_map_t = std::flat_map<std::size_t, std::size_t>;
        using all_states_map_t = std::flat_map<tnfa::charset_t<char_type>, state_map_t>;

        transition_map_t sc_transitions; 

        for (std::size_t tr_idx{ 0 }; tr_idx < transitions_.size(); ++tr_idx)
        {
            const auto& tr{ transitions_[tr_idx] };
            if (const auto* ptr{ std::get_if<tnfa::lookahead_1_tr<char_type>>(&tr.type) }; ptr != nullptr)
                sc_transitions[ptr->cs].emplace_back(tr_idx);
        }

        /* create copy of the e-closures of destinations of lookahead_1 transitions  */

        all_states_map_t all_mapped_states;

        std::vector<std::pair<char_set_type, std::vector<state_t>>> outer_visit;
        
        for (const auto& [edge, tr_vec] : sc_transitions)
        {
            outer_visit.emplace_back(edge, std::vector<state_t>{});
            outer_visit.back().second.reserve(tr_vec.size());
            for (const std::size_t tr_idx : tr_vec | std::views::reverse)
                outer_visit.back().second.emplace_back(transitions_[tr_idx].dst);
        }

        while (not outer_visit.empty())
        {
            /* modified epsilon closure */
            const auto edge{ std::move(outer_visit.back().first) };
            std::vector to_visit{ std::move(outer_visit.back().second) };
            outer_visit.pop_back();

            std::vector<bool> visited(node_count(), false);
            std::vector<state_t> ec;

            for (const auto q : to_visit)
                visited.at(q) = true;  

            while (not to_visit.empty())
            {
                const state_t current{ to_visit.back() };
                to_visit.pop_back();
                ec.emplace_back(current);
                
                for (const std::size_t tr_idx : get_node(current).out_tr | std::views::reverse)
                {
                    const auto& tr{ get_tr(tr_idx) };

                    if (visited.at(tr.dst))
                        continue;

                    /* skip normal transitions and eof anchors */
                    if (std::holds_alternative<tnfa::normal_tr<char_type>>(tr.type) or std::holds_alternative<tnfa::eof_anchor_tr>(tr.type) )
                        continue;

                    if (const auto* const ptr{ std::get_if<tnfa::lookahead_1_tr<char_type>>(&tr.type) }; ptr != nullptr)
                    {
                        if (auto new_edge{ edge & ptr->cs }; not new_edge.empty() and new_edge != ptr->cs)
                        {
                            /* intersection with lookahead_1 transition requires cloned subgraph */
                            outer_visit.emplace_back(std::move(new_edge), std::vector{ tr.dst });
                        }
                    }
                    else
                    {
                        visited[tr.dst] = true;
                        to_visit.emplace_back(tr.dst);
                    }
                }
            }

            /* duplicate all nodes in each subgraph at most once */
            auto& mapped_states{ all_mapped_states[edge] };
            for (const state_t q : ec)
                if (not mapped_states.contains(q)) /* probably inefficient */
                    mapped_states[q] = node_create();

        }

        /* duplicate transitions */

        std::optional<state_t> offset_end;

        for (const auto& [edge, mapped_states] : all_mapped_states) 
        {
            for (const auto [q, p] : mapped_states)
            {
                if (const auto& node{ nodes_.at(q) }; node.is_final and node.is_fallback)
                {
                    /* reminder: reference 'node' may be invalidated after calling node_create */

                    if (not offset_end.has_value())
                    {
                        /* create new offset end node */
                        offset_end = node_create();
                        auto& n{ nodes_.at(*offset_end) };
                        n.is_final = true;
                        n.is_fallback = true ;
                        n.final_offset = 1;
                    }

                    make_transition(p, *offset_end, edge);
                }

                for (const std::size_t tr_idx : nodes_.at(q).out_tr)
                {
                    /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
                    const auto& tr{ get_tr(tr_idx) }; 

                    if (const auto* const ptr{ std::get_if<tnfa::normal_tr<char_type>>(&tr.type) }; ptr != nullptr)
                    {
                        /* conditionally transition from copied e-closure to main graph */
                        if (auto new_edge{ edge & ptr->cs }; not new_edge.empty())
                            make_transition(p, tr.dst, std::move(new_edge));
                    }
                    else if (const auto* const ptr{ std::get_if<tnfa::lookahead_1_tr<char_type>>(&tr.type) }; ptr != nullptr)
                    {
                        /* conditionally e-transition between copied subgraphs */
                        if (auto new_edge{ edge & ptr->cs }; not new_edge.empty())
                            make_epsilon(p, all_mapped_states.at(std::move(new_edge)).at(tr.dst));                            
                    }
                    else if (not std::holds_alternative<tnfa::eof_anchor_tr>(tr.type))
                    {
                        /* transition within copied e-closure */
                        make_copy(p, mapped_states.at(tr.dst), tr.type);
                    }
                }
            }
        }

        /* replace lookahead_1 transitions with e-transitions into their respective subgraphs */

        for (const auto& [edge, tr_vec] : sc_transitions)
        {
            const auto& mapped_states{ all_mapped_states.at(edge) };
            for (const std::size_t tr_idx : tr_vec)
            {
                auto& tr{ transitions_.at(tr_idx) };
                tr.dst = mapped_states.at(tr.dst);
                tr.type = tnfa::epsilon_tr{};
            }
        }
    }



    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_sc_lookbehind()
    {

    } 

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_assertions() 
    {
        if (has_sof_anchor_) rewrite_sof_anchors();
        if (has_eof_anchor_) rewrite_eof_anchors();
        if (has_lookahead_1_) rewrite_sc_lookahead();
        // if (has_lookbehind_1_) rewrite_sc_lookbehind();


        if (has_sof_anchor_ or has_eof_anchor_ or has_lookahead_1_ or has_lookbehind_1_ or has_lookahead_n_) remove_dead_and_unreachable_states();
    }

    
    /* constructor */

    template<typename CharT>
    constexpr tagged_nfa<CharT>::tagged_nfa(const expr_tree<char_type>& ast, fsm_flags flags) : 
        capture_info_{ ast.get_capture_info() }, tag_count_{ ast.tag_count() }, flags_{ flags }
    {
        nodes_[default_final_node].is_final = true;
        nodes_[default_final_node].is_fallback = (flags.enable_fallback and not flags_.longest_match);

        if (flags_.is_iterator)
            cont_info_.emplace_back(default_start_node);

        thompson(ast);
        rewrite_assertions();
    }
}