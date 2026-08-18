// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <numeric>
#include <optional>
#include <string_view>
#include <variant>

#include "srx/api/regex_error.hpp"
#include "srx/ast/charclass.hpp"
#include "srx/ast/capstack.hpp"
#include "srx/etc/util.hpp"


/* Note: We assume the literal character encoding is a superset of ASCII */

namespace srx {
namespace detail {

/* RE2 limits counted repetitions to 1000 - we do the same here */
inline constexpr int counted_repetition_limit{ 1000 };

enum class assert_type : unsigned char
{
    text_start,
    text_end,
    line_start,
    line_end,
    // text_end_or_newline_before,
    ascii_word_boundary,
    not_ascii_word_boundary,
};

namespace parser {

template<typename CharT>
class ll1;

} // namespace parser

/* token definitions */

namespace tok {

struct end_of_input {};
struct vert {};
struct dot {};
struct hat {};
struct dollar {};
struct rparen {};
struct star {};
struct plus {};
struct quest {};

struct repeat_n_m
{
    int min;
    int max; /* use max=min for {min} or max<min for {min,} */

    friend constexpr bool operator==(const repeat_n_m& x, const repeat_n_m& y) = default;
};

struct assertion
{
    assert_type type;

    friend constexpr bool operator==(const assertion& x, const assertion& y) = default;
};

struct set_flags
{
    parser::capture_flags flags{};
};

template<typename CharT>
struct lparen
{
    parser::capture_flags flags{};
    parser::group_modes mode{ parser::group_modes::normal };
    bool is_named{ false };
    std::basic_string_view<CharT> name{};

    lparen() = default;

    constexpr explicit lparen(std::basic_string_view<CharT> sv)
        : is_named{ true }, name{ sv } {}

    constexpr explicit lparen(parser::group_modes mode)
        : mode{ mode } {}

    constexpr explicit lparen(parser::capture_flags flags)
        : flags{ flags }, mode{ parser::group_modes::non_capturing } {}
};

template<typename CharT>
struct char_class
{
    using impl_type = char_class_impl<std::same_as<char, CharT>>;
    using underlying_char_type = impl_type::char_type;

    impl_type data;

    template<typename... Args>
    constexpr explicit char_class(Args&&... args) : data{ std::forward<Args>(args)... } {}

    friend constexpr bool operator==(const char_class& x, const char_class& y) = default;
};

template<typename CharT>
struct char_str
{
    std::basic_string<CharT> data;

    char_str() = default;

    constexpr explicit char_str(CharT c) : data{ c } {}

    constexpr explicit char_str(char c)
        requires (not std::same_as<CharT, char>)
        : data{ static_cast<CharT>(c) } {};

    template<std::input_iterator I, std::sentinel_for<I> S>
        requires std::convertible_to<std::iter_value_t<I>, CharT>
    constexpr explicit char_str(I first, S last) : data(first, last) {}

    constexpr explicit char_str(std::basic_string_view<CharT> str) : data{ str } {}

    constexpr explicit char_str(std::basic_string_view<char> str)
        requires (not std::same_as<CharT, char>)
        : data{ std::from_range, str | std::views::transform([](char c){ return static_cast<CharT>(c); })} {}

    [[nodiscard]] constexpr std::optional<typename char_class<CharT>::underlying_char_type> get_if_single()
    {
        // TODO: update this to be aware of multibyte characters
        if (data.size() == 1)
            return data.front();
        return {};
    }

    friend constexpr bool operator==(const char_str& x, const char_str& y) = default;
};

struct backref
{
    unsigned int number;

    friend constexpr bool operator==(const backref& x, const backref& y) = default;
};

} // namespace tok


template<typename CharT>
using token_type = std::variant<tok::end_of_input, tok::dot, tok::hat, tok::dollar,
                                tok::lparen<CharT>, tok::rparen, tok::vert,
                                tok::star, tok::plus, tok::quest, tok::repeat_n_m,
                                tok::char_str<CharT>, tok::char_class<CharT>,
                                tok::backref, tok::assertion, tok::set_flags>;


/* lexer concept definitions */

template<typename T>
concept lexer_like = requires (T l) {
    typename T::token_t;
    { l.empty() } -> std::same_as<bool>;
    { l.nexttok() } -> std::same_as<typename T::token_t>;
};

template<typename T>
concept extended_lexer_like = lexer_like<T> and requires (T l) {
    { l.set_extended() } -> std::same_as<void>;
    { l.set_extended_more() } -> std::same_as<void>;
    { l.reset_extended() } -> std::same_as<void>;
};


namespace lexer {

template<typename CharT>
class generated
{
public:
    static_assert(character<CharT>);
    using token_t = token_type<CharT>;

