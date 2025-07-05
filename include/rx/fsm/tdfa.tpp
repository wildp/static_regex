#pragma once

#include <rx/fsm/tdfa.hpp>

#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <ranges>
#include <utility>

#if RX_TDFA_ENABLE_DUMPER
#include <print>
#include <format>
#endif // RX_TDFA_ENABLE_DUMPER


namespace rx::detail
{
    /* tdfa optimisations */

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
                    while (local_end != o.end() and std::holds_alternative<regop::set>(it->op))
                        ++local_end;

                    local_end = dedup_regops(o, it, local_end);
                    std::ranges::sort(it, local_end, std::ranges::less{}, &regop::dst);
                    it = local_end;
                }
                else if (std::holds_alternative<regop::copy>(it->op))
                {
                    while (local_end != o.end() and std::holds_alternative<regop::copy>(it->op))
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
}


namespace rx::detail
{
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
    
    template<typename CharT>
    struct multistep_closure
    {
        CharT       lower;
        CharT       upper;
        closure_t   closure;
    };

    template<typename CharT>
    using multistep_closure_t = std::vector<multistep_closure<CharT>>;

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
        using tnfa_t = tagged_nfa<CharT>;
        using tdfa_t = tagged_dfa<CharT>;

        constexpr explicit factory(const tnfa_t& input, tdfa_t& result, std::size_t tag_count, bool is_search = false);

    private:
        [[nodiscard]] constexpr closure_t e_closure(closure_t&& c) const;
        [[nodiscard]] constexpr std::size_t add_state(tdfa_t& result, const closure_t& c, const precedence_t& p, regops_t& o);
        [[nodiscard]] constexpr multistep_closure_t<CharT> multistep(std::size_t state) const;
        [[nodiscard]] constexpr regops_t transition_regops(closure_t& c, reg_t& regcount, tag_op_map& map) const;
        [[nodiscard]] constexpr regops_t final_regops(const reg_vec& r, const tag_sequence_t& tag_seq) const;
        [[nodiscard]] constexpr regop::op_t regop_rhs(const reg_vec& r, std::vector<bool>& hist, tag_t tag) const;
        [[nodiscard]] constexpr std::vector<bool> history(const tag_sequence_t& hist, tag_t tag) const;
        [[nodiscard]] constexpr bool mappable(const node_info& state, std::size_t mapped_state, regops_t& o, reg_t regcount) const;

        constexpr void add_initial_state(tdfa_t& result, const closure_t& c, const precedence_t& p);

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

        const auto& current_cfg = state_info_.back().config;
        const auto it{ std::ranges::find(current_cfg, tnfa_t::end, &configuration::tnfa_state) };
        if (it != std::ranges::end(current_cfg))
            result.final_nodes_.emplace(0, final_regops(it->registers, it->tag_seq));
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

        const auto& current_cfg = state_info_.back().config;
        const auto it{ std::ranges::find(current_cfg, tnfa_t::end, &configuration::tnfa_state) };
        if (it != std::ranges::end(current_cfg))
            result.final_nodes_.emplace(new_state, final_regops(it->registers, it->tag_seq));
        
