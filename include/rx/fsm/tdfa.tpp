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
#include <ranges>
#include <utility>
#include <variant>

#include <boost/dynamic_bitset.hpp>

#include "rx/api/regex_error.hpp"


namespace rx::detail::tdfa
{
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
    using continue_info_t = std::flat_map<std::size_t, std::uint16_t>;

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
            auto it{ std::ranges::lower_bound(data_, tag, std::less{}, &entry::tag) };

            while (it != std::ranges::end(data_) and it->tag == tag)
            {
                if (it->op == op)
                    return { it->reg, true };
                ++it;
            }

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
        using bitset_t = boost::dynamic_bitset<std::size_t>;

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

                auto newer_tag_seq{ ce.new_tag_seq };
                if (e.tag != 0)
                    newer_tag_seq.push_back(e.tag);
                stack.emplace_back(next, ce.registers, ce.tag_seq, std::move(newer_tag_seq));
            }
        }

        if (flags_.longest_match)
        {
            /* this version is needed for full matches, but below is needed for laziness in prefix matches */
            std::erase_if(new_closure, [this](const closure_entry& ce) -> bool {
                const auto& node{ tnfa_ptr_->get_node(ce.tnfa_state) };
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
                const auto& node{ tnfa_ptr_->get_node(ce.tnfa_state) };
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

        node_info current_info{ .config{ std::from_range, c | std::views::transform(closure_entry::next_config) } };
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
            const auto sh_eq_range{ std::ranges::equal_range(sh_zv, sh_key, {}, key_proj) };

            for (const auto& [_, state] : sh_eq_range)
                if (current_info == state_info_.at(state))
                    return state;

            /* check if state can be mapped to an existing state */
            auto mc_zv = std::views::zip(mappable_candidate_keys_, mappable_candidate_values_);
            const std::size_t mc_key{ hash_mappability(current_info) };
            const auto mc_eq_range{ std::ranges::equal_range(mc_zv, mc_key, {}, key_proj) };

            for (const auto& [_, mapped_state] : mc_eq_range)
                if (mappable(current_info, mapped_state, o, result.register_count_))
                    return mapped_state;

            /* create new state */
            result.nodes_.emplace_back();
            state_info_.emplace_back(std::move(current_info));

            const auto sh_offset{ std::ranges::distance(sh_zv.begin(), sh_eq_range.end()) };
            state_hashes_keys_.emplace(state_hashes_keys_.cbegin() + sh_offset, sh_key);
            state_hashes_values_.emplace(state_hashes_values_.cbegin() + sh_offset, new_state);

            const auto mc_offset{ std::ranges::distance(mc_zv.begin(), mc_eq_range.end()) };
            mappable_candidate_keys_.emplace(mappable_candidate_keys_.cbegin() + mc_offset, mc_key);
            mappable_candidate_values_.emplace(mappable_candidate_values_.cbegin() + mc_offset, new_state);
        }

        /* make final regops if state is an accepting state */
        const auto& current_cfg = state_info_.back().config;
        const auto is_final = [this](std::size_t arg) { return tnfa_ptr_->get_node(arg).is_final; };
        const auto it{ std::ranges::find_if(current_cfg, is_final, &configuration::tnfa_state) };
        std::optional<continue_at_t> continue_at;
        if (it != std::ranges::end(current_cfg))
        {
            auto final_ops{ final_regops(result.final_registers_, it->registers, it->tag_seq) };
            const auto& node{ tnfa_ptr_->get_node(it->tnfa_state) };
            const auto final_offset{ node.final_offset };

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
            const auto it2{ std::ranges::find_if(current_cfg, is_fallback, &configuration::tnfa_state) };
            if (it2 != std::ranges::end(current_cfg))
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
                auto h{ history(ce.tag_seq, t) };

                if (not h.empty())
                {
                    regop::op_t op_rhs{ regop_rhs(h) };

                    auto [value_ref, existing]{ map.lookup(t, op_rhs) };

                    if (not existing)
                    {
                        auto i{ regcount++ };
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
            auto h{ history(tag_seq, t) };

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
        const auto& mapped_state_info{ state_info_.at(mapped_state) };

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

                    const auto it{ map.lower_bound(i) };
                    const auto jt{ rmap.lower_bound(j) };

                    bool not_i_match{ it == map.end() or (*it).first != i };
                    bool not_j_match{ jt == rmap.end() or (*jt).first != j };

                    if (not_i_match and not_j_match)
                    {
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

        for (auto it{ o_copy.begin() }; it != o_copy.end();)
        {
            const auto i{ it->dst };

            const auto map_it{ map.find(i) };
            const auto map_end{ map.end() };

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

            const auto& final_ops{ result.get_regops(fni.op_index) };

            /* determine clobbered registers */

            // maybe switch to sets?
            bitset_t added(result.nodes_.size(), false);
            bitset_t clobbered(result.register_count_, false);

            std::vector<std::pair<std::size_t, std::size_t>> stack;
            stack.emplace_back(state, 0);
            added.at(state) = true;

            while (not stack.empty())
            {
                auto& [s, i]{ stack.back() };

                if (result.final_nodes_.contains(s) or i >= result.nodes_.at(s).tr.size())
                {
                    stack.pop_back();
                }
                else
                {
                    const auto& tr{ result.nodes_.at(s).tr.at(i) };

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

            auto continuation_index{ tdfa::no_continue };
            if (auto it{ cont_info_.find(state) }; it != cont_info_.end())
                continuation_index = it->second;

            /* insert fallback regops */

            regops_t o;

            for (const auto& f : final_ops)
            {
                if (auto* cpy{ get_if<regop::copy>(&f.op) }; cpy != nullptr and clobbered.at(cpy->src))
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
            auto& current_etr{ epsilon_transitions_.at(q) };
            auto& current_tr{ normal_transitions_.at(q) };

            for (const auto tr_idx : tnfa_ptr_->get_node(q).out_tr)
            {
                const auto& tr{ tnfa_ptr_->get_tr(tr_idx) };

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
        : tnfa_ptr_{ &input }, tag_count_{ std::saturate_cast<reg_t>(tag_count) }, flags_{ result.flags_ }
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

        for (std::size_t state{ initial }; state < result.nodes_.size(); ++state)
        {
            tag_op_map map;

            for (auto& [cs, cfg] : multistep(state))
            {
                cfg = e_closure(std::move(cfg));
                auto o{ transition_regops(cfg, result.register_count_, map) };
                const auto s{ add_state(result, cfg, o) };

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
}

namespace rx::detail
{
    template<typename CharT>
    constexpr tagged_dfa<CharT>::tagged_dfa(const tagged_nfa<char_type>& tnfa)
        : capture_info_{ tnfa.get_capture_info() }, tag_count_{ tnfa.tag_count() }, flags_{ tnfa.get_flags() }
    {
        tdfa::factory<char_type>{ tnfa, *this, tag_count_ };
    }
}
