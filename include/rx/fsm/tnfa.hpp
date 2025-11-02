#pragma once

#include <cstdint>
#include <variant>
#include <vector>

#include <rx/ast/tree.hpp>
#include <rx/fsm/flags.hpp>


namespace rx::detail::tnfa
{
    using state_t = std::size_t;

    template<typename CharT>
    using char_set = char_class_impl<CharT>;

    enum class assert_category : std::int8_t
    {
        eof, sof, lookahead, lookbehind
    };

    template<assert_category Value>
    struct ac {};

    /* tnfa transitions */

    template<typename CharT>
    struct normal_tr
    {
        CharT lower;
        CharT upper;
    };

    struct epsilon_tr
    {
        int priority;
        int tag; /* use tag = 0 when tag is absent, tag > 0 for regular tags, and tag < 0 for negative tags */
    };

    struct sof_anchor_tr {};
    struct eof_anchor_tr {};

    template<typename CharT>
    struct lookahead_1_tr
    {
        char_set<CharT> valid; /* use single codepoint only */
    };

    template<typename CharT>
    struct lookbehind_1_tr
    {
        char_set<CharT> valid; /* use single codepoint only */
    };

    struct lookahead_assert_tr
    {
        state_t lookahead_start, lookahead_end;
    };

    template<typename CharT>
    struct transition
    {
        using transition_type = std::variant<normal_tr<CharT>, epsilon_tr, eof_anchor_tr, sof_anchor_tr,
                                             lookahead_1_tr<CharT>, lookbehind_1_tr<CharT>, lookahead_assert_tr>;

        state_t         next;
        transition_type type; 

        template<typename... Args>
        constexpr transition(state_t qf, Args&&... args) :
            next{ qf }, type{ (std::forward<Args>(args))... } {}
    };


    /* tnfa nodes */

    template<typename CharT>
    struct node
    {
        std::vector<transition<CharT>> tr;
    };

    struct final_node_info
    {
        std::size_t continuation_index;
        bool        is_fallback;
    };

    struct continue_info
    {
        using sub_e_closure = std::optional<std::vector<state_t>>;

        state_t         value;
        sub_e_closure   sub_ec;
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

        using state_t = tnfa::state_t;

    private:
        using ast_t = expr_tree<CharT>;
        using acat_t = tnfa::assert_category;
        using transition_info = tnfa::transition<CharT>::transition_type;

        template<in_variant<typename ast_t::type> T>
        static constexpr state_t ast_index{ index_of_impl<typename ast_t::type, T>::value };

        constexpr state_t node_create();
        constexpr void make_epsilon(state_t q0, state_t qf, int priority = 0, int tag = 0);
        constexpr void make_transition(state_t q0, state_t qf, CharT lower, CharT upper);
        constexpr void make_transition(state_t q0, state_t qf, CharT c);
        template<acat_t V> constexpr void make_assert(state_t q0, state_t qf, tnfa::ac<V> category);
        template<acat_t V> constexpr void make_assert(state_t q0, state_t qf, tnfa::char_set<CharT> cs, tnfa::ac<V> category);
        constexpr void make_assert(state_t q0, state_t qf, state_t p0, state_t pf);
        constexpr void make_copy(state_t q0, state_t qf, const transition_info& type);
        constexpr void make_ntags(state_t q0, state_t qf, const std::vector<int>& ntags);
        constexpr void make_wildcard(state_t q0, state_t qf);
        constexpr void thompson(const expr_tree<CharT>& ast);

        [[nodiscard]] constexpr std::vector<state_t> strict_e_closure(state_t q) const;
        [[nodiscard]] constexpr std::vector<state_t> relaxed_e_closure(state_t q) const;
        constexpr void remove_dead_states();
        constexpr void rewrite_sof_anchors();
        constexpr void rewrite_eof_anchors();
        constexpr void rewrite_sc_lookaround();
        constexpr void rewrite_assertions();

        static constexpr state_t default_final_node{ 1 };
    
    public:
        [[nodiscard]] constexpr auto get_flags() const noexcept { return flags_; }
        [[nodiscard]] constexpr const tnfa::node<CharT>& get_node(state_t i) const { return nodes_.at(i); }
        [[nodiscard]] constexpr std::size_t node_count() const noexcept { return nodes_.size(); }
        [[nodiscard]] constexpr std::size_t tag_count() const noexcept { return tag_count_; }
        [[nodiscard]] constexpr const capture_info& get_capture_info() const noexcept { return capture_info_; }
        [[nodiscard]] constexpr bool node_is_final(state_t i) const { return final_nodes_.contains(i); }
        [[nodiscard]] constexpr bool node_is_fallback(state_t i) const { const auto it{ final_nodes_.find(i) }; return (it != final_nodes_.end() and it->second.is_fallback); }
        [[nodiscard]] constexpr state_t start_node() const noexcept { return start_node_; }

    private:
        std::vector<tnfa::node<CharT>>                  nodes_{ 2 };
        capture_info                                    capture_info_;
        std::size_t                                     tag_count_;
        state_t                                         start_node_{ 0 };
        std::flat_map<state_t, tnfa::final_node_info>   final_nodes_;
        std::vector<tnfa::continue_info>                cont_info_;

        fsm_flags flags_;

        bool has_eof_anchor_    : 1 { false };
        bool has_sof_anchor_    : 1 { false };
        bool has_lookahead_1_   : 1 { false };
        bool has_lookbehind_1_  : 1 { false };
        bool has_lookahead_n_   : 1 { false };
    };
}

#include <rx/fsm/tnfa.tpp>