        return new_state;
    }

    template<typename CharT>
    constexpr auto factory<CharT>::multistep(std::size_t state) const -> multistep_closure_t<CharT>
    {
        multistep_closure_t<CharT> multi_closure;

        auto configs{ state_info_.at(state).config };
        const auto& p{ state_info_.at(state).precedence };

        std::ranges::sort(configs, [&p](std::size_t lhs, std::size_t rhs){ return p(lhs) < p(rhs); }, &configuration::tnfa_state);

        using elem_t = std::pair<n_tr<CharT>, std::reference_wrapper<const configuration>>;
        std::vector<elem_t> transitions;

        // TODO: reimplement to only use 1 pass (in a new partition() function)

        for (const auto& cfg : configs)
        {
            transitions.append_range(tnfa_ptr_->get_node(cfg.tnfa_state).tr
                                     | std::views::filter([](const auto& t) { return std::holds_alternative<n_tr<CharT>>(t); })
                                     | std::views::transform([&cfg](const auto& t) -> elem_t { return { std::get<n_tr<CharT>>(t), std::cref(cfg) }; }));
        }

        std::vector<std::pair<CharT, CharT>> symbol_pairs;

        for (const auto& [tr, _] : transitions)
            symbol_pairs.emplace_back(tr.lower, tr.upper);

        partition(symbol_pairs);

        for (auto [lower, upper] : symbol_pairs)
        {
            multi_closure.emplace_back(lower, upper, closure_t{});

            for (const auto& [t, cfg] : transitions)
            {
                if (t.lower <= lower and upper <= t.upper)
                {
                    multi_closure.back().closure.emplace_back(t.next, cfg.get().registers, cfg.get().tag_seq, tag_sequence_t{});
                }
            }   
        }

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
                    regop::op_t op_rhs{ regop_rhs(ce.registers, h, t) };

                    auto [value_refs, existing]{ map.lookup(t, op_rhs) };

                    if (not existing)
                    {
                        auto i{ regcount++ };
                        value_refs = i;
                        regops.emplace_back(i, /* std::move( */ op_rhs /* ) */);
                    }

                    ce.registers.at(t - 1) = value_refs;
                }
            }
        }

        return regops;
    }

    template<typename CharT>
    constexpr auto factory<CharT>::final_regops(const reg_vec& r, const tag_sequence_t& tag_seq) const -> regops_t 
    {
        regops_t regops;

        for (tag_t t{ 1 }; std::cmp_less_equal(t, tag_count_); ++t)
        {
            auto h{ history(tag_seq, t) };

            if (not h.empty()) /* assign final registers */
                regops.emplace_back(tag_count_ + t - 1, regop_rhs(r, h, t));
            else
                regops.emplace_back(tag_count_ + t - 1, regop::copy{ .src = r.at(t - 1) }); /* copy into final register */
        }

        return regops;
    }

    template<typename CharT>
    constexpr auto factory<CharT>::regop_rhs(const reg_vec& /* r */, std::vector<bool>& hist, tag_t /* tag */) const -> regop::op_t
    {
        // if (/* t is multi-valued tag */)
            // return regop::append{ .src = r.at(tag), .hist = hist };
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
        constexpr auto print_regop = [](const regop& op, std::string_view indent = "") {
            if (auto* set{ std::get_if<regop::set>(&op.op) }; set != nullptr)
                std::println("{}r{} <- {}", indent, op.dst, (set->val) ? 'p' : 'n');
            else if (auto* copy{ std::get_if<regop::copy>(&op.op) }; copy != nullptr)
                std::println("{}r{} <- r{}", indent, op.dst, copy->src);
            else
                std::println("{}ERROR", indent);
        };


        const auto& mapped_state_info{ state_info_.at(mapped_state) };

        /* different precedences also imply differing sets of states (for now at least);
         * we ensure tnfa states have the same tag sequences later                       */
        if (state.precedence != mapped_state_info.precedence)
            return false;

        
        register_map_t map, rmap;

        if not consteval { std::println("\nAttempting to map to {}:", mapped_state); }
        
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

                        if ((not map.contains(i) and not rmap.contains(j)))
                            map[i] = j, rmap[j] = i;
                        else if (not (map.contains(i) and map.at(i) == j) or not (rmap.contains(j) and rmap.at(j) == i))
                            return false;
                    }
                    else
                    {
                        const reg_t i{ ce1.registers.at(t - 1) };
                        const reg_t j{ ce2.registers.at(t - 1) };
                        if not consteval { std::println("Hist: {}<=>{} = {}", i, j, h); }
                    }
                }
            }
        }

        regops_t o_copy{ o };

        if not consteval { std::println("Current Regops:"); for (const auto& op: o_copy) print_regop(op, "\t"); std::println("Map: {}", map); }

        for (auto it{ o_copy.begin() }; it != o_copy.end();)
        {
            const auto i{ it->dst };

            if (not map.contains(i))
            {
                if not consteval { std::println("{} not in Map", i); }

                // /* skip mapping registers with history */
                it = o_copy.erase(it);
                // ++it;
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

        if not consteval { std::println("New Regops:"); for (const auto& op: o_new) print_regop(op, "\t"); }

        const bool success{ toposort_regops(o_new.begin(), o_new.end(), regcount) };

        if not consteval { std::println("Sorted Regops:"); for (const auto& op: o_new) print_regop(op, "\t"); }

        if (success)
        {
            o = std::move(o_new);
            if not consteval { std::println("Successfully mapped to {}:", mapped_state); }
        }

        return success;
    }

    template<typename CharT>
    constexpr factory<CharT>::factory(const tnfa_t& input, tdfa_t& result, std::size_t tag_count , bool is_search) :
        tnfa_ptr_{ &input }, tag_count_{ std::saturate_cast<reg_t>(tag_count) }
    {
        result.register_count_ = tag_count_ * 2;
        reg_vec initial_reg(tag_count_);
        std::ranges::iota(initial_reg, 0);

        closure_t c{{ (is_search ? tnfa_t::substr_start : tnfa_t::match_start), std::move(initial_reg), {}, {} }};
        c = e_closure(std::move(c));
        add_initial_state(result, c, precedence_t{ c });

        for (std::size_t state{ 0 }; state < result.nodes_.size(); ++state)
        {
            tag_op_map map;

            for (auto& [lower, upper, c] : multistep(state))
            {
                c = e_closure(std::move(c));
                auto o{ transition_regops(c, result.register_count_, map) };
                const auto s{ add_state(result, c, precedence_t{ c }, o) };

                /* Add transition to tdfa */
                result.nodes_.at(state).tr.emplace_back(s, lower, upper, std::move(o));
            }
        }
    }
}

