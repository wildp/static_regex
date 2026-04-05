// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "tdfa.hpp"

#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <optional>
#include <ranges>
#include <utility>

#include "rx/etc/util.hpp"
#include "rx/api/regex_error.hpp"
#include "rx/etc/vec_bool_adaptor.hpp"


namespace rx::detail::tdfa
{
    constexpr bool toposort_regops(regops_t::iterator beg, regops_t::iterator end, reg_t regcount);

    /* tnfa -> tdfa conversion */

    using tag_t = int;
    using tag_sequence_t = std::vector<tag_t>;

    using reg_vec = std::vector<reg_t>;

    struct configuration
    {
        std::size_t    tnfa_state{ 0 };
        reg_vec        registers;
        tag_sequence_t tag_seq;

        friend constexpr bool operator==(const configuration&, const configuration&) = default;

        constexpr configuration(std::size_t state, reg_vec reg, tag_sequence_t seq)
            : tnfa_state{ state }, registers{ std::move(reg) }, tag_seq{ std::move(seq) } {}

        constexpr configuration() = default;
    };

    struct closure_entry
    {
        std::size_t    tnfa_state{ 0 };
        reg_vec        registers;
        tag_sequence_t tag_seq;
        tag_sequence_t new_tag_seq;

        [[maybe_unused]] friend constexpr bool operator==(const closure_entry&, const closure_entry&) = default;

        constexpr closure_entry(std::size_t state, reg_vec reg, tag_sequence_t seq = {}, tag_sequence_t newseq = {})
            : tnfa_state{ state }, registers{ std::move(reg) }, tag_seq{ std::move(seq) }, new_tag_seq{ std::move(newseq) } {}

        constexpr closure_entry() = default;

        static constexpr configuration next_config(const closure_entry& ce)
        {
            return { ce.tnfa_state, ce.registers, ce.new_tag_seq };
        }
    };

    using config_set_t = std::vector<configuration>;
    using closure_t = std::vector<closure_entry>;
    using register_map_t = std::flat_map<reg_t, reg_t>;

    template<typename CharT>
    using multistep_closures_t = charset_t<CharT>::template partition_pair_result<closure_entry>;

    template<typename CharT>
    using multistep_closure_t = multistep_closures_t<CharT>::value_type;


    struct node_info
    {
        config_set_t config;
        bool         is_fallback{ false };

        friend constexpr bool operator==(const node_info&, const node_info&) = default;
    };

    using state_info_t = std::vector<node_info>;
    using continue_info_t = std::flat_map<std::size_t, tdfa::continue_at_t>;

    class tag_op_map
    {
        struct entry
        {
            tag_t       tag;
            regop::op_t op;
            reg_t       reg;
        };

    public:
        constexpr std::pair<reg_t&, bool> lookup(tag_t tag, const regop::op_t& op)
        {
            auto it = std::ranges::lower_bound(data_, tag, std::less{}, &entry::tag);

            for (const auto end = data_.end(); it != end and it->tag == tag; ++it)
                if (it->op == op)
                    return { it->reg, true };

            /* entry not found; insert new entry */
            it = data_.insert(it, entry{ .tag = tag, .op = op, .reg = 0 });
            return { it->reg, false };
        }

    private:
        std::vector<entry> data_;
    };

    template<typename CharT>
    class factory
    {
    public:
        using char_type = CharT;
        using tnfa_t = tagged_nfa<char_type>;
        using tdfa_t = tagged_dfa<char_type>;

        constexpr explicit factory(const tnfa_t& input, tdfa_t& result, std::size_t tag_count);

    private:
        using epsilon_tr_t = std::pair<tnfa::state_t, tnfa::epsilon_tr>;
        using normal_tr_t  = std::pair<tnfa::state_t, std::reference_wrapper<const tnfa::charset_t<char_type>>>;
        using epsilon_tr_info_t = std::vector<std::vector<epsilon_tr_t>>;
        using normal_tr_info_t  = std::vector<std::vector<normal_tr_t>>;

        [[nodiscard]] constexpr closure_t e_closure(closure_t&& c) const;
        [[nodiscard]] constexpr std::size_t add_state(tdfa_t& result, const closure_t& c, regops_t& o);
        [[nodiscard]] constexpr multistep_closures_t<char_type> multistep(std::size_t state) const;
        [[nodiscard]] constexpr regops_t transition_regops(closure_t& c, reg_t& regcount, tag_op_map& map) const;
        [[nodiscard]] constexpr regops_t final_regops(const final_regs_t& final_registers, const reg_vec& r, const tag_sequence_t& tag_seq) const;
        [[nodiscard]] constexpr regop::op_t regop_rhs(const std::vector<bool>& hist) const;
        [[nodiscard]] constexpr std::vector<bool> history(const tag_sequence_t& hist, tag_t tag) const;
        [[nodiscard]] constexpr bool has_history(const tag_sequence_t& hist, tag_t tag) const;
        [[nodiscard]] constexpr bool mappable(const node_info& state, std::size_t mapped_state, regops_t& o, reg_t regcount) const;

        constexpr void fallback_regops(tdfa_t& result);
        constexpr void backup_regops(tdfa_t& result, std::size_t state, reg_t reg_dst, reg_t reg_src);

        constexpr std::size_t make_initial_state(tdfa_t& result, tnfa::state_t tnfa_state);

        constexpr void factory_init();

        static constexpr std::size_t hash_state(const node_info& state);
        static constexpr std::size_t hash_mappability(const node_info& state);

        const tnfa_t* tnfa_ptr_;
        state_info_t state_info_;
        continue_info_t cont_info_;
        epsilon_tr_info_t epsilon_transitions_;
        normal_tr_info_t normal_transitions_;
        reg_t tag_count_;
        fsm_flags flags_;

        // TODO: eventually replace these with (flat|unordered)_multimap when supported?
        std::vector<std::size_t> state_hashes_keys_;
        std::vector<std::size_t> state_hashes_values_;
        std::vector<std::size_t> mappable_candidate_keys_;
        std::vector<std::size_t> mappable_candidate_values_;
    };

    template<typename CharT>
    constexpr std::size_t factory<CharT>::hash_state(const node_info& state)
    {
        std::size_t hash{ hash::init() };
        hash::append(hash, state);
        return hash;
    }

    template<typename CharT>
    constexpr std::size_t factory<CharT>::hash_mappability(const node_info& state)
    {
        std::size_t hash{ hash::init() };
        for (const auto& cfg: state.config)
            hash::append(hash, cfg.tnfa_state);
        return hash;
    }

    template<typename CharT>
    constexpr auto factory<CharT>::e_closure(closure_t&& c) const -> closure_t
    {
        closure_t new_closure;

        closure_t stack{ std::move(c) };
        std::erase_if(stack, [](const closure_entry& ce) { return not ce.new_tag_seq.empty(); });
        std::ranges::reverse(stack);
        bitset_t visited(tnfa_ptr_->node_count(), false);

        while (not stack.empty())
        {
            if (visited.at(stack.back().tnfa_state))
            {
                stack.pop_back();
                continue;
            }

            new_closure.push_back(std::move(stack.back()));
            stack.pop_back();

            const closure_entry& ce{ new_closure.back() };
            visited.at(ce.tnfa_state) = true;

            for (const auto& [next, e] : epsilon_transitions_.at(ce.tnfa_state))
            {
                if (visited.at(next))
                    continue;

                auto newer_tag_seq = ce.new_tag_seq;
                if (e.tag != 0)
                    newer_tag_seq.push_back(e.tag);
                stack.emplace_back(next, ce.registers, ce.tag_seq, std::move(newer_tag_seq));
            }
        }

        if (flags_.longest_match)
        {
            /* this version is needed for full matches, but below is needed for laziness in prefix matches */
            std::erase_if(new_closure, [this](const closure_entry& ce) -> bool {
                const auto& node = tnfa_ptr_->get_node(ce.tnfa_state);
                if (node.is_final or node.is_fallback)
                    return false;
                return normal_transitions_.at(ce.tnfa_state).empty();
            });
        }
        else
        {
            /* remove all (non-final) states with only e-transitions, and remove all states after first fallback state encountered */
            bool end_found{ false };
            std::erase_if(new_closure, [this, &end_found](const closure_entry& ce) -> bool {
                if (end_found) return true;
                const auto& node = tnfa_ptr_->get_node(ce.tnfa_state);
                if (node.is_fallback)
                {
                    end_found = true;
                    return false;
                }
                if (node.is_final) return false;
                return normal_transitions_.at(ce.tnfa_state).empty();
            });
        }

        return new_closure;
    }

