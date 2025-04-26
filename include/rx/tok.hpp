#pragma once

#include <string_view>
#include <numeric>
#include <variant>

#include <rx/error.hpp>

namespace rx::detail
{
    /* RE2 limits counted repetitions to 1000 - we do the same here.
     * Note: clang reaches the consteval step limit with >512 repetitions */
    inline static constexpr std::int16_t counted_repetition_limit{ 1000 };

    /* token definitions */

    namespace tok
    {
        struct end_of_input {};
        struct vert {};
        struct dot {};
        struct hat {};
        struct dollar {};
        struct lparen {};
        struct rparen {};
        struct star {};
        struct plus {};
        struct quest {};
        
        struct repeat_n_m
        {
            std::int_least16_t min;
            std::int_least16_t max; /* use max=min for {min} or max<min for {min,} */ 
        };

        template<typename CharT>
        struct char_lit
        {
            CharT c;
        };

        template<typename CharT>
        struct char_class
        {
            // TODO: implement
            std::basic_string_view<CharT> name;
        };

        struct backref
        {
            std::uint_least16_t number;
        };
    }        


    /* lexer class definition */

    template<typename CharT>
    class lexer
    {
        using sv_type = std::basic_string_view<CharT>;
        using it_type = sv_type::const_iterator;
    public:
        using token_t = std::variant<tok::end_of_input, tok::dot, tok::hat, tok::dollar,
                                     tok::lparen, tok::rparen, tok::vert,
                                     tok::star, tok::plus, tok::quest, tok::repeat_n_m,
                                     tok::char_lit<CharT>, tok::char_class<CharT>,
                                     tok::backref>;

        constexpr lexer(const sv_type& sv) : it_{ sv.cbegin() }, end_{ sv.cend() } {}; 
        constexpr token_t nexttok();

    private:
        it_type it_;
        it_type end_;
    };


    /* lexer implementation */

    template<typename CharT>
    constexpr lexer<CharT>::token_t lexer<CharT>::nexttok()
    {
        using namespace tok;
        using char_lit = tok::char_lit<CharT>;
        using char_class = tok::char_class<CharT>;

        if (it_ == end_)
            return end_of_input{};

        const auto current{ it_ };

        switch (*it_++) 
        {
        case '(': return lparen{};
        case ')': return rparen{};
        case '.': return dot{};
        case '*': return star{};
        case '+': return plus{};
        case '?': return quest{};
        case '^': return hat{};
        case '$': return dollar{};
        case '|': return vert{};

        case '\\':
            {
                if (it_ == end_)
                    throw pattern_error("Pattern cannot end with '\\'");
                
                switch (*it_++)
                {
                    /* escape sequences */

                case 'a': return char_lit{ '\a' };
                case 'f': return char_lit{ '\f' };
                case 't': return char_lit{ '\t' };
                case 'n': return char_lit{ '\n' };
                case 'r': return char_lit{ '\r' };
                case 'v': return char_lit{ '\v' };

                    /* escaped control chars */

                case '.': return char_lit{ '.' };
                case '^': return char_lit{ '^' };
                case '$': return char_lit{ '$' };
                case '*': return char_lit{ '*' };
                case '+': return char_lit{ '+' };
                case '?': return char_lit{ '?' };
                case '(': return char_lit{ '(' };
                case ')': return char_lit{ ')' };
                case '[': return char_lit{ '[' };
                case ']': return char_lit{ ']' };
                case '{': return char_lit{ '{' };
                case '}': return char_lit{ '}' };
                case '\\': return char_lit{ '\\' };
                    
                    /* perl character classes */

                case 'd':
                case 'D':
                case 's':
                case 'S':
                case 'w':
                case 'W': return char_class{{ current, it_ }};

                default: throw pattern_error("Invalid control character");
                }
            }

        case '{': 
        {
            static constexpr std::int_least16_t base{ 10 };

            std::int_least16_t min{ -1 };
            std::int_least16_t max{ -1 };

            bool parse_min{ true };
            bool parse_max{ true };

            while  (parse_min)
            {
                if (it_ == end_)
                    throw pattern_error("Repeater is incomplete");

                auto c{ *it_++ };

                if ('0' <= c and c <= '9')
                {
                    if (min == -1)
                        min = c - '0';
                    else
                        min = std::add_sat<std::int_least16_t>(std::mul_sat(min, base), c - '0');
                }
                else if (c == ',')
                {
                    parse_min = false;
                }
                else if (c == '}')
                {
                    parse_min = false;
                    
                    /* skip parsing max */
                    max = min;
                    parse_max = false;
                }
                else
                {
                    throw pattern_error("Invalid character in repeater");
                }
            }

            while (parse_max)
            {
                if (it_ == end_)
                    throw pattern_error("Repeater is incomplete");

                auto c{ *it_++ };

                if ('0' <= c and c <= '9')
                {
                    if (max == -1)
                        max = c - '0';
                    else
                        max = std::add_sat<std::int_least16_t>(std::mul_sat(max, base), c - '0');
                }
                else if (c == '}')
                {
                    if (max != -1 and max < min)
                        throw pattern_error("Invalid repeater (max is less than min)");

                    parse_max = false; 
                }
                else
                {
                    throw pattern_error("Invalid character in repeater");
                }
            }

            if (min > counted_repetition_limit)
            {
                if (min == max)
                    throw pattern_error("Finite number of counted repetitions exceeds limit");
                else
                    throw pattern_error("Lower bound of counted repetitions exceeds limit");
            }
            else if (max > counted_repetition_limit)
            {
                throw pattern_error("Finite upper bound of counted repetitions exceeds limit");
            }

            return repeat_n_m{ .min = min, .max = max };
        }

        case '[':
        {
            // TODO: actually parse character class 

            for (int bkt_depth{ 1 }; bkt_depth > 0;)
            {
                if (it_ == end_)
                    throw pattern_error("Character class missing closing bracket");

                auto c{ *it_++ };

                /* NOTE: this code here is incorrect */
                // TODO: IMPLEMENT

                if (c == '[')
                {
                    ++bkt_depth;
                }
                else if (c == ']')
                {
                    --bkt_depth;
                }
                else if (c == '\\')
                {
                    if (it_ == end_)
                        throw pattern_error("Character class missing closing bracket");
                    
                    /* WARNING:
                     * For now, we assume that escape codes are 2 chars long.
                     * This is not actually the case, but it's good enough to 
                     * help us ignore escaped square brackets (at least for now). */
                     ++it_;
                }
            }

            // TODO: if character class only has one character, return a char_lit instead

            return char_class{{ std::next(current), std::next(it_, -1) }};
        }

        default:
            return char_lit{ *current };
        }
    }
}