    constexpr explicit generated(std::basic_string_view<CharT> sv) : it_{ sv.begin() }, end_{ sv.end() } {}
    constexpr explicit generated(const CharT* cstr) : generated(std::basic_string_view<CharT>{ cstr }) {}
    [[nodiscard]] constexpr token_t nexttok();
    [[nodiscard]] constexpr bool empty() { return it_ == end_; }

    constexpr void set_extended() { extended_mode_ = 1; }
    constexpr void set_extended_more() { extended_mode_ = 2; }
    constexpr void reset_extended() { extended_mode_ = 0; }

private:
    using it_type = std::basic_string_view<CharT>::const_iterator;

    it_type it_;
    it_type end_;
    bool literal_string_mode_{ false };
    unsigned char extended_mode_{ 0 };
};

template<typename CharT>
class lexer
{
public:
    static_assert(character<CharT>);
    using token_t = token_type<CharT>;

    constexpr explicit lexer(std::basic_string_view<CharT> sv) : it_{ sv.begin() }, end_{ sv.end() } {}
    [[nodiscard]] constexpr token_t nexttok();
    [[nodiscard]] constexpr bool empty() { return it_ == end_; }

    constexpr void set_extended() { extended_mode_ = 1; }
    constexpr void set_extended_more() { extended_mode_ = 2; }
    constexpr void reset_extended() { extended_mode_ = 0; }

private:
    using it_type = std::basic_string_view<CharT>::const_iterator;

    constexpr std::size_t            parse_hex(std::size_t fixed_amt);
    constexpr std::size_t            parse_remaining_oct(std::size_t init);
    constexpr std::size_t            parse_arbitrary_oct();
    constexpr tok::backref           parse_bref();
    constexpr tok::repeat_n_m        parse_repeat();
    constexpr token_t                parse_lparen();
    constexpr token_t                parse_bref_or_octal(CharT init);
    constexpr token_t                parse_literal_string();
    constexpr tok::char_class<CharT> parse_char_class();
    constexpr named_character_class  parse_posix_char_class(it_type first, it_type last, bool& ncc_negated);

    it_type it_;
    it_type end_;
    bool literal_string_mode_{ false };
    unsigned char extended_mode_{ 0 };
};


/* lexer implementation */

template<typename CharT>
constexpr lexer<CharT>::token_t lexer<CharT>::nexttok()
{
    using namespace tok;
    using char_str = char_str<CharT>;
    using char_class = char_class<CharT>;

    if (literal_string_mode_)
        return parse_literal_string();

    while (true)
    {
        if (it_ == end_)
            return end_of_input{};

        const auto current = it_;

        switch (*it_++)
        {
        case '(': return parse_lparen();
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

            const auto escaped = *it_++;

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

            case 'o': return char_str{ static_cast<CharT>(parse_arbitrary_oct()) };
            case 'x': return char_str{ static_cast<CharT>(parse_hex(0)) };
            case 'u': return char_str{ static_cast<CharT>(parse_hex(4)) };
            case 'U': return char_str{ static_cast<CharT>(parse_hex(8)) };

            /* perl character classes */

            case 'd': return char_class{ ncc::digits };
            case 'D': return char_class{ ncc::digits, negated_cc_tag };
            case 's': return char_class{ ncc::perl_whitespace };
            case 'S': return char_class{ ncc::perl_whitespace, negated_cc_tag };
            case 'w': return char_class{ ncc::word };
            case 'W': return char_class{ ncc::word, negated_cc_tag };

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
            case '9': return backref{ static_cast<unsigned int>(escaped - '0') };

            case 'g': return parse_bref();

            /* assertions */

            case 'A': return assertion{ assert_type::text_start };
            case 'b': return assertion{ assert_type::ascii_word_boundary };
            case 'B': return assertion{ assert_type::not_ascii_word_boundary };
            case 'G': throw parser_error("Assertion (\\G) is not implemented");
            case 'Z': throw parser_error("End of text or newlines followed by end of text (\\Z) is not implemented");
            case 'z': return assertion{ assert_type::text_end };

            /* literal string */

            case 'Q': literal_string_mode_ = true; return parse_literal_string();

            default:
                if (('A' <= escaped and escaped <= 'Z') or ('a' <= escaped and escaped <= 'z'))
                    throw pattern_error("Invalid control character");
                else
                    return char_str{ escaped }; /* TODO: extract multibyte character */
            }
        }

        case '{': return parse_repeat();
        case '[': return parse_char_class();

        case '\f':
        case '\t':
        case '\n':
        case '\r':
        case '\v':
        case ' ':
            if (extended_mode_ >= 1)
                break;
        [[fallthrough]];

        default:
            return char_str{ *current }; /* TODO: extract multibyte character */
        }
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

