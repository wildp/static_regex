#pragma once

#include <rx/fsm/tdfa.hpp>

#include <algorithm>
#include <functional>
#include <iterator>
#include <ranges>
#include <utility>
#include <variant>


// TODO: improve implementation of tdfa optimisation to reduce number of steps taken by constant evaluator!!!

namespace rx::detail::tdfa
{
    /* regops sorting */

    inline constexpr bool toposort_regops(const regops_t::iterator beg, const regops_t::iterator end, const reg_t regcount)
    {
        std::vector<std::size_t> indeg(regcount, 0);

        for (auto it{ beg }; it != end; ++it)
        {
            if (const auto* copy{ std::get_if<regop::copy>(&it->op) }; copy != nullptr)
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
                    if (const auto* copy{ std::get_if<regop::copy>(&it->op) }; copy != nullptr)
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
                    if (const auto* copy{ std::get_if<regop::copy>(&oc.op) }; copy != nullptr and copy->src != oc.dst)
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


    /* tdfa normalisation */

    namespace 
    {
        constexpr regops_t::iterator dedup_regops(regops_t& o, const regops_t::iterator beg, const regops_t::iterator end)
        {
            regops_t::iterator new_end{ end };

            for (auto it{ beg }; it != new_end; ++it)
                new_end = std::remove(it + 1, new_end, *it);

            return o.erase(new_end, end);
        }

        constexpr void normalise_regops(regops_t& o, const reg_t regcount)
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
}

namespace rx::detail
{
    template<typename CharT>
    constexpr void tagged_dfa<CharT>::optimise_registers() 
    {
        std::invoke(tdfa::opt<char_type>{}, *this);
    }
}