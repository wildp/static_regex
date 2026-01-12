// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <limits>
#include <numeric>
#include <optional>
#include <string_view>
#include <type_traits>
#include <variant>

#include <rx/api/regex_error.hpp>
#include <rx/ast/charclass.hpp>
#include <rx/etc/util.hpp>


/* Note: We assume the literal character encoding is a superset of ASCII */

namespace rx::detail
{
    /* RE2 limits counted repetitions to 1000 - we do the same here.
     * Note: clang reaches the consteval step limit with >512 repetitions */
    inline constexpr std::int_least16_t counted_repetition_limit{ 1000 };

    enum class assert_type : int_least8_t // TODO: consider moving to another header?
    {
        text_start,
        text_end,
        line_start,
        line_end,
        // text_end_or_newline_before,
        word_boundary,
        not_word_boundary,
    };

    namespace parser
    {
        template<typename CharT>
        class ll1;
    }

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

        struct assertion
        {
            assert_type type;
        };

        template<typename CharT>
        struct char_class
        {
            using impl_type = char_class_impl<std::same_as<char, CharT>>;
            using underlying_char_type = impl_type::char_type;

            impl_type data;

            template<typename... Args>
            constexpr explicit char_class(Args&&... args) : data{ std::forward<Args>(args)... } {}
        };

        template<typename CharT>
        struct char_str
        {
            std::basic_string<CharT> data;

            constexpr explicit char_str() : data{} {} /* empty string */
            constexpr explicit char_str(CharT c) : data{ c } {}

            template<std::input_iterator I, std::sentinel_for<I> S>
            requires std::convertible_to<std::iter_value_t<I>, CharT>
            constexpr explicit char_str(I first, S last) : data(first, last) {}

            constexpr explicit char_str(char c) requires (not std::same_as<CharT, char>)
            {
                data += c;
            }

            constexpr explicit char_str(std::size_t parse_result)
            {
                if (parse_result <= std::numeric_limits<std::make_unsigned_t<CharT>>::max())
                {
                    data = static_cast<CharT>(parse_result);
                }
                else
                {
                    // TODO: construct string corresponding to multibyte char
                    throw pattern_error("Multibyte characters are unimplemented");
                }
            }

            [[nodiscard]] constexpr std::optional<typename char_class<CharT>::underlying_char_type> get_if_single()
            {
                // TODO: update this to be aware of multibyte characters
                if (data.size() == 1)
                    return data.front();
                return {};
            }
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
                                     tok::char_str<CharT>, tok::char_class<CharT>,
                                     tok::backref, tok::assertion>;

        // TODO: rangify API?

        constexpr explicit lexer(const sv_type& sv) : it_{ sv.cbegin() }, end_{ sv.cend() } {}
        constexpr token_t nexttok();

        friend class parser::ll1<CharT>;

    private:
        constexpr std::size_t            parse_hex(std::size_t fixed_amt);
        constexpr std::size_t            parse_remaining_oct(std::size_t init);
        constexpr std::size_t            parse_arbitrary_oct();
        constexpr tok::backref           parse_bref();
        constexpr tok::repeat_n_m        parse_repeat();
        constexpr token_t                parse_bref_or_octal(CharT init);
        constexpr tok::char_str<CharT>   parse_literal_string(it_type begin);
        constexpr tok::char_class<CharT> parse_char_class();
        constexpr named_character_class  parse_posix_char_class();

        it_type it_;
        it_type end_;
    };


    /* lexer implementation */

