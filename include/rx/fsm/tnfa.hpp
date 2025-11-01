#pragma once

#include <variant>
#include <vector>

#include <rx/ast/tree.hpp>
#include <rx/fsm/flags.hpp>


namespace rx::detail::tnfa
{
    /* tnfa transitions */

    template<typename CharT>
    struct transition
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

    struct simple_assert_tr
    {
        enum class type : bool { sof, eof };

        std::size_t next;
        type mode;
    };

    template<typename CharT>
    struct advanced_assert_tr
    {
        enum class type : bool { lookahead, lookbehind };
        using char_set_t = char_class_impl<CharT>; /* use single codepoint only */

        std::size_t next;
        char_set_t valid;
        type mode;
    };

    struct lookahead_assert_tr
    {
        std::size_t next;
        std::size_t lookahead_start;
        std::size_t lookahead_end;
    };


    /* tnfa nodes */

    template<typename CharT>
    struct node
    {
        // TODO: refactor transition to hold next outside of data;

        using transition_t = std::variant<transition<CharT>, epsilon_tr, simple_assert_tr, advanced_assert_tr<CharT>, lookahead_assert_tr>;

        std::vector<transition_t> tr;
    };
}


    /* tnfa class */

namespace rx::detail
{
    template<typename CharT>
    class tagged_nfa
    {
    public:
        explicit constexpr tagged_nfa(const expr_tree<CharT>& ast, fsm_flags flags);

    private:
        using ast_t = expr_tree<CharT>;
        using satr_t = tnfa::simple_assert_tr;
        using aatr_t = tnfa::advanced_assert_tr<CharT>;
        using latr_t = tnfa::lookahead_assert_tr;

        template<in_variant<typename ast_t::type> T>
        static constexpr std::size_t ast_index{ index_of_impl<typename ast_t::type, T>::value };

        constexpr std::size_t node_create();
        constexpr void make_epsilon(std::size_t q0, std::size_t qf, int priority = 0, int tag = 0);
        constexpr void make_transition(std::size_t q0, std::size_t qf, CharT lower, CharT upper);
        constexpr void make_transition(std::size_t q0, std::size_t qf, CharT c);
        constexpr void make_assert(std::size_t q0, std::size_t qf, satr_t::type ty);
        constexpr void make_assert(std::size_t q0, std::size_t qf, aatr_t::type ty, aatr_t::char_set_t cs);
        constexpr void make_assert(std::size_t q0, std::size_t qf, std::size_t p0, std::size_t pf);
        constexpr void make_ntags(std::size_t q0, std::size_t qf, const std::vector<int>& ntags);
        constexpr void make_wildcard(std::size_t q0, std::size_t qf);
        constexpr void thompson(const expr_tree<CharT>& ast);

        [[nodiscard]] constexpr std::vector<std::size_t> strict_e_closure(std::size_t q) const;
        [[nodiscard]] constexpr std::vector<std::size_t> relaxed_e_closure(std::size_t q) const;
        constexpr void remove_dead_states();
        constexpr void rewrite_sof_anchors();
        constexpr void rewrite_eof_anchors();
        constexpr void rewrite_sc_lookaround();
        constexpr void rewrite_assertions();
    
    public:
        [[nodiscard]] constexpr auto get_flags() const noexcept { return flags_; }
        [[nodiscard]] constexpr const tnfa::node<CharT>& get_node(std::size_t i) const { return nodes_.at(i); }
        [[nodiscard]] constexpr std::size_t node_count() const noexcept { return nodes_.size(); }
        [[nodiscard]] constexpr std::size_t tag_count() const noexcept { return tag_count_; }
        [[nodiscard]] constexpr const capture_info& get_capture_info() const noexcept { return capture_info_; }
        [[nodiscard]] constexpr bool node_is_final(std::size_t i) const { return i == final_node_ or i == strict_final_node_; }
        [[nodiscard]] constexpr bool node_is_fallback(std::size_t i) const { return flags_.enable_fallback and i == final_node_; } // temporary implementation
        [[nodiscard]] constexpr std::size_t start_node() const noexcept { return start_node_; }

    private:
        std::vector<tnfa::node<CharT>> nodes_{ 2 };
        capture_info capture_info_;
        std::size_t tag_count_;
        std::size_t start_node_{ 0 };
        std::size_t final_node_{ 1 };
        std::vector<std::size_t> continue_nodes_;
        std::optional<std::size_t> strict_final_node_;
        fsm_flags flags_;

        bool has_eof_anchor_ : 1 { false };
        bool has_sof_anchor_ : 1 { false };
        bool has_lookahead_1_ : 1 { false };
        bool has_lookbehind_1_ : 1 { false };
        bool has_lookahead_n_ : 1 { false };
    };
}

#include <rx/fsm/tnfa.tpp>