    const auto lookahead = *it_;

    if (lookahead == '{')
    {
        /* arbitrary number of digits contained in {} */

        std::size_t digits{ 0 };
        ++it_;

        while (true)
        {
            if (it_ == end_)
                throw pattern_error("EOF in escape sequence");

            const auto c = *it_;
            ++it_;

            if (c == '}')
                break;
            else if ('0' <= c and c <= '9')
                result = (result * hexadecimal_base) + (c - '0');
            else if ('A' <= c and c <= 'F')
                result = (result * hexadecimal_base) + decimal_base + (c - 'A');
            else if ('a' <= c and c <= 'f')
                result = (result * hexadecimal_base) + decimal_base + (c - 'a');
            else
                throw pattern_error("Invalid character in hexadecimal escape sequence");

            ++digits;
        }

        if (digits == 0)
            throw pattern_error("Delimited escape sequence cannot be empty");
    }
    else if (fixed_amt != 0)
    {
        for (std::size_t i{ 0 }; i < fixed_amt; ++i)
        {
            if (it_ == end_)
                throw pattern_error("EOF in escape sequence");

            const auto c = *it_;

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

            const auto c = *it_;

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

        const auto c = *it_;

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

    const auto lookahead = *it_;

    if (lookahead != '{')
        throw pattern_error("Invalid escape sequence");

    /* arbitrary number of digits contained in {} */

    std::size_t digits{ 0 };
    ++it_;

    while (true)
    {
        if (it_ == end_)
            throw pattern_error("EOF in escape sequence");

        const auto c = *it_;
        ++it_;

        if (c == '}')
            break;
        else if ('0' <= c and c <= '7')
            result = (result * octal_base) + (c - '0');
        else
            throw pattern_error("Invalid character in octal escape sequence");

        ++digits;
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

    auto next = *it_++;

    if (next == '{')
    {
        /* \g{n...} */

        while (true)
        {
            if (it_ == end_)
                throw pattern_error("Incomplete escape sequence");

            next = *it_++;

            if (next == '}')
                break;

            if (not ('0' <= next and next <= '9'))
                throw pattern_error("Incomplete escape sequence");

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
    static constexpr int base{ 10 };

    repeat_n_m rep{ .min = -1, .max = -1 };

    bool parse_min{ true };
    bool parse_max{ true };

    while (parse_min)
    {
        if (it_ == end_)
            throw pattern_error("Repeater is incomplete");

        auto c = *it_++;

        if ('0' <= c and c <= '9')
        {
            if (rep.min == -1)
                rep.min = c - '0';
            else
                rep.min = std::saturating_add(std::saturating_mul(rep.min, base), static_cast<int>(c - '0'));
        }
        else if (c == ',')
        {
            parse_min = false;
        }
        else if (c == '}')
        {
            if (rep.min == -1)
                throw pattern_error("Repeater is empty");

            parse_min = false;

            /* skip parsing max */
            rep.max = rep.min;
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

        auto c = *it_++;

        if ('0' <= c and c <= '9')
        {
            if (rep.max == -1)
                rep.max = c - '0';
            else
                rep.max = std::saturating_add(std::saturating_mul(rep.max, base), static_cast<int>(c - '0'));
        }
        else if (c == '}')
        {
            if (rep.max != -1 and rep.max < rep.min)
                throw pattern_error("Invalid repeater (max is less than min)");

            parse_max = false;
        }
        else
        {
            throw pattern_error("Invalid character in repeater");
        }
    }

    if (rep.min > counted_repetition_limit)
    {
        if (rep.min == rep.max)
            throw pattern_error("Finite number of counted repetitions exceeds limit");
        else
            throw pattern_error("Lower bound of counted repetitions exceeds limit");
    }
    else if (rep.max > counted_repetition_limit)
    {
        throw pattern_error("Finite upper bound of counted repetitions exceeds limit");
    }
    else if (rep.max < 0)
    {
        /* normalise for merging purposes */
        rep.max = rep.min - 1;
    }

    return rep;
}

template<typename CharT>
constexpr lexer<CharT>::token_t lexer<CharT>::parse_lparen()
{
    if (it_ == end_ or *it_ != '?')
        return tok::lparen<CharT>{};

    if (++it_ == end_)
        throw pattern_error("Invalid Pattern");

    using cf = parser::capture_flags::flag_value;
    using gm = parser::group_modes;

    switch (*it_)
    {
    case '#':
        ++it_;
        while (it_ != end_)
            if (*it_++ == ')')
                break;
        return nexttok();

    case '|':
        ++it_;
        return tok::lparen<CharT>{ gm::branch_reset };

    case '>':
        ++it_;
        return tok::lparen<CharT>{ gm::atomic };

    case 'P':
    case '<':
    case '\'':
        ++it_;
        throw pattern_error("Named capture groups are unsupported");

    default: /* parse options */
    {
        parser::capture_flags flags{};
        bool flag_value{ true };

        while (true)
        {
            if (it_ == end_)
                throw pattern_error("Invalid Pattern");

            switch (*it_++)
            {
            case 'i':
                flags.caseless = (flag_value) ? cf::enabled : cf::disabled;
                break;
            case 'm':
                flags.multiline = (flag_value) ? cf::enabled : cf::disabled;
                break;
            case 'n':
                flags.noautocap = (flag_value) ? cf::enabled : cf::disabled;
                break;
            case 's':
                flags.dotall = (flag_value) ? cf::enabled : cf::disabled;
                break;
            case 'U':
                flags.ungreedy = (flag_value) ? cf::enabled : cf::disabled;
                break;
            case 'x':
                flags.extended = (flag_value) ? cf::enabled : cf::disabled;
                if (it_ != end_ and *it_ == 'x')
                    if (++it_, flag_value)
                        flags.extended = cf::enabled_more;
                break;
            case '-':
                if (not flag_value)
                    throw pattern_error("Capturing group arguments can only contain one hyphen");
                flag_value = false;
                break;

            case ':':
                return tok::lparen<CharT>{ flags };

            case ')':
                return tok::set_flags{ flags };

            default:
                throw pattern_error("Invalid capturing group");
            }
        }
        break;
    }
    }
}

template<typename CharT>
constexpr lexer<CharT>::token_t lexer<CharT>::parse_bref_or_octal(const CharT init)
{
    using namespace tok;
    static constexpr unsigned int base{ 010 };

    backref bref{ static_cast<unsigned int>(init - '0') };
    std::size_t result{ bref.number };

    for (int i{ 0 }; i < 2; ++i)
    {
        if (it_ == end_)
            break;

        const auto lookahead = *it_;

        if (not ('0' <= lookahead and lookahead <= '7'))
            break;

        bref.number = 0;

        result *= base;
        result += lookahead - '0';
        ++it_;
    }

    if (bref.number == 0)
        return char_str<CharT>{ static_cast<CharT>(result) };
    else
        return bref;
}

template<typename CharT>
constexpr lexer<CharT>::token_t lexer<CharT>::parse_literal_string()
{
    using namespace std::string_view_literals;
    static constexpr auto proj = [](CharT c){ return static_cast<char>(c); };

    if (it_ == end_)
        throw pattern_error("Reached end of input in literal text");

    if (std::ranges::starts_with(std::ranges::subrange{ it_, end_ }, "\\E"sv, {}, proj))
    {
        std::ranges::advance(it_, 2);
        literal_string_mode_ = false;
        return nexttok();
    }

    return tok::char_str<CharT>{ *it_++ }; /* TODO: extract multibyte character */
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

    char_class result{};
    std::size_t count{ 0 };

    std::optional<underlying_char_t> c;
    bool is_range{ false };

    for (bool loop{ true }; loop;)
    {
        bool ncc_negated{ false };

        if (it_ == end_)
            throw pattern_error("EOF in character class");

        const auto current = it_;

        std::optional<ncc> selected_cc;
        std::optional<underlying_char_t> next;

        switch (*it_++)
        {
        case ']':
            if (count == 0)
                next = ']';
            else
                loop = false;
            break;

        case '-':
            if (c.has_value() and not is_range)
                is_range = true;
            else
                next = '-';
            break;

        case '\\':
        {
            if (it_ == end_)
                throw pattern_error("Pattern cannot end with '\\'");

            const auto escaped = *it_++;

            switch (escaped)
            {
            /* standard escape sequences */

            case 'a': next = '\a'; break;
            case 'b': next = '\b'; break;
            case 'f': next = '\f'; break;
            case 't': next = '\t'; break;
            case 'n': next = '\n'; break;
            case 'r': next = '\r'; break;
            case 'v': next = '\v'; break;

            /* numeric escape sequences */

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7': next = parse_remaining_oct(escaped - '0'); break;

            case 'o': next = parse_arbitrary_oct(); break;
            case 'x': next = parse_hex(0); break;
            case 'u': next = parse_hex(4); break;
            case 'U': next = parse_hex(8); break;

            /* perl character classes */

            case 'D': ncc_negated = true; [[fallthrough]];
            case 'd': selected_cc = ncc::digits; break;
            case 'S': ncc_negated = true; [[fallthrough]];
            case 's': selected_cc = ncc::perl_whitespace; break;
            case 'W': ncc_negated = true; [[fallthrough]];
            case 'w': selected_cc = ncc::word; break;

            default:
                if (('A' <= escaped and escaped <= 'Z') or ('a' <= escaped and escaped <= 'z') or escaped == '8' or escaped == '9')
                    throw pattern_error("Invalid control character");
                else
                    next = escaped; /* TODO: extract multibyte character */
                break;
            }
            break;
        }

        case '[':
            if (it_ != end_ and *it_ == ':')
            {
                using namespace std::string_view_literals;

                static constexpr auto proj = [](CharT c){ return static_cast<char>(c); };
                const auto& [first, last] = std::ranges::search(std::ranges::subrange{ it_, end_ }, ":]"sv, {}, proj);

                if (first != end_)
                {
                    selected_cc = parse_posix_char_class(it_ + 1, first, ncc_negated);
                    it_ = last;
                }
            }
            if (not selected_cc)
                next  = '[';
            break;

        case '\f':
        case '\t':
        case '\n':
        case '\r':
        case '\v':
        case ' ':
            if (extended_mode_ >= 2)
                continue;
            [[fallthrough]];

        default:
            next = *current; /* TODO: extract multibyte character */
            break;
        }

        ++count;

        if (selected_cc)
        {
            if (is_range)
                throw pattern_error("Invalid range in character class");

            if (c)
            {
                /* insert c */
                result.data.insert(*c);
                c.reset();
            }

            /* insert char class */
            if (ncc_negated)
                result.data.insert(*selected_cc, negated_cc_tag);
            else
                result.data.insert(*selected_cc);

            selected_cc.reset();
        }
        else if (next)
        {
            if (not c)
            {
                /* delay insert */
                c = next;
            }
            else if (is_range)
            {
                if (*c > *next)
                    throw pattern_error("Invalid character class range");

                /* insert c - nc */
                result.data.insert(*c, *next);
                is_range = false;
                c.reset();
            }
            else
            {
                /* insert c */
                result.data.insert(*c);
                c = next;
            }
        }
        else if (not loop)
        {
            if (c)
                result.data.insert(*c);

            if (c and is_range)
                result.data.insert(underlying_char_t{ '-' });
        }
    }

    if (is_negated)
        result.data.negate();

    return result;
}

template<typename CharT>
constexpr named_character_class lexer<CharT>::parse_posix_char_class(it_type first, it_type last, bool& ncc_negated)
{
    using namespace std::string_view_literals;
    using ncc = named_character_class;

    static constexpr auto proj = [](CharT c){ return static_cast<char>(c); };
    std::string str{ std::from_range, std::ranges::subrange{ first, last } | std::views::transform(proj) };
    std::string_view sv{ str };

    if (sv.starts_with('^'))
    {
        sv.remove_prefix(1);
        ncc_negated = true;
    }

    if (sv == "alnum"sv)
        return ncc::alphanumeric;
    else if (sv == "alpha"sv)
        return ncc::alphabetic;
    else if (sv == "ascii"sv)
        return ncc::ascii;
    else if (sv == "blank"sv)
        return ncc::blank;
    else if (sv == "cntrl"sv)
        return ncc::control;
    else if (sv == "digit"sv)
        return ncc::digits;
    else if (sv == "graph"sv)
        return ncc::graphical;
    else if (sv == "lower"sv)
        return ncc::lowercase;
    else if (sv == "print"sv)
        return ncc::printable;
    else if (sv == "punct"sv)
        return ncc::punctuation;
    else if (sv == "space"sv)
        return ncc::posix_whitespace;
    else if (sv == "upper"sv)
        return ncc::uppercase;
    else if (sv == "word"sv)
        return ncc::word;
    else if (sv == "xdigit"sv)
        return ncc::hexdigits;
    else
        throw pattern_error("Invalid POSIX Character Class");
}

} // namespace lexer
} // namespace detail
} // namespace srx
