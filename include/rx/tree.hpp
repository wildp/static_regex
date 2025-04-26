#pragma once

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <ranges>
#include <variant>
#include <vector>

#include <rx/util.hpp>
#include <rx/error.hpp>
#include <rx/tok.hpp>

#ifndef RX_TREE_DEBUG_PARSER
#define RX_TREE_DEBUG_PARSER 0
#endif // RX_TREE_DEBUG_PARSER

#if RX_TREE_DEBUG_PARSER
#include <print>
#endif // RX_TREE_DEBUG_PARSER

namespace rx::detail
{
    template<typename CharT>
    class tagged_nfa;

    /* types */

    enum class assert_type : int_least8_t
    {
        text_start,
        text_end,
        line_start,
        line_end
    };

    enum class repeater_mode : int_least8_t
    {
        greedy = 0,
        lazy,
        possessive
    };

    struct parser_flags
    {
        bool enable_captures    : 1 { true };
        bool enable_possessive  : 1 { false };
        bool enable_backrefs    : 1 { false };
    };
    

    /* ast definition */

    template<typename CharT>
    class expr_tree
    {
    public:
        using sv_type = std::basic_string_view<CharT>;

        struct empty {};    
        using any = tok::dot;

        struct assertion
        {
            assert_type type;
        };

        struct alt
        {
            std::vector<std::size_t> idxs;
        };
        
        struct concat
        {
            std::vector<std::size_t> idxs;
        };

        struct capture
        {
            std::size_t idx;
            std::uint_least16_t capture_num;
        };

        using backref = tok::backref;  

        struct repeat
        {
            std::size_t idx;
            std::int_least16_t min;
            std::int_least16_t max; /* use max=min for {min} or max<min for {min,} */ 
            repeater_mode mode;     /* default = greedy */
        };

        using char_lit = tok::char_lit<CharT>;
        using char_class = tok::char_class<CharT>;

        // TODO: replace char_lit with strings
        // using char_str = std::basic_string<CharT>;

        using type = std::variant<empty, any, assertion, char_lit, char_class, backref, alt, concat, capture, repeat>;

        constexpr expr_tree(sv_type sv);
        
        friend class tagged_nfa<CharT>;
        
    protected:
        [[nodiscard]] constexpr const type& get_expr(std::size_t i) const { return expressions_.at(i); }
        [[nodiscard]] constexpr std::size_t root_idx() const { return root_idx_; }
        [[nodiscard]] constexpr std::size_t tag_count() const { return 1 + (2 * capture_count_); }

        constexpr void make_tag_vec(std::vector<std::vector<int>>& tag_vec) const;

    private:
        template<in_variant<typename lexer<CharT>::token_t> T>
        static constexpr std::size_t tok_index{ index_of_impl<typename lexer<CharT>::token_t, T>::value };

        template<in_variant<type> T>
        static constexpr std::size_t ast_index{ index_of_impl<type, T>::value };

        std::size_t root_idx_{ 0 };
        parser_flags flags_;
        std::vector<type> expressions_;
        std::uint_least16_t capture_count_{ 0 };
    };

    /* helper classes (and enum) for parser */

    namespace parser
    {
        enum class nonterminal : std::uint_least8_t { S, E, E_, F, F_, G, R, R_, H };

        enum class semantic_action : std::int_least8_t
        {
            identity,

            make_hat,
            make_dollar,

            make_alt,
            make_concat,

            make_capture,
            make_bref,

            make_star,
            make_plus,
            make_quest,
            make_repeat,

            rep_greedy,
            rep_lazy,
            rep_possessive,
        };

        template<typename CharT>
        class ll1_stack
        {
        public:
            using terminal = lexer<CharT>::token_t;
            using elem_t = std::variant<terminal, nonterminal, semantic_action>;
            
