module;

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <limits>
#include <ranges>
#include <utility>
#include <variant>
#include <vector>

export module rx.ast;

// import std;
import rx.util;
import :tok;
import :captures;

// TODO: move the parser to parser.cpp

namespace rx::detail
{
    /* types */

    export enum class assert_type : int_least8_t
    {
        text_start,
        text_end,
        line_start,
        line_end
    };

    export enum class repeater_mode : int_least8_t
    {
        greedy = 0,
        lazy,
        possessive
    };

    export struct parser_flags
    {
        bool enable_captures    : 1 { true };
        bool enable_possessive  : 1 { false };
        bool enable_backrefs    : 1 { false };
        bool enable_branchreset : 1 { false };
        bool enable_alttocc     : 1 { true };
    };


    /* ast definition */

    export template<typename CharT>
    class expr_tree
    {
    public:
        using sv_type = std::basic_string_view<CharT>;

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

        struct tag
        {
            tag_number_t number;
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

        using type = std::variant<assertion, char_str, char_class, backref, alt, concat, tag, repeat>;

        constexpr expr_tree(sv_type sv, parser_flags flags = {});
        
        // friend class tagged_nfa<CharT>;
        
        [[nodiscard]] constexpr const type& get_expr(std::size_t i) const { return expressions_.at(i); }
        [[nodiscard]] constexpr std::size_t root_idx() const { return root_idx_; }
        [[nodiscard]] constexpr std::size_t tag_count() const { return tag_count_; }
        [[nodiscard]] constexpr const capture_info& get_capture_info() const { return capture_info_; };

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
        capture_info capture_info_;
        tag_number_t tag_count_{ 0 };
    };


    /* helper classes (and enum) for parser */

    namespace parser
    {
        enum class nonterminal : std::uint_least8_t { S, E, E_, F, F_, G, R, R_, H, P };

        enum class semantic_action : std::int_least8_t
        {
            make_empty,
            make_dot,
            make_hat,
            make_dollar,