    template<typename CharT>
    constexpr auto factory<CharT>::add_state(tdfa_t& result, const closure_t& c, regops_t& o) -> std::size_t
    {
        static constexpr std::size_t map_usage_threshold{ 128 };
        static constexpr auto key_proj = [](const auto& v) -> decltype(auto) { return get<0>(v); }; // TODO: remove later

        node_info current_info{ .config{ c | std::views::transform(closure_entry::next_config) | std::ranges::to<std::vector>() } };
        std::size_t new_state{ state_info_.size() };

        if (new_state < map_usage_threshold) [[likely]]
        {
            /* check if state already exists */
            const std::size_t sh_key{ hash_state(current_info) };
            for (std::size_t existing_state{ 0 }, size{ state_hashes_keys_.size() }; existing_state < size; ++existing_state)
                if (sh_key == state_hashes_keys_[existing_state]) [[unlikely]]
                    if (mappable(current_info, existing_state, o, result.register_count_))
                        return existing_state;

            /* check if state can be mapped to an existing state */
            const std::size_t mc_key{ hash_mappability(current_info) };
            for (std::size_t mapped_state{ 0 }, size{ mappable_candidate_keys_.size() }; mapped_state < size; ++mapped_state)
                if (mc_key == mappable_candidate_keys_[mapped_state]) [[unlikely]]
                    if (mappable(current_info, mapped_state, o, result.register_count_))
                        return mapped_state;

            /* create new state */
            result.nodes_.emplace_back();
            state_info_.emplace_back(std::move(current_info));

            state_hashes_keys_.emplace_back(sh_key);
            mappable_candidate_keys_.emplace_back(mc_key);

            /* switch to using maps at threshold */
            if (state_info_.size() == map_usage_threshold)
            {
                state_hashes_values_.assign_range(std::views::iota(0uz, new_state));
                mappable_candidate_values_.assign_range(std::views::iota(0uz, new_state));

                std::ranges::sort(std::views::zip(state_hashes_keys_, state_hashes_values_), {}, key_proj);
                std::ranges::sort(std::views::zip(mappable_candidate_keys_, mappable_candidate_values_), {}, key_proj);
            }
        }
        else
        {
            /* check if state already exists */
            auto sh_zv = std::views::zip(state_hashes_keys_, state_hashes_values_);
            const std::size_t sh_key{ hash_state(current_info) };
            const auto sh_eq_range = std::ranges::equal_range(sh_zv, sh_key, {}, key_proj);

            for (const auto& [_, state] : sh_eq_range)
                if (current_info == state_info_.at(state))
                    return state;

            /* check if state can be mapped to an existing state */
            auto mc_zv = std::views::zip(mappable_candidate_keys_, mappable_candidate_values_);
            const std::size_t mc_key{ hash_mappability(current_info) };
            const auto mc_eq_range = std::ranges::equal_range(mc_zv, mc_key, {}, key_proj);

            for (const auto& [_, mapped_state] : mc_eq_range)
                if (mappable(current_info, mapped_state, o, result.register_count_))
                    return mapped_state;

            /* create new state */
            result.nodes_.emplace_back();
            state_info_.emplace_back(std::move(current_info));

            const auto sh_offset = std::ranges::distance(std::ranges::begin(sh_zv), std::ranges::end(sh_eq_range));
            state_hashes_keys_.emplace(state_hashes_keys_.cbegin() + sh_offset, sh_key);
            state_hashes_values_.emplace(state_hashes_values_.cbegin() + sh_offset, new_state);

            const auto mc_offset = std::ranges::distance(std::ranges::begin(mc_zv), std::ranges::end(mc_eq_range));
            mappable_candidate_keys_.emplace(mappable_candidate_keys_.cbegin() + mc_offset, mc_key);
            mappable_candidate_values_.emplace(mappable_candidate_values_.cbegin() + mc_offset, new_state);
        }

        /* make final regops if state is an accepting state */
        const auto& current_cfg = state_info_.back().config;
        const auto is_final = [this](std::size_t arg) { return tnfa_ptr_->get_node(arg).is_final; };
        const auto it = std::ranges::find_if(current_cfg, is_final, &configuration::tnfa_state);
        std::optional<continue_at_t> continue_at;
        if (it != current_cfg.end())
        {
            auto final_ops = final_regops(result.final_registers_, it->registers, it->tag_seq);
            const auto& node = tnfa_ptr_->get_node(it->tnfa_state);
            const auto final_offset = node.final_offset;

            if (node.continue_at < tnfa_ptr_->get_cont_info().size())
                continue_at = node.continue_at;

            if (final_ops.empty())
            {
                /* avoid creating empty regop blocks */
                result.final_nodes_.emplace(new_state, final_node_info{ .op_index = no_transition_regops, .final_offset = final_offset });
            }
            else
            {
                result.final_nodes_.emplace(new_state, final_node_info{ .op_index = result.regops_.size(), .final_offset = final_offset });
                result.regops_.emplace_back(std::move(final_ops));
            }
        }

        if (flags_.enable_fallback)
        {
            /* set fallback state status for fallback_regops */
            const auto is_fallback = [&](std::size_t arg) { return tnfa_ptr_->get_node(arg).is_fallback; };
            const auto it2 = std::ranges::find_if(current_cfg, is_fallback, &configuration::tnfa_state);
            if (it2 != current_cfg.end())
            {
                state_info_.back().is_fallback = true;
                if (continue_at.has_value())
                    cont_info_.emplace_hint(cont_info_.end(), new_state, *continue_at);
            }
        }

        return new_state;
    }

    template<typename CharT>
    constexpr auto factory<CharT>::multistep(std::size_t state) const -> multistep_closures_t<char_type>
    {
        using elem_t = tnfa::charset_t<char_type>::template ref_pair<closure_entry>;

        std::vector<elem_t> transitions;

        for (const auto& cfg : state_info_.at(state).config)
            for (const auto& [dst, cs] : normal_transitions_.at(cfg.tnfa_state))
                transitions.emplace_back(cs, closure_entry{ dst, cfg.registers, cfg.tag_seq });

        return charset_t<char_type>::partition_ext(transitions);
    }

    template<typename CharT>
    constexpr auto factory<CharT>::transition_regops(closure_t& c, reg_t& regcount, tag_op_map& map) const -> regops_t
    {
        regops_t regops;

        for (closure_entry& ce : c)
        {
            for (tag_t t{ 1 }; std::cmp_less_equal(t, tag_count_); ++t)
            {
                auto h = history(ce.tag_seq, t);

                if (not h.empty())
                {
                    regop::op_t op_rhs{ regop_rhs(h) };

                    auto [value_ref, existing] = map.lookup(t, op_rhs);

                    if (not existing)
                    {
                        auto i = regcount++;
                        value_ref = i;
                        regops.emplace_back(i, op_rhs);
                    }
                    else if (not std::ranges::contains(regops, value_ref, &regop::dst))
                    {
                        regops.emplace_back(value_ref, op_rhs);
                    }

                    ce.registers.at(t - 1) = value_ref;
                }
            }
        }

        return regops;
    }

    template<typename CharT>
    constexpr auto factory<CharT>::final_regops(const final_regs_t& final_registers, const reg_vec& r, const tag_sequence_t& tag_seq) const -> regops_t
    {
        regops_t regops;

        for (tag_t t{ 1 }; std::cmp_less_equal(t, tag_count_); ++t)
        {
            auto h = history(tag_seq, t);

            if (not h.empty()) /* assign final registers */
                regops.emplace_back(final_registers.at(t - 1), regop_rhs(h));
            else
                regops.emplace_back(final_registers.at(t - 1), regop::copy{ .src = r.at(t - 1) }); /* copy into final register */
        }

        return regops;
    }

    template<typename CharT>
    constexpr auto factory<CharT>::regop_rhs(const std::vector<bool>& hist) const -> regop::op_t
    {
        return regop::set{ .val = hist.back() };
    }

    template<typename CharT>
    constexpr auto factory<CharT>::history(const tag_sequence_t& h, tag_t tag) const -> std::vector<bool>
    {
        std::vector<bool> result;
        for (const tag_t x : h)
            if (tag == (x < 0 ? -x : x))
                result.push_back(x >= 0);
        return result;
    }

    template<typename CharT>
    constexpr bool factory<CharT>::has_history(const tag_sequence_t& h, tag_t tag) const
    {
        return std::ranges::contains(h, tag, [](auto x) { return x < 0 ? -x : x; });
    }

