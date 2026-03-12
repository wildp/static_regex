// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "tnfa.hpp"

#include <algorithm>
#include <concepts>
#include <functional>
#include <iterator>
#include <limits>
#include <numeric>
#include <ranges>
#include <type_traits>
#include <utility>

#include "rx/api/regex_error.hpp"
#include "rx/etc/static_charset.hpp"
#include "rx/etc/vec_bool_adaptor.hpp"


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
        const state_t p{ nodes_.size() };
        nodes_.emplace_back();
        return p;
    }

    template<typename CharT>
    constexpr auto tagged_nfa<CharT>::node_copy(const state_t q) -> state_t
    {
        const state_t p{ nodes_.size() };
        auto& new_n{ nodes_.emplace_back() };
        const auto& old_n{ nodes_.at(q) };

        new_n.is_final = old_n.is_final;
        new_n.is_fallback = old_n.is_fallback;
        new_n.final_offset = old_n.final_offset;
        new_n.continue_at = old_n.continue_at;

        return p;
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
        transition_create(q0, qf, std::in_place_type<epsilon_tr>, priority, tag);
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::make_transition(const state_t q0, const state_t qf, char_type c)
    {
        transition_create(q0, qf, std::in_place_type<normal_tr>, charset_type{ c });
    }

    template<typename CharT>
    template<typename CharSet>
    requires std::convertible_to<std::remove_cvref_t<CharSet>, tnfa::charset_t<CharT>>
    constexpr void tagged_nfa<CharT>::make_transition(const state_t q0, const state_t qf, CharSet&& cs)
    {
        transition_create(q0, qf, std::in_place_type<normal_tr>, std::forward<CharSet>(cs));
    }

    template<typename CharT>
    template<typename T>
    requires one_of<T, tnfa::assert_category::eof_tag_t, tnfa::assert_category::sof_tag_t>
    constexpr void tagged_nfa<CharT>::make_assert(const state_t q0, const state_t qf, T /* category */)
    {
        using type = std::conditional_t<std::same_as<T, tnfa::assert_category::eof_tag_t>, eof_anchor_tr, sof_anchor_tr>;

        transition_create(q0, qf, std::in_place_type<type>);
    }

    template<typename CharT>
    template<typename T, typename CharSet>
    requires one_of<T, tnfa::assert_category::lookahead1_tag_t, tnfa::assert_category::lookbehind1_tag_t>
             and std::convertible_to<std::remove_cvref_t<CharSet>, tnfa::charset_t<CharT>>
    constexpr void tagged_nfa<CharT>::make_assert(const state_t q0, const state_t qf, T /* category */, CharSet&& cs)
    {
        using type = std::conditional_t<std::same_as<T, tnfa::assert_category::lookahead1_tag_t>, lookahead_1_tr, lookbehind_1_tr>;

        transition_create(q0, qf, std::in_place_type<type>, std::forward<CharSet>(cs));
    }

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
                if (const auto& str{ get<typename ast_t::char_str>(entry) }; not str.data.empty())
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
                const auto& cla{ get<typename ast_t::char_class>(entry) };
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
                else if constexpr (std::same_as<charset_type, typename ast_t::char_class::impl_type::underlying_type>)
                {
                    make_transition(q0, qf, cla.data.get());
                }
                else
                {
                    // Should be unreachable?
                    throw tnfa_error("Cannot convert character class to representation used by tnfa");
                }
                break;
            }

            case ast_index<typename ast_t::concat>:
                if (const auto& cat{ get<typename ast_t::concat>(entry) }; not cat.idxs.empty())
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
                if (const auto& alt{ get<typename ast_t::alt>(entry) }; not alt.idxs.empty())
                {
                    if (tag_vec.empty() or tag_vec.at(idx).empty())
                    {
                        /* generate naive tag-free nfa */
                        for (std::size_t i{ 0 }, i_end{ alt.idxs.size() }; i < i_end; ++i)
                        {
                            const state_t qi{ node_create() };
                            make_epsilon(q0, qi, i);
                            stack.emplace_back(qi, qf, alt.idxs.at(i));
                        }
                    }
                    else
                    {
                        /* generate tag-aware nfa */
                        for (std::size_t i{ 0 }, i_end{ alt.idxs.size() }; i < i_end; ++i)
                        {
                            if (i + 1 == i_end)
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
                                for (std::size_t j{ i + 1 }; j < i_end; ++j)
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
                const auto& rep{ get<typename ast_t::repeat>(entry) };

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
                break;
            }

            case ast_index<typename ast_t::tag>:
            {
                /* in tree_expr tags start at 0, whereas here they start at 1 */
                const auto& tag_entry{ get<typename ast_t::tag>(entry) };
                make_epsilon(q0, qf, 0, tag_entry.number + 1);
                break;
            }

            case ast_index<typename ast_t::assertion>:
            {
                const typename ast_t::assertion& assertion{ get<typename ast_t::assertion>(entry) };

                using cs = nontransient_constexpr_version_of_t<charset_type>;
                using p = cs::char_interval;
                namespace ac = tnfa::assert_category;

                /* note: these are not Unicode-aware; TODO: FIX */
                static constexpr cs newline_cs{ '\n' };
                static constexpr cs word_cs{ p{ '0', '9' }, p{ 'A', 'Z' }, p{ 'a', 'z' }, '_' };
                static constexpr cs not_word_cs{ ~word_cs };

                switch (assertion.type)
                {
                case assert_type::line_start:
                    make_assert(q0, qf, ac::lookbehind1_tag, newline_cs);
                    has_lookbehind_1_ = true;
                    [[fallthrough]];

                case assert_type::text_start:
                    make_assert(q0, qf, ac::sof_tag);
                    has_sof_anchor_ = true;
                    break;

                case assert_type::line_end:
                    make_assert(q0, qf, ac::lookahead1_tag, newline_cs);
                    has_lookahead_1_ = true;
                    [[fallthrough]];

                case assert_type::text_end:
                    make_assert(q0, qf, ac::eof_tag);
                    has_eof_anchor_ = true;
                    break;

                case assert_type::ascii_word_boundary:
                {
                    const state_t q1{ node_create() };
                    const state_t q2{ node_create() };

                    make_assert(q0, q1, ac::sof_tag);
                    make_assert(q0, q1, ac::lookbehind1_tag, not_word_cs);
                    make_assert(q1, qf, ac::lookahead1_tag, word_cs);

                    make_assert(q0, q2, ac::lookbehind1_tag, word_cs);
                    make_assert(q2, qf, ac::lookahead1_tag, not_word_cs);
                    make_assert(q2, qf, ac::eof_tag);

                    has_eof_anchor_ = true;
                    has_sof_anchor_ = true;
                    has_lookahead_1_ = true;
                    has_lookbehind_1_ = true;
                    break;
                }

                case assert_type::not_ascii_word_boundary:
                {
                    const state_t q1{ node_create() };
                    const state_t q2{ node_create() };

                    make_assert(q0, q1, ac::sof_tag);
                    make_assert(q0, q1, ac::lookbehind1_tag, not_word_cs);
                    make_assert(q1, qf, ac::lookahead1_tag, not_word_cs);
                    make_assert(q1, qf, ac::eof_tag);

                    make_assert(q0, q2, ac::lookbehind1_tag, word_cs);
                    make_assert(q2, qf, ac::lookahead1_tag, word_cs);

                    has_eof_anchor_ = true;
                    has_sof_anchor_ = true;
                    has_lookahead_1_ = true;
                    has_lookbehind_1_ = true;
                    break;
                }

                default:
                    throw tnfa_error("Non-trivial assertions are unimplemented");
                    break;
                }
                break;
            }


            case ast_index<typename ast_t::backref>:
                throw tnfa_error("Backreferences are unsupported");

            default:
                throw tree_error("Invalid tree");
            }
        }
    }


    /* observers */

    template<typename CharT>
    template<bool B, typename Vec, typename Pred, typename NodeProj, typename TrProj>
    constexpr auto tagged_nfa<CharT>::closure_impl(Vec&& qs, Pred pred, NodeProj node_proj, TrProj tr_proj) const
    {
        using result_t = maybe_type_t<B, std::vector<state_t>>;

        std::vector to_visit{ std::forward<Vec>(qs) };
        std::ranges::reverse(to_visit);

        bitset_t visited(node_count(), false);
        result_t result;

        for (const auto q : to_visit)
            visited.at(q) = true;

        while (not to_visit.empty())
        {
            const state_t current{ to_visit.back() };
            to_visit.pop_back();

            if constexpr (B)
                result.emplace_back(current);

            for (const tr_index i : std::invoke(node_proj, get_node(current)) | std::views::reverse)
            {
                const auto& tr{ get_tr(i) };
                const auto val{ std::invoke(tr_proj, tr) };

                if constexpr (requires { { std::invoke(pred, tr, i) } -> std::convertible_to<bool>; })
                {
                    if (not std::invoke(pred, tr, i))
                        continue;
                }
                else if constexpr (requires { { std::invoke(pred, tr) } -> std::convertible_to<bool>; })
                {
                    if (not std::invoke(pred, tr))
                        continue;
                }
                else
                {
                    static_assert(false, "tagged_nfa::closure_impl: invalid predicate");
                }

                if (visited.at(val))
                    continue;


                visited[val] = true;
                to_visit.emplace_back(val);
            }
        }

        if constexpr (B)
            return result;
        else
            return visited;
    }

    template<typename CharT>
    template<bool B, typename Vec, typename Pred>
    constexpr auto tagged_nfa<CharT>::epsilon_closure(Vec&& qs, Pred pred) const
    {
        return closure_impl<B>(std::forward<Vec>(qs), pred, &tnfa::node::out_tr, &tnfa::transition<char_type>::dst);
    }

    template<typename CharT>
    template<bool B, typename Vec, typename Pred>
    constexpr auto tagged_nfa<CharT>::backwards_epsilon_closure(Vec&& qs, Pred pred) const
    {
        return closure_impl<B>(std::forward<Vec>(qs), pred, &tnfa::node::in_tr, &tnfa::transition<char_type>::src);
    }


    /* dead state elimination */

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::remove_dead_and_unreachable_states()
    {
        /* determine reachable states */

        std::vector<state_t> initial_nodes{ start_node_ };

        for (const auto& ci : cont_info_)
            if (ci.value != start_node_)
                initial_nodes.emplace_back(ci.value);

        const auto reachable_nodes{ epsilon_closure<false>(std::move(initial_nodes), tnfa::reachable_predicate{}) };

        /* determine live states */

        std::vector<state_t> final_nodes{};

        for (state_t q{ 0 }, q_end{ nodes_.size() }; q < q_end; ++q)
            if (nodes_[q].is_final)
                final_nodes.emplace_back(q);

        const auto live_nodes{ backwards_epsilon_closure<false>(std::move(final_nodes), tnfa::reachable_predicate{}) };

        /* remove transitions containing dead and unreachable nodes */

        const auto live_and_reachable{ live_nodes & reachable_nodes };

        bitset_t removed_transitions(transitions_.size(), false);

        for (tr_index i{ 0 }, i_end{ transitions_.size() }; i < i_end; ++i)
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

        const auto pred = [&rt = std::as_const(removed_transitions)](const std::size_t i) { return rt.at(i); };

        for (state_t q{ 0 }, q_end{ nodes_.size() }; q < q_end; ++q)
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
        static constexpr auto pred = [](const tnfa::transition<char_type>& tr) static {
            return not holds_alternative<normal_tr>(tr.type) and not holds_alternative<lookbehind_1_tr>(tr.type);
        };

        const auto ec{ epsilon_closure({ start_node() }, pred) };

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
            for (const tr_index i : nodes_.at(q).out_tr)
            {
                /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
                const auto& tr{ get_tr(i) };

                if (holds_alternative<normal_tr>(tr.type))
                {
                    /* transition from copied e-closure to main graph */
                    make_copy(p, tr.dst, tr.type);
                }
                else if (holds_alternative<sof_anchor_tr>(tr.type))
                {
                    /* replace sof anchor with e-transition in copied subgraph */
                    make_epsilon(p, mapped_states.at(tr.dst));
                }
                else if (not holds_alternative<lookbehind_1_tr>(tr.type))
                {
                    /* transition within copied e-closure */
                    make_copy(p, mapped_states.at(tr.dst), tr.type);
                }
            }
        }

        /* remove all sof anchors */

        for (tr_index i{ 0 }; i < transitions_.size(); ++i)
        {
            auto& tr{ transitions_[i] };

            if (not holds_alternative<sof_anchor_tr>(tr.type))
                continue;

            /* delete transition */
            std::erase(nodes_.at(tr.src).out_tr, i);
            std::erase(nodes_.at(tr.dst).in_tr, i);
            tr.unset();
        }


        /* reassign start node */

        start_node_ = mapped_states.at(start_node_);
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_eof_anchors()
    {
        /* NOTE: this function must be called before any other function which adds final nodes */

        static constexpr auto pred = [](const tnfa::transition<char_type>& tr) static {
            return not holds_alternative<normal_tr>(tr.type) and not holds_alternative<lookahead_1_tr>(tr.type);
        };

        const auto bec{ backwards_epsilon_closure<false>({ default_final_node }, pred) };

        std::vector<state_t> initial;
        std::vector<tr_index> to_revisit;

        for (tr_index i{ 0 }; i < transitions_.size(); ++i)
        {
            const auto& tr{ transitions_[i] };

            if (not holds_alternative<eof_anchor_tr>(tr.type))
                continue;

            to_revisit.emplace_back(i);

            if (bec.at(tr.dst))
                initial.emplace_back(tr.dst);
        }

        const auto ec{ epsilon_closure(std::move(initial), pred) };

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
            for (const tr_index i : nodes_.at(q).out_tr)
            {
                /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
                const auto& tr{ get_tr(i) };

                if (holds_alternative<normal_tr>(tr.type))
                {
                    /* do not insert transition */
                }
                else if (holds_alternative<eof_anchor_tr>(tr.type))
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

        for (const tr_index i : to_revisit)
        {
            auto& tr{ transitions_.at(i) };

            if (auto it{ mapped_states.find(tr.dst) }; it != mapped_states.end())
            {
                /* remap rhs and replace with e-transition */
                const auto [q, p]{ *it };
                tr.type.template emplace<tnfa::epsilon_tr>(0, 0);

                if (q != p)
                {
                    std::erase(nodes_.at(q).in_tr, i);
                    nodes_.at(p).in_tr.emplace_back(i);
                    tr.dst = p;
                }
            }
            else
            {
                /* delete transition */
                std::erase(nodes_.at(tr.src).out_tr, i);
                std::erase(nodes_.at(tr.dst).in_tr, i);
                tr.unset();
            }
        }
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_sc_lookahead()
    {
        using transition_map_t = std::flat_map<charset_type, std::vector<tr_index>>;
        using state_map_t = std::flat_map<state_t, state_t>;
        using all_states_map_t = std::flat_map<charset_type, state_map_t>;

        transition_map_t sc_transitions;

        for (tr_index i{ 0 }; i < transitions_.size(); ++i)
        {
            const auto& tr{ transitions_[i] };
            if (const auto* ptr{ get_if<lookahead_1_tr>(&tr.type) }; ptr != nullptr)
                sc_transitions[ptr->cs].emplace_back(i);
        }

        /* create copy of the e-closures of destinations of lookahead_1 transitions  */

        all_states_map_t all_mapped_states;

        std::vector<std::pair<charset_type, std::vector<state_t>>> outer_visit;

        for (const auto& [edge, tr_vec] : sc_transitions)
        {
            outer_visit.emplace_back(
                edge,
                std::vector<state_t>{
                    std::from_range,
                    tr_vec | std::views::transform([this](const tr_index i) { return transitions_.at(i).dst; })
                }
            );
        }

        while (not outer_visit.empty())
        {
            const auto edge{ std::move(outer_visit.back().first) };
            std::vector tmp{ std::move(outer_visit.back().second) };
            outer_visit.pop_back();

            /* perform a modified epsilon closure */

            auto pred = [&](const tnfa::transition<char_type>& tr) {
                /* skip normal transitions and eof anchors */
                if (holds_alternative<normal_tr>(tr.type) or holds_alternative<eof_anchor_tr>(tr.type))
                    return false;

                if (const auto* const ptr{ get_if<lookahead_1_tr>(&tr.type) }; ptr != nullptr)
                {
                    if (auto new_edge{ edge & ptr->cs }; not new_edge.empty() and new_edge != ptr->cs)
                    {
                        /* intersection with lookahead_1 transition requires a cloned subgraph */
                        outer_visit.emplace_back(std::move(new_edge), std::vector{ tr.dst });
                    }

                    return false;
                }

                return true;
            };

            const auto ec{ epsilon_closure(std::move(tmp), pred) };


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
                        const auto saved_cont{ node.continue_at };
                        offset_end = node_create();
                        auto& n{ nodes_.at(*offset_end) };
                        n.is_final = true;
                        n.is_fallback = true;
                        n.final_offset = 1;
                        n.continue_at = saved_cont;
                    }

                    make_transition(p, *offset_end, edge);
                }

                for (const tr_index i : nodes_.at(q).out_tr)
                {
                    /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
                    const auto& tr{ get_tr(i) };

                    if (const auto* const ptr{ get_if<normal_tr>(&tr.type) }; ptr != nullptr)
                    {
                        /* conditionally transition from copied e-closure to main graph */
                        if (auto new_edge{ edge & ptr->cs }; not new_edge.empty())
                            make_transition(p, tr.dst, std::move(new_edge));
                    }
                    else if (const auto* const ptr{ get_if<lookahead_1_tr>(&tr.type) }; ptr != nullptr)
                    {
                        /* conditionally e-transition between copied subgraphs */
                        if (auto new_edge{ edge & ptr->cs }; not new_edge.empty())
                            make_epsilon(p, all_mapped_states.at(std::move(new_edge)).at(tr.dst));
                    }
                    else if (not holds_alternative<eof_anchor_tr>(tr.type))
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
            for (const tr_index i : tr_vec)
            {
                auto& tr{ transitions_.at(i) };

                /* assign the lowest priority to avoid clashes with eof_anchor */
                using priority_t = decltype(epsilon_tr::priority);
                const auto p{ std::saturate_cast<priority_t>(std::sub_sat(nodes_.at(tr.src).out_tr.size(), 1uz)) };

                std::erase(nodes_.at(tr.dst).in_tr, i);
                tr.dst = mapped_states.at(tr.dst);
                tr.type = tnfa::epsilon_tr{ .priority = p, .tag = 0 };
                nodes_.at(tr.dst).in_tr.emplace_back(i);
            }
        }
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_sc_lookbehind()
    {
        if (cont_info_.size() > 1)
            throw tnfa_error("tagged_nfa::rewrite_sc_lookbehind: cont_info_.size() > 1");

        using transition_map_t = std::flat_map<charset_type, std::vector<tr_index>>;
        using state_map_t = std::flat_map<state_t, state_t>;
        using all_states_map_t = std::flat_map<charset_type, state_map_t>;

        using charset_set_t = std::flat_map<charset_type, std::monostate>;
        using start_tr_map_t = std::flat_map<tr_index, charset_set_t>;

        // consider narrow -> wide -> narrow combination of lookbehinds

        /* find fallback and continuation states */

        std::optional<state_t> continue_state;
        if (not cont_info_.empty())
            continue_state = cont_info_.front().value;

        std::vector<state_t> fallback_states;
        for (state_t q{ 0 }, q_end{ nodes_.size() }; q < q_end; ++q)
            if (const auto& node{ nodes_[q] }; node.is_final and node.is_fallback and not node.in_tr.empty())
                fallback_states.emplace_back(q);


        /* setup for finding transitions leading into backwards e-closures */

        struct lb_info
        {
            std::vector<tr_index> tr_into;
            std::vector<tr_index> tr_between;
        };

        static constexpr auto search_predicate = [](lb_info& lb, const charset_type& edge, const tnfa::transition<char_type>& tr, const tr_index i) static {
            /* assign results here instead of using the normal return value */
            if (const auto* const ptr{ get_if<normal_tr>(&tr.type) }; ptr != nullptr)
            {
                if (const auto new_edge{ edge & ptr->cs }; not new_edge.empty())
                    lb.tr_into.emplace_back(i);

                return false;
            }

            /* skip sof anchors */
            if (holds_alternative<sof_anchor_tr>(tr.type))
                return false;

            /* intersection with lookbehind_1 transition requires cloned subgraph */
            if (const auto* const ptr{ get_if<lookbehind_1_tr>(&tr.type) }; ptr != nullptr)
            {
                if (const auto new_edge{ edge & ptr->cs }; not new_edge.empty() and new_edge != ptr->cs)
                    lb.tr_between.emplace_back(i);

                return false;
            }

            return true;
        };

        static constexpr auto dedup_transitions_map = [](transition_map_t& map) static {
            for (auto it{ map.begin() }, end{ map.end() }; it != end; ++it)
            {
                auto& vec{ it->second };
                std::ranges::sort(vec);
                const auto [first, last]{ std::ranges::unique(vec) };
                vec.erase(first, last);
            }
        };


        /* find lookbehind transitions and group by edge */

        transition_map_t sc_transitions;
        std::flat_map<tr_index, lb_info> lb_closures;

        std::vector<tr_index> wraparounds; /* sorted; i.e. flat_set */
        charset_type wraparound_union;

        for (tr_index i{ 0 }, i_end{ transitions_.size() }; i < i_end; ++i)
        {
            const auto& tr{ transitions_[i] };
            const auto* ptr{ get_if<lookbehind_1_tr>(&tr.type) };

            if (ptr == nullptr)
                continue;

            auto [it, _]{ lb_closures.try_emplace(i) };
            auto& lb{ it->second };
            std::vector bec{ backwards_epsilon_closure({ tr.src }, std::bind_front(search_predicate, std::ref(lb), std::cref(ptr->cs))) };
            // TODO: replace with static bind_front;

            std::ranges::sort(lb.tr_into);
            std::ranges::sort(lb.tr_between);

            /* check if backwards e-closure includes any continue states */
            if (continue_state.has_value() /* and bec.at(continue_state.value()) */)
            {
                wraparounds.emplace_back(i);
                wraparound_union |= ptr->cs;
            }

            sc_transitions[ptr->cs].emplace_back(i);
        }

        dedup_transitions_map(sc_transitions);

        /* find wraparound transitions */

        std::optional<lb_info> wraparound_lb_closure;

        if (continue_state.has_value() and not fallback_states.empty())
        {
            wraparound_lb_closure.emplace();
            std::ignore = backwards_epsilon_closure<false>(fallback_states, std::bind_front(search_predicate, std::ref(*wraparound_lb_closure), std::cref(wraparound_union)));
            // TODO: replace with static bind_front;

            std::ranges::sort(wraparound_lb_closure->tr_into);
            std::ranges::sort(wraparound_lb_closure->tr_between);
        }

        /* determine closure start bitvec */

        bitset_t is_closure_start(transitions_.size(), false);

        for (const auto& [_, lb] : lb_closures)
            for (const tr_index i : lb.tr_into)
                is_closure_start.at(i) = true;

        if (wraparound_lb_closure.has_value())
            for (const tr_index i : wraparound_lb_closure->tr_into)
                is_closure_start.at(i) = true;


        /* partition wraparound starts */

        /* always empty if there are no continue states */
        std::vector<charset_type> wrap_starts;

        if (wraparound_lb_closure.has_value())
        {
            const auto fn = [this](const tr_index i) {
                const auto& tr{ get_tr(i) };
                return std::cref(get<lookbehind_1_tr>(tr.type).cs);
            };

            const std::vector refs{ std::from_range, wraparounds | std::views::transform(fn) };
            wrap_starts = charset_type::partition(refs);
            std::ranges::sort(wrap_starts);
        }


        /* determine starts transitions */

        start_tr_map_t start_transitions;
        std::vector to_visit{ std::from_range, sc_transitions };

        if (wraparound_lb_closure.has_value())
        {
            /* insert wraparound edges */
            transition_map_t to_insert;

            for (const auto& edge : wrap_starts)
            {
                for (const tr_index i : wraparound_lb_closure->tr_into)
                    if (auto new_edge{ edge & get<normal_tr>(get_tr(i).type).cs }; not new_edge.empty())
                        start_transitions[i].try_emplace(std::move(new_edge));

                for (const tr_index i : wraparound_lb_closure->tr_between)
                    if (auto new_edge{ edge & get<lookbehind_1_tr>(get_tr(i).type).cs }; not new_edge.empty() and new_edge != edge)
                        to_insert[std::move(new_edge)].emplace_back(i);
            }

            dedup_transitions_map(to_insert);
            to_visit.append_range(std::move(to_insert));
        }

        while (not to_visit.empty())
        {
            using visit_type = transition_map_t::value_type;
            const visit_type current{ std::move(to_visit.back()) };
            to_visit.pop_back();

            const auto& [edge, idxs]{ current };
            transition_map_t to_insert;

            for (const tr_index lb_tr_index : idxs)
            {
                const auto& lb{ lb_closures.at(lb_tr_index) };

                for (const tr_index i : lb.tr_into)
                    if (auto new_edge{ edge & get<normal_tr>(get_tr(i).type).cs }; not new_edge.empty())
                        start_transitions[i].try_emplace(std::move(new_edge));

                for (const tr_index i : lb.tr_between)
                    if (auto new_edge{ edge & get<lookbehind_1_tr>(get_tr(i).type).cs }; not new_edge.empty() and new_edge != edge)
                        to_insert[std::move(new_edge)].emplace_back(i);
            }

            dedup_transitions_map(to_insert);
            to_visit.append_range(std::move(to_insert));
        }


        /* partition closure starts */

        using wraparound_index = std::size_t;
        std::flat_map<charset_type, wraparound_index> closure_wraparounds;

        std::flat_map<charset_type, std::vector<state_t>> closure_starts;

        for (const auto& edge : wrap_starts)
            closure_starts[edge].emplace_back(continue_state.value());

        for (const auto& [i, edges] : start_transitions)
        {
            if (edges.empty())
                continue;

            const auto fn = [&](const charset_type& edge) {
                std::size_t cont_index{ std::numeric_limits<wraparound_index>::max() };
                if (const auto it{ std::ranges::lower_bound(wrap_starts, edge) }; it != wrap_starts.end() and edge == *it)
                    cont_index = static_cast<wraparound_index>(it - wrap_starts.begin());
                return std::pair{ std::cref(edge), cont_index };
            };

            const std::vector ref_pairs{ std::from_range, edges.keys() | std::views::transform(fn) };

            for (auto&& [new_edge, vec] : charset_type::partition_ext(ref_pairs))
            {
                std::erase(vec, std::numeric_limits<std::size_t>::max());
                if (vec.size() > 1)
                    throw tnfa_error("tagged_nfa::rewrite_sc_lookbehind: vec.size() > 1");

                if (vec.size() == 1)
                    if (const auto [it, success]{ closure_wraparounds.try_emplace(new_edge, vec.front()) }; not success and it->second != vec.front())
                        throw tnfa_error("tagged_nfa::rewrite_sc_lookbehind: new_edge already exists in closure_wraparounds with different value");

                closure_starts[std::move(new_edge)].emplace_back(transitions_.at(i).dst);
            }
        }


        /* create copies of e-closures leading to lookbehind_1 transitions */

        all_states_map_t all_mapped_states;

        for (auto&& [edge, initial_states] : closure_starts)
        {
            auto& mapped_states{ all_mapped_states[edge] };

            auto pred = [&e = std::as_const(edge)](const tnfa::transition<char_type>& tr) {
                if (holds_alternative<tnfa::normal_tr<char_type>>(tr.type))
                    return false;

                if (const auto* const ptr{ get_if<lookbehind_1_tr>(&tr.type) }; ptr != nullptr)
                    return (e | ptr->cs) == ptr->cs;

                return true;
            };

            std::vector ec{ epsilon_closure(std::move(initial_states), pred) };

            /* set ec to be keys */
            std::ranges::sort(ec, mapped_states.key_comp());
            const auto [first, last]{ std::ranges::unique(ec) };
            ec.erase(first, last);

            /* create new state for each state in ec; defer making nodes fallback to later */
            std::vector new_states{ std::from_range, ec | std::views::transform([this](auto&&) { return node_create(); }) };
            mapped_states.replace(std::move(ec), std::move(new_states));
        }

        closure_starts.clear();


        /* set continue info and make nodes fallback */

        std::vector<tnfa::continue_at_t> continue_ats;

        for (const auto& edge : wrap_starts)
        {
            const auto& mapped_states{ all_mapped_states.at(edge) };
            const auto mapped_cont{ mapped_states.at(continue_state.value()) };

            if (cont_info_.size() >= std::numeric_limits<tnfa::continue_at_t>::max())
                throw tnfa_error("tagged_nfa::rewrite_sc_lookbehind: maximum size of cont_info_ exceeded");

            continue_ats.emplace_back(std::saturate_cast<tnfa::continue_at_t>(cont_info_.size()));
            cont_info_.emplace_back(mapped_cont, edge);
            cont_info_.at(0).cs -= edge; /* note: cont_info should be non-empty to begin with */
        }

        if (continue_state.has_value())
        {
            for (const auto& [edge, mapped_states] : all_mapped_states)
            {
                auto it{ closure_wraparounds.find(edge) };

                if (it == closure_wraparounds.end())
                    continue;

                const auto continue_at{ continue_ats.at(it->second) };

                for (const state_t qf : fallback_states)
                {
                    auto it2{ mapped_states.find(qf) };

                    if (it2 == mapped_states.end())
                        continue;

                    const auto& old_n{ nodes_.at(qf) };
                    auto& new_n{ nodes_.at(it2->second) };

                    new_n.is_final = old_n.is_final;
                    new_n.is_fallback = old_n.is_fallback;
                    new_n.final_offset = old_n.final_offset;
                    new_n.continue_at = continue_at;
                }
            }
        }


        /* duplicate transitions inside and leaving copied closures */

        std::vector<tr_index> to_remove{ lb_closures.keys() };

        for (const auto& [edge, mapped_states] : all_mapped_states)
        {
            for (const auto [q, p] : mapped_states)
            {
                if (nodes_.at(q).is_final and not nodes_.at(p).is_final)
                    make_epsilon(p, q); /* a quick hack to make (^\n) work? */

                for (const tr_index i : nodes_.at(q).out_tr)
                {
                    /* reminder: reference may be invalidated after one call to emplace_back (when transitions_ is resized) */
                    const auto& tr{ get_tr(i) };

                    if (const auto* const ptr{ get_if<normal_tr>(&tr.type) }; ptr != nullptr)
                    {
                        /* this should be valid since in our tnfa, nodes by construction either only have outgoing
                         * normal transitions, or have outgoing e-transitions (or assertions), but never both */
                        make_epsilon(p, q);
                    }
                    else if (const auto* const ptr{ get_if<lookbehind_1_tr>(&tr.type) }; ptr != nullptr)
                    {
                        if (const auto new_edge{ edge & ptr->cs }; not new_edge.empty())
                        {
                            if (const auto it{ mapped_states.find(tr.dst) }; it != mapped_states.end())
                                make_epsilon(p, it->second); /* continue within copied subgraph if possible  */
                            else
                                make_epsilon(p, tr.dst); /* transition from copied subgraph back to main graph */
                        }
                    }
                    else if (not holds_alternative<sof_anchor_tr>(tr.type))
                    {
                        /* transition within copied e-closure (if necessary) */
                        if (const auto it{ mapped_states.find(tr.dst) }; it != mapped_states.end())
                            make_copy(p, it->second, tr.type);
                    }
                }
            }
        }


        /* add transitions leading into copied e-closures */

        is_closure_start.resize(transitions_.size(), false);

        for (const auto& [edge, mapped_states] : all_mapped_states)
        {
            for (const auto [q, p] : mapped_states)
            {
                for (const tr_index i : nodes_.at(q).in_tr)
                {
                    if (not is_closure_start.at(i))
                        continue;

                    auto& tr{ transitions_.at(i) };
                    auto* const ptr{ get_if<normal_tr>(&tr.type) };

                    if (ptr == nullptr)
                        continue;

                    auto new_edge{ edge & ptr->cs };

                    /* remove edge from existing transition */
                    ptr->cs -= edge;
                    if (ptr->cs.empty())
                        to_remove.emplace_back(i);

                    /* transition from main graph to copied e-closure */
                    if (not new_edge.empty())
                        make_transition(tr.src, p, std::move(new_edge));
                }
            }
        }


        /* remove lookbehind_1 transitions and empty regular transitions */

        std::ranges::sort(to_remove);
        const auto [new_end, old_end]{ std::ranges::unique(to_remove) };
        to_remove.erase(new_end, old_end);

        for (const tr_index i : to_remove)
        {
            auto& tr{ transitions_.at(i) };

            /* delete transition */
            std::erase(nodes_.at(tr.src).out_tr, i);
            std::erase(nodes_.at(tr.dst).in_tr, i);
            tr.unset();
        }
    }

    template<typename CharT>
    constexpr void tagged_nfa<CharT>::rewrite_assertions()
    {
        if (has_sof_anchor_) rewrite_sof_anchors();
        if (has_eof_anchor_) rewrite_eof_anchors();
        if (has_lookbehind_1_) rewrite_sc_lookbehind();
        if (has_lookahead_1_) rewrite_sc_lookahead();

        if (has_sof_anchor_ or has_eof_anchor_ or has_lookbehind_1_ or has_lookahead_1_) remove_dead_and_unreachable_states();
    }


    /* constructor */

    template<typename CharT>
    constexpr tagged_nfa<CharT>::tagged_nfa(const expr_tree<char_type>& ast, fsm_flags flags)
        : capture_info_{ ast.get_capture_info() }, tag_count_{ ast.tag_count() }, flags_{ flags }
    {
        auto& dfn{ nodes_.at(default_final_node) };
        dfn.is_final = true;
        dfn.is_fallback = (flags_.enable_fallback and not flags_.longest_match);

        if (flags_.is_iterator)
            cont_info_.emplace_back(default_start_node, ~charset_type{});

        thompson(ast);
    }
}