            make_char_lit,
            make_char_class,

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
            constexpr void push(const std::vector<elem_t>& elems) { data_.append_range(elems | std::views::reverse); }
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
                    case tok_index<end_of_input>:
                    case tok_index<hat>:
                    case tok_index<dot>:
                    case tok_index<dollar>:
                    case tok_index<lparen>:
                    case tok_index<vert>:
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
                    case tok_index<end_of_input>:
                    case tok_index<hat>:
                    case tok_index<dot>:
                    case tok_index<dollar>:
                    case tok_index<lparen>:
                    case tok_index<rparen>:
                    case tok_index<vert>:
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
                    case tok_index<end_of_input>:
                    case tok_index<rparen>:
                    case tok_index<vert>:
                        stack.push({ /* epsilon */ sa::make_empty });
                        break;
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
                        /* epsilon */
                        break;
                    case tok_index<hat>:
                    case tok_index<dot>:
                    case tok_index<dollar>:
                    case tok_index<lparen>:
                    case tok_index<char_str>:
                    case tok_index<char_class>:
                    case tok_index<backref>:
                        stack.push({ nt::G, nt::F_, sa::make_concat });
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
                        stack.push({ dot{}, sa::make_dot });
                        break;
                    case tok_index<hat>:
                        stack.push({ hat{}, sa::make_hat });
                        break;
                    case tok_index<dollar>:
                        stack.push({ dollar{}, sa::make_dollar });
                        break;
                    case tok_index<lparen>:
                        stack.push({ sa::cap_push, lparen{}, nt::P, rparen{}, sa::cap_pop });
                        break;
                    case tok_index<char_str>:
                        stack.push({ char_str{}, sa::make_char_lit });
                        break;
                    case tok_index<char_class>:
                        stack.push({ char_class{}, sa::make_char_class });
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
                        stack.push({ nt::E });
                        break;
                    case tok_index<quest>:
                        stack.push({ sa::cap_parse_flag, quest{}, sa::cap_parse_flag_done , nt::E });
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
                case sa::make_empty:
                    {                 
                        semstack.push(new_expression<char_str>(/* empty string */));
                    }
                    break;
                case sa::make_dot:
                    {
                        std::ignore = semstack.pop(); /* pop tok::dot */                        

                        /* depending on flags, insert true wildcard instead of [^\n] */
                        if (capstack.dotall())
                        { 
                            using uct = char_class::underlying_char_type;
                            char_class result{ false };
                            result.data.insert(std::numeric_limits<uct>::min(), std::numeric_limits<uct>::max());
                            semstack.push(new_expression<char_class>(std::move(result)));
                        }
                        else
                        {
                            char_class result{ true };
                            result.data.insert('\n');
                            semstack.push(new_expression<char_class>(std::move(result)));
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
                case sa::make_char_lit:
                    {
                        char_str lit{ std::get<char_str>(std::get<terminal>(semstack.pop())) }; /* pop tok::char_str */

                        bool normal_insert{ true };

                        if (capstack.caseless())
                        {
                            static constexpr auto is_alphabetic = [](CharT c) -> bool { return ('A' <= c and c <= 'Z') or ('a' <= c and c <= 'z'); };

                            if (auto c{ lit.get_if_single() })
                            {
                                /* single character (therefore easier to implement) */

                                if (is_alphabetic(*c))
                                {
                                    const auto new_idx{ new_expression<char_class>() }; 
                                    auto& target{ std::get<char_class>(expressions_.at(new_idx)).data };
                                    semstack.push(new_idx);

                                    target.insert(*c);
                                    target.make_caseless();
                                    normal_insert = false;
                                }
                            }
                            else if (not lit.data.empty())
                            {
                                /* several characters (need to insert concat) */

                                if constexpr(char_is_multibyte<CharT>)
                                {
                                    // TODO: implement this using utf32 proxy iterators
                                    throw tree_error("Caseless flag on multibyte strings not implemented");
                                }
                                
                                auto lit_it{ std::ranges::begin(lit.data) };
                                const auto lit_end{ std::ranges::end(lit.data) };

                                if (std::ranges::any_of(lit_it, lit_end, is_alphabetic))
                                {
                                    /* create new concat to insert caseless string into */
                                    const auto cat_idx{ new_expression<concat>() }; 
                                    semstack.push(cat_idx);

                                    while (lit_it != lit_end)
                                    {
                                        const auto c{ *lit_it++ };

                                        if (is_alphabetic(c))
                                        {
                                            /* insert character class of [cC] into cat */
                                            const auto new_idx{ new_expression<char_class>() }; 
                                            auto& target{ std::get<char_class>(expressions_.at(new_idx)).data };
                                            std::get<concat>(expressions_.at(cat_idx)).idxs.push_back(new_idx);

                                            target.insert(c);
                                            target.make_caseless();
                                            
                                        }
                                        else
                                        {
                                            /* insert character string into cat */
                                            const auto new_idx{ new_expression<char_str>() }; 
                                            auto& target{ std::get<char_str>(expressions_.at(new_idx)).data };

                                            target.push_back(c);

                                            while (lit_it != lit_end and is_alphabetic(*lit_it))
                                                target.push_back(*lit_it++);

                                            std::get<concat>(expressions_.at(cat_idx)).idxs.push_back(new_idx);
                                        }
                                    }

                                    normal_insert = false;
                                }
                            }
                        }
                        
                        if (normal_insert)
                            semstack.push(new_expression<char_str>(std::move(lit)));
                        
                    }
                    break;
                case sa::make_char_class:
                    {
                        char_class cc{ std::get<char_class>(std::get<terminal>(semstack.pop())) }; /* pop tok::char_str */

                        if (capstack.caseless())
                            cc.data.make_caseless();
                        
                        semstack.push(new_expression<char_class>(std::move(cc)));
                    }
                    break;
                case sa::make_alt:
                    {
                        const auto rhs_idx{ std::get<std::size_t>(semstack.pop()) };
                        std::ignore = semstack.pop(); /* pop tok::vert */
                        const auto lhs_idx{ std::get<std::size_t>(semstack.pop()) };

                        bool merged{ false };                        

                        if (type& ast{ expressions_.at(rhs_idx) }; std::holds_alternative<alt>(ast))
                        {
                            semstack.push(rhs_idx);
                            auto& ast_alt{ std::get<alt>(ast) };
                            
                            if (flags_.enable_alttocc and not ast_alt.idxs.empty())
                            {
                                /* attempt to replace a|b with [ab] */
                                type& rhs{ expressions_.at(ast_alt.idxs.front()) };
                                type& lhs{ expressions_.at(lhs_idx) };

                                if (std::holds_alternative<char_class>(rhs))
                                {
                                    auto& target{ std::get<char_class>(rhs).data };

                                    if (std::holds_alternative<char_class>(lhs))
                                    {
                                        /* merge char classes */
                                        auto& other{ std::get<char_class>(lhs).data };
                                        target.insert(other);
                                        overwritable_.push_back(lhs_idx);
                                        merged = true;
                                    }
                                    else if (std::holds_alternative<char_str>(lhs))
                                    {
                                        if (auto to_insert{ std::get<char_str>(rhs).get_if_single() })
                                        {
                                            /* insert char into char class */
                                            target.insert(*to_insert);
                                            overwritable_.push_back(lhs_idx);
                                            merged = true;
                                        }
                                    }
                                }
                                else if (std::holds_alternative<char_str>(rhs))
                                {
                                    const auto saved_idx{ ast_alt.idxs.front() }; 

                                    if (auto to_insert{ std::get<char_str>(rhs).get_if_single() })
                                    {
                                        if (std::holds_alternative<char_class>(lhs))
                                        {
                                            /* replace rhs with lhs in alt, and insert char into char class */
                                            auto& target{ std::get<char_class>(lhs).data };
                                            ast_alt.idxs.front() = lhs_idx;
                                            target.insert(*to_insert);
                                            overwritable_.push_back(saved_idx);
                                            merged = true;
                                        }
                                        else if (std::holds_alternative<char_str>(lhs))
                                        {
                                            if (auto other_insert{ std::get<char_str>(lhs).get_if_single() })
                                            {
                                                /* replace rhs with new char class in alt */
                                                const auto new_idx{ new_expression<char_class>() }; 
                                                auto& target{ std::get<char_class>(expressions_.at(new_idx)).data };

                                                /* calling new_expression invalidates references, so we must re-get ast_alt */ 
                                                auto& ast_alt2{ std::get<alt>(expressions_.at(rhs_idx)) };
                                                ast_alt2.idxs.front() = new_idx;

                                                target.insert(*to_insert);
                                                target.insert(*other_insert);
                                                overwritable_.push_back(lhs_idx);
                                                overwritable_.push_back(saved_idx);
                                                merged = true;
                                            }
                                        }
                                    }
                                }
                            }
                                
                            if (not merged)
                            {
                                /* insert lhs into existing alt */
                                ast_alt.idxs.insert(ast_alt.idxs.begin(), lhs_idx);
                            }
                        }
                        else
                        {
                            if (flags_.enable_alttocc)
                            {
                                /* attempt to replace a|b with [ab] */
                                type& rhs{ expressions_.at(rhs_idx) };
                                type& lhs{ expressions_.at(lhs_idx) };

                                if (std::holds_alternative<char_class>(rhs))
                                {
                                    auto& target{ std::get<char_class>(rhs).data };

                                    if (std::holds_alternative<char_class>(lhs))
                                    {
                                        /* merge char classes */
                                        auto& other{ std::get<char_class>(lhs).data };
                                        semstack.push(rhs_idx);
                                        target.insert(other);
                                        overwritable_.push_back(lhs_idx);
                                        merged = true;
                                    }
                                    else if (std::holds_alternative<char_str>(lhs))
                                    {
                                        if (auto to_insert{ std::get<char_str>(lhs).get_if_single() })
                                        {
                                            /* insert char into char class */
                                            semstack.push(rhs_idx);
                                            target.insert(*to_insert);
                                            overwritable_.push_back(lhs_idx);
                                            merged = true;
                                        }
                                    }
                                }
                                else if (std::holds_alternative<char_str>(rhs))
                                {
                                    if (auto to_insert{ std::get<char_str>(rhs).get_if_single() })
                                    {
                                        if (std::holds_alternative<char_class>(lhs))
                                        {
                                            /* insert (rhs) char into (lhs) char class */
                                            auto& target{ std::get<char_class>(lhs).data };
                                            semstack.push(lhs_idx);
                                            target.insert(*to_insert);
                                            overwritable_.push_back(rhs_idx);
                                            merged = true;
                                        }
                                        else if (std::holds_alternative<char_str>(lhs))
                                        {
                                            if (auto other_insert{ std::get<char_str>(lhs).get_if_single() })
                                            {
                                                /* create new char class */
                                                const auto new_idx{ new_expression<char_class>() };
                                                auto& target{ std::get<char_class>(expressions_.at(new_idx)).data };
                                                semstack.push(new_idx);
                                                
                                                target.insert(*to_insert);
                                                target.insert(*other_insert);
                                                overwritable_.push_back(lhs_idx);
                                                overwritable_.push_back(rhs_idx);
                                                merged = true;
                                            }
                                        }
                                    }
                                }
                            }

                            if (not merged)
                            {
                                /* create new alt */
                                semstack.push(new_expression<alt>(std::vector<std::size_t>{ lhs_idx, rhs_idx }));
                            }
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
                                    auto& target{ std::get<char_str>(rhs).data };
                                    auto& lhs_str{ std::get<char_str>(lhs).data };
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
                                auto& target{ std::get<char_str>(rhs).data };
                                auto& lhs_str{ std::get<char_str>(lhs).data };
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
                        {
                            type& ast{ expressions_.at(child_idx) };

                            if (char_str* lit{ std::get_if<char_str>(&ast) }; lit != nullptr and lit->data.empty())
                            {
                                /* empty capturing group; only insert one tag */

                                tag_number_t tag_num{ tag_count_++ };
                                capture_info_.insert(*cap_number, tag_num, tag_num);

                                if (tag_num < 0)
                                    throw tree_error("Capture limit exceed");

                                overwritable_.push_back(child_idx);
                                semstack.push(new_expression<tag>(tag_num));
                            }
                            else if (std::holds_alternative<concat>(ast))
                            {
                                /* insert tags on either end of existing concat */
                                tag_number_t lhs_tag{ tag_count_++ };
                                tag_number_t rhs_tag{ tag_count_++ };
                                capture_info_.insert(*cap_number, lhs_tag, rhs_tag);

                                if (lhs_tag < 0 or rhs_tag < 0)
                                    throw tree_error("Capture limit exceed");
                                
                                const auto lhs_tag_entry{ new_expression<tag>(lhs_tag) };
                                const auto rhs_tag_entry{ new_expression<tag>(rhs_tag) };

                                /* calling new_expression invalidates references, so we must re-get ast for target */ 
                                auto& target{ std::get<concat>(expressions_.at(child_idx)).idxs };
                                target.insert(target.cbegin(), lhs_tag_entry);
                                target.insert(target.cend(), rhs_tag_entry);
                                semstack.push(child_idx);
                            }
                            else
                            {
                                /* create new concat and put tags on either side */
                                tag_number_t lhs_tag{ tag_count_++ };
                                tag_number_t rhs_tag{ tag_count_++ };
                                capture_info_.insert(*cap_number, lhs_tag, rhs_tag);

                                if (lhs_tag < 0 or rhs_tag < 0)
                                    throw tree_error("Capture limit exceed");

                                const auto lhs_tag_entry{ new_expression<tag>(lhs_tag) };
                                const auto rhs_tag_entry{ new_expression<tag>(rhs_tag) };
                                semstack.push(new_expression<concat>(std::vector{ lhs_tag_entry, child_idx, rhs_tag_entry }));
                            }
                        }
                        else
                        {
                            semstack.push(child_idx); /* non capturing group */
                        }
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
                            capstack.set_flag_assigning();
                            for (bool loop{ true }; loop;)
                            {
                                if (lit == lend)
                                    throw pattern_error("Invalid Pattern"); 

                                const auto lookahead{ *lit };
                                bool increment{ true };

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
                                    capstack.set_non_capturing();
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
                            break;
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

            case ast_index<tag>:
                {
                    const auto& tag_entry{ std::get<tag>(entry) };

                    auto& vec{ tag_vec.at(idx) };
                    vec.emplace_back(tag_entry.number);
                    stack.pop_back();
                }
                break;

            default:
                throw tree_error("Invalid tree");
            }
        }
    }
}