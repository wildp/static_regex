// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "tdfa.hpp"

#include <algorithm>
#include <optional>
#include <vector>


namespace rx::detail::tdfa
{
    template<typename CharT>
    class min
    {
    public:
        using char_type = CharT;
        using tdfa_t = tagged_dfa<CharT>;

        static constexpr void operator()(tdfa_t& dfa);
        static constexpr void compact_regop_blocks(tdfa_t& dfa);
        static constexpr std::vector<std::vector<std::size_t>> dry_run(const tdfa_t& dfa);

    private:
        // TODO: switch to boost::dynamic_bitset
        using bitset_t = std::vector<bool>;

        // TODO: maybe switch to using unordered_set or flat_set?
        using partition_t = std::vector<bitset_t>;

        static constexpr partition_t init_hopcroft_partition(const tdfa_t& dfa);
        static constexpr partition_t hopcroft(const tdfa_t& dfa);
    };

    template<typename CharT>
    constexpr void min<CharT>::compact_regop_blocks(tdfa_t& dfa)
    {
        std::vector<std::size_t> regop_block_map(dfa.regops_.size());
        std::flat_map<regops_t, std::size_t> regop_map;
        typename tdfa_t::regop_data_t new_regops;

        for (std::size_t i{ 0 }; i < dfa.regops_.size(); ++i)
        {
            auto [it, inserted]{ regop_map.try_emplace(dfa.regops_[i], new_regops.size()) };

            if (inserted)
                new_regops.emplace_back(dfa.regops_[i]);

            regop_block_map[i] = it->second;
        }

        /* remap regop block indicies in dfa */

        for (auto& node : dfa.nodes_)
            for (auto& tr : node.tr)
                tr.op_index = (tr.op_index < regop_block_map.size()) ? regop_block_map[tr.op_index] : no_transition_regops;

        for (auto it{ dfa.final_nodes_.begin() }, last{ dfa.final_nodes_.end() }; it != last; ++it)
            it->second.op_index = (it->second.op_index < regop_block_map.size()) ? regop_block_map[it->second.op_index] : no_transition_regops;

        for (auto it{ dfa.fallback_nodes_.begin() }, last{ dfa.fallback_nodes_.end() }; it != last; ++it)
            it->second.op_index = (it->second.op_index < regop_block_map.size()) ? regop_block_map[it->second.op_index] : no_transition_regops;

        dfa.regops_ = std::move(new_regops);
    }

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
            if (const auto it{ dfa.fallback_nodes_.find(state) }; it != dfa.fallback_nodes_.end())
                key.second = it->second;

            auto [it, _]{ final_node_map.try_emplace(std::move(key), bitset_size, false) };
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

            for (std::size_t i{ 0 }; i < dfa.nodes_.size(); ++i)
                for (const auto& tr : dfa.nodes_[i].tr)
                    if (transitions_to[tr.next])
                        symbol_pairs_map[tr.op_index].emplace_back(std::cref(tr.cs), i);

