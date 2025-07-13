#pragma once

#include <rx/fsm/tdfa.hpp>

#include <algorithm>
#include <functional>
#include <iterator>
#include <limits>
#include <numeric>
#include <ranges>
#include <utility>
#include <variant>

#include <rx/etc/partition.hpp>


namespace rx::detail::tdfa
{
    /* tdfa normalisation */

    namespace 
    {
        constexpr bool toposort_regops(const regops_t::iterator beg, const regops_t::iterator end, const reg_t regcount)
        {
            std::vector<std::size_t> indeg(regcount, 0);

            for (auto it{ beg }; it != end; ++it)
            {
                if (auto* copy{ std::get_if<regop::copy>(&it->op) }; copy != nullptr)
                    ++indeg.at(copy->src);
            }

            regops_t o_new;
            regops_t o_copy{ beg, end };
            bool cycle_detected{ false };

            while (not o_copy.empty())
            {
                bool added{ false };
                
                for (auto it{ o_copy.begin() }; it != o_copy.end();)
                {
                    if (indeg.at(it->dst) == 0)
                    {
                        if (auto* copy{ std::get_if<regop::copy>(&it->op) }; copy != nullptr)
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
                        if (auto* copy{ std::get_if<regop::copy>(&oc.op) }; copy != nullptr and copy->src != oc.dst)
                        {
                            cycle_detected = true;
                            break;
                        }
                    }   

                    o_new.append_range(o_copy);
                    break;
                }
            }

            if (std::cmp_not_equal(std::ranges::distance(beg, end), o_new.size()))
                cycle_detected = true; /* unknown error */

            if (not cycle_detected)
                std::ranges::move(o_copy, beg);

            return not cycle_detected;
        }

        constexpr regops_t::iterator dedup_regops(regops_t& o, const regops_t::iterator beg, const regops_t::iterator end)
        {
            regops_t::iterator new_end{ end };

            for (auto it{ beg }; it != new_end; ++it)
                new_end = std::remove(it + 1, new_end, *it);

            return o.erase(new_end, end);
        }

