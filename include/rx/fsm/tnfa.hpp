#pragma once

#include <variant>
#include <vector>

#include <rx/ast/tree.hpp>
#include <rx/fsm/flags.hpp>


namespace rx::detail
{
    template<typename CharT>
    class tagged_dfa;

    // template<typename CharT>
    // class multipass_tagged_dfa;

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

        friend class tagged_dfa<CharT>;
        // friend class multipass_tagged_dfa<CharT>;

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
        static constexpr std::size_t match_start{ 0 };
        static constexpr std::size_t substr_start{ 1 };
        static constexpr std::size_t end{ 2 };

        [[nodiscard]] constexpr const tnfa_node<CharT>& get_node(std::size_t i) const { return nodes_.at(i); }
        [[nodiscard]] constexpr std::size_t node_count() const { return nodes_.size(); }
        [[nodiscard]] constexpr std::size_t tag_count() const { return tag_count_; }
        [[nodiscard]] constexpr const capture_info& get_capture_info() const { return capture_info_; }

    private:
        std::vector<tnfa_node<CharT>> nodes_{ 3 };
        capture_info capture_info_;
        std::size_t tag_count_;
        fsm_flags flags_;
    };
}

#include <rx/fsm/tnfa.tpp>