    template<typename CharT>
    constexpr lexer<CharT>::token_t lexer<CharT>::nexttok()
    {
        using namespace tok;
        using char_str = char_str<CharT>;
        using char_class = char_class<CharT>;

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
            using ncc = named_character_class;

            if (it_ == end_)
                throw pattern_error("Pattern cannot end with '\\'");

            const auto escaped{ *it_++ };

            switch (escaped)
            {
            /* standard escape sequences */

            case 'a': return char_str{ '\a' };
            // case 'b': return char_str{ '\b' }; /* use \010 instead */¬
            case 'f': return char_str{ '\f' };
            case 't': return char_str{ '\t' };
            case 'n': return char_str{ '\n' };
            case 'r': return char_str{ '\r' };
            case 'v': return char_str{ '\v' };

            /* numeric escape sequences */

            case 'o': return char_str{ parse_arbitrary_oct() };
            case 'x': return char_str{ parse_hex(0) };
            case 'u': return char_str{ parse_hex(4) };
            case 'U': return char_str{ parse_hex(8) };

            /* perl character classes */

            case 'd': return char_class{ ncc::digits };
            case 'D': return char_class{ ncc::digits, true };
            case 's': return char_class{ ncc::perl_whitespace };
            case 'S': return char_class{ ncc::perl_whitespace, true };
            case 'w': return char_class{ ncc::word };
            case 'W': return char_class{ ncc::word, true };

            /* octal escape sequences and backreferences */

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7': return parse_bref_or_octal(escaped);

            case '8':
            case '9': return backref{ std::saturate_cast<std::uint_least16_t>(escaped - '0') };

            case 'g': return parse_bref();

            /* assertions */

            case 'A': return assertion{ assert_type::text_start };
            case 'b': return assertion{ assert_type::word_boundary };
            case 'B': return assertion{ assert_type::not_word_boundary };
            case 'G': throw parser_error("Assertion (\\G) is not implemented");
            case 'Z': throw parser_error("End of text or newlines followed by end of text (\\Z) is not implemented");
            case 'z': return assertion{ assert_type::text_end };

            /* literal string */

            case 'Q': return parse_literal_string(current);

            default:
                if (('A' <= escaped and escaped <= 'Z') or ('a' <= escaped and escaped <= 'z'))
                    throw pattern_error("Invalid control character");
                else
                    return char_str{ escaped }; /* TODO: extract multibyte character */
            }
        }

        case '{': return parse_repeat();
        case '[': return parse_char_class();

