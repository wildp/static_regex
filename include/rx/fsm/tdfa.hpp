#pragma once

#define RX_TDFA_ENABLE_DUMPER 1

#include <cstddef>
#include <cstdlib>
#include <compare>
#include <flat_map>
#include <vector>
#include <variant>

#include <rx/etc/partition.hpp>
#include <rx/fsm/tnfa.hpp>


// TODO: switch from vectors and std::find to std::unordered_map when supported?

namespace rx::detail
{
    template<typename CharT>
    class factory;
}

namespace rx::detail
{
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

    using final_nodes_t = std::flat_map<std::size_t, regops_t>;

    template<typename CharT>
    struct transition
    {
        std::size_t next{ 0 };
        CharT lower;
        CharT upper;

        regops_t ops;
    };

    template<typename CharT>
    struct tdfa_node
    {
        std::vector<transition<CharT>> tr;
    };

    template<typename CharT>
    class tagged_dfa
    {
    public:
        using tnfa_t = tagged_nfa<CharT>;

        explicit constexpr tagged_dfa(const tnfa_t& tnfa);

        friend class factory<CharT>;

#if RX_TDFA_ENABLE_DUMPER
        void dump() const;
#endif // RX_TDFA_ENABLE_DUMPER

    protected:
        static constexpr std::size_t match_start{ 0 };

        [[nodiscard]] constexpr const tdfa_node<CharT>& get_node(std::size_t i) const { return nodes_.at(i); }
        [[nodiscard]] constexpr const final_nodes_t& final_nodes() const { return final_nodes_; }
        [[nodiscard]] constexpr size_t reg_count() const { return register_count_; }
        [[nodiscard]] constexpr size_t tag_count() const { return tag_count_; }

    private:
        using data_t = std::vector<tdfa_node<CharT>>;

        data_t          nodes_;
        final_nodes_t   final_nodes_;
        reg_t           register_count_{ 0 };

        std::size_t     tag_count_; // TODO: replace with register to tag mapping information
    };
}

#include <rx/fsm/tdfa.tpp>