            for (auto smit{ symbol_pairs_map.begin() }, end{ symbol_pairs_map.end() }; smit != end; ++smit)
            {
                for (const auto& states : charset_t<CharT>::partition_contents(smit->second))
                {
                    bitset_t transitions_from(bitset_size, false);

                    for (const std::size_t s : states)
                        transitions_from[s] = true;

                    for (std::size_t p{ 0 }; p < partitions.size(); ++p)
                    {
                        bitset_t intersection(bitset_size, false);
                        bitset_t rel_complement(bitset_size, false);

                        for (std::size_t i{ 0 }; i < bitset_size; ++i)
                        {
                            intersection[i] = partitions[p][i] and transitions_from[i];
                            rel_complement[i] = partitions[p][i] and not transitions_from[i];
                        }

                        const auto i_count{ std::ranges::count(intersection, true) };
                        const auto c_count{ std::ranges::count(rel_complement, true) };

                        using gt = std::ranges::greater;

                        if (i_count > 0 and c_count > 0)
                        {
                            if (std::ranges::contains(work, partitions[p]))
                            {
                                if (const auto it{ std::ranges::lower_bound(work, intersection, gt{}) }; it == work.end() or *it != intersection)
                                    work.emplace(it, intersection);

                                if (const auto it{ std::ranges::lower_bound(work, rel_complement, gt{}) }; it == work.end() or *it != rel_complement)
                                    work.emplace(it, rel_complement);
                            }
                            else if (i_count <= c_count)
                            {
                                if (const auto it{ std::ranges::lower_bound(work, intersection, gt{}) }; it == work.end() or *it != intersection)
                                    work.emplace(it, intersection);
                            }
                            else
                            {
                                if (const auto it{ std::ranges::lower_bound(work, rel_complement, gt{}) }; it == work.end() or *it != rel_complement)
                                    work.emplace(it, rel_complement);
                            }

                            partitions.erase(partitions.begin() + static_cast<std::ptrdiff_t>(p));

                            if (const auto it{ std::ranges::lower_bound(partitions, intersection, gt{}) }; it == partitions.end() or *it != intersection)
                                partitions.emplace(it, std::move(intersection));

                            if (const auto it{ std::ranges::lower_bound(partitions, rel_complement, gt{}) }; it == partitions.end() or *it != rel_complement)
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
        compact_regop_blocks(dfa);
        const partition_t partitions{ hopcroft(dfa) };

        /* create map for node remapping */

        std::vector<std::size_t> state_remap(dfa.node_count(), -1);

        for (std::size_t i{ 0 }; i < partitions.size(); ++i)
        {
            const auto& part{ partitions[i] };
            for (std::size_t j{ 0 }; j < part.size(); ++j)
                if (part[j])
                    state_remap[j] = i;
        }

        /* remap unneeded nodes */

        typename tdfa_t::data_t new_nodes(partitions.size());
        final_nodes_t new_final_nodes;
        fallback_nodes_t new_fallback_nodes;

        bitset_t new_states_visited(partitions.size(), false);

        for (std::size_t i{ 0 }; i < state_remap.size(); ++i)
        {
            std::size_t remapped_state{ state_remap[i] };
            if (not new_states_visited.at(remapped_state))
            {
                new_states_visited.at(remapped_state) = true;

                new_nodes.at(remapped_state) = std::move(dfa.nodes_.at(i));

                for (auto& tr : new_nodes[remapped_state].tr)
                    tr.next = state_remap.at(tr.next);

                if (const auto it{ dfa.final_nodes_.find(i) }; it != dfa.final_nodes_.end())
                    new_final_nodes.try_emplace(remapped_state, it->second);

                if (const auto it{ dfa.fallback_nodes_.find(i) }; it != dfa.fallback_nodes_.end())
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
        for (std::size_t i{ 0 }; i < partitions.size(); ++i)
            for (std::size_t j{ 0 }; j < partitions[i].size(); ++j)
                if (partitions[i][j])
                    result[i].emplace_back(j);
        return result;
    }
}

namespace rx::detail
{
    template<typename CharT>
    constexpr void tagged_dfa<CharT>::minimise_states()
    {
        std::invoke(tdfa::min<char_type>{}, *this);
    }

    template<typename CharT>
    constexpr void tagged_dfa<CharT>::minimise_transitions()
    {
        using tr_type = tdfa::transition<char_type>;

        for (auto& node : nodes_)
        {
            if (node.tr.empty())
                continue;

            const std::vector sizes{ std::from_range, node.tr | std::views::transform([](auto& t){ return t.cs.count(); }) };
            const std::size_t largest_index{ static_cast<std::size_t>(std::ranges::distance(std::ranges::begin(sizes), std::ranges::max_element(sizes))) };

            auto& largest{ node.tr[largest_index] };
            tdfa::charset_t<char_type> largest_cs{ largest.cs };
            std::vector<tr_type> new_tr;
            
            for (std::size_t i{ 0 }; i < node.tr.size(); ++i)
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
}