    template<typename CharT>
    constexpr bool factory<CharT>::mappable(const node_info& state, std::size_t mapped_state, regops_t& o, const reg_t regcount) const
    {
        const auto& mapped_state_info = state_info_.at(mapped_state);

        /* different precedences also imply differing sets of states (for now at least);
         * we ensure tnfa states have the same tag sequences later                       */
        // if (state.precedence != mapped_state_info.precedence)
        if (not std::ranges::equal(state.config, mapped_state_info.config, {}, &configuration::tnfa_state, &configuration::tnfa_state))
            return false;

        register_map_t map, rmap;

        for (const auto& ce1 : state.config)
        {
            for (const auto& ce2 : mapped_state_info.config)
            {
                if (ce1.tnfa_state != ce2.tnfa_state)
                    continue;
                if (ce1.tag_seq != ce2.tag_seq)
                    return false; /* see first comment of function */

                for (tag_t t{ 1 }; std::cmp_less_equal(t, tag_count_); ++t)
                {
                    if (has_history(ce1.tag_seq, t))
                        continue;

                    const reg_t i{ ce1.registers.at(t - 1) };
                    const reg_t j{ ce2.registers.at(t - 1) };

                    auto it = map.lower_bound(i);
                    auto jt = rmap.lower_bound(j);

                    const bool not_i_match{ it == map.end() or (*it).first != i };
                    const bool not_j_match{ jt == rmap.end() or (*jt).first != j };

                    if (not_i_match and not_j_match)
                    {
                        if (it != map.end()) ++it;
                        if (jt != rmap.end()) ++jt;

                        map.emplace_hint(it, i, j);
                        rmap.emplace_hint(jt, j, i);
                    }
                    else if (not_i_match or not_j_match or (*it).second != j or (*jt).second != i)
                    {
                        return false;
                    }
                }
            }
        }

        regops_t o_copy{ o };

        for (auto it = o_copy.begin(); it != o_copy.end();)
        {
            const auto i = it->dst;

            const auto map_it = map.find(i);
            const auto map_end = map.end();

            if (map_it == map_end)
            {
                /* skip mapping registers with history */
                it = o_copy.erase(it);
                continue;
            }

            it->dst = (*map_it).second;
            map.erase(map_it);
            ++it;
        }

        regops_t o_new;

        for (const auto [j, i] : map)
        {
            if (j == i) continue;
            o_new.emplace_back(i, regop::copy{ .src = j });
        }

        o_new.append_range(o_copy);

        const bool success{ toposort_regops(o_new.begin(), o_new.end(), regcount) };

        if (success)
            o = std::move(o_new);

        return success;
    }

    template<typename CharT>
    constexpr void factory<CharT>::fallback_regops(tdfa_t& result)
    {
        for (const auto [state, fni] : result.final_nodes_)
        {
            /* check if current state is a fallback state */

            if (not state_info_.at(state).is_fallback)
                continue;

            const auto& final_ops = result.get_regops(fni.op_index);

            /* determine clobbered registers */

            // maybe switch to sets?
            bitset_t added(result.nodes_.size(), false);
            bitset_t clobbered(result.register_count_, false);

            std::vector<std::pair<std::size_t, std::size_t>> stack;
            stack.emplace_back(state, 0);
            added.at(state) = true;

            while (not stack.empty())
            {
                auto& [s, i] = stack.back();

                if (result.final_nodes_.contains(s) or i >= result.nodes_.at(s).tr.size())
                {
                    stack.pop_back();
                }
                else
                {
                    const auto& tr = result.nodes_.at(s).tr.at(i);

                    for (const auto& op : result.get_regops(tr.op_index))
                        clobbered.at(op.dst) = true;

                    ++i;
                    if (not added.at(tr.next))
                    {
                        stack.emplace_back(tr.next, 0);
                        added.at(tr.next) = true;
                    }
                }
            }

            /* determine state to restart from in repeated searches */

            auto continuation_index = tdfa::no_continue;
            if (auto it = cont_info_.find(state); it != cont_info_.end())
                continuation_index = it->second;

            /* insert fallback regops */

            regops_t o;

            for (const auto& f : final_ops)
            {
                if (auto* cpy = get_if<regop::copy>(&f.op); cpy != nullptr and clobbered.at(cpy->src))
                    backup_regops(result, state, f.dst, cpy->src);
                else
                    o.emplace_back(f);
            }

            if (o.empty())
            {
                /* avoid creating empty regop blocks */
                result.fallback_nodes_.emplace(state, fallback_node_info{ .op_index = no_transition_regops, .continue_at = continuation_index });
            }
            else
            {
                result.fallback_nodes_.emplace(state, fallback_node_info{ .op_index = result.regops_.size(), .continue_at = continuation_index });
                result.regops_.emplace_back(std::move(o));
            }
        }
    }

    template<typename CharT>
    constexpr void factory<CharT>::backup_regops(tdfa_t& result, std::size_t state, reg_t reg_dst, reg_t reg_src)
    {
        for (auto& tr : result.nodes_.at(state).tr)
        {
            /* not final node => non-accepting path exists from tr.next (end of inputs can be anywhere) */
            if (not result.final_nodes_.contains(tr.next))
            {
                if (tr.op_index == no_transition_regops)
                {
                    tr.op_index = result.regops_.size();
                    result.regops_.emplace_back();
                }

                result.regops_.at(tr.op_index).emplace_back(reg_dst, regop::copy{ reg_src });
            }
        }
    }

    template<typename CharT>
    constexpr std::size_t factory<CharT>::make_initial_state(tdfa_t& result, const tnfa::state_t tnfa_state)
    {
        reg_vec initial_reg(tag_count_);
        std::ranges::iota(initial_reg, 0);

        closure_t initial_cfg;
        initial_cfg.emplace_back(tnfa_state, initial_reg);
        initial_cfg = e_closure(std::move(initial_cfg));
        regops_t regs;
        return add_state(result, initial_cfg, regs);
    }

    template<typename CharT>
    constexpr void factory<CharT>::factory_init()
    {
        static constexpr auto compose = [](const auto& g, const auto& f) {
            return [=]<typename T>(T&& arg) {
                return std::invoke(g, std::invoke(f, std::forward<T>(arg)));
            };
        };

        const std::size_t node_count{ tnfa_ptr_->node_count() };
        epsilon_transitions_.resize(node_count);
        normal_transitions_.resize(node_count);

        for (std::size_t q{ 0 }; q < node_count; ++q)
        {
            auto& current_etr = epsilon_transitions_.at(q);
            auto& current_tr = normal_transitions_.at(q);

            for (const auto tr_idx : tnfa_ptr_->get_node(q).out_tr)
            {
                const auto& tr = tnfa_ptr_->get_tr(tr_idx);

                if (const auto* const ptr{ get_if<tnfa::normal_tr<char_type>>(&tr.type) }; ptr != nullptr)
                {
                    current_tr.emplace_back(tr.dst, std::cref(ptr->cs));
                }
                else if (const auto* const ptr{ get_if<tnfa::epsilon_tr>(&tr.type) }; ptr != nullptr)
                {
                    current_etr.emplace_back(tr.dst, *ptr);
                }
                else
                {
                    throw tnfa_error("Input contains un-rewritten assertions");
                }
            }

            std::ranges::sort(current_etr, std::ranges::greater{}, compose(&tnfa::epsilon_tr::priority, &epsilon_tr_t::second));
        }
    }

    template<typename CharT>
    constexpr factory<CharT>::factory(const tnfa_t& input, tdfa_t& result, const std::size_t tag_count)
#if __cpp_lib_saturation_arithmetic >= 202603L
        : tnfa_ptr_{ &input }, tag_count_{ std::saturating_cast<reg_t>(tag_count) }, flags_{ result.flags_ }
#else
        : tnfa_ptr_{ &input }, tag_count_{ std::saturate_cast<reg_t>(tag_count) }, flags_{ result.flags_ }
#endif
    {
        factory_init();

        result.register_count_ = tag_count_ * 2;
        result.final_registers_.resize(tag_count_);
        std::ranges::iota(result.final_registers_, tag_count_);

        const std::size_t initial{ make_initial_state(result, tnfa_ptr_->start_node()) };

        for (const auto& cont : tnfa_ptr_->get_cont_info())
        {
            if (cont.value == tnfa_ptr_->start_node())
                result.continue_nodes_.emplace_back(initial);
            else
                result.continue_nodes_.emplace_back(make_initial_state(result, cont.value));
        }

        if (const auto& ac = tnfa_ptr_->get_additional_cont(); not ac.empty())
        {
            const auto& ci = tnfa_ptr_->get_cont_info();

            for (std::size_t i{ 0 }, i_max{ ci.size() }; i < i_max; ++i)
            {
                if (ac.at(i) == ci.at(i).value)
                    result.additional_continue_nodes_.emplace_back(result.continue_nodes_.at(i));
                else
                    result.additional_continue_nodes_.emplace_back(make_initial_state(result, ac.at(i)));
            }

            if (const auto acb = ac.back(); acb == tnfa_ptr_->start_node())
                result.additional_continue_nodes_.emplace_back(initial);
            else
                result.additional_continue_nodes_.emplace_back(make_initial_state(result, ac.back()));
        }

        for (std::size_t state{ initial }; state < result.nodes_.size(); ++state)
        {
            tag_op_map map;

            for (auto& [cs, cfg] : multistep(state))
            {
                cfg = e_closure(std::move(cfg));
                auto o = transition_regops(cfg, result.register_count_, map);
                const auto s = add_state(result, cfg, o);

                /* Add transition to tdfa */
                if (o.empty())
                {
                    /* avoid creating empty regop blocks */
                    result.nodes_.at(state).tr.emplace_back(s, no_transition_regops, std::move(cs));
                }
                else
                {
                    result.nodes_.at(state).tr.emplace_back(s, result.regops_.size(), std::move(cs));
                    result.regops_.emplace_back(std::move(o));
                }
            }
        }

        if (flags_.enable_fallback)
            fallback_regops(result);
    }