        [[maybe_unused]] constexpr void normalise_regops(regops_t& o, const reg_t regcount)
        {
            for (auto it{ o.begin() }; it != o.end();)
            {   
                auto local_end{ it + 1 };
                if (std::holds_alternative<regop::set>(it->op))
                {
                    while (local_end != o.end() and std::holds_alternative<regop::set>(local_end->op))
                        ++local_end;

                    local_end = dedup_regops(o, it, local_end);
                    std::ranges::sort(it, local_end, std::ranges::less{}, &regop::dst);
                    it = local_end;
                }
                else if (std::holds_alternative<regop::copy>(it->op))
                {
                    while (local_end != o.end() and std::holds_alternative<regop::copy>(local_end->op))
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
    }


    /* tdfa optimisation */

    class liveness_matrix
    {
    public:
        explicit constexpr liveness_matrix(std::size_t block_count, reg_t reg_count) : data_(block_count * reg_count, false), reg_count_{ reg_count } {}
        [[nodiscard]] constexpr auto operator[](std::size_t block_idx, reg_t reg) { return data_[(block_idx * reg_count_) + reg]; }
        [[nodiscard]] constexpr auto operator[](std::size_t block_idx, reg_t reg) const { return data_[(block_idx * reg_count_) + reg]; }
        [[nodiscard]] constexpr auto at(std::size_t block_idx, reg_t reg) { return data_.at((block_idx * reg_count_) + reg); }
        [[nodiscard]] constexpr auto at(std::size_t block_idx, reg_t reg) const { return data_.at((block_idx * reg_count_) + reg); }
        [[nodiscard]] constexpr auto row_begin(std::size_t block_idx) { return data_.begin() + static_cast<std::ptrdiff_t>(block_idx * reg_count_); }
        [[nodiscard]] constexpr auto row_end(std::size_t block_idx) { return row_begin(block_idx + 1); }
        [[nodiscard]] constexpr auto row_begin(std::size_t block_idx) const { return data_.begin() + static_cast<std::ptrdiff_t>(block_idx * reg_count_); }
        [[nodiscard]] constexpr auto row_end(std::size_t block_idx) const { return row_begin(block_idx + 1); }
        [[nodiscard]] constexpr reg_t reg_count() const { return reg_count_; }
        [[nodiscard]] constexpr bool block_valid(std::size_t block_idx) const { return (block_idx * reg_count_) < data_.size(); }

    private:
        std::vector<bool> data_;
        reg_t reg_count_;
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
        constexpr void set_symmetric(std::size_t reg1, std::size_t reg2, bool value) { at(reg1, reg2) = value; at(reg2, reg1) = value; }

    private:
        std::vector<bool> data_;
        std::size_t reg_count_;
    };


    template<typename CharT>
    class opt
    {
    public:
        using tdfa_t = tagged_dfa<CharT>;

        constexpr opt(std::size_t i = 2) noexcept : iterations_{ i } {};
        constexpr void operator()(tdfa_t& dfa);

    private:
        using remap_t = std::vector<reg_t>;

        [[nodiscard]] static constexpr remap_t compact_registers(const tdfa_t& dfa);
        [[nodiscard]] static constexpr remap_t allocate_registers(const tdfa_t& dfa, const square_matrix& overlapping_lifetimes);
        [[nodiscard]] constexpr liveness_matrix liveness(const tdfa_t& dfa) const;
        [[nodiscard]] constexpr square_matrix interference(const tdfa_t& dfa, const liveness_matrix& live) const;

        static constexpr void rename_registers(tdfa_t& dfa, const remap_t& remap);
        static constexpr void deadcode_elim(tdfa_t& dfa, const liveness_matrix& live);
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

        for (std::size_t node_idx{ 0 }; node_idx < dfa.nodes_.size(); ++node_idx)
        {
            for (const auto& tr: dfa.nodes_.at(node_idx).tr)
            {
                if (tr.op_index == no_transition_regops)
                    reachable.at(node_idx, tr.next) = true;
                else
                    successor_blocks.at(node_idx).emplace_back(tr.op_index);
            }

            /* add final transitions too */

            if (dfa.final_nodes_.contains(node_idx))
                 if (auto final_block_idx{ dfa.final_nodes_.at(node_idx) }; final_block_idx != no_transition_regops)
                    successor_blocks.at(node_idx).emplace_back(final_block_idx);
        }

        /* calculate reachability matrix (inital reachability is added above) */

        for (std::size_t i{ 0 }; i < reachable.side_length(); ++i)
            reachable[i, i] = true;

        for (std::size_t k{ 0 }; k < reachable.side_length(); ++k)
            for (std::size_t i{ 0 }; i < reachable.side_length(); ++i)
                for (std::size_t j{ 0 }; j < reachable.side_length(); ++j)
                    if (not reachable[i, j])
                        reachable[i, j] = reachable[i, k] and reachable[k, j];

        /* construct graph */

        std::vector<std::vector<std::size_t>> nodes_to_edges(dfa.nodes_.size());

        for (std::size_t node_idx{ 0 }; node_idx < dfa.nodes_.size(); ++node_idx)
        {
            std::vector<std::size_t> tmp;

            for (std::size_t i{ 0 }; i < reachable.side_length(); ++i)
                if (reachable.at(node_idx, i))
                    tmp.insert_range(tmp.end(), successor_blocks.at(i));

            std::ranges::sort(tmp);
            auto [beg, end]{ std::ranges::unique(tmp) };
            tmp.erase(beg, end);
            tmp.shrink_to_fit();

            nodes_to_edges.at(node_idx) = std::move(tmp);
        }

        for (std::size_t node_idx{ 0 }; node_idx < dfa.nodes_.size(); ++node_idx)
            for (const auto& tr: dfa.nodes_.at(node_idx).tr)
                if (tr.op_index != no_transition_regops)
                    block_graph_.at(tr.op_index) = nodes_to_edges.at(tr.next);

        block_graph_start_ = std::move(nodes_to_edges.at(dfa.match_start));
    }

    template<typename CharT>
    constexpr auto opt<CharT>::compact_registers(const tdfa_t& dfa) -> remap_t
    {
        std::vector<bool> visited(dfa.register_count_, false);
        remap_t remap_result(dfa.register_count_, std::numeric_limits<reg_t>::max());

        for (auto& block : dfa.regops_)
        {
            for (auto& op : block)
            {
                if (auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr)
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
            for (auto it{ std::ranges::begin(block) }; it != std::ranges::end(block);)
            {
                it->dst = remap.at(it->dst);
                if (auto* cpy{ std::get_if<regop::copy>(&it->op) }; cpy != nullptr)
                {
                    cpy->src = remap.at(cpy->src);

                    if (cpy->src == it->dst)
                        it = block.erase(it);  /* remove trivial cycles */
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
        /* unfortunately coroutines can't be constexpr */

        class postorder_visitor
        {
        public:

            explicit constexpr postorder_visitor(const std::vector<std::size_t>& block_graph_start, std::size_t block_count) : 
                block_added(block_count, false)
            {
                for (const auto i: block_graph_start | std::views::reverse)
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
                    const auto [block_idx, i]{ stack.back() };

                    if (i == block_graph.at(block_idx).size())
                    {
                        result = block_idx;
                        stack.pop_back();
                    }
                    else
                    {
                        const auto next{ block_graph.at(block_idx).at(i) };
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
            std::vector<bool> block_added;
        };

        /* resume liveness implementation */

        liveness_matrix liveness(dfa.regops_.size(), dfa.register_count_);
        
        /* make registers assigned to in final transitions live */

        for (const auto [_, block_idx] : dfa.final_nodes_)
            for (const reg_t final_reg : dfa.final_registers_)
                liveness.at(block_idx, final_reg) = true;
                
        for (bool loop{ true }; loop; )
        {
            loop = false;
            postorder_visitor vis{ block_graph_start_, dfa.regops_.size() };

            while (true)
            {
                const auto opt{ vis(block_graph_) };
                if (not opt.has_value())
                    break;

                const auto block_idx{ opt.value() };

                std::vector<bool> current_row_copy(liveness.row_begin(block_idx), liveness.row_end(block_idx));

                for (const std::size_t sblock_idx : block_graph_.at(block_idx))
                {
                    std::vector<bool> successor_row_copy(liveness.row_begin(sblock_idx), liveness.row_end(sblock_idx));

                    for (const auto& op : dfa.regops_.at(sblock_idx) | std::views::reverse) /* todo: in post order? */
                    {
                        if (std::holds_alternative<regop::set>(op.op))
                        {
                            successor_row_copy.at(op.dst) = false; 
                        }
                        else if (const auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr)
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

                    // /* This is apparently slower than using a naive loop */
                    // std::transform(current_row_copy.begin(), current_row_copy.end(),
                    //                successor_row_copy.begin(), current_row_copy.begin(), std::logical_and{});

                    for (std::size_t i{ 0 }; i < current_row_copy.size(); ++i)
                        current_row_copy[i] = current_row_copy[i] or successor_row_copy[i];
                }

                if (not std::equal(current_row_copy.begin(), current_row_copy.end(), liveness.row_begin(block_idx)))
                {
                    std::copy(current_row_copy.begin(), current_row_copy.end(), liveness.row_begin(block_idx));
                    loop = true;
                }
            }
        }

        for (const auto [fallback_state, block_idx] : dfa.fallback_nodes_)
        {
            std::vector<bool> current_row;

            if (block_idx == no_transition_regops)
            {
                current_row.assign(liveness.reg_count(), false);

                for (const std::size_t final_reg : dfa.final_registers_)
                    current_row.at(final_reg) = true;
            }
            else
            {
                for (const std::size_t final_reg : dfa.final_registers_)
                    liveness.at(block_idx, final_reg) = true;

                current_row.assign(liveness.row_begin(block_idx), liveness.row_end(block_idx));

                for (const auto& op : dfa.regops_.at(block_idx))
                    current_row.at(op.dst) = false;

                for (const auto& op : dfa.regops_.at(block_idx))
                    if (auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr)
                        current_row.at(cpy->src) = false;
            }

            /* traverse nodes that can possibly fallback to fallback_state */

            std::vector<bool> added(dfa.nodes_.size(), false);
            std::vector<std::pair<std::size_t, std::size_t>> stack;
            stack.emplace_back(fallback_state, 0);
            added.at(fallback_state) = true;

            while (not stack.empty())
            {
                auto& [s, i]{ stack.back() };

                if (dfa.final_nodes_.contains(s) or i >= dfa.nodes_.at(s).tr.size())
                {
                    stack.pop_back();
                }
                else
                {
                    const auto& tr{ dfa.nodes_.at(s).tr.at(i) };

                    /* note: we overapproximate here by assuming the transition function from a state is
                    *       not a total function, making a fallback possible from every non-final state */

                    if (liveness.block_valid(tr.op_index))
                        for (std::size_t i{ 0 }; i < liveness.reg_count(); ++i)
                            liveness[tr.op_index, i] = liveness[tr.op_index, i] or current_row[i];

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
        for (std::size_t block_idx{ 0 }; block_idx < dfa.regops_.size(); ++block_idx)
        {
            auto& block{ dfa.regops_.at(block_idx) };
            std::vector<bool> current_row_copy(liveness.row_begin(block_idx), liveness.row_end(block_idx));

            for (std::size_t i{ block.size() }; i > 0; --i)
            {
                const auto& op{ block.at(i - 1) };

                if (current_row_copy.at(op.dst))
                {   
                    if (std::holds_alternative<regop::set>(op.op))
                        current_row_copy.at(op.dst) = false; 
                    else if (const auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr)
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

        for (std::size_t block_idx{ 0 }; block_idx < dfa.regops_.size(); ++block_idx)
        {
            const auto& block{ dfa.regops_.at(block_idx) };

            for (const auto& op : block)
                if (const auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr)
                    histories.at(cpy->src) = op.op;

            for (const auto& op: block)
            {
                if (const auto* set{ std::get_if<regop::set>(&op.op) }; set != nullptr)
                    histories.at(op.dst) = op.op;
                else if (const auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr)
                    histories.at(op.dst) = histories.at(cpy->src);
                else
                    std::unreachable();
            }

            for (const auto& op : block)
            {
                std::vector<bool> current_row_copy(liveness.row_begin(block_idx), liveness.row_end(block_idx));

                if (const auto* set{ std::get_if<regop::set>(&op.op) }; set != nullptr)
                    current_row_copy.at(op.dst) = false; 
                else if (const auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr)
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
        constexpr reg_t no_register{ std::numeric_limits<reg_t>::max() };

        remap_t remap_result(dfa.register_count_, no_register);
        remap_t representative_map(dfa.register_count_, no_register);
        std::vector<std::vector<reg_t>> equivalence_classes(dfa.register_count_); // TODO: change to std::vector<std::flat_set<reg_t>> when available

        for (const auto& block : dfa.regops_)
        {
            for (const auto& op : block)
            {
                if (const auto* cpy{ std::get_if<regop::copy>(&op.op) }; cpy != nullptr and op.dst != cpy->src)
                {
                    reg_t x{ representative_map.at(op.dst) };
                    reg_t y{ representative_map.at(cpy->src) };

                    if (x == no_register and y == no_register)
                    {
                        if (not overlapping_lifetimes.at(op.dst, cpy->src))
                        {
                            representative_map.at(op.dst) = op.dst;
                            representative_map.at(cpy->src) = op.dst;

                            auto& set{ equivalence_classes.at(op.dst) };
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
                        if (std::ranges::all_of(equivalence_classes.at(x), [&](reg_t t){ return not overlapping_lifetimes.at(t, cpy->src); }))
                        {
                            representative_map.at(cpy->src) = x;

                            auto& set{ equivalence_classes.at(x) };
                            auto it{ std::ranges::lower_bound(set, cpy->src) };
                            if (it == set.end() or *it != cpy->src)
                                set.insert(it, cpy->src);
                        }
                    }
                    else if (x == no_register and y != no_register)
                    {
                        if (std::ranges::all_of(equivalence_classes.at(y), [&](reg_t t){ return not overlapping_lifetimes.at(t, op.dst); }))
                        {
                            representative_map.at(op.dst) = y;

                            auto& set{ equivalence_classes.at(y) };
                            auto it{ std::ranges::lower_bound(set, op.dst) };
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
                //                         [&](const std::tuple<reg_t, reg_t>& pair) { return not overlapping_lifetimes.at(std::get<0>(pair), std::get<1>(pair)); }))
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

                if (std::ranges::all_of(equivalence_classes.at(j), [&](reg_t t){ return not overlapping_lifetimes.at(i, t); }))
                {
                    representative_map.at(i) = j;

                    auto& set{ equivalence_classes.at(j) };
                    auto it{ std::ranges::lower_bound(set, i) };
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
        auto w{ compact_registers(dfa) };
        rename_registers(dfa, w);

        for (std::size_t count{ 0 }; count < iterations_; ++count)
        {
            auto l{ liveness(dfa) };
            deadcode_elim(dfa, l);
            auto i{ interference(dfa, l) };
            auto v{ allocate_registers(dfa, i) };
            rename_registers(dfa, v);
            normalise(dfa);
        }
    }

  
    /* tnfa -> tdfa conversion */

    using tag_t = int;
    using tag_sequence_t = std::vector<tag_t>;

    using reg_vec = std::vector<reg_t>;

    struct configuration
    {
        std::size_t     tnfa_state{ 0 };
        reg_vec         registers;
        tag_sequence_t  tag_seq;

        friend constexpr bool operator==(const configuration&, const configuration&) = default;
    };

    struct closure_entry
    {
        std::size_t     tnfa_state{ 0 };
        reg_vec         registers;
        tag_sequence_t  tag_seq;
        tag_sequence_t  new_tag_seq;

        [[maybe_unused]] friend constexpr bool operator==(const closure_entry&, const closure_entry&) = default;

        constexpr closure_entry(std::size_t state, reg_vec reg, tag_sequence_t seq = {}, tag_sequence_t newseq = {}) :
            tnfa_state{ state }, registers{ std::move(reg) }, tag_seq{ std::move(seq) }, new_tag_seq{ std::move(newseq) } {}
    };

    namespace
    {
        [[maybe_unused]] inline constexpr configuration next_config_from_ce(const closure_entry& ce)
        {
            return { .tnfa_state = ce.tnfa_state, .registers = ce.registers, .tag_seq = ce.new_tag_seq }; 
        }
    }

    using config_set_t = std::vector<configuration>;
    using closure_t = std::vector<closure_entry>;
    using register_map_t = std::flat_map<reg_t, reg_t>;

    template<typename CharT>
    using multistep_closure = partition_entry<CharT, closure_entry>;

    template<typename CharT>
    using multistep_closures_t = std::vector<multistep_closure<CharT>>;

    class precedence_v1
    {
    public:
        explicit constexpr precedence_v1(const closure_t& c)
        {
            std::size_t i{ 0 };
            for (const auto& ce : c)
                if (auto [_, success]{ data_.try_emplace(ce.tnfa_state, i) }; success)
                    ++i;
        }

        [[nodiscard]] constexpr std::size_t operator()(std::size_t tnfa_state) const
        {
            auto it{ data_.find(tnfa_state) };
            if (it != data_.end())
                return it->second;
            return std::numeric_limits<std::size_t>::max();
        }
        
        friend constexpr bool operator==(const precedence_v1&, const precedence_v1&) = default; 

    private:
        std::flat_map<std::size_t, std::size_t> data_;
    };

    class [[maybe_unused]] precedence_v2
    {
    public:
        explicit constexpr precedence_v2(const closure_t& c, std::size_t size) :
                data_(size, std::numeric_limits<std::size_t>::max())
        {
            std::size_t i{ 0 };
            for (const auto& ce : c)
                if (data_.at(ce.tnfa_state) == std::numeric_limits<std::size_t>::max())
                    data_.at(ce.tnfa_state) = i++;
        }

        constexpr std::size_t operator()(std::size_t tnfa_state) const
        {
            return data_.at(tnfa_state);
        }

        [[maybe_unused]] friend constexpr bool operator==(const precedence_v2&, const precedence_v2&) = default; 

    private:
        std::vector<std::size_t> data_;
    };

    // TODO: benchmark precedence_v1 vs precedence_v2
    using precedence_t = precedence_v1;

    struct node_info
    {
        config_set_t    config;
        precedence_t    precedence;

        friend constexpr bool operator==(const node_info&, const node_info&) = default;
    };

    using state_info_t = std::vector<node_info>;

    class tag_op_map
    {
        struct entry
        {
            tag_t           tag;
            regop::op_t     op;
            reg_t           reg;
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

        constexpr explicit factory(const tnfa_t& input, tdfa_t& result, std::size_t tag_count, bool is_search = false);

    private:
        [[nodiscard]] constexpr closure_t e_closure(closure_t&& c) const;
        [[nodiscard]] constexpr std::size_t add_state(tdfa_t& result, const closure_t& c, const precedence_t& p, regops_t& o);
        [[nodiscard]] constexpr multistep_closures_t<char_type> multistep_v1(std::size_t state) const;
        [[nodiscard]] constexpr multistep_closures_t<char_type> multistep_v2(std::size_t state) const;
        [[nodiscard]] constexpr regops_t transition_regops(closure_t& c, reg_t& regcount, tag_op_map& map) const;
        [[nodiscard]] constexpr regops_t final_regops(const final_regs_t& final_registers, const reg_vec& r, const tag_sequence_t& tag_seq) const;
        [[nodiscard]] constexpr regop::op_t regop_rhs(const std::vector<bool>& hist) const;
        [[nodiscard]] constexpr std::vector<bool> history(const tag_sequence_t& hist, tag_t tag) const;
        [[nodiscard]] constexpr bool mappable(const node_info& state, std::size_t mapped_state, regops_t& o, reg_t regcount) const;

        constexpr void add_initial_state(tdfa_t& result, const closure_t& c, const precedence_t& p);

        constexpr void fallback_regops(tdfa_t& result);
        constexpr void backup_regops(tdfa_t& result, std::size_t state, reg_t reg_dst, reg_t reg_src);

        const tnfa_t* tnfa_ptr_;
        state_info_t state_info_;
        reg_t tag_count_;
    };

    template<typename CharT>
    constexpr auto factory<CharT>::e_closure(closure_t&& c) const -> closure_t
    {
        closure_t new_closure;

        closure_t stack(std::move(c));
        std::erase_if(stack, [](const closure_entry& ce) { return not ce.new_tag_seq.empty(); });
        std::ranges::reverse(stack);

        while (not stack.empty())
        {   
            new_closure.push_back(std::move(stack.back()));
            stack.pop_back();
            const closure_entry& ce{ new_closure.back() };

            // TODO: maybe reimplement to be more efficient, using getif?
            std::vector<epsilon_tr> et{ std::from_range,
                                        tnfa_ptr_->get_node(ce.tnfa_state).tr
                                        | std::views::filter([](const auto& t) { return std::holds_alternative<epsilon_tr>(t); })
                                        | std::views::transform([](const auto& t) { return std::get<epsilon_tr>(t); }) };
            
            std::ranges::sort(et, std::ranges::greater{}, &epsilon_tr::priority);

            for (const epsilon_tr& e : et)
            {
                if (not std::ranges::contains(new_closure, e.next, &closure_entry::tnfa_state))
                {
                    auto newer_tag_seq{ ce.new_tag_seq };
                    newer_tag_seq.push_back(e.tag);
                    stack.emplace_back(e.next, ce.registers, ce.tag_seq, std::move(newer_tag_seq));
                }   
            }
        }

        std::erase_if(new_closure, [this](const closure_entry& ce) -> bool {
            if (ce.tnfa_state == tnfa_ptr_->end)
                return false;
            return 0 != std::ranges::count_if(tnfa_ptr_->get_node(ce.tnfa_state).tr,
                                              [](const auto& t) { return not std::holds_alternative<n_tr<CharT>>(t); });
        });

        return new_closure;
    }

    template<typename CharT>
    constexpr void factory<CharT>::add_initial_state(tdfa_t& result, const closure_t& c, const precedence_t& p)
    {
        config_set_t configs{ std::from_range, c | std::views::transform(next_config_from_ce) };

        /* guarantee initial state has number 0 */
        result.nodes_.clear();
        state_info_.clear();

        /* create new state */
        result.nodes_.emplace_back();
        state_info_.emplace_back(std::move(configs), p);

        /* make final regops if initial state is an accepting state */
        const auto& current_cfg = state_info_.back().config;
        const auto it{ std::ranges::find(current_cfg, tnfa_t::end, &configuration::tnfa_state) };
        if (it != std::ranges::end(current_cfg))
        {
            auto final_ops{ final_regops(result.final_registers_, it->registers, it->tag_seq) };
            if (final_ops.empty())
            {
                /* avoid creating empty regop blocks */
                result.final_nodes_.emplace(0, no_transition_regops);
            }
            else
            {
                result.final_nodes_.emplace(0, result.regops_.size());
                result.regops_.emplace_back(std::move(final_ops));
            }
        }
    }

    template<typename CharT>
    constexpr auto factory<CharT>::add_state(tdfa_t& result, const closure_t& c, const precedence_t& p, regops_t& o) -> std::size_t
    {
        node_info current_info{ .config{ std::from_range, c | std::views::transform(next_config_from_ce) },.precedence = p };

        const std::size_t new_state{ static_cast<std::size_t>(std::ranges::distance(state_info_.begin(), std::ranges::find(state_info_, current_info))) };

        /* check if state already exists */
        if (new_state < state_info_.size())
            return new_state;
        
        /* check if state can be mapped to an existing state */
        for (std::size_t mapped_state{ 0 }; mapped_state < state_info_.size(); ++mapped_state)
            if (mappable(current_info, mapped_state, o, result.register_count_))
                return mapped_state;

        /* create new state */
        result.nodes_.emplace_back();
        state_info_.emplace_back(std::move(current_info));

        /* make final regops if state is an accepting state */
        const auto& current_cfg = state_info_.back().config;
        const auto it{ std::ranges::find(current_cfg, tnfa_t::end, &configuration::tnfa_state) };
        if (it != std::ranges::end(current_cfg))
        {
            auto final_ops{ final_regops(result.final_registers_, it->registers, it->tag_seq) };
            if (final_ops.empty())
            {
                /* avoid creating empty regop blocks */
                result.final_nodes_.emplace(new_state, no_transition_regops);
            }
            else
            {
                result.final_nodes_.emplace(new_state, result.regops_.size());
                result.regops_.emplace_back(std::move(final_ops));
            }
        }
        
        return new_state;
    }

    template<typename CharT>
    constexpr auto factory<CharT>::multistep_v1(std::size_t state) const -> multistep_closures_t<char_type>
    {
        multistep_closures_t<char_type> multi_closures;

        auto configs{ state_info_.at(state).config };
        const auto& p{ state_info_.at(state).precedence };

        std::ranges::sort(configs, [&p](std::size_t lhs, std::size_t rhs){ return p(lhs) < p(rhs); }, &configuration::tnfa_state);

        using elem_t = std::pair<n_tr<char_type>, std::reference_wrapper<const configuration>>;
        std::vector<elem_t> transitions;

        for (const auto& cfg : configs)
        {
            transitions.append_range(tnfa_ptr_->get_node(cfg.tnfa_state).tr
                                     | std::views::filter([](const auto& t) { return std::holds_alternative<n_tr<char_type>>(t); })
                                     | std::views::transform([&cfg](const auto& t) -> elem_t { return { std::get<n_tr<char_type>>(t), std::cref(cfg) }; }));
        }

        std::vector<std::pair<char_type, char_type>> symbol_pairs;

        for (const auto& [tr, _] : transitions)
            symbol_pairs.emplace_back(tr.lower, tr.upper);

        partition_v1(symbol_pairs);

        for (auto [lower, upper] : symbol_pairs)
        {
            multi_closures.emplace_back(lower, upper);

            for (const auto& [t, cfg] : transitions)
            {
                if (t.lower <= lower and upper <= t.upper)
                {
                    multi_closures.back().data.emplace_back(t.next, cfg.get().registers, cfg.get().tag_seq);
                }
            }   
        }

        return multi_closures;
    }

    template<typename CharT>
    constexpr auto factory<CharT>::multistep_v2(std::size_t state) const -> multistep_closures_t<char_type>
    {
        // TODO: benchmark against multistep v1

        multistep_closures_t<char_type> multi_closure;

        // TODO: make comparisons constant time instead of linear w.r.t. size of closure contents
        auto configs{ state_info_.at(state).config };
        const auto& p{ state_info_.at(state).precedence };

        std::ranges::sort(configs, [&p](std::size_t lhs, std::size_t rhs){ return p(lhs) < p(rhs); }, &configuration::tnfa_state);

        using elem_t = std::pair<n_tr<char_type>, std::reference_wrapper<const configuration>>;
        std::vector<elem_t> transitions;

        for (const auto& cfg : configs)
        {
            multi_closure.append_range(tnfa_ptr_->get_node(cfg.tnfa_state).tr
                                       | std::views::filter([](const auto& t) { return std::holds_alternative<n_tr<char_type>>(t); })
                                       | std::views::transform([&cfg](const auto& t) -> multistep_closure<char_type> {
                                            n_tr<char_type> tr{ std::get<n_tr<char_type>>(t) };
                                            multistep_closure<char_type> ret{ tr.lower, tr.upper };
                                            ret.data.emplace_back(tr.next, cfg.registers, cfg.tag_seq);
                                            return ret;
                                        }));
        }

        partition_v2(multi_closure);

        return multi_closure;
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
        return h | std::views::filter([tag](tag_t i) { return /* std::abs(i) == tag */ (i == -tag) or (i == tag); }) // TODO: replace implementation when supported
                 | std::views::transform([](tag_t i) -> bool { return i >= 0; })
                 | std::ranges::to<std::vector>();
    }

    template<typename CharT>
    constexpr bool factory<CharT>::mappable(const node_info& state, std::size_t mapped_state, regops_t& o, const reg_t regcount) const
    {
        const auto& mapped_state_info{ state_info_.at(mapped_state) };

        /* different precedences also imply differing sets of states (for now at least);
         * we ensure tnfa states have the same tag sequences later                       */
        if (state.precedence != mapped_state_info.precedence)
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
                    auto h{ history(ce1.tag_seq, t) };

                    if (h.empty())
                    {
                        const reg_t i{ ce1.registers.at(t - 1) };
                        const reg_t j{ ce2.registers.at(t - 1) };

                        if (not map.contains(i) and not rmap.contains(j))
                            map[i] = j, rmap[j] = i;
                        else if (not (map.contains(i) and map.at(i) == j) or not (rmap.contains(j) and rmap.at(j) == i))
                            return false;
                    }
                }
            }
        }

        regops_t o_copy{ o };

        for (auto it{ o_copy.begin() }; it != o_copy.end();)
        {
            const auto i{ it->dst };

            if (not map.contains(i))
            {
                /* skip mapping registers with history */
                it = o_copy.erase(it);
                continue;
            }

            it->dst = map.at(i);
            map.erase(i);
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
        for (const auto [state, fop_index] : result.final_nodes_)
        {
            /* we assume all final states can be a fallback state, which is an overapproximation */

            const auto& final_ops{ result.final_nodes_.at(fop_index) };

            /* determine clobbered registers */

            // maybe switch to sets?
            std::vector<bool> added(result.nodes_.size(), false);
            std::vector<bool> clobbered(result.register_count_, false);

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

                    for (const auto& op : result.get_node(tr.op_index))
                        clobbered.at(op.dst) = true;

                    ++i;
                    if (not added.at(tr.next))
                    {
                        stack.push_back(tr.next);
                        added.at(tr.next) = true;
                    }
                }
            }

             /* insert fallback regops */

            regops_t o;

            for (const auto& f : final_ops)
            {
                if (auto* cpy{ std::get_if<regop::copy>(&f.op) }; cpy != nullptr and clobbered.at(cpy->src))
                    backup_regops(result, state, f.dst, cpy->src);
                else
                    o.emplace_back(f);
            }
            
            if (o.empty())
            {
                /* avoid creating empty regop blocks */
                result.fallback_nodes_.emplace(state, no_transition_regops);
            }
            else
            {
                result.fallback_nodes_.emplace(state, result.regops_.size());
                result.regops_.emplace_back(std::move(o));
            }
        }
    }

    template<typename CharT>
    constexpr void factory<CharT>::backup_regops(tdfa_t& result, std::size_t state, reg_t reg_dst, reg_t reg_src)
    {
        for (auto& tr : result.nodes_.at(state).tr)
        {
            /* not final node => non accepting path exists from tr.next (end of inputs can be anywhere) */
            if (not result.final_nodes.contains(tr.next))
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
    constexpr factory<CharT>::factory(const tnfa_t& input, tdfa_t& result, const std::size_t tag_count , bool is_search) :
        tnfa_ptr_{ &input }, tag_count_{ std::saturate_cast<reg_t>(tag_count) }
    {
        result.register_count_ = tag_count_ * 2;
        reg_vec initial_reg(tag_count_);
        std::ranges::iota(initial_reg, 0);

        result.final_registers_.resize(tag_count_);
        std::ranges::iota(result.final_registers_, tag_count_);

        closure_t c;
        c.emplace_back(is_search ? tnfa_t::substr_start : tnfa_t::match_start, std::move(initial_reg));
        c = e_closure(std::move(c));
        add_initial_state(result, c, precedence_t{ c });

        for (std::size_t state{ 0 }; state < result.nodes_.size(); ++state)
        {
            tag_op_map map;

            for (auto& [pair, c] : multistep_v2(state) /* multistep_v1(state) */)
            {
                c = e_closure(std::move(c));
                auto o{ transition_regops(c, result.register_count_, map) };
                const auto s{ add_state(result, c, precedence_t{ c }, o) };

                /* Add transition to tdfa */
                const auto [lower, upper]{ pair };
                if (o.empty())
                {
                    /* avoid creating empty regop blocks */
                    result.nodes_.at(state).tr.emplace_back(s, no_transition_regops, lower, upper);
                }   
                else
                {
                    result.nodes_.at(state).tr.emplace_back(s, result.regops_.size(), lower, upper);
                    result.regops_.emplace_back(std::move(o));
                }
            }
        }
    }
}

namespace rx::detail
{
    template<typename CharT>
    constexpr tagged_dfa<CharT>::tagged_dfa(const tnfa_t& tnfa) : 
        capture_info_{ tnfa.get_capture_info() }, tag_count_{ tnfa.tag_count() }
    {
        tdfa::factory<char_type>{ tnfa, *this, tag_count_, false };
    }

    template<typename CharT>
    constexpr void tagged_dfa<CharT>::optimise_registers() 
    {
        std::invoke(tdfa::opt<char_type>{}, *this);
    }
}