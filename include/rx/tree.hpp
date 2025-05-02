#pragma once

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <ranges>
#include <utility>
#include <variant>
#include <vector>

#include <rx/captures.hpp>
#include <rx/util.hpp>
#include <rx/error.hpp>
#include <rx/tok.hpp>

// Optimisations to add:
// - merge alternations of single characters into character classes

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
        bool enable_branchreset : 1 { false };
    };
    

    /* ast definition */

    template<typename CharT>
    class expr_tree
    {
    public:
        using sv_type = std::basic_string_view<CharT>;
 
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
            std::uint_least16_t number;
        };

        using backref = tok::backref;  

        struct repeat
        {
            std::size_t idx;
            std::int_least16_t min;
            std::int_least16_t max; /* use max=min for {min} or max<min for {min,} */ 
            repeater_mode mode;     /* default = greedy */
        };

        using char_str = tok::char_str<CharT>;
        using char_class = tok::char_class<CharT>;

        // TODO: replace char_lit with strings
        // using char_str = std::basic_string<CharT>;

        using type = std::variant<any, assertion, char_str, char_class, backref, alt, concat, capture, repeat>;

        constexpr expr_tree(sv_type sv, parser_flags flags = {});
        
        friend class tagged_nfa<CharT>;
        
    protected:
        [[nodiscard]] constexpr const type& get_expr(std::size_t i) const { return expressions_.at(i); }
        [[nodiscard]] constexpr std::size_t root_idx() const { return root_idx_; }
        [[nodiscard]] constexpr std::size_t tag_count() const { return 1 + (2 * capture_count_); }
        [[nodiscard]] constexpr std::size_t capture_count() const { return capture_count_; }

        constexpr void make_tag_vec(std::vector<std::vector<int>>& tag_vec) const;

    private:
        template<in_variant<typename lexer<CharT>::token_t> T>
        static constexpr std::size_t tok_index{ index_of_impl<typename lexer<CharT>::token_t, T>::value };

        template<in_variant<type> T>
        static constexpr std::size_t ast_index{ index_of_impl<type, T>::value };

        template<in_variant<type> T, typename... Args>
        [[nodiscard]] constexpr std::size_t new_expression(Args... args)
        {
            if (overwritable_.empty())
            {
                const std::size_t idx{ expressions_.size() };
                expressions_.emplace_back(std::in_place_type<T>, std::forward<Args>(args)...);
                return idx;
            }
            else
            {
                const std::size_t idx{ overwritable_.back() };
                overwritable_.pop_back();
                expressions_.at(idx) = T{ std::forward<Args>(args)... };
                return idx;
            }
        }

        std::size_t root_idx_{ 0 };
        parser_flags flags_;
        std::vector<type> expressions_;
        std::vector<std::size_t> overwritable_;
        std::uint_least16_t capture_count_{ 0 };
    };


    /* helper classes (and enum) for parser */

    namespace parser
    {
        enum class nonterminal : std::uint_least8_t { S, E, E_, F, F_, G, R, R_, H, P, P_ };

        enum class semantic_action : std::int_least8_t
        {
            identity,

            make_hat,
            make_dollar,

            make_alt,
            make_concat,

            make_bref,

            make_star,
            make_plus,
            make_quest,
            make_repeat,

            rep_greedy,
            rep_lazy,
            rep_possessive,

            cap_push,
            cap_pop,
            cap_pop_empty,
            cap_parse_flag,
            cap_parse_flag_done,

            begin_alt,
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
            
            constexpr auto begin() const noexcept { return data_.cbegin(); }
            constexpr auto end() const noexcept { return data_.cend(); }
            constexpr auto rbegin() const noexcept { return data_.crbegin(); }
            constexpr auto rend() const noexcept { return data_.crend(); }
            constexpr auto cbegin() const noexcept { return data_.cbegin(); }
            constexpr auto cend() const noexcept { return data_.cend(); }
            constexpr auto crbegin() const noexcept { return data_.crbegin(); }
            constexpr auto crend() const noexcept { return data_.crend(); }
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

            constexpr auto begin() const noexcept { return data_.cbegin(); }
            constexpr auto end() const noexcept { return data_.cend(); }
            constexpr auto rbegin() const noexcept { return data_.crbegin(); }
            constexpr auto rend() const noexcept { return data_.crend(); }
            constexpr auto cbegin() const noexcept { return data_.cbegin(); }
            constexpr auto cend() const noexcept { return data_.cend(); }
            constexpr auto crbegin() const noexcept { return data_.crbegin(); }
            constexpr auto crend() const noexcept { return data_.crend(); }
            [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }

        private:
            std::vector<elem_t> data_{};
        };
    }


    /* parser implemenation */

    template<typename CharT>
    constexpr expr_tree<CharT>::expr_tree(const sv_type sv, parser_flags flags) :
            flags_{ flags } 
    {
        using namespace parser;
        using terminal = ll1_stack<CharT>::terminal;

        if (sv.empty())
        {
            /* ensure expressions is non-empty */
            std::ignore = new_expression<char_str>(/* empty string */);
            return;
        }

        lexer<CharT> l{ sv };
        ll1_stack<CharT> stack;
        semantic_stack<CharT> semstack;
        capture_stack capstack;

        auto token{ l.nexttok() };

        for (bool loop{ true }; loop;)
        {
            if (stack.empty())
                throw pattern_error("Invalid pattern");

            #if RX_TREE_DEBUG_PARSER
            if not consteval
            {
                std::println("-------------------------");
                std::print("stack = ");
                for (const auto& elem: stack | std::views::reverse)
                    std::print(" {}", pretty_print_2(elem));
                std::println();
                std::print("semstack = ");
                for (const auto& elem: semstack | std::views::reverse)
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
                    case tok_index<char_str>:
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
                    case tok_index<char_str>:
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
                        stack.push({ sa::begin_alt, vert{}, nt::E, sa::make_alt });
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
                    case tok_index<char_str>:
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
                    case tok_index<char_str>:
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
                    case tok_index<char_str>:
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
                    case tok_index<char_str>:
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
                    case tok_index<char_str>:
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
                        stack.push({ sa::cap_push, lparen{}, nt::P });
                        break;
                    case tok_index<char_str>:
                        stack.push({ char_str{}, sa::identity });
                        break;
                    case tok_index<char_class>:
                        stack.push({ char_class{}, sa::identity });
                        break;
                    case tok_index<backref>:
                        stack.push({ backref{}, sa::make_bref });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::P:
                    switch (token.index())
                    {
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<lparen>:
                    case tok_index<rparen>:
                    case tok_index<char_str>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ nt::P_ });
                        break;
                    case tok_index<quest>:
                        stack.push({ sa::cap_parse_flag, quest{}, sa::cap_parse_flag_done , nt::P_ });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                case nt::P_:
                    switch (token.index())
                    {
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<lparen>:
                    case tok_index<char_str>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ nt::E, rparen{}, sa::cap_pop });
                        break;
                    case tok_index<rparen>:
                        stack.push({ rparen{}, sa::cap_pop_empty });
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
                        
                        type result{ term.visit(overloads{
                            [](const in_variant<type> auto& arg) constexpr -> type { return arg; },
                            [](const auto&) constexpr -> type { throw pattern_error("Unknown error during parsing pattern"); }
                        }) };

                        /* we duplicate functionality of new_expression here */
                        if (overwritable_.empty())
                        {
                            semstack.push(expressions_.size());
                            expressions_.emplace_back(std::move(result));
                        }
                        else
                        {
                            const std::size_t idx{ overwritable_.back() };
                            overwritable_.pop_back();
                            semstack.push(idx);
                            expressions_.at(idx) = std::move(result);
                        }
                    }
                    break;
                case sa::make_hat:
                    {
                        std::ignore = semstack.pop(); /* pop tok::hot */                        

                        /* depending on flags, insert assert_type::line_start instead of assert_type::text_start */
                        if (capstack.multiline()) 
                            semstack.push(new_expression<assertion>(assert_type::line_start));
                        else
                            semstack.push(new_expression<assertion>(assert_type::text_start));
                        
                    }
                    break;
                case sa::make_dollar:
                    {
                        std::ignore = semstack.pop(); /* pop tok::dollar */

                        /* depending on flags, insert assert_type::line_end instead of assert_type::text_end */
                        if (capstack.multiline()) 
                            semstack.push(new_expression<assertion>(assert_type::line_end));
                        else
                            semstack.push(new_expression<assertion>(assert_type::text_end));
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
                            semstack.push(new_expression<alt>(std::vector<std::size_t>{ lhs_idx, rhs_idx }));
                        }
                    }
                    break;
                case sa::make_concat:
                    {
                        const auto rhs_idx{ std::get<std::size_t>(semstack.pop()) };
                        const auto lhs_idx{ std::get<std::size_t>(semstack.pop()) };

                        if (type& ast{ expressions_.at(rhs_idx) }; std::holds_alternative<concat>(ast))
                        {
                            /* insert lhs into existing concat */
                            semstack.push(rhs_idx);
                            auto& ast_concat{ std::get<concat>(ast) };
                            bool merged{ false };

                            if (not ast_concat.idxs.empty())
                            {
                                type& rhs{ expressions_.at(ast_concat.idxs.front()) };
                                type& lhs{ expressions_.at(lhs_idx) };

                                if (std::holds_alternative<char_str>(rhs) and std::holds_alternative<char_str>(lhs))
                                {
                                    /* merge lhs string with first entry of rhs (also a string) */
                                    auto& target{ std::get<char_str>(rhs).str };
                                    auto& lhs_str{ std::get<char_str>(lhs).str };
                                    lhs_str.append(target);
                                    std::swap(lhs_str, target);
                                    overwritable_.push_back(lhs_idx);
                                    merged = true;
                                }
                            }

                            
                            if (not merged)
                            {
                                /* insert lhs into existing concat */
                                ast_concat.idxs.insert(ast_concat.idxs.begin(), lhs_idx);
                            }
                        }
                        else 
                        {
                            type& rhs{ expressions_.at(rhs_idx) };
                            type& lhs{ expressions_.at(lhs_idx) };

                            if (std::holds_alternative<char_str>(rhs) and std::holds_alternative<char_str>(lhs))
                            {
                                /* lhs and rhs are both strings: merge strings into one instead of creating concat  */
                                semstack.push(rhs_idx);
                                auto& target{ std::get<char_str>(rhs).str };
                                auto& lhs_str{ std::get<char_str>(lhs).str };
                                lhs_str.append(target);
                                std::swap(lhs_str, target);
                                overwritable_.push_back(lhs_idx);
                            }
                            else
                            {
                                /* create new concat */
                                semstack.push(new_expression<concat>(std::vector{ lhs_idx, rhs_idx }));
                            }
                        }
                    }
                    break;
                case sa::make_bref:
                    {
                        const auto bref{ std::get<tok::backref>(std::get<terminal>(semstack.pop())) }; /* pop tok::backref */

                        if (flags_.enable_backrefs)
                        {
                            /* note: we don't perform a check (bref.number <= capture_count) to match the behaviour of other engines */

                            semstack.push(new_expression<backref>(bref.number));
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
                        
                        semstack.push(new_expression<repeat>(child_idx, std::int_least16_t{ 0 }, std::int_least16_t{ -1 }, mode));
                    }
                    break;
                case sa::make_plus:
                    {
                        const auto mode{ std::get<repeater_mode>(semstack.pop()) };
                        std::ignore = semstack.pop(); /* pop tok::plus */
                        const auto child_idx{ std::get<std::size_t>(semstack.pop()) };
                        
                        semstack.push(new_expression<repeat>(child_idx, std::int_least16_t{ 1 }, std::int_least16_t{ 0 }, mode));
                    }
                    break;
                case sa::make_quest:
                    {
                        const auto mode{ std::get<repeater_mode>(semstack.pop()) };
                        std::ignore = semstack.pop(); /* pop tok::quest */
                        const auto child_idx{ std::get<std::size_t>(semstack.pop()) };
                        
                        semstack.push(new_expression<repeat>(child_idx, std::int_least16_t{ 0 }, std::int_least16_t{ 1 }, mode));
                    }
                    break;
                case sa::make_repeat:
                    {
                        const auto mode{ std::get<repeater_mode>(semstack.pop()) };
                        const auto rep{ std::get<tok::repeat_n_m>(std::get<terminal>(semstack.pop())) };
                        const auto child_idx{ std::get<std::size_t>(semstack.pop()) };

                        semstack.push(new_expression<repeat>(child_idx, rep.min, rep.max, mode));
                    }
                    break;
                case sa::rep_greedy:
                    {
                        /* swap greedy and lazy quantifiers if 'ungreedy' flag is set */
                        if (capstack.ungreedy())
                            semstack.push(repeater_mode::lazy);  /* swapped */
                        else
                            semstack.push(repeater_mode::greedy); /* default */
                    }
                    break;
                case sa::rep_lazy:
                    {
                        std::ignore = semstack.pop();  /* pop tok::quest */

                        /* swap greedy and lazy quantifiers if 'ungreedy' flag is set */
                        if (capstack.ungreedy())
                            semstack.push(repeater_mode::greedy); /* swapped */
                        else
                            semstack.push(repeater_mode::lazy);  /* default */
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
                case sa::cap_push:
                    {
                        if (flags_.enable_captures)
                        {
                            if (capstack.push()) /* i.e. unsigned integer overflow has occurred */
                                throw pattern_error("Capturing group limit exceeded");
                        }
                        else
                        {
                            capstack.push_non_capturing();
                        }   
                    }
                    break;
                case sa::cap_pop:
                    {
                        std::ignore = semstack.pop(); /* pop tok::rparen */
                        const auto child_idx{ std::get<std::size_t>(semstack.pop()) };
                        std::ignore = semstack.pop(); /* pop tok::lparen */

                        const auto cap_number{ capstack.pop() };

                        if (cap_number.has_value())
                            semstack.push(new_expression<capture>(child_idx, *cap_number));
                        else
                            semstack.push(child_idx); /* non capturing group */
                    }
                    break;
                case sa::cap_pop_empty:
                    {
                        std::ignore = semstack.pop(); /* pop tok::rparen */
                        std::ignore = semstack.pop(); /* pop tok::lparen */

                        const auto cap_number{ capstack.pop_empty() };
                        const auto empty_idx{ new_expression<char_str>(/* empty string */) };

                        if (cap_number.has_value())
                            semstack.push(new_expression<capture>(empty_idx, *cap_number));
                        else
                            semstack.push(empty_idx); /* non capturing group */
                    }
                    break;
                case sa::cap_parse_flag:
                    {
                        /* manually parse flags */

                        auto& lit{ l.it_ };
                        const auto& lend{ l.end_ };

                        bool flag_value{ true };

                        if (lit == lend)
                            break;

                        switch (*lit)
                        {
                        case '#':
                            ++lit;
                            while (lit != lend and *lit != ')')
                                ++lit; /* skip comment */
                            capstack.set_non_capturing();
                            break;

                        case '|':
                            if (not flags.enable_branchreset)
                                throw parser_error("Branch resetting in captures is not enabled");
                            ++lit;
                            capstack.set_branch_reset();
                            break;
                        
                        case '>':
                            throw pattern_error("Atomic capture groups are unsupported");

                        case 'P':
                        case '<':
                        case '\'':
                            throw pattern_error("Named capture groups are unsupported");

                        default: /* parse options */
                            for (bool loop{ true }; loop;)
                            {
                                if (lit == lend)
                                    throw pattern_error("Invalid Pattern"); 

                                const auto lookahead{ *lit };
                                bool increment{ true };

                                capstack.set_non_capturing();

                                switch(lookahead)
                                {
                                case 'i':
                                    capstack.set_caseless(flag_value);
                                    break;
                                case 'm':
                                    capstack.set_multiline(flag_value);
                                    break;
                                case 's':
                                    capstack.set_dotall(flag_value);
                                    break;
                                case 'U':
                                    capstack.set_ungreedy(flag_value);
                                    break;
                                case 'x':
                                    throw pattern_error("Capturing group flag 'x' is unsupported");
                                case '-':
                                    if (not flag_value)
                                        throw pattern_error("Capturing group arguments can only contain one hyphen");
                                    flag_value = false;
                                    break;
                                    
                                case ':':
                                    loop = false;
                                    break;

                                case ')':
                                    loop = false;
                                    increment = false;
                                    break;

                                default:
                                    throw pattern_error("Invalid capturing group");
                                }

                                if (increment)
                                    ++lit;
                            }   
                        }
                    }
                    break;
                case sa::cap_parse_flag_done:
                    {
                        std::ignore = semstack.pop(); /* pop tok::quest */

                        /* this exists only so we can use capture_pop(_empty)? for captures with flags */
                    }
                    break;
                case sa::begin_alt:
                    {
                        /* exists only so branch resetting captures work */
                        if (flags.enable_branchreset)
                            capstack.branch_reset_if_enabled();
                    }
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

        if (auto cc{ capstack.capture_count() }; cc.has_value())
            capture_count_ = cc.value();
        else
            throw pattern_error("Parse Error");
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
            case ast_index<any>:
            case ast_index<assertion>:
            case ast_index<char_str>:
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

                        vec.emplace_back(cap_num_to_tag(cap.number, false));
                        vec.emplace_back(cap_num_to_tag(cap.number, true));

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