    // TODO: improve implementation of tdfa optimisation to reduce number of steps taken by constant evaluator!!!

    /* regops sorting */

    constexpr bool toposort_regops(const regops_t::iterator beg, const regops_t::iterator end, const reg_t regcount)
    {
        static constexpr reg_t regop_threshold{ 64 };

        if (regcount < regop_threshold) [[likely]]
        {
            std::vector<unsigned int> indeg(regcount, 0);

            for (auto it = beg; it != end; ++it)
            {
                if (const auto* copy = get_if<regop::copy>(&it->op); copy != nullptr)
                    ++indeg.at(copy->src);
            }

            regops_t o_new;
            regops_t o_copy{ beg, end };
            bool cycle_detected{ false };

            while (not o_copy.empty())
            {
                bool added{ false };

                for (auto it = o_copy.begin(); it != o_copy.end();)
                {
                    if (indeg.at(it->dst) == 0)
                    {
                        if (const auto* copy = get_if<regop::copy>(&it->op); copy != nullptr)
                            --indeg.at(copy->src);

                        o_new.emplace_back(*it);
                        it = o_copy.erase(it);
                        added = true;
                    }
                    else
                    {
                        ++it;
                    }
                }

                if (not added and not o_copy.empty())
                {
                    for (const auto& oc : o_copy)
                    {
                        /* ignore copying to self */
                        if (const auto* copy = get_if<regop::copy>(&oc.op); copy != nullptr and copy->src != oc.dst)
                        {
                            cycle_detected = true;
                            break;
                        }
                    }

                    o_new.append_range(o_copy);
                    break;
                }
            }

            if (std::cmp_not_equal(end - beg, o_new.size()))
                cycle_detected = true; /* unknown error */

            if (not cycle_detected)
                std::ranges::move(o_copy, beg);

            return not cycle_detected;
        }
        else
        {
            std::flat_map<reg_t, unsigned int> indeg;

            for (auto it = beg; it != end; ++it)
            {
                if (const auto* copy = get_if<regop::copy>(&it->op); copy != nullptr)
                    ++indeg[copy->src];
            }

            regops_t o_new;
            regops_t o_copy{ beg, end };
            bool cycle_detected{ false };

            while (not o_copy.empty())
            {
                bool added{ false };

                for (auto it = o_copy.begin(); it != o_copy.end();)
                {
                    if (indeg[it->dst] == 0)
                    {
                        if (const auto* copy = get_if<regop::copy>(&it->op); copy != nullptr)
                            --indeg[copy->src];

                        o_new.emplace_back(*it);
                        it = o_copy.erase(it);
                        added = true;
                    }
                    else
                    {
                        ++it;
                    }
                }

                if (not added and not o_copy.empty())
                {
                    for (const auto& oc : o_copy)
                    {
                        /* ignore copying to self */
                        if (const auto* copy = get_if<regop::copy>(&oc.op); copy != nullptr and copy->src != oc.dst)
                        {
                            cycle_detected = true;
                            break;
                        }
                    }

                    o_new.append_range(o_copy);
                    break;
                }
            }

            if (std::cmp_not_equal(end - beg, o_new.size()))
                cycle_detected = true; /* unknown error */

            if (not cycle_detected)
                std::ranges::move(o_copy, beg);

            return not cycle_detected;
        }
    }


    /* tdfa normalisation */

    constexpr regops_t::iterator dedup_regops(regops_t& o, const regops_t::iterator beg, const regops_t::iterator end)
    {
        regops_t::iterator new_end{ end };

        for (auto it = beg; it != new_end; ++it)
            new_end = std::remove(it + 1, new_end, *it);

        return o.erase(new_end, end);
    }

    constexpr void normalise_regops(regops_t& o, const reg_t regcount)
    {
        for (auto it = o.begin(); it != o.end();)
        {
            auto local_end = it + 1;
            if (holds_alternative<regop::set>(it->op))
            {
                while (local_end != o.end() and holds_alternative<regop::set>(local_end->op))
                    ++local_end;

                local_end = dedup_regops(o, it, local_end);
                std::ranges::sort(it, local_end, std::ranges::less{}, &regop::dst);
                it = local_end;
            }
            else if (holds_alternative<regop::copy>(it->op))
            {
                while (local_end != o.end() and holds_alternative<regop::copy>(local_end->op))
                    ++local_end;

                local_end = dedup_regops(o, it, local_end);
                toposort_regops(it, local_end, regcount);
                it = local_end;
            }
            else
            {
                std::unreachable();
            }
        }
    }


    /* tdfa optimisation */

    class liveness_matrix
    {
    public:
        explicit constexpr liveness_matrix(std::size_t block_count, reg_t reg_count) : data_(block_count, bitset_t(reg_count, false)) {}
        [[nodiscard]] constexpr auto operator[](std::size_t block_idx, reg_t reg) { return data_[block_idx][reg]; }
        [[nodiscard]] constexpr auto operator[](std::size_t block_idx, reg_t reg) const { return data_[block_idx][reg]; }
        [[nodiscard]] constexpr auto at(std::size_t block_idx, reg_t reg) { return data_.at(block_idx).at(reg); }
        [[nodiscard]] constexpr auto at(std::size_t block_idx, reg_t reg) const { return data_.at(block_idx).at(reg); }
        [[nodiscard]] constexpr auto& row(std::size_t block_idx) { return data_.at(block_idx); }
        [[nodiscard]] constexpr const auto& row(std::size_t block_idx) const { return data_.at(block_idx); }
        [[nodiscard]] constexpr bool block_valid(std::size_t block_idx) const { return block_idx < data_.size(); }

    private:
        std::vector<bitset_t> data_;
    };

    class square_matrix
    {
    public:
        explicit constexpr square_matrix(std::size_t reg_count) : data_(reg_count * reg_count, false), reg_count_{ reg_count } {}
        [[nodiscard]] constexpr auto operator[](std::size_t reg1, std::size_t reg2) { return data_[(reg1 * reg_count_) + reg2]; }
        [[nodiscard]] constexpr auto operator[](std::size_t reg1, std::size_t reg2) const { return data_[(reg1 * reg_count_) + reg2]; }
        [[nodiscard]] constexpr auto at(std::size_t reg1, std::size_t reg2) { return data_.at((reg1 * reg_count_) + reg2); }
        [[nodiscard]] constexpr auto at(std::size_t reg1, std::size_t reg2) const { return data_.at((reg1 * reg_count_) + reg2); }
        [[nodiscard]] constexpr std::size_t side_length() const { return reg_count_; }

        constexpr void set_symmetric(std::size_t reg1, std::size_t reg2, bool value)
        {
            at(reg1, reg2) = value;
            at(reg2, reg1) = value;
        }

    private:
        bitset_t data_;
        std::size_t reg_count_;
    };


    template<typename CharT>
    class opt
    {
    public:
        using tdfa_t = tagged_dfa<CharT>;

        constexpr explicit opt(std::size_t i = 2) noexcept : iterations_{ i } {}
        constexpr void operator()(tdfa_t& dfa);

    private:
        using remap_t = std::vector<reg_t>;

        [[nodiscard]] static constexpr remap_t compact_registers(const tdfa_t& dfa);
        [[nodiscard]] static constexpr remap_t allocate_registers(const tdfa_t& dfa, const square_matrix& overlapping_lifetimes);
        [[nodiscard]] constexpr liveness_matrix liveness(const tdfa_t& dfa) const;
        [[nodiscard]] constexpr square_matrix interference(const tdfa_t& dfa, const liveness_matrix& liveness) const;

        static constexpr void rename_registers(tdfa_t& dfa, const remap_t& remap);
        static constexpr void deadcode_elim(tdfa_t& dfa, const liveness_matrix& liveness);
        static constexpr void normalise(tdfa_t& dfa);

        constexpr void make_cfg(const tdfa_t& dfa);

        std::vector<std::vector<std::size_t>> block_graph_;
        std::vector<std::size_t> block_graph_start_;
        std::size_t iterations_;
    };