namespace rx::detail
{
    template<typename CharT>
    constexpr tagged_dfa<CharT>::tagged_dfa(const tnfa_t& tnfa) : 
        tag_count_{ tnfa.tag_count() }
    {
        factory<CharT>{ tnfa, *this, tag_count_, false };
    }

    
#if RX_TDFA_ENABLE_DUMPER
    template<typename CharT>
    void tagged_dfa<CharT>::dump() const
    {
        constexpr auto print_regop = [](const regop& op, std::string_view indent = "") {
            if (auto* set{ std::get_if<regop::set>(&op.op) }; set != nullptr)
                std::println("{}r{} <- {}", indent, op.dst, (set->val) ? 'p' : 'n');
            else if (auto* copy{ std::get_if<regop::copy>(&op.op) }; copy != nullptr)
                std::println("{}r{} <- r{}", indent, op.dst, copy->src);
            else
                std::println("{}ERROR", indent);
        };

        for (std::size_t i{ 0 }; i < this->nodes_.size(); ++i)
        {
            if (this->final_nodes_.contains(i))
                std::println("Node {}: (ACCEPTING)", i);
            else
                std::println("Node {}:", i);

            for (const auto& tr : this->nodes_.at(i).tr)
            {
                if (tr.lower == tr.upper)
                    std::println("\t\'{}\' -> Node {}:", tr.lower, tr.next);
                else
                    std::println("\t[\'{}\',\'{}\'] -> Node {}:", tr.lower, tr.upper, tr.next);

                for (const regop& op : tr.ops)
                    print_regop(op, "\t\t");
            }

            if (this->final_nodes_.contains(i))
            {
                std::println("\t'' -> ACCEPT:");

                for (const auto& op : this->final_nodes_.at(i))
                    print_regop(op, "\t\t");
            }
        }
    }
#endif // RX_TDFA_ENABLE_DUMPER
}