            constexpr void pop() { data_.pop_back(); }
            constexpr void push(const std::vector<elem_t>& elems){ data_.append_range(elems | std::views::reverse); }
            [[nodiscard]] constexpr auto& root() { return data_.back(); }
            [[nodiscard]] constexpr const auto& root() const { return data_.back(); }
            
            constexpr auto& begin() const noexcept { return data_.cbegin(); }
            constexpr auto& end() const noexcept { return data_.cend(); }
            constexpr auto& rbegin() const noexcept { return data_.crbegin(); }
            constexpr auto& rend() const noexcept { return data_.crend(); }
            constexpr auto& cbegin() const noexcept { return data_.cbegin(); }
            constexpr auto& cend() const noexcept { return data_.cend(); }
            constexpr auto& crbegin() const noexcept { return data_.crbegin(); }
            constexpr auto& crend() const noexcept { return data_.crend(); }
            [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }

        private:
            std::vector<elem_t> data_{ nonterminal::S };
        };

        template<typename CharT>
        class semantic_stack
        {
        public:
            using terminal = ll1_stack<CharT>::terminal;
            using elem_t = std::variant<std::size_t, typename lexer<CharT>::token_t, repeater_mode>;
            
            [[nodiscard]] constexpr elem_t pop() { elem_t tmp{ std::move(data_.back()) }; data_.pop_back(); return tmp; }
            constexpr void push(elem_t&& elems) { data_.push_back(std::move(elems)); }

            constexpr auto& begin() const noexcept { return data_.cbegin(); }
            constexpr auto& end() const noexcept { return data_.cend(); }
            constexpr auto& rbegin() const noexcept { return data_.crbegin(); }
            constexpr auto& rend() const noexcept { return data_.crend(); }
            constexpr auto& cbegin() const noexcept { return data_.cbegin(); }
            constexpr auto& cend() const noexcept { return data_.cend(); }
            constexpr auto& crbegin() const noexcept { return data_.crbegin(); }
            constexpr auto& crend() const noexcept { return data_.crend(); }
            [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }

        private:
            std::vector<elem_t> data_{};
        };
    }

    /* print-statement debugging tools */

#if RX_TREE_DEBUG_PARSER
    template <typename E>
    requires std::is_enum_v<E>
    constexpr std::string enum_to_string(E value)
    {
        template for (constexpr auto e : std::meta::define_static_array(std::meta::enumerators_of(^^E)))
        {
            if (value == [:e:]) 
                return std::string(std::meta::identifier_of(e));
        }

        return "<unnamed>";
    }

    template <typename... Ts>
    constexpr std::string variant_alternative_name(const std::variant<Ts...>& value)
    {
        template for (constexpr auto e : { (^^Ts)... })
        {
            if (std::holds_alternative<[: e :]>(value))
                return std::string{ std::meta::display_string_of(e) };
        }

        return "<error>";
    }

    template<typename... Ts>
    constexpr std::string pretty_print_2(const std::variant<Ts...>& value)
    {
        template for (constexpr auto e : { (^^Ts)... })
        {
            if (std::holds_alternative<[: e :]>(value))
            {
                if constexpr (std::meta::is_enum_type(e))
                    return enum_to_string(std::get<[: e :]>(value));
                if constexpr (std::meta::has_template_arguments(e) and std::meta::template_of(e) == ^^std::variant)
                    return variant_alternative_name(std::get<[: e :]>(value)) ;
                else
                    return std::string{ std::meta::display_string_of(e) };
            }
        }

        return "<error>";
    }
#endif // RX_TREE_DEBUG_PARSER

    /* parser implemenation */