    template<typename CharT>
    constexpr void opt<CharT>::make_cfg(const tdfa_t& dfa)
    {
        block_graph_start_.clear();
        block_graph_.clear();
        block_graph_.resize(dfa.regops_.size());

        std::vector<std::vector<std::size_t>> successor_blocks(dfa.nodes_.size());
        square_matrix reachable(dfa.nodes_.size());

        for (std::size_t node_idx{ 0 }, node_count{ dfa.nodes_.size() }; node_idx < node_count; ++node_idx)
        {
            for (const auto& tr : dfa.nodes_[node_idx].tr)
            {
                if (tr.op_index == no_transition_regops)
                    reachable.at(node_idx, tr.next) = true;
                else
                    successor_blocks.at(node_idx).emplace_back(tr.op_index);
            }

            /* add final transitions too */

            if (dfa.final_nodes_.contains(node_idx))
                if (const auto fni = dfa.final_nodes_.at(node_idx); fni.op_index != no_transition_regops)
                    successor_blocks.at(node_idx).emplace_back(fni.op_index);
        }

        /* calculate reachability matrix (initial reachability is added above) */

        const auto side_length = reachable.side_length();
        for (std::size_t i{ 0 }; i < side_length; ++i)
            reachable[i, i] = true;

        for (std::size_t k{ 0 }; k < side_length; ++k)
            for (std::size_t i{ 0 }; i < side_length; ++i)
                for (std::size_t j{ 0 }; j < side_length; ++j)
                    if (not reachable[i, j])
                        reachable[i, j] = reachable[i, k] and reachable[k, j];

        /* construct graph */

        std::vector<std::vector<std::size_t>> nodes_to_edges(dfa.nodes_.size());

        for (std::size_t node_idx{ 0 }, node_count{ dfa.nodes_.size() }; node_idx < node_count; ++node_idx)
        {
            std::vector<std::size_t> tmp;

            for (std::size_t i{ 0 }; i < reachable.side_length(); ++i)
                if (reachable.at(node_idx, i))
                    tmp.insert_range(tmp.end(), successor_blocks.at(i));

            std::ranges::sort(tmp);
            auto [beg, end] = std::ranges::unique(tmp);
            tmp.erase(beg, end);
            tmp.shrink_to_fit();

            nodes_to_edges.at(node_idx) = std::move(tmp);
        }

        for (std::size_t node_idx{ 0 }; node_idx < dfa.nodes_.size(); ++node_idx)
            for (const auto& tr : dfa.nodes_.at(node_idx).tr)
                if (tr.op_index != no_transition_regops)
                    block_graph_.at(tr.op_index) = nodes_to_edges.at(tr.next);

        block_graph_start_ = std::move(nodes_to_edges.at(dfa.match_start));

        /* add additional start nodes to block_graph_start */
        std::vector to_visit(dfa.continue_nodes_);
        to_visit.append_range(dfa.additional_continue_nodes_);
        std::ranges::sort(to_visit);
        auto [efirst, elast] = std::ranges::unique(to_visit);
        to_visit.erase(efirst, elast);
        std::erase(to_visit, dfa.match_start);

        for (const std::size_t c : to_visit)
            block_graph_start_.append_range(std::move(nodes_to_edges.at(c)));
    }

    template<typename CharT>
    constexpr auto opt<CharT>::compact_registers(const tdfa_t& dfa) -> remap_t
    {
        bitset_t visited(dfa.register_count_, false);
        remap_t remap_result(dfa.register_count_, std::numeric_limits<reg_t>::max());

        for (auto& block : dfa.regops_)
        {
            for (auto& op : block)
            {
                if (auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr)
                    visited.at(cpy->src) = true;
                visited.at(op.dst) = true;
            }
        }

        reg_t new_reg{ 0 };
        for (std::size_t i{ 0 }; i < dfa.register_count_; ++i)
            if (visited.at(i))
                remap_result.at(i) = new_reg++;

        return remap_result;
    }

    template<typename CharT>
    constexpr void opt<CharT>::rename_registers(tdfa_t& dfa, const remap_t& remap)
    {
        for (auto& block : dfa.regops_)
        {
            for (auto it = block.begin(); it != block.end();)
            {
                it->dst = remap.at(it->dst);
                if (auto* cpy = get_if<regop::copy>(&it->op); cpy != nullptr)
                {
                    cpy->src = remap.at(cpy->src);

                    if (cpy->src == it->dst)
                        it = block.erase(it); /* remove trivial cycles */
                    else
                        ++it;
                }
                else
                {
                    ++it;
                }
            }
        }

        for (reg_t& reg : dfa.final_registers_)
            reg = remap.at(reg);

        dfa.register_count_ = 1 + std::ranges::max(remap | std::views::filter([](reg_t r) { return r != std::numeric_limits<reg_t>::max(); }));
    }

    template<typename CharT>
    constexpr liveness_matrix opt<CharT>::liveness(const tdfa_t& dfa) const
    {
        /* unfortunately coroutines can't be constexpr yet */

        class postorder_visitor
        {
        public:
            explicit constexpr postorder_visitor(const std::vector<std::size_t>& block_graph_start, std::size_t block_count)
                : block_added(block_count, false)
            {
                for (const auto i : block_graph_start | std::views::reverse)
                {
                    stack.emplace_back(i, 0);
                    block_added.at(i) = true;
                }
            }

            constexpr std::optional<std::size_t> operator()(const std::vector<std::vector<std::size_t>>& block_graph)
            {
                std::optional<std::size_t> result;

                while (not stack.empty() and not result.has_value())
                {
                    const auto [block_idx, i] = stack.back();

                    if (i == block_graph.at(block_idx).size())
                    {
                        result = block_idx;
                        stack.pop_back();
                    }
                    else
                    {
                        const auto next = block_graph.at(block_idx).at(i);
                        stack.back().second += 1;
                        if (not block_added.at(next))
                        {
                            stack.emplace_back(next, 0);
                            block_added.at(next) = true;
                        }
                    }
                }

                return result;
            }

        private:
            std::vector<std::pair<std::size_t, std::size_t>> stack;
            bitset_t block_added;
        };

        /* resume liveness implementation */

        const std::size_t block_count{ dfa.regops_.size() };
        const std::size_t reg_count{ dfa.register_count_ };
        liveness_matrix liveness(block_count, reg_count);

        /* make registers assigned to in final transitions live */

        for (const auto [_, fni] : dfa.final_nodes_)
            for (const reg_t final_reg : dfa.final_registers_)
                liveness.at(fni.op_index, final_reg) = true;

        bitset_t current_row_copy;
        bitset_t successor_row_copy;

        for (bool loop{ true }; loop;)
        {
            loop = false;
            postorder_visitor vis{ block_graph_start_, block_count };

            while (true)
            {
                const auto opt = vis(block_graph_);
                if (not opt.has_value())
                    break;

                const auto block_idx = opt.value();

                current_row_copy = liveness.row(block_idx);

                for (const std::size_t sblock_idx : block_graph_.at(block_idx))
                {
                    successor_row_copy = liveness.row(sblock_idx);

                    for (const auto& op : dfa.regops_.at(sblock_idx) | std::views::reverse) /* todo: in post order? */
                    {
                        if (holds_alternative<regop::set>(op.op))
                        {
                            successor_row_copy.at(op.dst) = false;
                        }
                        else if (const auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr)
                        {
                            if (successor_row_copy.at(op.dst))
                            {
                                successor_row_copy.at(op.dst) = false;
                                successor_row_copy.at(cpy->src) = true;
                            }
                        }
                        else
                        {
                            std::unreachable();
                        }
                    }

                    current_row_copy |= successor_row_copy;
                }

                if (current_row_copy != liveness.row(block_idx))
                {
                    liveness.row(block_idx) = std::move(current_row_copy);
                    loop = true;
                }
            }
        }

        bitset_t current_row(reg_count, false);
        bitset_t added(dfa.nodes_.size(), false);

        for (const auto [fallback_state, fbni] : dfa.fallback_nodes_)
        {
            if (fbni.op_index == no_transition_regops)
            {
                current_row.reset();

                for (const std::size_t final_reg : dfa.final_registers_)
                    current_row.at(final_reg) = true;
            }
            else
            {
                for (const std::size_t final_reg : dfa.final_registers_)
                    liveness.at(fbni.op_index, final_reg) = true;

                current_row = liveness.row(fbni.op_index);

                for (const auto& op : dfa.regops_.at(fbni.op_index))
                    current_row.at(op.dst) = false;

                for (const auto& op : dfa.regops_.at(fbni.op_index))
                    if (auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr)
                        current_row.at(cpy->src) = false;
            }

            /* traverse nodes that can possibly fallback to fallback_state */

            added.reset();
            std::vector<std::pair<std::size_t, std::size_t>> stack;
            stack.emplace_back(fallback_state, 0);
            added.at(fallback_state) = true;

            while (not stack.empty())
            {
                auto& [s, i] = stack.back();

                if (dfa.final_nodes_.contains(s) or i >= dfa.nodes_.at(s).tr.size())
                {
                    stack.pop_back();
                }
                else
                {
                    const auto& tr = dfa.nodes_.at(s).tr.at(i);

                    /* note: we overapproximate here by assuming the transition function from a state is
                    *       not a total function, making a fallback possible from every non-final state */

                    if (liveness.block_valid(tr.op_index))
                        liveness.row(tr.op_index) |= current_row;

                    ++i;
                    if (not added.at(tr.next))
                    {
                        stack.emplace_back(tr.next, 0);
                        added.at(tr.next) = true;
                    }
                }
            }
        }

        return liveness;
    }

