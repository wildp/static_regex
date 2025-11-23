#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>
#include <variant>
#include <vector>

#include <rx/ast/tree.hpp>
#include <rx/etc/charset.hpp>
#include <rx/etc/bitcharset.hpp>
#include <rx/fsm/flags.hpp>


namespace rx::detail::tnfa
{
    using state_t = std::size_t;
    using tr_index = std::size_t;

    template<typename CharT>
    using charset_t = std::conditional_t<sizeof(CharT) == 1, bitcharset<CharT>, charset<CharT>>;

    enum class assert_category : std::int8_t
    {
        eof, sof, lookahead, lookbehind
    };

    template<assert_category Value>
    struct ac {};

    enum class ec_mode : std::int8_t
    {
        ec, aec, reach 
    };

    /* tnfa transitions */

    template<typename CharT>
    struct normal_tr
    {
        charset_t<CharT> cs;
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
        charset_t<CharT> cs;
    };

    template<typename CharT>
    struct lookbehind_1_tr
    {
        charset_t<CharT> cs;
    };

    // struct lookahead_tr 
    // {
    //     state_t p0, pf;
    //     bool negate;
    // };

    // struct lookbehind_tr
    // {
    //     state_t p0, pf;
    //     bool negate;
    // };

    template<typename CharT>
    struct transition
    {
        using transition_type = std::variant<std::monostate
                                             , normal_tr<CharT>, epsilon_tr
                                             , eof_anchor_tr, sof_anchor_tr
                                             , lookahead_1_tr<CharT>, lookbehind_1_tr<CharT>
                                             //  , lookahead_tr, lookbehind_tr
                                            >;

        state_t         src, dst;
        transition_type type; 

        template<typename... Args>
        constexpr transition(state_t q0, state_t qf, Args&&... args) :
            src{ q0 }, dst{ qf }, type{ (std::forward<Args>(args))... } {}

        constexpr void unset()
        {
            src = std::numeric_limits<state_t>::max();
            dst = std::numeric_limits<state_t>::max();
            type.template emplace<std::monostate>();
        }
    };


    /* tnfa nodes */

    struct node
    {
        std::vector<tr_index> in_tr;
        std::vector<tr_index> out_tr;

        bool is_final{ false };
        bool is_fallback{ false };          /* must equal false if not is_final */
        std::uint16_t final_offset{ 0 };    /* only meaningful if is_final */
        std::uint16_t continue_at{ 0 };     /* only meaningful if is_final */
    };

    struct continue_info
    {
        using sub_e_closure = std::optional<std::vector<state_t>>;

        state_t         value;
        sub_e_closure   sub_ec;
    };
}

namespace rx::detail
{
    /* tnfa class */

    template<typename CharT>
    class tagged_nfa
    {
    public:
        using char_type = CharT;
        using state_t = tnfa::state_t;
        using tr_index = tnfa::tr_index;

        explicit constexpr tagged_nfa(const expr_tree<char_type>& ast, fsm_flags flags);

        [[nodiscard]] constexpr auto get_flags() const noexcept { return flags_; }
        [[nodiscard]] constexpr const tnfa::node& get_node(state_t i) const { return nodes_.at(i); }
        [[nodiscard]] constexpr const tnfa::transition<char_type>& get_tr(tr_index idx) const { return transitions_.at(idx); }
        [[nodiscard]] constexpr std::size_t node_count() const noexcept { return nodes_.size(); }
        [[nodiscard]] constexpr std::size_t tag_count() const noexcept { return tag_count_; }
        [[nodiscard]] constexpr const capture_info& get_capture_info() const noexcept { return capture_info_; }
        [[nodiscard]] constexpr state_t start_node() const noexcept { return start_node_; }

    private:
        using ast_t = expr_tree<char_type>;
        using acat_t = tnfa::assert_category;
        using transition_info = tnfa::transition<char_type>::transition_type;

        template<in_variant<typename ast_t::type> T>
        static constexpr state_t ast_index{ index_of_impl<typename ast_t::type, T>::value };

        /* fundamental helpers */

        constexpr state_t node_create();
        constexpr state_t node_copy(state_t q);
        
        template<typename... Args>
        constexpr void transition_create(state_t q0, state_t qf, Args&&... args);

        /* transition creation */

        constexpr void make_epsilon(state_t q0, state_t qf, int priority = 0, int tag = 0);
        
        constexpr void make_transition(state_t q0, state_t qf, char_type c);

        template<typename CharSet>
        requires std::convertible_to<std::remove_cvref_t<CharSet>, tnfa::charset_t<char_type>>
        constexpr void make_transition(state_t q0, state_t qf, CharSet&& cs);

        template<acat_t V>
        constexpr void make_assert(state_t q0, state_t qf, tnfa::ac<V> category);

        template<typename CharSet, acat_t V>
        requires std::convertible_to<std::remove_cvref_t<CharSet>, tnfa::charset_t<char_type>>
        constexpr void make_assert(state_t q0, state_t qf, CharSet&& cs, tnfa::ac<V> category);

        // template<acat_t V>
        // constexpr void make_assert(state_t q0, state_t qf, state_t p0, state_t pf, tnfa::ac<V> category);

        constexpr void make_copy(state_t q0, state_t qf, const transition_info& type);

        constexpr void make_ntags(state_t q0, state_t qf, const std::vector<int>& ntags);
        
        constexpr void thompson(const expr_tree<char_type>& ast);

        template<bool RetBitVec>
        using ec_result = std::conditional_t<RetBitVec, std::vector<bool>, std::vector<state_t>>;

        template<tnfa::ec_mode Mode, bool RetBitVec = false>
        [[nodiscard]] constexpr ec_result<RetBitVec> epsilon_closure(std::vector<state_t>&& qs) const;

        template<tnfa::ec_mode Mode, bool RetBitVec = false>
        [[nodiscard]] constexpr ec_result<RetBitVec> backwards_epsilon_closure(std::vector<state_t>&& qs) const;

        constexpr std::flat_map<state_t, state_t> copy_subgraph(const std::vector<state_t>& qs);

        constexpr void remove_dead_and_unreachable_states();
        constexpr void rewrite_sof_anchors();
        constexpr void rewrite_eof_anchors();
        constexpr void rewrite_sc_lookahead();
        constexpr void rewrite_sc_lookbehind();
        constexpr void rewrite_assertions();

        /* constants */

        static constexpr state_t default_start_node{ 0 };
        static constexpr state_t default_final_node{ 1 };

        /* data members */
    
        std::vector<tnfa::node>                         nodes_{ 2 };
        std::vector<tnfa::transition<char_type>>        transitions_;
        capture_info                                    capture_info_;
        std::size_t                                     tag_count_;
        state_t                                         start_node_{ default_start_node };
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