    template<typename CharT>
    constexpr expr_tree<CharT>::expr_tree(const sv_type sv) 
    {
        using namespace parser;
        using terminal = ll1_stack<CharT>::terminal;

        if (sv.empty())
        {
            /* ensure expressions is non-empty */
            expressions_.emplace_back(std::in_place_type<empty>);
            return;
        }

        lexer<CharT> l{ sv };
        ll1_stack<CharT> stack;
        semantic_stack<CharT> semstack;

        auto token{ l.nexttok() };

        for (bool loop{ true }; loop;)
        {
            if (stack.empty())
                throw pattern_error("Invalid pattern");

#if RX_TREE_DEBUG_PARSER
            if not consteval
            {
                std::print("stack = ");
                for (const auto& elem: stack | std::views::reverse)
                    std::print(" {}", pretty_print_2(elem));
                std::println();
            }
#endif // RX_TREE_DEBUG_PARSER

            const auto top{ std::move(stack.root()) };
            stack.pop();

            if (const auto* const term{ std::get_if<terminal>(&top) })
            {
                if (std::holds_alternative<tok::end_of_input>(*term))
                {
                    /* parsing is done */
                    loop = false;
                }
                else if (term->index() == token.index())
                {
#if RX_TREE_DEBUG_PARSER
                    if not consteval
                    {
                        std::println("matched: {}", variant_alternative_name(token));
                    }
#endif // RX_TREE_DEBUG_PARSER

                    /* match */
                    semstack.push(std::move(token));
                    token = l.nexttok();
                }
                else
                {
                    /* no match */
                    throw pattern_error("Parse Error");
                }
            }
            else if (const auto* const nonterm{ std::get_if<nonterminal>(&top) })
            {
                /* predict */

                using nt = nonterminal;
                using sa = semantic_action;
                using namespace tok;

#if RX_TREE_DEBUG_PARSER
                if not consteval
                {
                    std::println("lookup[{}, {}]", enum_to_string(*nonterm), variant_alternative_name(token));
                }
#endif // RX_TREE_DEBUG_PARSER

                switch (*nonterm)
                {
                case nt::S:
                    switch (token.index())
                    {
                    case tok_index<hat>:
                    case tok_index<dot>:
                    case tok_index<dollar>:
                    case tok_index<lparen>:
                    case tok_index<char_lit>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ nt::E, end_of_input{} });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::E:
                    switch (token.index())
                    {
                    case tok_index<hat>:
                    case tok_index<dot>:
                    case tok_index<dollar>:
                    case tok_index<lparen>:
                    case tok_index<char_lit>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ nt::F, nt::E_ });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::E_:
                    switch (token.index())
                    {
                    case tok_index<end_of_input>:
                    case tok_index<rparen>:
                        /* epsilon */
                        break;
                    case tok_index<vert>:
                        stack.push({ vert{}, nt::E, sa::make_alt });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::F:
                    switch (token.index())
                    {
                    case tok_index<hat>:
                    case tok_index<dot>:
                    case tok_index<dollar>:
                    case tok_index<lparen>:
                    case tok_index<char_lit>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ nt::G, nt::F_ });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::F_:
                    switch (token.index())
                    {
                    case tok_index<end_of_input>:
                    case tok_index<rparen>:
                    case tok_index<vert>:
                        break;
                    case tok_index<hat>:
                    case tok_index<dot>:
                    case tok_index<dollar>:
                    case tok_index<lparen>:
                    case tok_index<char_lit>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ nt::F , sa::make_concat });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::G:
                    switch (token.index())
                    {
                    case tok_index<hat>:
                    case tok_index<dot>:
                    case tok_index<dollar>:
                    case tok_index<lparen>:
                    case tok_index<char_lit>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ nt::H, nt::R });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::R:
                    switch (token.index())
                    {
                    case tok_index<end_of_input>:
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<lparen>:
                    case tok_index<rparen>:
                    case tok_index<vert>:
                    case tok_index<char_lit>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        /* epsilon */
                        break;
                    case tok_index<star>:
                        stack.push({ star{}, nt::R_, sa::make_star });
                        break;
                    case tok_index<plus>:
                        stack.push({ plus{}, nt::R_, sa::make_plus });
                        break;
                    case tok_index<quest>:
                        stack.push({ quest{}, nt::R_, sa::make_quest });
                        break;
                    case tok_index<repeat_n_m>:
                        stack.push({ repeat_n_m{}, nt::R_, sa::make_repeat });
                        break;
                    }
                    break;
                case nt::R_:
                    switch (token.index())
                    {
                    case tok_index<end_of_input>:
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<lparen>:
                    case tok_index<rparen>:
                    case tok_index<vert>:
                    case tok_index<char_lit>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ /* epsilon, */ sa::rep_greedy });
                        break;
                    case tok_index<quest>:
                        stack.push({ quest{}, sa::rep_lazy });
                        break;
                    case tok_index<plus>:
                        stack.push({ plus{}, sa::rep_possessive });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::H:
                    switch (token.index())
                    {
                    case tok_index<dot>:
                        stack.push({ dot{}, sa::identity });
                        break;
                    case tok_index<hat>:
                        stack.push({ hat{}, sa::make_hat });
                        break;
                    case tok_index<dollar>:
                        stack.push({ dollar{}, sa::make_dollar });
                        break;
                    case tok_index<lparen>:
                        stack.push({ lparen{}, nt::E, rparen{}, sa::make_capture });
                        break;
                    case tok_index<char_lit>:
                        stack.push({ char_lit{}, sa::identity });
                        break;
                    case tok_index<char_class>:
                        stack.push({ char_class{}, sa::identity });
                        break;
                    case tok_index<backref>:
                        stack.push({ char_class{}, sa::make_bref });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                }
            }
            else if(const auto* const action{ std::get_if<semantic_action>(&top) })
            {
                using sa = semantic_action;

                switch (*action)
                {
                case sa::identity:
                    {
                        /* make an arbitrary token an ast entry */
                        const terminal term{ std::get<terminal>(semstack.pop()) };
                        
                        semstack.push(expressions_.size());
                        expressions_.emplace_back(term.visit(overloads{
                            [](const in_variant<type> auto& arg) constexpr -> type { return arg; },
                            [](const auto&) constexpr -> type { throw pattern_error("Unknown error during parsing pattern"); }
                        }));
                    }
                    break;
                case sa::make_hat:
                    {
                        std::ignore = semstack.pop(); /* pop tok::hot */

                        semstack.push(expressions_.size());
                        expressions_.emplace_back(std::in_place_type<assertion>, assert_type::text_start);
                        // TODO: depending on flags, insert assert_type::text_start instead
                    }
                    break;
                case sa::make_dollar:
                    {
                        std::ignore = semstack.pop(); /* pop tok::dollar */

                        semstack.push(expressions_.size());
                        expressions_.emplace_back(std::in_place_type<assertion>, assert_type::text_end);
                        // TODO: depending on flags, insert assert_type::line_end instead
                    }
                    break;
                case sa::make_alt:
                    {
                        const auto rhs_idx{ std::get<std::size_t>(semstack.pop()) };
                        std::ignore = semstack.pop(); /* pop tok::vert */
                        const auto lhs_idx{ std::get<std::size_t>(semstack.pop()) };

                        if (type& ast{ expressions_.at(rhs_idx) }; std::holds_alternative<alt>(ast))
                        {
                            /* insert lhs into existing alt */
                            semstack.push(rhs_idx);
                            auto& ast_alt{ std::get<alt>(ast) };
                            ast_alt.idxs.insert(ast_alt.idxs.begin(), lhs_idx);
                        }
                        else 
                        {
                            /* create new alt */
                            semstack.push(expressions_.size());
                            expressions_.emplace_back(std::in_place_type<alt>, std::vector<std::size_t>{ lhs_idx, rhs_idx });
                        }
                    }
                    break;
                case sa::make_concat:
                    {
                        const auto rhs_idx{ std::get<std::size_t>(semstack.pop()) };
                        const auto lhs_idx{ std::get<std::size_t>(semstack.pop()) };

                        // TODO: perform string merging? 

                        if (type& ast{ expressions_.at(rhs_idx) }; std::holds_alternative<concat>(ast))
                        {
                            /* insert lhs into existing concat */
                            semstack.push(rhs_idx);
                            auto& ast_concat { std::get<concat>(ast) };
                            ast_concat.idxs.insert(ast_concat.idxs.begin(), lhs_idx);
                        }
                        else 
                        {
                            /* create new concat */
                            semstack.push(expressions_.size());
                            expressions_.emplace_back(std::in_place_type<concat>, std::vector{ lhs_idx, rhs_idx });
                        }
                    }
                    break;
                case sa::make_capture:
                    {
                        std::ignore = semstack.pop(); /* pop tok::rparen */
                        const auto child_idx{ std::get<std::size_t>(semstack.pop()) };
                        std::ignore = semstack.pop(); /* pop tok::lparen */
                        
                        if (flags_.enable_captures)
                        {
                            semstack.push(expressions_.size());
                            expressions_.emplace_back(std::in_place_type<capture>, child_idx, capture_count_++);
                            if (capture_count_ == 0) /* i.e. unsigned integer overflow has occurred */
                                throw pattern_error("Capturing group limit exceeded");
                        }
                        else
                        {
                            semstack.push(child_idx);
                        }
                    }
                    break;
                case sa::make_bref:
                    {
                        const auto bref{ std::get<tok::backref>(std::get<terminal>(semstack.pop())) }; /* pop tok::backref */

                        if (flags_.enable_backrefs)
                        {
                            /* note: we don't perform a check (bref.number <= capture_count) to match the behaviour of other engines */

                            semstack.push(expressions_.size());
                            expressions_.emplace_back(std::in_place_type<backref>, bref.number);
                        }
                        else
                        {
                            throw parser_error("Backreferences are not enabled");
                        }
                        
                    }
                    break;
                case sa::make_star:
                    {
                        const auto mode{ std::get<repeater_mode>(semstack.pop()) };
                        std::ignore = semstack.pop(); /* pop tok::star */
                        const auto child_idx{ std::get<std::size_t>(semstack.pop()) };
                        
                        semstack.push(expressions_.size());
                        expressions_.emplace_back(std::in_place_type<repeat>, child_idx, 0, -1, mode);
                    }
                    break;
                case sa::make_plus:
                    {
                        const auto mode{ std::get<repeater_mode>(semstack.pop()) };
                        std::ignore = semstack.pop(); /* pop tok::plus */
                        const auto child_idx{ std::get<std::size_t>(semstack.pop()) };
                        
                        semstack.push(expressions_.size());
                        expressions_.emplace_back(std::in_place_type<repeat>, child_idx, 1, 0, mode);
                    }
                    break;
                case sa::make_quest:
                    {
                        const auto mode{ std::get<repeater_mode>(semstack.pop()) };
                        std::ignore = semstack.pop(); /* pop tok::quest */
                        const auto child_idx{ std::get<std::size_t>(semstack.pop()) };
                        
                        semstack.push(expressions_.size());
                        expressions_.emplace_back(std::in_place_type<repeat>, child_idx, 0, 1, mode);
                    }
                    break;
                case sa::make_repeat:
                    {
                        const auto mode{ std::get<repeater_mode>(semstack.pop()) };
                        const auto rep{ std::get<tok::repeat_n_m>(std::get<terminal>(semstack.pop())) };
                        const auto child_idx{ std::get<std::size_t>(semstack.pop()) };

                        semstack.push(expressions_.size());
                        expressions_.emplace_back(std::in_place_type<repeat>, child_idx, rep.min, rep.max, mode);
                    }
                    break;
                case sa::rep_greedy:
                    {
                        semstack.push(repeater_mode::greedy);
                    }
                    break;
                case sa::rep_lazy:
                    {
                        std::ignore = semstack.pop();  /* pop tok::quest */
                        semstack.push(repeater_mode::lazy);
                    }
                    break;
                case sa::rep_possessive:
                    {
                        std::ignore = semstack.pop();  /* pop tok::plus */

                        if (flags_.enable_possessive)
                            semstack.push(repeater_mode::possessive);
                        else
                            throw parser_error("Possessive repetition is not enabled");
                    }
                    break;
                }
            }
            else
            {
                /* no match */
                throw pattern_error("Parse Error");
            }
        }

        if (not semstack.empty())
            root_idx_ = std::get<std::size_t>(semstack.pop());
    }

    /* helper for tagged nfa conversion */

    template<typename CharT>
    constexpr void expr_tree<CharT>::make_tag_vec(std::vector<std::vector<int>>& tag_vec) const
    {
        tag_vec.clear();
        tag_vec.resize(expressions_.size());

        using stack_elem_t = std::pair<std::size_t, std::size_t>;

        std::vector<stack_elem_t> stack;
        stack.emplace_back(root_idx(), false);

        while (not stack.empty())
        {
            auto& [idx, pos]{ stack.back() };
            const auto& entry{ expressions_.at(idx) };

            switch (entry.index())
            {
            case ast_index<empty>:
            case ast_index<any>:
            case ast_index<assertion>:
            case ast_index<char_lit>:
            case ast_index<char_class>:
            case ast_index<backref>:
                stack.pop_back();
                break;

            case ast_index<concat>:
                {
                    const auto& cat{ std::get<concat>(entry) };

                    if (pos == cat.idxs.size())
                    {
                        auto& vec{ tag_vec.at(idx) };

                        for (std::size_t i : cat.idxs)
                            std::ranges::copy(tag_vec.at(i), std::back_inserter(vec));

                        std::ranges::sort(vec);
                        auto [_, last]{ std::ranges::unique(vec) };
                        vec.erase(last, vec.end());

                        stack.pop_back();
                    }
                    else
                    {
                        pos += 1;
                        stack.emplace_back(cat.idxs.at(pos - 1), 0);
                    }
                }
                break;

            case ast_index<alt>:
                {
                    const auto& atl{ std::get<alt>(entry) };

                    if (pos == atl.idxs.size())
                    {
                        auto& vec{ tag_vec.at(idx) };

                        for (std::size_t i : atl.idxs)
                            std::ranges::copy(tag_vec.at(i), std::back_inserter(vec));

                        std::ranges::sort(vec);
                        auto [_, last]{ std::ranges::unique(vec) };
                        vec.erase(last, vec.end());

                        stack.pop_back();
                    }
                    else
                    {
                        pos += 1;
                        stack.emplace_back(atl.idxs.at(pos - 1), 0);
                    }
                }
                break;

            case ast_index<repeat>:
                {
                    const auto& rep{ std::get<repeat>(entry) };

                    if (pos == 1)
                    {
                        auto& vec{ tag_vec.at(idx) };
                        std::ranges::copy(tag_vec.at(rep.idx), std::back_inserter(vec));

                        stack.pop_back();
                    }
                    else
                    {
                        pos += 1;
                        stack.emplace_back(rep.idx, 0);
                    }
                }
                break;

            case ast_index<capture>:
                {
                    const auto& cap{ std::get<capture>(entry) };

                    if (pos == 1)
                    {
                        auto& vec{ tag_vec.at(idx) };

                        vec.emplace_back(cap_num_to_tag(cap.capture_num, false));
                        vec.emplace_back(cap_num_to_tag(cap.capture_num, true));

                        std::ranges::copy(tag_vec.at(cap.idx), std::back_inserter(vec));
                        std::ranges::sort(vec);

                        stack.pop_back();
                    }
                    else
                    {
                        pos += 1;
                        stack.emplace_back(cap.idx, 0);
                    }

                }
                break;

            default:
                throw tree_error("Invalid tree");
            }
        }
    }
}