        default:
            return char_str{ *current }; /* TODO: extract multibyte character */
        }
    }


    /* general helpers for lexer implementation */


    template<typename CharT>
    constexpr std::size_t lexer<CharT>::parse_hex(const std::size_t fixed_amt)
    {
        static constexpr std::size_t hexadecimal_base{ 0x10 };
        static constexpr std::size_t decimal_base{ 10 };

        std::size_t result{ 0 };

        if (it_ == end_)
            throw pattern_error("EOF in escape sequence");

        const auto lookahead{ *it_ };

        if (lookahead == '{')
        {
            /* arbitrary number of digits contained in {} */

            std::size_t digits{ 0 };
            ++it_;

            for (bool loop{ true }; loop; ++digits)
            {
                if (it_ == end_)
                    throw pattern_error("EOF in escape sequence");;

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
                    throw pattern_error("Invalid character in hexadecimal escape sequence");

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
                    throw pattern_error("EOF in escape sequence");

                const auto c{ *it_ };

                if ('0' <= c and c <= '9')
                    result = (result * hexadecimal_base) + (c - '0');
                else if ('A' <= c and c <= 'F')
                    result = (result * hexadecimal_base) + decimal_base + (c - 'A');
                else if ('a' <= c and c <= 'f')
                    result = (result * hexadecimal_base) + decimal_base + (c - 'a');
                else
                    throw pattern_error("Invalid character in hexadecimal escape sequence");

                ++it_;
            }
        }
        else
        {
            std::size_t digits{ 0 };

            while (true)
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
                    break;

                ++it_;
                ++digits;
            }

            if (digits == 0)
                throw pattern_error("Invalid escape sequence");
        }

        return result;
    }

    template<typename CharT>
    constexpr std::size_t lexer<CharT>::parse_remaining_oct(const std::size_t init)
    {
        static constexpr std::size_t octal_base{ 010 };

        std::size_t result{ init };

        for (std::size_t i{ 0 }; i < 2; ++i)
        {
            if (it_ == end_)
                break;

            const auto c{ *it_ };

            if ('0' <= c and c <= '7')
                result = (result * octal_base) + (c - '0');
            else
                break;

            ++it_;
        }

        return result;
    }

    template<typename CharT>
    constexpr std::size_t lexer<CharT>::parse_arbitrary_oct()
    {
        static constexpr std::size_t octal_base{ 010 };

        std::size_t result{ 0 };

        if (it_ == end_)
            throw pattern_error("EOF in escape sequence");

        const auto lookahead{ *it_ };

        if (lookahead != '{')
            throw pattern_error("Invalid escape sequence");

        /* arbitrary number of digits contained in {} */

        std::size_t digits{ 0 };
        ++it_;

        for (bool loop{ true }; loop; ++digits)
        {
            if (it_ == end_)
                throw pattern_error("EOF in escape sequence");;

            const auto c{ *it_ };

            if (c == '}')
                loop = false;
            else if ('0' <= c and c <= '7')
                result = (result * octal_base) + (c - '0');
            else
                throw pattern_error("Invalid character in octal escape sequence");

            ++it_;
        }

        if (digits == 0)
            throw pattern_error("Delimited escape sequence cannot be empty");

        return result;
    }

    /* specific helpers for lexer implementation */

    template<typename CharT>
    constexpr tok::backref lexer<CharT>::parse_bref()
    {
        using namespace tok;
        static constexpr std::size_t base{ 10 };

        backref bref{ 0 };

        if (it_ == end_)
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

                bref.number *= base;
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

        return bref;
    }

    template<typename CharT>
    constexpr tok::repeat_n_m lexer<CharT>::parse_repeat()
    {
        using namespace tok;
        static constexpr std::int_least16_t base{ 10 };

        std::int_least16_t min{ -1 };
        std::int_least16_t max{ -1 };

        bool parse_min{ true };
        bool parse_max{ true };

        while (parse_min)
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

    template<typename CharT>
    constexpr lexer<CharT>::token_t lexer<CharT>::parse_bref_or_octal(const CharT init)
    {
        using namespace tok;
        static constexpr std::size_t base{ 010 };

        std::size_t result{ static_cast<std::size_t>(init - '0') };
        backref bref{ std::saturate_cast<std::uint_least16_t>(init - '0') };

        for (int i{ 0 }; i < 2; ++i)
        {
            if (it_ == end_)
                break;

            const auto lookahead{ *it_ };

            if (not ('0' <= lookahead and lookahead <= '7'))
                break;

            bref.number = 0;

            result *= base;
            result += lookahead - '0';
            ++it_;
        }

        if (bref.number == 0)
            return char_str<CharT>{ result };
        else
            return bref;
    }

    template<typename CharT>
    constexpr tok::char_str<CharT> lexer<CharT>::parse_literal_string(const it_type begin)
    {
        using namespace tok;

        for (bool slash{ true }; true;)
        {
            if (it_ == end_)
                throw pattern_error("Reached end of input in literal text");

            const auto cur{ *it_++ };

            if (not slash and cur == '\\')
                slash = true;
            else if (slash and cur == 'E')
                break;
            else
                slash = false;
        }

        return char_str<CharT>{ std::next(begin, 2), std::prev(it_, 2) };
    }

    template<typename CharT>
    constexpr tok::char_class<CharT> lexer<CharT>::parse_char_class()
    {
        using namespace tok;
        using ncc = named_character_class;
        using char_class = char_class<CharT>;
        using underlying_char_t = char_class::underlying_char_type;

        if (it_ == end_)
            throw pattern_error("EOF in character class");

        const bool is_negated{ *it_ == '^' };

        if (is_negated)
            ++it_;

        char_class result{ is_negated };

        std::optional<underlying_char_t> c{};
        bool is_range{ false };

        for (bool loop{ true }; loop;)
        {
            if (it_ == end_)
                throw pattern_error("EOF in character class");

            const auto current{ it_ };

            std::optional<std::pair<ncc, bool>> selected_cc;
            std::optional<underlying_char_t> nc;

            switch (*it_++)
            {
            case ']':
                if (result.data.empty() and not c.has_value())
                    nc = ']';
                else
                    loop = false;
                break;

            case '-':
                if (c.has_value() and not is_range)
                    is_range = true;
                else
                    nc = '-';
                break;

            case '\\':
            {
                if (it_ == end_)
                    throw pattern_error("Pattern cannot end with '\\'");

                const auto escaped{ *it_++ };

                switch (escaped)
                {
                /* standard escape sequences */

                case 'a': nc = '\a'; break;
                case 'b': nc = '\b'; break;
                case 'f': nc = '\f'; break;
                case 't': nc = '\t'; break;
                case 'n': nc = '\n'; break;
                case 'r': nc = '\r'; break;
                case 'v': nc = '\v'; break;

                /* numeric escape sequences */

                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7': nc = parse_remaining_oct(escaped - '0'); break;

                case 'o': nc = parse_arbitrary_oct(); break;
                case 'x': nc = parse_hex(0); break;
                case 'u': nc = parse_hex(4); break;
                case 'U': nc = parse_hex(8); break;
                        
                /* perl character classes */

                case 'd': selected_cc = { ncc::digits, false }; break;
                case 'D': selected_cc = { ncc::digits, true }; break;
                case 's': selected_cc = { ncc::perl_whitespace, false }; break;
                case 'S': selected_cc = { ncc::perl_whitespace, true }; break;
                case 'w': selected_cc = { ncc::word, false }; break;
                case 'W': selected_cc = { ncc::word, true }; break;

                default:
                    if (('A' <= escaped and escaped <= 'Z') or ('a' <= escaped and escaped <= 'z'))
                        throw pattern_error("Invalid control character");
                    else
                        nc = escaped; /* TODO: extract multibyte character */
                    break;
                }
                break;
            }

            case '[':
                selected_cc = { parse_posix_char_class(), false };
                break;

            default:
                nc = *current; /* TODO: extract multibyte character */
                break;
            }


            if (selected_cc)
            {
                if (c)
                {
                    /* insert c */
                    result.data.insert(c.value());
                    c.reset();
                }

                if (selected_cc->second)
                {
                    /* insert negated char class */
                    typename char_class::impl_type tmp{ selected_cc->first, true };
                    result.data.insert(tmp);
                }
                else
                {
                    /* insert char class */
                    result.data.insert(selected_cc->first);
                }

                selected_cc.reset();
            }
            else if (nc)
            {
                if (not c)
                {
                    /* delay insert */
                    c = nc;
                }
                else if (is_range)
                {
                    if (*c > *nc)
                        throw pattern_error("Invalid character class range");

                    /* insert c - nc */
                    result.data.insert(*c, *nc);
                    is_range = false;
                    c.reset();
                }
                else
                {
                    /* insert c */
                    result.data.insert(*c);
                    c = nc;
                }
            }
            else if (not loop)
            {
                if (c)
                    result.data.insert(*c);

                if (is_range)
                    result.data.insert(underlying_char_t{ '-' });
            }
        }

        result.data.normalise();
        return result;
    }


    template<typename CharT>
    constexpr named_character_class lexer<CharT>::parse_posix_char_class()
    {
        using ncc = named_character_class;

        static constexpr auto test = [](it_type& it, const it_type& end, std::string_view rest) -> bool {
            if (std::ranges::starts_with(it, end, std::ranges::begin(rest), std::ranges::end(rest)))
            {
                std::ranges::advance(it, std::ranges::size(rest));
                return true;
            }
            return false;
        };


        if (it_ == end_ or *it_ != ':')
            throw pattern_error("Invalid POSIX Character Class");
        
        ++it_;

        if (it_ != end_)
        {
            switch (*it_++)
            {
            case 'a':
                if (it_ == end_)
                    break;
                if (*it_ == 'l')
                {
                    ++it_;
                    if (it_ == end_)
                    {
                        break;
                    }
                    else if (*it_ == 'n')
                    {
                        if (test(++it_, end_, "um:]"))
                            return ncc::alphanumeric;
                    }
                    else if (*it_ == 'p')
                    {
                        if (test(++it_, end_, "ha:]"))
                            return ncc::alphabetic;
                    }
                }
                else if (*it_ == 's')
                {
                    if (test(++it_, end_, "cii:]"))
                        return ncc::ascii;
                }
                break;

            case 'b':
                if (test(it_, end_, "lank:]"))
                    return ncc::blank;
                break;

            case 'c':
                if (test(it_, end_, "ntrl:]"))
                    return ncc::control;
                break;

            case 'd':
                if (test(it_, end_, "igit:]"))
                    return ncc::digits;
                break;

            case 'g':
                if (test(it_, end_, "raph:]"))
                    return ncc::graphical;
                break;

            case 'l':
                if (test(it_, end_, "ower:]"))
                    return ncc::lowercase;
                break;

            case 'p':
                if (it_ == end_)
                    break;
                if (*it_ == 'r')
                {
                    if (test(++it_, end_, "int:]"))
                        return ncc::printable;
                }
                else if (*it_ == 'u')
                {
                    if (test(++it_, end_, "nct:]"))
                        return ncc::punctuation;
                }
                break;

            case 's':
                if (test(it_, end_, "pace:]"))
                    return ncc::posix_whitespace;
                break;

            case 'u':
                if (test(it_, end_, "pper:]")) return ncc::uppercase;
                break;

            case 'w':
                if (test(it_, end_, "ord:]"))
                    return ncc::word;
                break;

            case 'x':
                if (test(it_, end_, "digit:]"))
                    return ncc::hexdigits;
                break;

            default:
                break;
            }
        }

        throw pattern_error("Invalid POSIX Character Class");
    }
}
