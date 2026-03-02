// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "tree.hpp"

#include <ranges>

#include "rx/api/regex_error.hpp"
#include "rx/ast/capstack.hpp"


namespace rx::detail::parser
{
    template<typename CharT>
    class ll1
    {
    public:
        using char_type = CharT;
        using ast_t = expr_tree<char_type>;
        using sv_type = std::basic_string_view<char_type>;

        constexpr ll1(ast_t& ast, sv_type sv);

    private:
        using assertion  = ast_t::assertion;
        using alt        = ast_t::alt;
        using concat     = ast_t::concat;
        using tag        = ast_t::tag;
        using backref    = ast_t::backref;
        using repeat     = ast_t::repeat;
        using char_str   = ast_t::char_str;
        using char_class = ast_t::char_class;
        using type       = ast_t::type;

        [[nodiscard]] constexpr std::size_t sa_make_empty();
        [[nodiscard]] constexpr std::size_t sa_make_dot();
        [[nodiscard]] constexpr std::size_t sa_make_hat();
        [[nodiscard]] constexpr std::size_t sa_make_dollar();
        [[nodiscard]] constexpr std::size_t sa_make_assert(assertion&& as);
        [[nodiscard]] constexpr std::size_t sa_make_char_lit(char_str&& lit);
        [[nodiscard]] constexpr std::size_t sa_make_char_class(char_class&& cc);
        [[nodiscard]] constexpr std::size_t sa_make_alt(std::size_t lhs_idx, std::size_t rhs_idx);
        [[nodiscard]] constexpr std::size_t sa_make_concat(std::size_t lhs_idx, std::size_t rhs_idx);
        [[nodiscard]] constexpr std::size_t sa_make_bref(backref bref);
        [[nodiscard]] constexpr std::size_t sa_make_star(std::size_t child_idx, repeater_mode mode);
        [[nodiscard]] constexpr std::size_t sa_make_plus(std::size_t child_idx, repeater_mode mode);
        [[nodiscard]] constexpr std::size_t sa_make_quest(std::size_t child_idx, repeater_mode mode);
        [[nodiscard]] constexpr std::size_t sa_make_repeat(std::size_t child_idx, tok::repeat_n_m rep, repeater_mode mode);
        [[nodiscard]] constexpr repeater_mode sa_rep_greedy() const;
        [[nodiscard]] constexpr repeater_mode sa_rep_lazy() const;
        [[nodiscard]] constexpr repeater_mode sa_rep_possessive() const;
        [[nodiscard]] constexpr std::size_t sa_cap_pop(std::size_t child_idx);
        constexpr void sa_cap_push();
        constexpr void sa_cap_parse_flag();
        constexpr void sa_begin_alt();

        [[nodiscard]] constexpr parser_flags& flags() { return ref_.get().flags_; }
        [[nodiscard]] constexpr const parser_flags& flags() const { return ref_.get().flags_; }
        [[nodiscard]] constexpr type& get_expr(std::size_t idx) { return ref_.get().expressions_.at(idx); }
        [[nodiscard]] constexpr std::size_t& root_idx() { return ref_.get().root_idx_; }
        [[nodiscard]] constexpr capture_info& get_capture_info() { return ref_.get().capture_info_; }
        [[nodiscard]] constexpr tag_number_t& tag_count() { return ref_.get().tag_count_; }

        template<typename T>
        static constexpr std::size_t tok_index{ index_in_variant(^^T, ^^typename lexer<char_type>::token_t) };

        template<in_variant<type> T, typename... Args>
        [[nodiscard]] constexpr std::size_t new_expression(Args... args)
        {
            auto& exprs{ ref_.get().expressions_ };

            if (overwritable_.empty())
            {
                const std::size_t idx{ exprs.size() };
                exprs.emplace_back(std::in_place_type<T>, std::forward<Args>(args)...);
                return idx;
            }
            else
            {
                const std::size_t idx{ overwritable_.back() };
                overwritable_.pop_back();
                exprs.at(idx) = T{ std::forward<Args>(args)... };
                return idx;
            }
        }

        std::reference_wrapper<ast_t> ref_;
        std::vector<std::size_t> overwritable_;
        lexer<char_type> lex_;
        capture_stack capstack_;
    };