    template<typename CharT>
    constexpr void opt<CharT>::deadcode_elim(tdfa_t& dfa, const liveness_matrix& liveness)
    {
        for (std::size_t block_idx{ 0 }, block_count{ dfa.regops_.size() }; block_idx < block_count; ++block_idx)
        {
            auto& block = dfa.regops_.at(block_idx);
            bitset_t current_row_copy{ liveness.row(block_idx) };

            for (std::size_t i{ block.size() }; i > 0; --i)
            {
                const auto& op = block.at(i - 1);

                if (current_row_copy.at(op.dst))
                {
                    if (holds_alternative<regop::set>(op.op))
                        current_row_copy.at(op.dst) = false;
                    else if (const auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr)
                        current_row_copy.at(op.dst) = false, current_row_copy.at(cpy->src) = true;
                    else
                        std::unreachable();
                }
                else
                {
                    /* erase dead operation */
                    block.erase(block.begin() + i - 1);
                }
            }
        }
    }

    template<typename CharT>
    constexpr square_matrix opt<CharT>::interference(const tdfa_t& dfa, const liveness_matrix& liveness) const
    {
        square_matrix overlapping_lifetimes(dfa.register_count_);
        std::vector<std::optional<regop::op_t>> histories(dfa.register_count_);

        for (std::size_t block_idx{ 0 }, block_count{ dfa.regops_.size() }; block_idx < block_count; ++block_idx)
        {
            const auto& block = dfa.regops_.at(block_idx);

            for (const auto& op : block)
                if (const auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr)
                    histories.at(cpy->src) = op.op;

            for (const auto& op : block)
            {
                if (const auto* set = get_if<regop::set>(&op.op); set != nullptr)
                    histories.at(op.dst) = op.op;
                else if (const auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr)
                    histories.at(op.dst) = histories.at(cpy->src);
                else
                    std::unreachable();
            }

            for (const auto& op : block)
            {
                bitset_t current_row_copy{ liveness.row(block_idx) };

                if (const auto* set = get_if<regop::set>(&op.op); set != nullptr)
                    current_row_copy.at(op.dst) = false;
                else if (const auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr)
                    current_row_copy.at(op.dst) = false, current_row_copy.at(cpy->src) = false;
                else
                    std::unreachable();

                for (const auto& op2 : block)
                    if (histories.at(op2.dst) == histories.at(op.dst))
                        current_row_copy.at(op2.dst) = false;

                for (reg_t i{ 0 }; i < current_row_copy.size(); ++i)
                    if (current_row_copy.at(i))
                        overlapping_lifetimes.set_symmetric(i, op.dst, true);
            }
        }

        return overlapping_lifetimes;
    }

    template<typename CharT>
    constexpr auto opt<CharT>::allocate_registers(const tdfa_t& dfa, const square_matrix& overlapping_lifetimes) -> remap_t
    {
        static constexpr reg_t no_register{ std::numeric_limits<reg_t>::max() };

        remap_t remap_result(dfa.register_count_, no_register);
        remap_t representative_map(dfa.register_count_, no_register);
        std::vector<std::vector<reg_t>> equivalence_classes(dfa.register_count_); // TODO: change to std::vector<std::flat_set<reg_t>> when available

        for (const auto& block : dfa.regops_)
        {
            for (const auto& op : block)
            {
                if (const auto* cpy = get_if<regop::copy>(&op.op); cpy != nullptr and op.dst != cpy->src)
                {
                    reg_t x{ representative_map.at(op.dst) };
                    reg_t y{ representative_map.at(cpy->src) };

                    if (x == no_register and y == no_register)
                    {
                        if (not overlapping_lifetimes.at(op.dst, cpy->src))
                        {
                            representative_map.at(op.dst) = op.dst;
                            representative_map.at(cpy->src) = op.dst;

                            auto& set = equivalence_classes.at(op.dst);
                            if (op.dst < cpy->src)
                                set.assign({ op.dst, cpy->src });
                            else if (op.dst > cpy->src)
                                set.assign({ cpy->src, op.dst });
                            else
                                set.assign({ op.dst });
                        }
                    }
                    else if (x != no_register and y == no_register)
                    {
                        if (std::ranges::all_of(equivalence_classes.at(x), [&](reg_t t) { return not overlapping_lifetimes.at(t, cpy->src); }))
                        {
                            representative_map.at(cpy->src) = x;

                            auto& set = equivalence_classes.at(x);
                            auto it = std::ranges::lower_bound(set, cpy->src);
                            if (it == set.end() or *it != cpy->src)
                                set.insert(it, cpy->src);
                        }
                    }
                    else if (x == no_register and y != no_register)
                    {
                        if (std::ranges::all_of(equivalence_classes.at(y), [&](reg_t t) { return not overlapping_lifetimes.at(t, op.dst); }))
                        {
                            representative_map.at(op.dst) = y;

                            auto& set = equivalence_classes.at(y);
                            auto it = std::ranges::lower_bound(set, op.dst);
                            if (it == set.end() or *it != op.dst)
                                set.insert(it, op.dst);
                        }
                    }
                }
            }
        }

        /* merge non-interfering equivalence classes */

        for (reg_t x{ 0 }; x < representative_map.size(); ++x)
        {
            if (representative_map.at(x) != x)
                continue;

            for (reg_t y{ 0 }; y < representative_map.size(); ++y)
            {
                if (representative_map.at(y) != y or y <= x)
                    continue;

                // /* we don't have ranges::cartesian_product yet :( */
                // if (std::ranges::all_of(std::views::cartesian_product(equivalence_classes.at(x), equivalence_classes.at(y)),
                //                         [&](const std::tuple<reg_t, reg_t>& pair) { return not overlapping_lifetimes.at(get<0>(pair), get<1>(pair)); }))
                // {
                //     /* do thing */
                // }

                bool fail{ false };
                for (const reg_t i : equivalence_classes.at(x))
                {
                    for (const reg_t j : equivalence_classes.at(y))
                    {
                        if (overlapping_lifetimes.at(i, j))
                        {
                            fail = true;
                            break;
                        }
                    }

                    if (fail)
                        break;
                }

                if (not fail)
                {
                    representative_map.at(y) = x;
                    std::vector<reg_t> set_union_result;
                    std::ranges::set_union(equivalence_classes.at(x), equivalence_classes.at(y), std::back_inserter(set_union_result));
                    equivalence_classes.at(x) = std::move(set_union_result);
                    equivalence_classes.at(y) = {};
                }
            }
        }


        /* assign remaining registers to an equivalence class (either new or existing) */

        for (reg_t i{ 0 }; i < representative_map.size(); ++i)
        {
            if (representative_map.at(i) != no_register)
                continue;

            bool success{ false };

            for (reg_t j{ 0 }; j < representative_map.size(); ++j)
            {
                if (representative_map.at(j) != j)
                    continue;

                if (std::ranges::all_of(equivalence_classes.at(j), [&](reg_t t) { return not overlapping_lifetimes.at(i, t); }))
                {
                    representative_map.at(i) = j;

                    auto& set = equivalence_classes.at(j);
                    auto it = std::ranges::lower_bound(set, i);
                    if (it == set.end() or *it != i)
                        set.insert(it, i);

                    success = true;
                    break;
                }
            }

            if (not success)
            {
                representative_map.at(i) = i;
                equivalence_classes.at(i).assign({ i });
            }
        }

        /* assign new registers */

        reg_t new_register_number{ 0 };

        for (reg_t i{ 0 }; i < representative_map.size(); ++i)
        {
            if (representative_map.at(i) != i)
                continue;

            for (const reg_t j : equivalence_classes.at(i))
                remap_result.at(j) = new_register_number;

            ++new_register_number;
        }

        return remap_result;
    }

    template<typename CharT>
    constexpr void opt<CharT>::normalise(tdfa_t& dfa)
    {
        for (auto& block : dfa.regops_)
            normalise_regops(block, dfa.register_count_);
    }


