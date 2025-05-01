#pragma once

#include <limits>
#include <string_view>
#include <numeric>
#include <type_traits>
#include <variant>

#include <rx/error.hpp>

namespace rx::detail
{
    /* RE2 limits counted repetitions to 1000 - we do the same here.
     * Note: clang reaches the consteval step limit with >512 repetitions */
    inline static constexpr std::int_least16_t counted_repetition_limit{ 1000 };

    template<typename CharT>
    class expr_tree;

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

        // TODO: rangify API?

        constexpr lexer(const sv_type& sv) : it_{ sv.cbegin() }, end_{ sv.cend() } {}; 
        constexpr token_t nexttok();

        friend class expr_tree<CharT>;

    private:
        constexpr std::size_t parse_hex(std::size_t fixed_amt);

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
                
                const auto escaped{ *it_++ };

                switch (escaped)
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

                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                {
                    static constexpr std::size_t octal_base{ 010 };
                    std::size_t result{ static_cast<std::size_t>(escaped - '0') };
                    tok::backref bref{ std::saturate_cast<std::uint_least16_t>(escaped - '0') };

                    for (int i{ 0 }; i < 2; ++i)
                    {
                        if (it_ == end_)
                            break;

                        const auto lookahead{ *it_ };

                        if (not ('0' <= lookahead and lookahead <= '7'))
                            break;

                        bref.number = 0;
                        
                        result *= octal_base;
                        result += lookahead - '0';
                        ++it_;
                    }

                    if (bref.number == 0)
                    {
                        if (result > std::numeric_limits<std::make_unsigned_t<CharT>>::max())
                        {
                            // TODO: return string corresponding to multibyte char
                            throw pattern_error("Octal escape sequence out of range");
                        }

                        return tok::char_lit{ static_cast<CharT>(result) };
                    }
                    else
                    {
                        bref.number -= 1;
                        return bref;
                    }
                }

                case '8':
                case '9': return tok::backref{ std::saturate_cast<std::uint_least16_t>(escaped - '0') };

                case 'g':
                {
                    static constexpr std::size_t decimal_base{ 10 };
                    tok::backref bref{ 0 };
                    
                    if (it_ == end_ )
                        throw pattern_error("Incomplete escape sequence");

                    auto next{ *it_++ };

                    if (next == '{')
                    {
                        /* \g{n...} */

                        while (true)
                        {
                            if (it_ == end_)
                                throw pattern_error("Incomplete escape sequence");;
    
                            next = *it_++;

                            if (next == '}')
                                break;
                            if (not ('0' <= next and next <= '9'))
                                throw pattern_error("Incomplete escape sequence");;
                            
                            bref.number *= decimal_base;
                            bref.number += next - '0';
                        }
                    }
                    else if ('0' <= next and next <= '9')
                    {
                        /* \gn */
                        bref.number = next - '0';
                    }
                    else
                    {
                        throw pattern_error("Incomplete escape sequence");
                    }
                 

                    if (bref.number == 0)
                        throw pattern_error("Backreference to non-existent submatch");

                    bref.number -= 1;
                    return bref;
                }

                case 'x':
                {
                    std::size_t result{ parse_hex(0) };

                    if (result > std::numeric_limits<std::make_unsigned_t<CharT>>::max())
                    {
                        // TODO: return string corresponding to multibyte char
                        throw pattern_error("Multibyte characters are unimplemented");
                    }

                    return tok::char_lit{ static_cast<CharT>(result) };
                }

                case 'u':
                {
                    std::size_t result{ parse_hex(4) };

                    if (result > std::numeric_limits<std::make_unsigned_t<CharT>>::max())
                    {
                        // TODO: return string corresponding to multibyte char
                        throw pattern_error("Multibyte characters are unimplemented");
                    }

                    return tok::char_lit{ static_cast<CharT>(result) };
                }

                case 'U':
                {
                    std::size_t result{ parse_hex(8) };

                    if (result > std::numeric_limits<std::make_unsigned_t<CharT>>::max())
                    {
                        // TODO: return string corresponding to multibyte char
                        throw pattern_error("Multibyte characters are unimplemented");
                    }

                    return tok::char_lit{ static_cast<CharT>(result) };
                }


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


    /* helpers for lexer implementation */


    template<typename CharT>
    constexpr std::size_t lexer<CharT>::parse_hex(std::size_t fixed_amt)
    {
        static constexpr std::size_t hexadecimal_base{ 0x10 };
        static constexpr std::size_t decimal_base{ 10 };
        std::size_t result{ 0 };

        if (it_ == end_ )
            throw pattern_error("Incomplete escape sequence");

        const auto lookahead{ *it_ };

        if (lookahead == '{')
        {
            /* arbitrary number of digits contained in {} */
            
            std::size_t digits{ 0 };
            ++it_;

            for (bool loop{ true }; loop; ++digits)
            {
                if (it_ == end_)
                    throw pattern_error("Incomplete escape sequence");;

                const auto c{ *it_ };

                if (c == '}')
                    loop = false;
                else if ('0' <= c and c <= '9')
                    result = (result * hexadecimal_base) + (c - '0');
                else if ('A' <= c and c <= 'F')
                    result = (result * hexadecimal_base) + decimal_base + (c - 'A');
                else if ('a' <= c and c <= 'f')
                    result = (result * hexadecimal_base) + decimal_base + (c - 'a');
                else
                    throw pattern_error("Incomplete escape sequence");

                ++it_;
            }

            if (digits == 0)
                throw pattern_error("Delimited escape sequence cannot be empty");;
        }
        else if (fixed_amt != 0)
        {
            for (std::size_t i{ 0 }; i < fixed_amt; ++i)
            {
                if (it_ == end_)
                    throw pattern_error("Incomplete escape sequence");

                const auto c{ *it_ };

                if ('0' <= c and c <= '9')
                    result = (result * hexadecimal_base) + (c - '0');
                else if ('A' <= c and c <= 'F')
                    result = (result * hexadecimal_base) + decimal_base + (c - 'A');
                else if ('a' <= c and c <= 'f')
                    result = (result * hexadecimal_base) + decimal_base + (c - 'a');
                else
                    throw pattern_error("Incomplete escape sequence");

                ++it_;
            }
        }
        else
        {
            std::size_t digits{ 0 };
            
            for (bool loop{ true }; loop; ++digits)
            {
                if (it_ == end_)
                    break;

                const auto c{ *it_ };

                if ('0' <= c and c <= '9')
                    result = (result * hexadecimal_base) + (c - '0');
                else if ('A' <= c and c <= 'F')
                    result = (result * hexadecimal_base) + decimal_base + (c - 'A');
                else if ('a' <= c and c <= 'f')
                    result = (result * hexadecimal_base) + decimal_base + (c - 'a');
                else
                    loop = false;

                ++it_;
            }

            if (digits == 0)
                throw pattern_error("Invalid escape sequence");
        }


        return result;
    }
}