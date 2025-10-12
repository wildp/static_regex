#pragma once

#include <cstddef>
#include <cstdlib>
#include <flat_map>
#include <vector>
#include <variant>

#include <rx/etc/captures.hpp>
#include <rx/fsm/flags.hpp>
#include <rx/fsm/tnfa.hpp>


namespace rx::detail::tdfa
{
    template<typename CharT>
    class factory;

    template<typename CharT>
    class opt;

        using reg_t = std::uint_least32_t;

    struct regop
    {
        struct set
        {
            bool val; /* use false for nil and true for current */

            friend constexpr bool operator==(set, set) noexcept = default;
        };

        struct copy
        {
            reg_t src;

            friend constexpr bool operator==(copy, copy) noexcept = default;
        };

        using op_t = std::variant<set, copy>;

        // TODO: optimise layout as necessary to be as compact as possible

        op_t op;
        reg_t dst;

        constexpr regop(reg_t destination, op_t operation) :
            op{ operation }, dst{ destination } {}

        friend constexpr bool operator==(regop lhs, regop rhs) = default;
    };

    using regops_t = std::vector<regop>;
    inline constexpr regops_t empty_regops{};

    using final_nodes_t = std::flat_map<std::size_t, std::size_t>;
    using final_regs_t = std::vector<reg_t>;

    inline constexpr std::size_t no_transition_regops{ std::numeric_limits<std::size_t>::max() };

    constexpr bool toposort_regops(regops_t::iterator beg, regops_t::iterator end, reg_t regcount);

    template<typename CharT>
    struct transition
    {
        std::size_t next;
        std::size_t op_index; /* use intmax for no ops */
        CharT lower;
        CharT upper;
    };

    template<typename CharT>
    struct node
    {
        std::vector<transition<CharT>> tr;
    };
}

namespace rx::detail
{
    template<typename CharT>
    class tagged_dfa
    {
    public:
        using char_type = CharT;
        using tnfa_t = tagged_nfa<char_type>;

        explicit constexpr tagged_dfa(const tnfa_t& tnfa);
        constexpr void optimise_registers();
        constexpr void to_structural_type();

        friend class tdfa::factory<char_type>;
        friend class tdfa::opt<char_type>;

        template<typename CharT2>
        friend struct tdfa_info;

    protected:
        static constexpr std::size_t match_start{ 0 };

        [[nodiscard]] constexpr const tdfa::node<CharT>& get_node(std::size_t i) const { return nodes_.at(i); }
        [[nodiscard]] constexpr const tdfa::regops_t& get_regops(std::size_t i) const { if (i == tdfa::no_transition_regops) return tdfa::empty_regops; else return regops_.at(i); }
        [[nodiscard]] constexpr const tdfa::final_nodes_t& final_nodes() const { return final_nodes_; }
        [[nodiscard]] constexpr const tdfa::final_nodes_t& fallback_nodes() const { return fallback_nodes_; }
        [[nodiscard]] constexpr const tdfa::final_regs_t& final_registers() const { return final_registers_; }
        [[nodiscard]] constexpr std::size_t node_count() const { return nodes_.size(); }
        [[nodiscard]] constexpr std::size_t reg_count() const { return register_count_; }
        [[nodiscard]] constexpr std::size_t tag_count() const { return tag_count_; }
        [[nodiscard]] constexpr const capture_info& get_capture_info() const { return capture_info_; }

    private:
        using data_t = std::vector<tdfa::node<char_type>>;
        using regop_data_t = std::vector<tdfa::regops_t>;

        data_t              nodes_;
        tdfa::final_nodes_t final_nodes_;
        tdfa::final_nodes_t fallback_nodes_;
        tdfa::final_regs_t  final_registers_;
        regop_data_t        regops_;
        capture_info        capture_info_;
        std::size_t         tag_count_;
        tdfa::reg_t         register_count_{ 0 };
        fsm_flags           flags_;
    };
}

#include <rx/fsm/tdfa.tpp>
#include <rx/fsm/regoptimise.tpp>
#include <rx/fsm/minimise.tpp>