    template<typename CharT>
    constexpr void opt<CharT>::operator()(tdfa_t& dfa)
    {
        if (dfa.register_count_ == 0)
            return;

        make_cfg(dfa);
        const auto w = compact_registers(dfa);
        rename_registers(dfa, w);

        for (std::size_t count{ 0 }; count < iterations_; ++count)
        {
            const auto l = liveness(dfa);
            deadcode_elim(dfa, l);
            const auto i = interference(dfa, l);
            const auto v = allocate_registers(dfa, i);
            rename_registers(dfa, v);
            normalise(dfa);
        }
    }


    /* tdfa minimisation */

    template<typename CharT>
    class min
    {
    public:
        using char_type = CharT;
        using tdfa_t = tagged_dfa<CharT>;

        static constexpr void operator()(tdfa_t& dfa);
        static constexpr std::vector<std::vector<std::size_t>> dry_run(const tdfa_t& dfa);

    private:
        // TODO: maybe switch to using unordered_set or flat_set?
        using partition_t = std::vector<bitset_t>;

        static constexpr partition_t init_hopcroft_partition(const tdfa_t& dfa);
        static constexpr partition_t hopcroft(const tdfa_t& dfa);
    };

    template<typename CharT>
    constexpr auto min<CharT>::init_hopcroft_partition(const tdfa_t& dfa) -> partition_t
    {
        const std::size_t bitset_size{ dfa.node_count() };

        partition_t partitions;

        /* add single partition for all non-final states */

        partitions.emplace_back(bitset_size, false);
        for (const std::size_t state : dfa.final_nodes().keys())
            partitions.back().at(state) = true;

        partitions.back().flip();

        /* add separate partitions of final states for each different regops block */

        using key_type = std::pair<final_node_info, std::optional<fallback_node_info>>;
        std::flat_map<key_type, bitset_t> final_node_map;

        for (const auto& [state, fni] : dfa.final_nodes_)
        {
            key_type key{ fni, std::nullopt };

            /* assume fallback states are a subset of final states */
            if (const auto it = dfa.fallback_nodes_.find(state); it != dfa.fallback_nodes_.end())
                key.second = it->second;

            auto [it, _] = final_node_map.try_emplace(std::move(key), bitset_size, false);
            it->second[state] = true;
        }

        partitions.append_range(final_node_map.values());

        /* we keep partitions sorted with the set containing 0 first */

        std::ranges::sort(partitions, std::ranges::greater{});

        return partitions;
    }

    template<typename CharT>
    constexpr auto min<CharT>::hopcroft(const tdfa_t& dfa) -> partition_t
    {
        // Adapted from https://en.wikipedia.org/wiki/DFA_minimization#Hopcroft's_algorithm
        // WARNING: this function is extremely slow

        const std::size_t bitset_size{ dfa.node_count() };

        /* set initial partitions; let work = partition  */
        partition_t partitions{ init_hopcroft_partition(dfa) };
        partition_t work(partitions);

        while (not work.empty())
        {
            const bitset_t transitions_to{ std::move(work.back()) };

            work.pop_back();

            // is this better as a flat_map or as a vector?
            // std::vector<std::vector<partition_entry<char_type, std::size_t>>> symbol_pairs_map(tdfa.reg);
            std::flat_map<std::size_t, std::vector<std::pair<std::reference_wrapper<const charset_t<CharT>>, std::size_t>>> symbol_pairs_map;

            for (std::size_t i{ 0 }, i_end{ dfa.nodes_.size() }; i < i_end; ++i)
                for (const auto& tr : dfa.nodes_[i].tr)
                    if (transitions_to[tr.next])
                        symbol_pairs_map[tr.op_index].emplace_back(std::cref(tr.cs), i);

            for (auto smit = symbol_pairs_map.begin(), end{ symbol_pairs_map.end() }; smit != end; ++smit)
            {
                for (const auto& states : charset_t<CharT>::partition_contents(smit->second))
                {
                    bitset_t transitions_from(bitset_size, false);

                    for (const std::size_t s : states)
                        transitions_from[s] = true;

                    for (std::size_t p{ 0 }; p < partitions.size(); ++p)
                    {
                        bitset_t intersection{ partitions[p] & transitions_from };
                        bitset_t rel_complement{ partitions[p] - transitions_from };

                        const auto i_count = intersection.count();
                        const auto c_count = rel_complement.count();

                        using gt = std::ranges::greater;

                        if (i_count > 0 and c_count > 0)
                        {
                            if (std::ranges::contains(work, partitions[p]))
                            {
                                if (const auto it = std::ranges::lower_bound(work, intersection, gt{}); it == work.end() or *it != intersection)
                                    work.emplace(it, intersection);

                                if (const auto it = std::ranges::lower_bound(work, rel_complement, gt{}); it == work.end() or *it != rel_complement)
                                    work.emplace(it, rel_complement);
                            }
                            else if (i_count <= c_count)
                            {
                                if (const auto it = std::ranges::lower_bound(work, intersection, gt{}); it == work.end() or *it != intersection)
                                    work.emplace(it, intersection);
                            }
                            else
                            {
                                if (const auto it = std::ranges::lower_bound(work, rel_complement, gt{}); it == work.end() or *it != rel_complement)
                                    work.emplace(it, rel_complement);
                            }

                            partitions.erase(partitions.begin() + static_cast<std::ptrdiff_t>(p));

                            if (const auto it = std::ranges::lower_bound(partitions, intersection, gt{}); it == partitions.end() or *it != intersection)
                                partitions.emplace(it, std::move(intersection));

                            if (const auto it = std::ranges::lower_bound(partitions, rel_complement, gt{}); it == partitions.end() or *it != rel_complement)
                                partitions.emplace(it, std::move(rel_complement));
                        }
                    }
                }
            }
        }

        /* note: the initial state must be in the first partition */

        /* if we change the type partition_t, we either need to reorder the
           partitions or keep track of the start state in tagged dfas separately */

        return partitions;
    }

    template<typename CharT>
    constexpr void min<CharT>::operator()(tdfa_t& dfa)
    {
        const partition_t partitions{ hopcroft(dfa) };

        /* create map for node remapping */

        std::vector<std::size_t> state_remap(dfa.node_count(), -1);

        for (std::size_t i{ 0 }, i_end{ partitions.size() }; i < i_end; ++i)
        {
            const auto& part = partitions[i];
            for (std::size_t j{ 0 }, j_end{ part.size() }; j < j_end; ++j)
                if (part[j])
                    state_remap[j] = i;
        }

        /* remap unneeded nodes */

        typename tdfa_t::data_t new_nodes(partitions.size());
        final_nodes_t new_final_nodes;
        fallback_nodes_t new_fallback_nodes;

        bitset_t new_states_visited(partitions.size(), false);

        for (std::size_t i{ 0 }, i_end{ state_remap.size() }; i < i_end; ++i)
        {
            std::size_t remapped_state{ state_remap[i] };
            if (not new_states_visited.at(remapped_state))
            {
                new_states_visited.at(remapped_state) = true;

                new_nodes.at(remapped_state) = std::move(dfa.nodes_.at(i));

                for (auto& tr : new_nodes[remapped_state].tr)
                    tr.next = state_remap.at(tr.next);

                if (const auto it = dfa.final_nodes_.find(i); it != dfa.final_nodes_.end())
                    new_final_nodes.try_emplace(remapped_state, it->second);

                if (const auto it = dfa.fallback_nodes_.find(i); it != dfa.fallback_nodes_.end())
                    new_fallback_nodes.try_emplace(remapped_state, it->second);
            }
        }

        dfa.nodes_ = std::move(new_nodes);
        dfa.final_nodes_ = std::move(new_final_nodes);
        dfa.fallback_nodes_ = std::move(new_fallback_nodes);
    }

    template<typename CharT>
    constexpr std::vector<std::vector<std::size_t>> min<CharT>::dry_run(const tdfa_t& dfa)
    {
        /* perform a dry run of hopcroft, but don't make changes to the dfa */
        const partition_t partitions{ hopcroft(dfa) };
        std::vector<std::vector<std::size_t>> result(partitions.size());
        for (std::size_t i{ 0 }, i_end{ partitions.size() }; i < i_end; ++i)
            for (std::size_t j{ 0 }, j_end{ partitions[i].size() }; j < j_end; ++j)
                if (partitions[i][j])
                    result[i].emplace_back(j);
        return result;
    }

    template<std::input_iterator I, std::sentinel_for<I> S, typename T>
    constexpr std::size_t hash_node(I first, const S last, const std::optional<T>& opt)
    {
        std::size_t hash{ hash::init() };
        for (; first != last; ++first)
            hash::append(hash, *first);
        if (opt.has_value())
            hash::append(hash, *opt);
        return hash;
    }
}

namespace rx::detail
{
    /* tdfa constructor and member functions */

