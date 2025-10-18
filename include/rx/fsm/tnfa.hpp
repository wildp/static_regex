#pragma once

#include <variant>
#include <vector>

#include <rx/ast/tree.hpp>
#include <rx/fsm/flags.hpp>


namespace rx::detail
{
    /* tnfa transitions */

    template<typename CharT>
    struct n_tr
    {
        std::size_t next;
        CharT lower;
        CharT upper;
    };

    struct epsilon_tr
    {
        std::size_t next;
        int priority;
        int tag; /* use tag = 0 when tag is absent, tag > 0 for regular tags, and tag < 0 for negative tags */
    };

    struct assert_tr
    {
        std::size_t next;
        assert_type assertion;
    };

    /* tnfa nodes */

    template<typename CharT>
    struct tnfa_node
    {
        using transition_t = std::variant<n_tr<CharT>, epsilon_tr, assert_tr>;

        std::vector<transition_t> tr;
    };

    /* tnfa class */

    template<typename CharT>
    class tagged_nfa
    {
    public:
        explicit constexpr tagged_nfa(const expr_tree<CharT>& ast, fsm_flags flags);

    private:
        using ast_t = expr_tree<CharT>;

        template<in_variant<typename ast_t::type> T>
        static constexpr std::size_t ast_index{ index_of_impl<typename ast_t::type, T>::value };

        constexpr std::size_t node_create();
        constexpr void epsilon(std::size_t q0, std::size_t qf, int priority = 0, int tag = 0);
        constexpr void make_ntags(std::size_t q0, std::size_t qf, const std::vector<int>& ntags);
        constexpr void make_wildcard(std::size_t q0, std::size_t qf);

        struct stack_elem
        {
            std::size_t q0, qf, idx;
        };
    
    public:
        [[nodiscard]] constexpr auto get_flags() const noexcept { return flags_; }
        [[nodiscard]] constexpr const tnfa_node<CharT>& get_node(std::size_t i) const { return nodes_.at(i); }
        [[nodiscard]] constexpr std::size_t node_count() const noexcept { return nodes_.size(); }
        [[nodiscard]] constexpr std::size_t tag_count() const noexcept { return tag_count_; }
        [[nodiscard]] constexpr const capture_info& get_capture_info() const noexcept { return capture_info_; }
        [[nodiscard]] constexpr bool node_is_final(std::size_t i) const { return i == final_node_ or i == strict_final_node_; }
        [[nodiscard]] constexpr std::size_t start_node() const noexcept { return start_node_; }

    private:
        std::vector<tnfa_node<CharT>> nodes_{ 2 };
        capture_info capture_info_;
        std::size_t tag_count_;
        std::size_t start_node_{ 0 };
        std::size_t final_node_{ 1 };
        std::optional<std::size_t> continue_node_;
        std::optional<std::size_t> strict_final_node_;
        fsm_flags flags_;
    };
}

#include <rx/fsm/tnfa.tpp>