    /* helper classes (and enum) for parser */

    enum class nonterminal : std::uint_least8_t { S, E, E_, F, F_, G, R, R_, H, P };

    enum class semantic_action : std::int_least8_t
    {
        make_empty,
        make_dot,
        make_hat,
        make_dollar,
        make_assert,

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
        using char_type = CharT;
        using terminal = lexer<char_type>::token_t;
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
        using char_type = CharT;
        using terminal = ll1_stack<char_type>::terminal;
        using elem_t = std::variant<std::size_t, typename lexer<char_type>::token_t, repeater_mode>;

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


    /* parser implemenation */

    template<typename CharT>
    constexpr ll1<CharT>::ll1(ast_t& ast, const sv_type sv)
        : ref_{ ast }, lex_{ sv }
    {
        using terminal = ll1_stack<char_type>::terminal;

        if (sv.empty())
        {
            /* ensure expressions is non-empty */
            std::ignore = new_expression<char_str>(/* empty string */);
            return;
        }

        ll1_stack<char_type> stack;
        semantic_stack<char_type> semstack;

        auto token{ lex_.nexttok() };

        for (bool loop{ true }; loop;)
        {
            if (stack.empty())
                throw pattern_error("Invalid pattern");

            const auto top{ std::move(stack.root()) };
            stack.pop();

            if (const auto* const term{ get_if<terminal>(&top) })
            {
                if (holds_alternative<tok::end_of_input>(*term))
                {
                    /* parsing is done */
                    loop = false;
                }
                else if (term->index() == token.index())
                {
                    /* match */
                    semstack.push(std::move(token));
                    token = lex_.nexttok();
                }
                else
                {
                    /* no match */
                    throw pattern_error("Parse Error");
                }
            }
            else if (const auto* const nonterm{ get_if<nonterminal>(&top) })
            {
                /* predict */

                using nt = nonterminal;
                using sa = semantic_action;
                using namespace tok;

                switch (*nonterm)
                {
                case nt::S:
                    switch (token.index())
                    {
                    case tok_index<end_of_input>:
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<assertion>:
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
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<assertion>:
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
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<assertion>:
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
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<assertion>:
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
                    case tok_index<dot>:
                    case tok_index<hat>:
                    case tok_index<dollar>:
                    case tok_index<assertion>:
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
                    case tok_index<assertion>:
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
                    case tok_index<assertion>:
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
                    case tok_index<assertion>:
                        stack.push({ assertion{}, sa::make_assert });
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
                    case tok_index<assertion>:
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
                        stack.push({ sa::cap_parse_flag, quest{}, sa::cap_parse_flag_done, nt::E });
                        break;
                    default:
                        throw pattern_error("Invalid pattern");
                    }
                    break;
                }
            }
            else if (const auto* const action{ get_if<semantic_action>(&top) })
            {
                using sa = semantic_action;

                switch (*action)
                {
                case sa::make_empty:
                {
                    semstack.push(sa_make_empty());
                    break;
                }
                case sa::make_dot:
                {
                    std::ignore = semstack.pop(); /* pop tok::dot */
                    semstack.push(sa_make_dot());
                    break;
                }
                case sa::make_hat:
                {
                    std::ignore = semstack.pop(); /* pop tok::hat */
                    semstack.push(sa_make_hat());
                    break;
                }
                case sa::make_dollar:
                {
                    std::ignore = semstack.pop(); /* pop tok::dollar */
                    semstack.push(sa_make_dollar());
                    break;
                }
                case sa::make_assert:
                {
                    assertion as{ get<assertion>(get<terminal>(semstack.pop())) }; /* pop tok::assertion */
                    semstack.push(sa_make_assert(std::move(as)));
                    break;
                }
                case sa::make_char_lit:
                {
                    char_str lit{ get<char_str>(get<terminal>(semstack.pop())) }; /* pop tok::char_str */
                    semstack.push(sa_make_char_lit(std::move(lit)));
                    break;
                }
                case sa::make_char_class:
                {
                    char_class cc{ get<char_class>(get<terminal>(semstack.pop())) }; /* pop tok::char_str */
                    semstack.push(sa_make_char_class(std::move(cc)));
                    break;
                }
                case sa::make_alt:
                {
                    const auto rhs_idx{ get<std::size_t>(semstack.pop()) };
                    std::ignore = semstack.pop(); /* pop tok::vert */
                    const auto lhs_idx{ get<std::size_t>(semstack.pop()) };
                    semstack.push(sa_make_alt(lhs_idx, rhs_idx));
                    break;
                }
                case sa::make_concat:
                {
                    const auto rhs_idx{ get<std::size_t>(semstack.pop()) };
                    const auto lhs_idx{ get<std::size_t>(semstack.pop()) };
                    semstack.push(sa_make_concat(lhs_idx, rhs_idx));
                    break;
                }
                case sa::make_bref:
                {
                    const auto bref{ get<tok::backref>(get<terminal>(semstack.pop())) }; /* pop tok::backref */
                    semstack.push(sa_make_bref(bref));
                    break;
                }
                case sa::make_star:
                {
                    const auto mode{ get<repeater_mode>(semstack.pop()) };
                    std::ignore = semstack.pop(); /* pop tok::star */
                    const auto child_idx{ get<std::size_t>(semstack.pop()) };
                    semstack.push(sa_make_star(child_idx, mode));
                    break;
                }
                case sa::make_plus:
                {
                    const auto mode{ get<repeater_mode>(semstack.pop()) };
                    std::ignore = semstack.pop(); /* pop tok::plus */
                    const auto child_idx{ get<std::size_t>(semstack.pop()) };
                    semstack.push(sa_make_plus(child_idx, mode));
                    break;
                }
                case sa::make_quest:
                {
                    const auto mode{ get<repeater_mode>(semstack.pop()) };
                    std::ignore = semstack.pop(); /* pop tok::quest */
                    const auto child_idx{ get<std::size_t>(semstack.pop()) };
                    semstack.push(sa_make_quest(child_idx, mode));
                    break;
                }
                case sa::make_repeat:
                {
                    const auto mode{ get<repeater_mode>(semstack.pop()) };
                    const auto rep{ get<tok::repeat_n_m>(get<terminal>(semstack.pop())) };
                    const auto child_idx{ get<std::size_t>(semstack.pop()) };
                    semstack.push(sa_make_repeat(child_idx, rep, mode));
                    break;
                }
                case sa::rep_greedy:
                {
                    semstack.push(sa_rep_greedy());
                    break;
                }
                case sa::rep_lazy:
                {
                    std::ignore = semstack.pop(); /* pop tok::quest */
                    semstack.push(sa_rep_lazy());
                    break;
                }
                case sa::rep_possessive:
                {
                    std::ignore = semstack.pop(); /* pop tok::plus */
                    semstack.push(sa_rep_possessive());
                    break;
                }
                case sa::cap_push:
                {
                    sa_cap_push();
                    break;
                }
                case sa::cap_pop:
                {
                    std::ignore = semstack.pop(); /* pop tok::rparen */
                    const auto child_idx{ get<std::size_t>(semstack.pop()) };
                    std::ignore = semstack.pop(); /* pop tok::lparen */
                    semstack.push(sa_cap_pop(child_idx));
                    break;
                }
                case sa::cap_parse_flag:
                {
                    sa_cap_parse_flag();
                    break;
                }
                case sa::cap_parse_flag_done:
                {
                    std::ignore = semstack.pop(); /* pop tok::quest */
                    /* this exists only so we can use capture_pop(_empty)? for captures with flags */
                    break;
                }
                case sa::begin_alt:
                {
                    /* exists only so branch resetting captures work */
                    sa_begin_alt();
                    break;
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
            root_idx() = get<std::size_t>(semstack.pop());
    }


    /* semantic action implementations */

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_empty()
    {
        return new_expression<char_str>(/* empty string */);
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_dot()
    {
        /* depending on flags, insert true wildcard instead of [^\n] */
        if (capstack_.dotall())
        {
            char_class result{ true };
            result.data.normalise();
            return new_expression<char_class>(std::move(result));
        }
        else
        {
            char_class result{ true };
            result.data.insert('\n');
            result.data.normalise();
            return new_expression<char_class>(std::move(result));
        }
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_hat()
    {
        /* depending on flags, insert assert_type::line_start instead of assert_type::text_start */
        if (capstack_.multiline())
            return new_expression<assertion>(assert_type::line_start);
        else
            return new_expression<assertion>(assert_type::text_start);
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_dollar()
    {
        /* depending on flags, insert assert_type::line_end instead of assert_type::text_end */
        if (capstack_.multiline())
            return new_expression<assertion>(assert_type::line_end);
        else
            return new_expression<assertion>(assert_type::text_end);
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_assert(assertion&& as)
    {
        /* todo: perform checks on implementability here */
        return new_expression<assertion>(std::move(as));
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_char_lit(char_str&& lit)
    {
        if (capstack_.caseless())
        {
            static constexpr auto is_alphabetic = [](char_type c) -> bool { return ('A' <= c and c <= 'Z') or ('a' <= c and c <= 'z'); };

            if (auto c{ lit.get_if_single() })
            {
                /* single character (therefore easier to implement) */

                if (is_alphabetic(*c))
                {
                    const auto new_idx{ new_expression<char_class>() };
                    auto& target{ get<char_class>(get_expr(new_idx)).data };

                    target.insert(*c);
                    target.make_caseless();
                    return new_idx;
                }
            }
            else if (not lit.data.empty())
            {
                /* several characters (need to insert concat) */

                if constexpr (char_is_multibyte<char_type>)
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

                    while (lit_it != lit_end)
                    {
                        const auto c{ *lit_it++ };

                        if (is_alphabetic(c))
                        {
                            /* insert character class of [cC] into cat */
                            const auto new_idx{ new_expression<char_class>() };
                            auto& target{ get<char_class>(get_expr(new_idx)).data };
                            get<concat>(get_expr(cat_idx)).idxs.push_back(new_idx);

                            target.insert(c);
                            target.make_caseless();
                        }
                        else
                        {
                            /* insert character string into cat */
                            const auto new_idx{ new_expression<char_str>() };
                            auto& target{ get<char_str>(get_expr(new_idx)).data };

                            target.push_back(c);

                            while (lit_it != lit_end and is_alphabetic(*lit_it))
                                target.push_back(*lit_it++);

                            get<concat>(get_expr(cat_idx)).idxs.push_back(new_idx);
                        }
                    }

                    return cat_idx;
                }
            }
        }

        return new_expression<char_str>(std::move(lit));
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_char_class(char_class&& cc)
    {
        if (capstack_.caseless())
            cc.data.make_caseless();

        return new_expression<char_class>(std::move(cc));
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_alt(const std::size_t lhs_idx, const std::size_t rhs_idx)
    {
        if (type& ast{ get_expr(rhs_idx) }; holds_alternative<alt>(ast))
        {
            auto& ast_alt{ get<alt>(ast) };

            if (flags().enable_alttocc and not ast_alt.idxs.empty())
            {
                /* attempt to replace a|b with [ab] */
                type& rhs{ get_expr(ast_alt.idxs.front()) };
                type& lhs{ get_expr(lhs_idx) };

                if (holds_alternative<char_class>(rhs))
                {
                    auto& target{ get<char_class>(rhs).data };

                    if (holds_alternative<char_class>(lhs))
                    {
                        /* merge char classes */
                        auto& other{ get<char_class>(lhs).data };
                        target.insert(other);
                        overwritable_.push_back(lhs_idx);
                        return rhs_idx;
                    }
                    else if (holds_alternative<char_str>(lhs))
                    {
                        if (auto to_insert{ get<char_str>(lhs).get_if_single() })
                        {
                            /* insert char into char class */
                            target.insert(*to_insert);
                            overwritable_.push_back(lhs_idx);
                            return rhs_idx;
                        }
                    }
                }
                else if (holds_alternative<char_str>(rhs))
                {
                    const auto saved_idx{ ast_alt.idxs.front() };

                    if (auto to_insert{ get<char_str>(rhs).get_if_single() })
                    {
                        if (holds_alternative<char_class>(lhs))
                        {
                            /* replace rhs with lhs in alt, and insert char into char class */
                            auto& target{ get<char_class>(lhs).data };
                            ast_alt.idxs.front() = lhs_idx;
                            target.insert(*to_insert);
                            overwritable_.push_back(saved_idx);
                            return rhs_idx;
                        }
                        else if (holds_alternative<char_str>(lhs))
                        {
                            if (auto other_insert{ get<char_str>(lhs).get_if_single() })
                            {
                                /* replace rhs with new char class in alt */
                                const auto new_idx{ new_expression<char_class>() };
                                auto& target{ get<char_class>(get_expr(new_idx)).data };

                                /* calling new_expression invalidates references, so we must re-get ast_alt */
                                auto& ast_alt2{ get<alt>(get_expr(rhs_idx)) };
                                ast_alt2.idxs.front() = new_idx;

                                target.insert(*to_insert);
                                target.insert(*other_insert);
                                overwritable_.push_back(lhs_idx);
                                overwritable_.push_back(saved_idx);
                                return rhs_idx;
                            }
                        }
                    }
                }
            }

            /* insert lhs into existing alt */
            ast_alt.idxs.insert(ast_alt.idxs.begin(), lhs_idx);
            return rhs_idx;
        }
        else
        {
            if (flags().enable_alttocc)
            {
                /* attempt to replace a|b with [ab] */
                type& rhs{ get_expr(rhs_idx) };
                type& lhs{ get_expr(lhs_idx) };

                if (holds_alternative<char_class>(rhs))
                {
                    auto& target{ get<char_class>(rhs).data };

                    if (holds_alternative<char_class>(lhs))
                    {
                        /* merge char classes */
                        auto& other{ get<char_class>(lhs).data };
                        target.insert(other);
                        overwritable_.push_back(lhs_idx);
                        return rhs_idx;
                    }
                    else if (holds_alternative<char_str>(lhs))
                    {
                        if (auto to_insert{ get<char_str>(lhs).get_if_single() })
                        {
                            /* insert char into char class */
                            target.insert(*to_insert);
                            overwritable_.push_back(lhs_idx);
                            return rhs_idx;
                        }
                    }
                }
                else if (holds_alternative<char_str>(rhs))
                {
                    if (auto to_insert{ get<char_str>(rhs).get_if_single() })
                    {
                        if (holds_alternative<char_class>(lhs))
                        {
                            /* insert (rhs) char into (lhs) char class */
                            auto& target{ get<char_class>(lhs).data };
                            target.insert(*to_insert);
                            overwritable_.push_back(rhs_idx);
                            return lhs_idx;
                        }
                        else if (holds_alternative<char_str>(lhs))
                        {
                            if (auto other_insert{ get<char_str>(lhs).get_if_single() })
                            {
                                /* create new char class */
                                const auto new_idx{ new_expression<char_class>() };
                                auto& target{ get<char_class>(get_expr(new_idx)).data };

                                target.insert(*to_insert);
                                target.insert(*other_insert);
                                overwritable_.push_back(lhs_idx);
                                overwritable_.push_back(rhs_idx);
                                return new_idx;
                            }
                        }
                    }
                }
            }

            /* create new alt */
            return new_expression<alt>(std::vector<std::size_t>{ lhs_idx, rhs_idx });
        }
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_concat(const std::size_t lhs_idx, const std::size_t rhs_idx)
    {
        if (type& ast{ get_expr(lhs_idx) }; holds_alternative<concat>(ast))
        {
            /* append rhs into existing concat */
            /* this case _should_ only arise from captures, so we can skip merging strings */
            auto& target{ get<concat>(ast).idxs };

            type& rhs{ get_expr(rhs_idx) };

            if (holds_alternative<concat>(rhs))
            {
                /* append contents of rhs concat to lhs concat  */
                auto& src{ get<concat>(rhs).idxs };
                target.append_range(src);
                src.clear();
                overwritable_.push_back(rhs_idx);
            }
            else
            {
                /* append rhs to existing concat */
                target.push_back(rhs_idx);
            }

            return lhs_idx;
        }
        else if (type& ast{ get_expr(rhs_idx) }; holds_alternative<concat>(ast))
        {
            /* insert lhs into existing concat */
            auto& ast_concat{ get<concat>(ast) };
            bool merged{ false };

            if (not ast_concat.idxs.empty())
            {
                type& rhs{ get_expr(ast_concat.idxs.front()) };
                type& lhs{ get_expr(lhs_idx) };

                if (holds_alternative<char_str>(rhs) and holds_alternative<char_str>(lhs))
                {
                    /* merge lhs string with first entry of rhs (also a string) */
                    auto& target{ get<char_str>(rhs).data };
                    auto& lhs_str{ get<char_str>(lhs).data };
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

            return rhs_idx;
        }
        else
        {
            type& rhs{ get_expr(rhs_idx) };
            type& lhs{ get_expr(lhs_idx) };

            if (holds_alternative<char_str>(rhs) and holds_alternative<char_str>(lhs))
            {
                /* lhs and rhs are both strings: merge strings into one instead of creating concat  */
                auto& target{ get<char_str>(rhs).data };
                auto& lhs_str{ get<char_str>(lhs).data };
                lhs_str.append(target);
                std::swap(lhs_str, target);
                overwritable_.push_back(lhs_idx);
                return rhs_idx;
            }
            else
            {
                /* create new concat */
                return new_expression<concat>(std::vector{ lhs_idx, rhs_idx });
            }
        }
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_bref(const backref bref)
    {
        if (flags().enable_backrefs)
        {
            /* note: we don't perform a check (bref.number <= capture_count) to match the behaviour of other engines */

            return new_expression<backref>(bref.number);
        }
        else
        {
            throw parser_error("Backreferences are not enabled");
        }
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_star(const std::size_t child_idx, const repeater_mode mode)
    {
        return new_expression<repeat>(child_idx, std::int_least16_t{ 0 }, std::int_least16_t{ -1 }, mode);
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_plus(const std::size_t child_idx, const repeater_mode mode)
    {
        return new_expression<repeat>(child_idx, std::int_least16_t{ 1 }, std::int_least16_t{ 0 }, mode);
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_quest(const std::size_t child_idx, const repeater_mode mode)
    {
        return new_expression<repeat>(child_idx, std::int_least16_t{ 0 }, std::int_least16_t{ 1 }, mode);
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_make_repeat(const std::size_t child_idx, const tok::repeat_n_m rep, const repeater_mode mode)
    {
        return new_expression<repeat>(child_idx, rep.min, rep.max, mode);
    }

    template<typename CharT>
    constexpr repeater_mode ll1<CharT>::sa_rep_greedy() const
    {
        /* swap greedy and lazy quantifiers if 'ungreedy' flag is set */
        if (capstack_.ungreedy())
            return repeater_mode::lazy; /* swapped */
        else
            return repeater_mode::greedy; /* default */
    }

    template<typename CharT>
    constexpr repeater_mode ll1<CharT>::sa_rep_lazy() const
    {
        /* swap greedy and lazy quantifiers if 'ungreedy' flag is set */
        if (capstack_.ungreedy())
            return repeater_mode::greedy; /* swapped */
        else
            return repeater_mode::lazy; /* default */
    }

    template<typename CharT>
    constexpr repeater_mode ll1<CharT>::sa_rep_possessive() const
    {
        if (flags().enable_possessive)
            return repeater_mode::possessive;
        else
            throw parser_error("Possessive repetition is not enabled");
    }

    template<typename CharT>
    constexpr std::size_t ll1<CharT>::sa_cap_pop(const std::size_t child_idx)
    {
        const auto cap_number{ capstack_.pop() };

        if (cap_number.has_value())
        {
            type& ast{ get_expr(child_idx) };

            if (char_str* lit{ get_if<char_str>(&ast) }; lit != nullptr and lit->data.empty())
            {
                /* empty capturing group; only insert one tag */

                const tag_number_t tag_num{ tag_count()++ };
                get_capture_info().insert(*cap_number, tag_num, tag_num);

                if (tag_num < 0)
                    throw tree_error("Capture limit exceed");

                overwritable_.push_back(child_idx);
                return new_expression<tag>(tag_num);
            }
            else if (holds_alternative<concat>(ast))
            {
                /* insert tags on either end of existing concat */
                const tag_number_t lhs_tag{ tag_count()++ };
                const tag_number_t rhs_tag{ tag_count()++ };
                get_capture_info().insert(*cap_number, lhs_tag, rhs_tag);

                if (lhs_tag < 0 or rhs_tag < 0)
                    throw tree_error("Capture limit exceed");

                const auto lhs_tag_entry{ new_expression<tag>(lhs_tag) };
                const auto rhs_tag_entry{ new_expression<tag>(rhs_tag) };

                /* calling new_expression invalidates references, so we must re-get ast for target */
                auto& target{ get<concat>(get_expr(child_idx)).idxs };
                target.insert(target.cbegin(), lhs_tag_entry);
                target.insert(target.cend(), rhs_tag_entry);
                return child_idx;
            }
            else
            {
                /* create new concat and put tags on either side */
                const tag_number_t lhs_tag{ tag_count()++ };
                const tag_number_t rhs_tag{ tag_count()++ };
                get_capture_info().insert(*cap_number, lhs_tag, rhs_tag);

                if (lhs_tag < 0 or rhs_tag < 0)
                    throw tree_error("Capture limit exceed");

                const auto lhs_tag_entry{ new_expression<tag>(lhs_tag) };
                const auto rhs_tag_entry{ new_expression<tag>(rhs_tag) };
                return new_expression<concat>(std::vector{ lhs_tag_entry, child_idx, rhs_tag_entry });
            }
        }
        else
        {
            return child_idx; /* non capturing group */
        }
    }

    template<typename CharT>
    constexpr void ll1<CharT>::sa_cap_push()
    {
        if (flags().enable_captures)
        {
            if (capstack_.push()) /* i.e. unsigned integer overflow has occurred */
                throw pattern_error("Capturing group limit exceeded");
        }
        else
        {
            capstack_.push_non_capturing();
        }
    }

    template<typename CharT>
    constexpr void ll1<CharT>::sa_cap_parse_flag()
    {
        /* manually parse flags */

        auto& lit{ lex_.it_ };
        const auto& lend{ lex_.end_ };

        bool flag_value{ true };

        if (lit == lend)
            return;

        switch (*lit)
        {
        case '#':
            ++lit;
            while (lit != lend and *lit != ')')
                ++lit; /* skip comment */
            capstack_.set_non_capturing();
            break;

        case '|':
            if (not flags().enable_branchreset)
                throw parser_error("Branch resetting in captures is not enabled");
            ++lit;
            capstack_.set_branch_reset();
            break;

        case '>':
            throw pattern_error("Atomic capture groups are unsupported");

        case 'P':
        case '<':
        case '\'':
            throw pattern_error("Named capture groups are unsupported");

        default: /* parse options */
            capstack_.set_flag_assigning();
            for (bool loop{ true }; loop;)
            {
                if (lit == lend)
                    throw pattern_error("Invalid Pattern");

                const auto lookahead{ *lit };
                bool increment{ true };

                switch (lookahead)
                {
                case 'i':
                    capstack_.set_caseless(flag_value);
                    break;
                case 'm':
                    capstack_.set_multiline(flag_value);
                    break;
                case 's':
                    capstack_.set_dotall(flag_value);
                    break;
                case 'U':
                    capstack_.set_ungreedy(flag_value);
                    break;
                case 'x':
                    throw pattern_error("Capturing group flag 'x' is unsupported");
                case '-':
                    if (not flag_value)
                        throw pattern_error("Capturing group arguments can only contain one hyphen");
                    flag_value = false;
                    break;

                case ':':
                    capstack_.set_non_capturing();
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

    template<typename CharT>
    constexpr void ll1<CharT>::sa_begin_alt()
    {
        if (flags().enable_branchreset)
            capstack_.branch_reset_if_enabled();
    }
}

namespace rx::detail
{
    /* constructor for tree */

    template<typename CharT>
    constexpr expr_tree<CharT>::expr_tree(const sv_type sv, const parser_flags flags)
        : flags_{ flags }
    {
        parser::ll1<char_type> ll1_parser(*this, sv);
    }
}