    template<typename CharT>
    constexpr tagged_dfa<CharT>::tagged_dfa(const tagged_nfa<char_type>& tnfa)
        : capture_info_{ tnfa.get_capture_info() }, tag_count_{ tnfa.tag_count() }, flags_{ tnfa.get_flags() }
    {
        tdfa::factory<char_type>{ tnfa, *this, tag_count_ };
    }

    template<typename CharT>
    constexpr void tagged_dfa<CharT>::optimise_registers()
    {
        tdfa::opt<char_type> regoptimise;
        regoptimise(*this);
        compact_regop_blocks();
    }

    template<typename CharT>
    constexpr void tagged_dfa<CharT>::compact_regop_blocks()
    {
        std::vector<std::size_t> regop_block_map(regops_.size());
        std::flat_map<tdfa::regops_t, std::size_t> regop_map;
        regop_data_t new_regops;

        for (std::size_t i{ 0 }, i_end{ regops_.size() }; i < i_end; ++i)
        {
            auto [it, inserted] = regop_map.try_emplace(regops_[i], new_regops.size());

            if (inserted)
                new_regops.emplace_back(regops_[i]);

            regop_block_map[i] = it->second;
        }

        /* remap regop block indicies in dfa */

        for (auto& node : nodes_)
            for (auto& tr : node.tr)
                tr.op_index = (tr.op_index < regop_block_map.size()) ? regop_block_map[tr.op_index] : tdfa::no_transition_regops;

        for (auto it = final_nodes_.begin(), last{ final_nodes_.end() }; it != last; ++it)
            it->second.op_index = (it->second.op_index < regop_block_map.size()) ? regop_block_map[it->second.op_index] : tdfa::no_transition_regops;

        for (auto it = fallback_nodes_.begin(), last{ fallback_nodes_.end() }; it != last; ++it)
            it->second.op_index = (it->second.op_index < regop_block_map.size()) ? regop_block_map[it->second.op_index] : tdfa::no_transition_regops;

        regops_ = std::move(new_regops);
    }

    template<typename CharT>
    constexpr void tagged_dfa<CharT>::minimise_states()
    {
        tdfa::min<char_type> minimise{};
        minimise(*this);
    }

    template<typename CharT>
    constexpr void tagged_dfa<CharT>::minimise_transition_edges()
    {
        /* mutually exclusive with make_default_transitions */

        /* Note: this function relaxes the requirement for a character to appear at most once in any
                 transition edge, and requires the transitions to be checked in the provided order */

        /* DO NOT USE THIS WITH A TABLE OR SWITCH BASED MATCHER */

        using tr_type = tdfa::transition<char_type>;

        for (auto& node : nodes_)
        {
            if (node.tr.empty())
                continue;

            const auto sizes = node.tr | std::views::transform([](auto& t){ return t.cs.count(); }) | std::ranges::to<std::vector>();
            const std::size_t largest_index{ static_cast<std::size_t>(std::ranges::max_element(sizes) - sizes.begin()) };

            // TODO: switch to using views::enumerate when supported by clang
            auto scored_pairs = std::views::zip(std::views::iota(0uz),
                                                node.tr
                                                | std::views::transform([](const auto& t) { return t.cs.score_intervals(); }))
                                | std::views::filter([largest_index](const auto& x) { return get<0>(x) != largest_index; })
                                | std::ranges::to<std::vector>();

            std::ranges::sort(scored_pairs, {}, [](const auto& x){ return get<1>(x); });
            scored_pairs.emplace_back(largest_index, 0 /* unimportant */);

            std::vector<tr_type> new_tr;
            std::vector<tdfa::charset_t<char_type>> dont_cares;
            tdfa::charset_t<char_type> acc;

            for (const auto& [i, _] : scored_pairs)
            {
                auto& tr = node.tr.at(i);
                dont_cares.emplace_back(acc);
                acc |= tr.cs;
                new_tr.emplace_back(std::move(tr));
            }

            if (acc.full())
            {
                const auto& largest = new_tr.back();
                node.default_tr = { .next = largest.next, .op_index = largest.op_index };
                new_tr.pop_back();
            }

            /* fill gaps where possible */

            // TODO: improve optimisations to be bit-aware
            // e.g. [A-Zc-z] with don't cares of [ab] should become [A-Za-z],
            //      which can be optimised to perform half the number of comparions

            for (const auto& [tr_ref, dont_cares] : std::views::zip(std::ranges::ref_view(new_tr), dont_cares))
            {
                tr_type& tr{ tr_ref };

                using interval_t = tdfa::charset_t<char_type>::char_interval;
                std::vector<interval_t> to_insert;

                std::ranges::set_intersection((~tr.cs).get_intervals(), dont_cares.get_intervals(), std::back_inserter(to_insert));

                for (const auto& [beg, end] : to_insert)
                {
                    if (beg == end)
                        tr.cs.insert(beg);
                    else
                        tr.cs.insert(beg, end);
                }
            }

            node.tr = std::move(new_tr);
        }
    }

    template<typename CharT>
    constexpr void tagged_dfa<CharT>::make_default_transitions()
    {
        /* mutually exclusive with minimise_transition_edges */

        using tr_type = tdfa::transition<char_type>;

        for (auto& node : nodes_)
        {
            if (node.tr.empty())
                continue;

            const auto sizes = node.tr | std::views::transform([](auto& t){ return t.cs.count(); }) | std::ranges::to<std::vector>();
            const auto largest_index = static_cast<std::size_t>(std::ranges::max_element(sizes) - sizes.begin());

            auto& largest = node.tr[largest_index];
            tdfa::charset_t<char_type> largest_cs{ largest.cs };
            std::vector<tr_type> new_tr;

            for (std::size_t i{ 0 }, i_end{ node.tr.size() }; i < i_end; ++i)
            {
                if (i == largest_index)
                    continue;

                largest_cs |= node.tr[i].cs;
                new_tr.emplace_back(std::move(node.tr[i]));
            }

            if (largest_cs.full())
                node.default_tr = { .next = largest.next, .op_index = largest.op_index };
            else
                new_tr.emplace_back(std::move(largest));

            node.tr = std::move(new_tr);
        }
    }

    template<typename CharT>
    constexpr void tagged_dfa<CharT>::make_shared_transitions()
    {
        using node_type = tdfa::node<char_type>;

        auto keys = nodes_
                    | std::views::transform([](const node_type& n) {
                        return tdfa::hash_node(n.tr.begin(), n.tr.end(), n.default_tr);
                    })
                    | std::ranges::to<std::vector>();

        auto values = std::views::iota(0uz, nodes_.size())
                    | std::ranges::to<std::vector>();

        // TODO: switch to using std::flat_multimap instead when constexpr is supported
        //       (but an unordered flat set would be much better)
        // const std::flat_multimap map{ std::move(keys), std::move(values) };
        static constexpr auto key_proj = [](const auto& v) -> decltype(auto) { return get<0>(v); }; // TODO: remove
        std::ranges::sort(std::views::zip(keys, values), {}, key_proj); // TODO: remove

        data_t new_nodes{};
        new_nodes.reserve(nodes_.size());

        for (std::size_t current_index{ 0 }, node_count{ nodes_.size() }; current_index < node_count; ++current_index)
        {
            const auto& current = nodes_[current_index];

            const auto beg = current.tr.begin();
            const auto end = current.tr.end();

            bool inserted{ false };

            const auto zv = std::views::zip(keys, values); // TODO: remove

            for (auto it = beg; it != end; ++it)
            {
                const std::size_t hash{ tdfa::hash_node(it, end, current.default_tr) };

                // for (auto [fst, snd] = map.equal_range(keys); fst != snd; ++fst)
                for (auto [fst, snd] = std::ranges::equal_range(zv, hash, {}, key_proj); fst != snd; ++fst)  // TODO: remove
                {
                    auto [_, index] = *fst;

                    if (index == current_index)
                        break; /* prevent replacement with self */


                    if (const auto& other = nodes_.at(index);
                        not (std::ranges::equal(it, end, other.tr.begin(), other.tr.end())
                        and current.default_tr == other.default_tr))
                    {
                        continue;
                    }

                    new_nodes.emplace_back(
                        std::vector<tdfa::transition<char_type>>(beg, it),
                        tdfa::default_transition{ .next = index, .op_index = tdfa::default_transition_is_not_state }
                    );

                    inserted = true;
                    break;
                }

                if (inserted)
                    break;
            }

            if (inserted)
                continue;

            new_nodes.emplace_back(current);
        }

        nodes_ = std::move(new_nodes);
    }

    template<typename CharT>
    constexpr void tagged_dfa<CharT>::de_default_edges()
    {
        for (auto& node : nodes_)
        {
            if (node.default_tr.has_value())
            {
                node.tr.emplace_back(node.default_tr->next, node.default_tr->op_index, ~(tdfa::charset_t<char_type>{}));
                node.default_tr.reset();
            }
        }
    }
}
