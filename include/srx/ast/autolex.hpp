// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <charconv>
#include <expected>
#include <iterator>
#include <meta>
#include <ranges>
#include <stdexcept>
#include <span>
#include <type_traits>
#include <utility>

#include "srx/etc/util.hpp"
#include "srx/api/regex_error.hpp"
#include "srx/api/submatch.hpp"
#include "srx/ast/capstack.hpp"
#include "srx/ast/charclass.hpp"
#include "srx/ast/tok.hpp"
#include "srx/ast/tree.hpp"
#include "srx/fsm/flags.hpp"
#include "srx/gen/compile.hpp"
#include "srx/gen/p1306.hpp"


namespace srx {
namespace detail {
namespace cctok {

struct end_of_cc {};
struct negate_or_hat {};
struct range_or_minus {};
struct literal_string {};
struct space {};
struct tab {};
struct named_cc { named_character_class ncc; bool negated; };

} // namespace cctok;

template<typename CharT>
using char_class_token_type = std::variant<cctok::end_of_cc, cctok::negate_or_hat, cctok::range_or_minus,
                                           cctok::literal_string, cctok::named_cc, cctok::space, cctok::tab, CharT>;


/* helper functions */

namespace autolex {

inline constexpr bool treat_char_as_char8_t{ false };

template<std::integral T, typename CharT>
constexpr std::expected<T, std::errc> parse_integer_impl(std::basic_string_view<CharT> sv, int base) noexcept
{
    if constexpr (std::same_as<T, char8_t>)
    {
        return parse_integer_impl<unsigned char>(sv, base).transform([](auto x){ return static_cast<char8_t>(x); });
    }
    else if constexpr (std::same_as<T, char16_t>)
    {
        return parse_integer_impl<std::uint_least16_t>(sv, base).transform([](auto x){ return static_cast<char16_t>(x); });
    }
    else if constexpr (std::same_as<T, char32_t>)
    {
        return parse_integer_impl<std::uint_least32_t>(sv, base).transform([](auto x){ return static_cast<char32_t>(x); });
    }
    else if constexpr (not std::same_as<CharT, char>)
    {
        /* temporary workaround to accommodate for lack of P3876 */
        std::string result;
        for (const auto& c : sv)
            result += static_cast<char>(c);
        return parse_integer_impl<T, char>(result, base);
    }
    else
    {
        T value{};
        if (const auto fcr = std::from_chars(sv.begin(), sv.end(), value, base); not fcr)
            return std::unexpected{ fcr.ec };
        return value;
    }
}

template<typename CharT>
constexpr CharT parse_oct(std::basic_string_view<CharT> sv)
{
    static constexpr int octal_base{ 010 }; /* n.b. in octal */

    if (const auto result = parse_integer_impl<std::make_unsigned_t<CharT>>(sv, octal_base))
        return static_cast<CharT>(*result);
    else if (result.error() == std::errc::result_out_of_range)
        throw pattern_error("octal escape sequence out of range");
    else
        throw pattern_error("invalid octal escape sequence");
}

template<typename CharT>
constexpr CharT parse_hex(std::basic_string_view<CharT> sv)
{
    static constexpr int hex_base{ 0x10 };

    if (const auto result = parse_integer_impl<std::make_unsigned_t<CharT>>(sv, hex_base))
        return static_cast<CharT>(*result);
    else if (result.error() == std::errc::result_out_of_range)
        throw pattern_error("hex escape sequence out of range");
    else
        throw pattern_error("invalid hex escape sequence");
}

template<typename CharT>
constexpr char32_t parse_ucn(std::basic_string_view<CharT> sv)
{
    static constexpr char32_t limit{ 0x0010FFFFu };
    static constexpr int hex_base{ 0x10 };

    const auto result = parse_integer_impl<char32_t>(sv, hex_base);

    if (result.has_value() and *result <= limit)
        return *result;
    else
        throw pattern_error("invalid universal character");
}

template<typename CharT>
constexpr char32_t parse_nuc(std::basic_string_view<CharT> sv)
{
    if constexpr (not std::same_as<CharT, char>)
    {
        /* temporary workaround to accommodate for lack of P3876 */
        std::string result;
        for (const auto& c : sv)
            result += static_cast<char>(c);
        return parse_nuc<char>(result);
    }
    else
    {
        // TODO: implement later
        throw std::logic_error("UNIMPLEMENTED");
    }
}

template<typename CharT>
constexpr std::basic_string<CharT> to_mb(char32_t c)
{
    if constexpr (std::same_as<CharT, char32_t> or (std::same_as<CharT, wchar_t> and sizeof(wchar_t) == 4))
    {
        return { c };
    }
    else if constexpr (std::same_as<CharT, char16_t> or (std::same_as<CharT, wchar_t> and sizeof(wchar_t) == 2))
    {
        // TODO: implement later
        throw std::logic_error("UNIMPLEMENTED");
    }
    else if constexpr (std::same_as<CharT, char8_t> or (std::same_as<CharT, char> and treat_char_as_char8_t))
    {
        // TODO: implement later
        throw std::logic_error("UNIMPLEMENTED");
    }
    else
    {
        return { static_cast<char>(c) };
    }
}

template<bool IsNarrow, typename CharT>
constexpr std::conditional_t<IsNarrow, char, char32_t> to_wc(std::basic_string_view<CharT> sv)
{
    if constexpr (IsNarrow)
    {
        return static_cast<char>(sv.front());
    }
    else if constexpr (std::same_as<CharT, char32_t> or (std::same_as<CharT, wchar_t> and sizeof(wchar_t) == 4))
    {
        return static_cast<char32_t>(sv.front());
    }
    else if constexpr (std::same_as<CharT, char16_t> or (std::same_as<CharT, wchar_t> and sizeof(wchar_t) == 2))
    {
        // TODO: implement later
        throw std::logic_error("UNIMPLEMENTED");
    }
    else if constexpr (std::same_as<CharT, char8_t> or (std::same_as<CharT, char> and treat_char_as_char8_t))
    {
        // TODO: implement later
        throw std::logic_error("UNIMPLEMENTED");
    }
    else
    {
        return static_cast<char32_t>(sv.front());
    }
}

template<typename CharT>
constexpr tok::backref make_bref(std::basic_string_view<CharT> sv)
{
    static constexpr int dec_base{ 10 };

    const auto result = parse_integer_impl<unsigned int>(sv, dec_base);

    if (result.has_value() and *result != 0)
        return { .number = *result };
    else if (*result == 0)
        throw pattern_error("backreference to non-existent submatch");
    else
        throw pattern_error("invalid backreference");
}

template<std::contiguous_iterator It>
constexpr int parse_repeater_value(It beg, It end)
{
    static constexpr int dec_base{ 10 };

    const auto result = parse_integer_impl<unsigned int>(std::basic_string_view{ beg, end }, dec_base);

    if (result and *result <= counted_repetition_limit)
        return static_cast<int>(*result);
    else if (not result and result.error() != std::errc::result_out_of_range)
        throw pattern_error("repeater value is not a valid number");
    else
        throw pattern_error("number of repetitions exceeds limit");
}


template<typename CharT>
constexpr tok::repeat_n_m make_repeat_fixed(std::basic_string_view<CharT> sv)
{
    auto it{ sv.begin() };
    const auto end{ sv.end() };

    while (it != end and not ('0' <= *it and *it <= '9'))
        ++it;

    const int x{ parse_repeater_value(it, end) };
    return { .min = x, .max = x };
}

template<typename CharT>
constexpr tok::repeat_n_m make_repeat_unbounded(std::basic_string_view<CharT> sv)
{
    auto it{ sv.begin() };
    const auto end{ sv.end() };

    while (it != end and not ('0' <= *it and *it <= '9'))
        ++it;

    const int x{ parse_repeater_value(it, end) };
    return { .min = x, .max = x - 1 };
}

template<typename CharT>
constexpr tok::repeat_n_m make_repeat_range(std::basic_string_view<CharT> sv)
{
    auto it{ sv.begin() };
    const auto end{ sv.end() };

    while (it != end and not ('0' <= *it and *it <= '9'))
        ++it;

    const int x{ parse_repeater_value(it, end) };

    while (it != end and ('0' <= *it and *it <= '9'))
        ++it;

    while (it != end and not ('0' <= *it and *it <= '9'))
        ++it;

    const int y{ parse_repeater_value(it, end) };

    if (x >= y)
        throw pattern_error("minimum number of repetitions exceeds maximum");

    return { .min = x, .max = y };
}

template<typename CharT>
constexpr parser::capture_flags make_flags(std::basic_string_view<CharT> sv)
{
    using cf = parser::capture_flags::flag_value;
    parser::capture_flags flags{};
    bool flag_value{ true };
    CharT prev{ '?' };

    for (const auto& c : sv)
    {
        switch (c)
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
            if (flag_value and prev == 'x')
                flags.extended = cf::enabled_more;
            else
                flags.extended = (flag_value) ? cf::enabled : cf::disabled;
            break;
        case '-':
            if (not flag_value)
                throw pattern_error("capturing group arguments can only contain one hyphen");
            flag_value = false;
            break;

        default:
            throw pattern_error("invalid capturing group");
        }

        prev = c;
    }

    return flags;
}

template<std::contiguous_iterator I>
constexpr auto trim(const submatch<I>& match, int begin_offset = 0, int end_offset = 0)
{
    auto sv = match.view();
    sv.remove_prefix(begin_offset);
    sv.remove_suffix(end_offset);
    return sv;
}

template<std::contiguous_iterator I, class Layout>
constexpr auto trim(const static_match_results<I, Layout>& match, int begin_offset = 0, int end_offset = 0)
{
    return trim(get<0>(match), begin_offset, end_offset);
}

} // namespace autolex


/* auxiliary lexer definitions */

namespace lexer {

template<typename CharT>
class identity
{
public:
    static_assert(character<CharT>);
    using token_t = token_type<CharT>;

    constexpr explicit identity(std::span<token_t> sp) : it_{ sp.begin() }, end_{ sp.end() } {}
    [[nodiscard]] constexpr token_t nexttok(){ return (it_ == end_) ? tok::end_of_input{} : *it_++; }
    [[nodiscard]] constexpr bool empty() { return it_ == end_; }

private:
    using iterator_type = std::span<token_t>::const_iterator;

    static constexpr std::size_t min_count{ 2 };

    iterator_type it_;
    iterator_type end_;

};

template<typename CharT>
class bootstrap
{
public:
    static_assert(character<CharT>);
    using token_t = token_type<CharT>;

    constexpr explicit bootstrap(std::string_view sv) : sv_{ sv }, it_{ sv_.begin() } {}
    [[nodiscard]] constexpr token_t nexttok();
    [[nodiscard]] constexpr bool empty() { return (count_ >= 2) and (it_ == sv_.cend()); }

private:
    using it_type = std::string_view::const_iterator;

    std::string_view sv_;
    it_type it_;
    std::size_t count_{ 0 };
};

template<typename CharT>
constexpr bootstrap<CharT>::token_t bootstrap<CharT>::nexttok()
{
    using namespace tok;
    using ncc = named_character_class;

    try {
        ++count_;

        if (count_ == 1)
        {
            using cf = parser::capture_flags::flag_value;

            parser::capture_flags flags{
                .multiline = cf::enabled,
                .dotall = cf::enabled,
            };

            return set_flags{ flags };
        }

        if (it_ == sv_.cend())
            return end_of_input{};

        const auto saved = std::ranges::next(it_);

        switch (*it_++)
        {
        case '(': return lparen<CharT>{ parser::group_modes::non_capturing };
        case ')': return rparen{};
        case '.': return dot{};
        case '*': return star{};
        case '+': return plus{};
        case '?': return quest{};
        case '^': return hat{};
        case '$': return dollar{};
        case '|': return vert{};

        case '"':
            if (it_ = std::ranges::find(it_, sv_.cend(), '"'); it_ == sv_.cend())
                throw pattern_error("unterminated string");
            return char_str<CharT>{ saved, it_++ };

        case '\'':
            if (it_ = std::ranges::find(it_, sv_.cend(), '\''); it_ == sv_.cend())
                throw pattern_error("unterminated string");
            return char_str<CharT>{ saved, it_++ };

        case '{':
            if (it_ = std::ranges::find(it_, sv_.cend(), '}'); it_ == sv_.cend())
                throw pattern_error("unterminated repeater");
            if (const auto comma_it = std::ranges::find(saved, it_, ','); comma_it == it_)
                return autolex::make_repeat_fixed(std::string_view{ saved, it_++ });
            else if (comma_it == std::ranges::prev(it_))
                return autolex::make_repeat_unbounded(std::string_view{ saved, it_++ });
            else
                return autolex::make_repeat_range(std::string_view{ saved, it_++ });

        case '[':
        {
            if (it_ = std::ranges::find(it_, sv_.cend(), ']'); it_ == sv_.cend())
                throw pattern_error("unterminated character class");
            std::string_view sv(std::string_view{ saved, it_++ });

            char_class<CharT> result;
            bool negate{ false };

            if (sv.starts_with('^'))
            {
                negate = true;
                sv.remove_prefix(1);
            }

            for (const auto& c : sv)
                result.data.insert(c);
            if (negate)
                result.data.negate();

            return result;
        }

        /* nonstandard character classes */
        case 'a': return char_class<CharT>{ ncc::alphanumeric };
        case 'A': return char_class<CharT>{ ncc::alphanumeric, negated_cc_tag };
        case 'd': return char_class<CharT>{ ncc::digits };
        case 'D': return char_class<CharT>{ ncc::digits, negated_cc_tag };
        case 's': return char_class<CharT>{ ncc::posix_whitespace };
        case 'S': return char_class<CharT>{ ncc::posix_whitespace, negated_cc_tag };
        case 'w': return char_class<CharT>{ ncc::word };
        case 'W': return char_class<CharT>{ ncc::word, negated_cc_tag };
        case 'n': return char_class<CharT>{ CharT{ '\n' } };
        case 'N': return char_class<CharT>{ CharT{ '\n' }, negated_cc_tag };
        case 'h': return char_class<CharT>{ ncc::hexdigits };
        case 'H': return char_class<CharT>{ ncc::hexdigits, negated_cc_tag };
        case 'o': return char_class<CharT>{ ncc::octdigits };
        case 'O': return char_class<CharT>{ ncc::octdigits, negated_cc_tag };
        case 'C': return char_class<CharT>{ ncc::nucs_char };
        case 't': return char_class<CharT>{ CharT{ ' ' }, CharT{ '\t' } };

        default:
            throw pattern_error("syntax error");
        }
    }
    catch (const pattern_error& e)
    {
        throw pattern_error(std::format("in bootstrap lexer: encountered error while lexing {:?}: {}", sv_, e.what()));
    }
}

} // namespace lexer


/* dfa lexer definitions */

namespace autolex {

struct rule
{
    std::string_view pattern{};
    std::meta::info action{};
};

template<typename CharT>
struct defs
{
    using ncc = named_character_class;
    using str = tok::char_str<CharT>;
    using cla = tok::char_class<CharT>;
    using lpn = tok::lparen<CharT>;
    using uct = cla::underlying_char_type;

    template<std::ranges::range R>
    static consteval auto compile_dfa(R&& patterns);

    // static consteval auto named_char_lexer();
    static consteval auto charclass_lexer();
    static consteval auto literal_string_lexer();
    static consteval auto main_lexer();
};

template<typename CharT>
template<std::ranges::range R>
consteval auto defs<CharT>::compile_dfa(R&& patterns)
{
    /* set flags */
    auto flags{ default_fsm_flags::match_sequential };
    flags.no_captures = true; /* note: branch reset doesn't work with dfas */

    /* parse pattern string into tree */
    expr_tree<CharT> ast{ std::forward<R>(patterns) | std::views::transform([](auto&& pat){ return lexer::bootstrap<CharT>{ pat }; }) };
    ast.optimise_tags();
    const auto mml = ast.min_max_length();

    /* convert to tnfa */
    tagged_nfa nfa{ ast, flags };
    nfa.rewrite_assertions();
    if (mml.first == 0)
        nfa.add_non_empty_match_pathway();

    /* convert to tdfa */
    tagged_dfa dfa{ nfa };
    dfa.optimise_registers();
    // dfa.minimise_states();

    /* optimise transition edges and their order to produce fewest comparisons */
    /* (if using tables, do `dfa.make_default_tr_if_possible()` instead) */
    dfa.minimise_transition_edges();
    dfa.de_default_edges();

    return tdfa_info{ dfa, nfa, mml, default_fsm_flags::match_sequential, ast.is_alt_mode() };
}


/* char class lexer definition */

template<typename CharT>
consteval auto defs<CharT>::charclass_lexer()
{
    using namespace tok;
    using namespace cctok;

    using std::meta::reflect_constant;

    const std::vector<rule> rules{
        /* basic char class entities */
        { "']'", ^^end_of_cc },
        { "'^'", ^^negate_or_hat },
        { "'-'", ^^range_or_minus },

        /* posix character classes */
        { "'[:alnum:]'",   reflect_constant(named_cc{ ncc::alphanumeric, false }) },
        { "'[:alpha:]'",   reflect_constant(named_cc{ ncc::alphabetic, false }) },
        { "'[:ascii:]'",   reflect_constant(named_cc{ ncc::ascii, false }) },
        { "'[:blank:]'",   reflect_constant(named_cc{ ncc::blank, false }) },
        { "'[:cntrl:]'",   reflect_constant(named_cc{ ncc::control, false }) },
        { "'[:digit:]'",   reflect_constant(named_cc{ ncc::digits, false }) },
        { "'[:graph:]'",   reflect_constant(named_cc{ ncc::graphical, false }) },
        { "'[:lower:]'",   reflect_constant(named_cc{ ncc::lowercase, false }) },
        { "'[:print:]'",   reflect_constant(named_cc{ ncc::printable, false }) },
        { "'[:punct:]'",   reflect_constant(named_cc{ ncc::punctuation, false }) },
        { "'[:space:]'",   reflect_constant(named_cc{ ncc::posix_whitespace, false }) },
        { "'[:upper:]'",   reflect_constant(named_cc{ ncc::uppercase, false }) },
        { "'[:word:]'",    reflect_constant(named_cc{ ncc::word, false }) },
        { "'[:xdigit:]'",  reflect_constant(named_cc{ ncc::hexdigits, false }) },
        { "'[:^alnum:]'",  reflect_constant(named_cc{ ncc::alphanumeric, true }) },
        { "'[:^alpha:]'",  reflect_constant(named_cc{ ncc::alphabetic, true }) },
        { "'[:^ascii:]'",  reflect_constant(named_cc{ ncc::ascii, true }) },
        { "'[:^blank:]'",  reflect_constant(named_cc{ ncc::blank, true }) },
        { "'[:^cntrl:]'",  reflect_constant(named_cc{ ncc::control, true }) },
        { "'[:^digit:]'",  reflect_constant(named_cc{ ncc::digits, true }) },
        { "'[:^graph:]'",  reflect_constant(named_cc{ ncc::graphical, true }) },
        { "'[:^lower:]'",  reflect_constant(named_cc{ ncc::lowercase, true }) },
        { "'[:^print:]'",  reflect_constant(named_cc{ ncc::printable, true }) },
        { "'[:^punct:]'",  reflect_constant(named_cc{ ncc::punctuation, true }) },
        { "'[:^space:]'",  reflect_constant(named_cc{ ncc::posix_whitespace, true }) },
        { "'[:^upper:]'",  reflect_constant(named_cc{ ncc::uppercase, true }) },
        { "'[:^word:]'",   reflect_constant(named_cc{ ncc::word, true }) },
        { "'[:^xdigit:]'", reflect_constant(named_cc{ ncc::hexdigits, true }) },
        { "'[:'.*?':]'",   ^^decltype([]{ throw pattern_error("Invalid POSIX character class"); }) },

        /* syntax-escaping escape codes */
        { "'\\'A", ^^decltype([](const auto& res){ return uct{ to_wc<std::same_as<uct, char>>(trim(res, 1)) }; }) },

        /* standard escape codes */
        { "'\\a'", reflect_constant(uct{ '\a' }) },
        { "'\\b'", reflect_constant(uct{ '\b' }) }, /* NB. only enabled for char classes */
        { "'\\e'", reflect_constant(uct{ '\x1b' }) },
        { "'\\f'", reflect_constant(uct{ '\f' }) },
        { "'\\t'", reflect_constant(uct{ '\t' }) },
        { "'\\n'", reflect_constant(uct{ '\n' }) },
        { "'\\r'", reflect_constant(uct{ '\r' }) },
        { "'\\v'", reflect_constant(uct{ '\v' }) },

        /* perl character classes */
        { "'\\d'", reflect_constant(named_cc{ ncc::digits, false }) },
        { "'\\D'", reflect_constant(named_cc{ ncc::digits, true }) },
        { "'\\s'", reflect_constant(named_cc{ ncc::perl_whitespace, false }) },
        { "'\\S'", reflect_constant(named_cc{ ncc::perl_whitespace, true }) },
        { "'\\w'", reflect_constant(named_cc{ ncc::word, false }) },
        { "'\\W'", reflect_constant(named_cc{ ncc::word, true }) },

        /* character escape codes */
        { "'\\'o{1,3}",  ^^decltype([](const auto& res){ return static_cast<uct>(parse_oct(trim(res, 1))); }) },
        { "'\\x'h+",     ^^decltype([](const auto& res){ return static_cast<uct>(parse_hex(trim(res, 2))); }) },
        { "'\\u'h{4}",   ^^decltype([](const auto& res){ return static_cast<uct>(parse_ucn(trim(res, 2))); }) },
        { "'\\U'h{8}",   ^^decltype([](const auto& res){ return static_cast<uct>(parse_ucn(trim(res, 2))); }) },
        { "'\\o{'o+'}'", ^^decltype([](const auto& res){ return static_cast<uct>(parse_oct(trim(res, 3, 1))); }) },
        { "'\\x{'h+'}'", ^^decltype([](const auto& res){ return static_cast<uct>(parse_hex(trim(res, 3, 1))); }) },
        { "'\\u{'h+'}'", ^^decltype([](const auto& res){ return static_cast<uct>(parse_ucn(trim(res, 3, 1))); }) },
        { "'\\N{'C+'}'", ^^decltype([](const auto& res){ return static_cast<uct>(parse_nuc(trim(res, 3, 1))); }) },

        /* literal string */
        { "'\\Q'", ^^literal_string },

        /* erroneous escape sequences */
        { "'\\'N?",               ^^decltype([]{ throw pattern_error("unknown escape sequence"); }) },
        { "'\\'[oxuN]'{}'",       ^^decltype([]{ throw pattern_error("empty delimited character escape sequence"); }) },
        { "'\\'[oxuN]'{'[^\n}]*", ^^decltype([]{ throw pattern_error("unterminated character escape sequence"); }) },
        { "'\\'[oxu]'{'N*?'}'",   ^^decltype([]{ throw pattern_error("invalid digit in character escape sequence"); }) },
        { "'\\N{'N*?'}'",         ^^decltype([]{ throw pattern_error("invalid character in Unicode character name"); }) },

        /* maybe skip spaces and tabs */
        { "' '",  ^^space }, // Note: with token sequences, we could eliminate these two tokens from cc_token_t
        { "'\t'", ^^tab },

        /* (catch-all: convert from utf8/16/32 to utf32, unless same_as<CharT, char>) */
        { ".", ^^decltype([](const auto& res){ return to_wc<std::same_as<uct, char>>(trim(res)); }) }
    };

    auto tokens = rules | std::views::transform(&rule::pattern);
    auto actions = rules | std::views::transform(&rule::action);

    const auto return_type  = ^^char_class_token_type<uct>;
    const auto eof_action   = ^^decltype([]{ throw pattern_error("unclosed character class"); });
    const auto error_action = ^^decltype([]{ throw pattern_error("unknown error in character class"); });

    return lexer_info{ compile_dfa(tokens), actions, eof_action, error_action, return_type };
}


/* literal string lexer definition */

template<typename CharT>
consteval auto defs<CharT>::literal_string_lexer()
{
    using namespace tok;
    using std::meta::reflect_constant;

    const std::vector<rule> rules{
        { "'\\E'", ^^decltype([](){ return str{ R"(\Q)" }; }) },
        { ".",     ^^decltype([](const auto& res){ return str{ trim(res) }; }) }
    };

    auto tokens = rules | std::views::transform(&rule::pattern);
    auto actions = rules | std::views::transform(&rule::action);

    const auto return_type  = ^^str;
    const auto eof_action   = ^^decltype([]{ throw pattern_error("unterminated literal string"); });
    const auto error_action = ^^decltype([]{ throw pattern_error("unknown error in literal string"); });

    return lexer_info{ compile_dfa(tokens), actions, eof_action, error_action, return_type };
}


/* main lexer definition */

template<typename CharT>
consteval auto defs<CharT>::main_lexer()
{
    using namespace tok;
    using namespace std::string_view_literals;
    using std::meta::reflect_constant;

    using gm = parser::group_modes;

    const std::vector<rule> rules{
        /* simple tokens */
        { "'.'", ^^dot },
        { "'*'", ^^star },
        { "'+'", ^^plus },
        { "'?'", ^^quest },
        { "'^'", ^^hat },
        { "'$'", ^^dollar },
        { "'|'", ^^vert },
        { "')'", ^^rparen },

        /* lparen */
        { "'(?#'.*?')'",       ^^void /* ignore comments */ },
        { "'(?|'",             ^^decltype([]{ return lpn{ gm::branch_reset }; }) },
        { "'(?>'",             ^^decltype([]{ return lpn{ gm::atomic }; }) },
        { "'(?='",             ^^decltype([]{ return lpn{ gm::positive_lookahead }; }) },
        { "'(?!'",             ^^decltype([]{ return lpn{ gm::negative_lookahead }; }) },
        { "'(?<='",            ^^decltype([]{ return lpn{ gm::positive_lookbehind }; }) },
        { "'(?<!'",            ^^decltype([]{ return lpn{ gm::negative_lookbehind }; }) },
        { "'(?<'w+'>'",        ^^decltype([](const auto& res){ return lpn{ trim(res, 3, 1) }; }), },
        { R"("(?'"w+"'")",     ^^decltype([](const auto& res){ return lpn{ trim(res, 3, 1) }; }), },
        { "'(?P<'w+'>'",       ^^decltype([](const auto& res){ return lpn{ trim(res, 3, 1) }; }), },
        { "'(?'[imnsUx-]*':'", ^^decltype([](const auto& res){ return lpn{ make_flags(trim(res, 2, 1)) }; }), },
        { "'(?'[imnsUx-]*')'", ^^decltype([](const auto& res){ return set_flags{ make_flags(trim(res, 2, 1)) }; }), },
        { "'(?'.*?')'",        ^^decltype([]{ throw pattern_error("group is incomplete"); }) },
        { "'('",               ^^decltype([]{ return lpn{}; }) },

        /* character classes (parse elsewhere) */
        { "'['", ^^cla },

        /* advanced repeaters (captures might be useful here) */
        { "'{'t*d+t*'}'", ^^decltype([](const auto& res){ return make_repeat_fixed(trim(res, 1, 1)); }) },
        { "'{'t*d+t*','t*'}'", ^^decltype([](const auto& res){ return make_repeat_unbounded(trim(res, 1, 1)); }) },
        { "'{'t*d+t*','t*d+t*'}'", ^^decltype([](const auto& res){ return make_repeat_range(trim(res, 1, 1)); }) },

        /* syntax-escaping escape codes */
        { "'\\'A", ^^decltype([](const auto& res){ return str{ trim(res, 1) }; }) },

        /* standard escape codes */
        { "'\\a'", ^^decltype([]{ return str{ '\a' }; }) },
        { "'\\e'", ^^decltype([]{ return str{ '\x1b' }; }) },
        { "'\\f'", ^^decltype([]{ return str{ '\f' }; }) },
        { "'\\t'", ^^decltype([]{ return str{ '\t' }; }) },
        { "'\\n'", ^^decltype([]{ return str{ '\n' }; }) },
        { "'\\r'", ^^decltype([]{ return str{ '\r' }; }) },
        { "'\\v'", ^^decltype([]{ return str{ '\v' }; }) },

        /* perl character classes */
        { "'\\d'", ^^decltype([]{ return cla{ ncc::digits }; }) },
        { "'\\D'", ^^decltype([]{ return cla{ ncc::digits, negated_cc_tag }; }) },
        { "'\\s'", ^^decltype([]{ return cla{ ncc::perl_whitespace }; }) },
        { "'\\S'", ^^decltype([]{ return cla{ ncc::perl_whitespace, negated_cc_tag }; }) },
        { "'\\w'", ^^decltype([]{ return cla{ ncc::word }; }) },
        { "'\\W'", ^^decltype([]{ return cla{ ncc::word, negated_cc_tag }; }) },

        /* assertions */
        { "'\\A'", reflect_constant(assertion{ assert_type::text_start }) },
        { "'\\b'", reflect_constant(assertion{ assert_type::ascii_word_boundary }) },
        { "'\\B'", reflect_constant(assertion{ assert_type::not_ascii_word_boundary }) },
        { "'\\G'", ^^decltype([]{ throw pattern_error("Assertion '\\G' is not implemented"); }) },
        { "'\\Z'", ^^decltype([]{ throw pattern_error("End of text or newlines followed by end of text '\\Z' is not implemented"); }) },
        { "'\\z'", reflect_constant(assertion{ assert_type::text_end }) },

        /* character escape codes */
        { "'\\'o{2,3}",  ^^decltype([](const auto& res){ return str{ parse_oct(trim(res, 1)) }; }) },
        { "'\\0'",       ^^decltype([]{ return str{ '\0' }; }) },
        { "'\\x'h+",     ^^decltype([](const auto& res){ return str{ parse_hex(trim(res, 2)) }; }) },
        { "'\\u'h{4}",   ^^decltype([](const auto& res){ return str{ to_mb<CharT>(parse_ucn(trim(res, 2))) }; }) },
        { "'\\U'h{8}",   ^^decltype([](const auto& res){ return str{ to_mb<CharT>(parse_ucn(trim(res, 2))) }; }) },
        { "'\\o{'o+'}'", ^^decltype([](const auto& res){ return str{ parse_oct(trim(res, 3, 1)) }; }) },
        { "'\\x{'h+'}'", ^^decltype([](const auto& res){ return str{ parse_hex(trim(res, 3, 1)) }; }) },
        { "'\\u{'h+'}'", ^^decltype([](const auto& res){ return str{ to_mb<CharT>(parse_ucn(trim(res, 3, 1))) }; }) },
        { "'\\N{'C+'}'", ^^decltype([](const auto& res){ return str{ to_mb<CharT>(parse_nuc(trim(res, 3, 1))) }; }) },

        /* backreferences */
        { "'\\'d",       ^^decltype([](const auto& res){ return make_bref(trim(res, 1)); }) },
        { "'\\g'd",      ^^decltype([](const auto& res){ return make_bref(trim(res, 2)); }) },
        { "'\\g{'d+'}'", ^^decltype([](const auto& res){ return make_bref(trim(res, 3, 1)); }) },

        /* literal string */
        { "'\\Q'", ^^decltype([]{ return str{ R"(\Q)"sv }; }) },

        /* erroneous escape sequences */
        { "'\\'N?",               ^^decltype([]{ throw parser_error("unknown escape sequence"); }) },
        { "'\\'[oxuN]'{}'",       ^^decltype([]{ throw parser_error("empty delimited character escape sequence"); }) },
        { "'\\'[oxuN]'{'[^\n}]*", ^^decltype([]{ throw parser_error("unterminated character escape sequence"); }) },
        { "'\\'[oxu]'{'N*?'}'",   ^^decltype([]{ throw parser_error("invalid digit in character escape sequence"); }) },
        { "'\\N{'N*?'}'",         ^^decltype([]{ throw parser_error("invalid character in unicode character name"); }) },

        /* maybe skip whitespace and comments */
        /* NB: this is an ugly hack ; with token sequences, we could instead conditionally return a token or
                instead skip the current item; as for comments, we would still need to modify the position of iterator */
        { "s",   ^^decltype([](const auto& res){ str x{ R"(\E)"sv }; x.data.push_back(trim(res).front()); return x; }) },
        { "'#'", ^^decltype([]{ return str{ R"(\E#)"sv }; }) },

        /* catch-all */
        { ".", ^^decltype([](const auto& res){ return str{ trim(res) }; }) }
    };

    auto tokens = rules | std::views::transform(&rule::pattern);
    auto actions = rules | std::views::transform(&rule::action);

    const auto return_type  = ^^token_type<CharT>;
    const auto eof_action   = ^^end_of_input;
    const auto error_action = ^^decltype([]{ throw pattern_error("unknown error in regex pattern"); });

    return lexer_info{ compile_dfa(tokens), actions, eof_action, error_action, return_type };
}

} // namespace autolex



template<typename CharT>
inline constexpr auto amlex = autolex::defs<CharT>::main_lexer();

template<typename CharT>
inline constexpr auto alslex = autolex::defs<CharT>::literal_string_lexer();

template<typename CharT>
inline constexpr auto acclex = autolex::defs<CharT>::charclass_lexer();


/* char class parser */

template<typename CharT>
class cc_parser
{
public:
    using char_class = tok::char_class<CharT>;
    using it_type = std::basic_string_view<CharT>::const_iterator;

    static constexpr char_class parse(it_type& it, const it_type end, bool extended_more);

private:
    using underlying_t = char_class::underlying_char_type;
    using string_t = std::basic_string<underlying_t>;
    using cc_token_t = char_class_token_type<underlying_t>;

    template<typename T>
    static constexpr std::size_t tok_index{ index_in_variant(^^T, ^^cc_token_t) };

    static constexpr string_t transcode_string(it_type first, it_type last);
};

template<typename CharT>
constexpr cc_parser<CharT>::string_t cc_parser<CharT>::transcode_string(it_type first, it_type last)
{
    if constexpr (std::same_as<underlying_t, char>)
    {
        static_assert(std::same_as<CharT, char>);
        return std::string{ first, last };
    }
    else if constexpr (not std::same_as<underlying_t, char32_t>)
    {
        static_assert(false, "Invalid underlying char type");
    }
    else if constexpr (std::same_as<CharT, char32_t>)
    {
        return std::u32string{ first, last };
    }
    else
    {
        static_assert(false, "UNIMPLEMENTED");
    }
}

template<typename CharT>
constexpr cc_parser<CharT>::char_class cc_parser<CharT>::parse(it_type& it, const it_type end, const bool extended_more)
{
    using namespace cctok;

    using lex_cc_impl = p1306dfa<(^^acclex<CharT>)>;
    constexpr lex_cc_impl charclass_lexer;

    char_class result;

    int count{ 0 };
    std::optional<underlying_t> saved;
    bool cc_negated{ false };
    bool is_range{ false };

    auto insert = [&](underlying_t c){
        if (not saved)
        {
            /* delay insert */
            saved = c;
        }
        else if (is_range)
        {
            if (*saved > c)
                throw pattern_error("Invalid character class range");

            /* insert saved - c */
            result.data.insert(*saved, c);
            is_range = false;
            saved.reset();
        }
        else
        {
            /* insert saved */
            result.data.insert(*saved);
            saved = c;
        }
    };

    for (bool loop{ true }; loop;)
    {
        cc_token_t tok{ charclass_lexer(it, end) };

        switch (tok.index())
        {
        case tok_index<end_of_cc>:
            if (count == 0)
                insert(underlying_t{ ']' });
            else
                loop = false;
            break;

        case tok_index<range_or_minus>:
            if (saved.has_value() and not is_range)
                is_range = true;
            else
                insert(underlying_t{ '-' });
            break;

        case tok_index<negate_or_hat>:
            if (count == 0 and not cc_negated)
            {
                cc_negated = true;
                continue;
            }
            insert(underlying_t{ '^' });
            break;

        case tok_index<literal_string>:
        {
            using namespace std::string_view_literals;

            static constexpr auto proj = [](CharT c){ return static_cast<char>(c); };
            const auto& [first, last] = std::ranges::search(std::ranges::subrange{ it, end }, R"(\E)"sv, {}, proj);
            string_t to_insert{ transcode_string(it, first) };
            it = last;

            for (const underlying_t& c : to_insert)
                insert(c);
            break;
        }

        case tok_index<named_cc>:
            if (is_range)
                throw pattern_error("Invalid range in character class");

            /* insert saved if necessary */
            if (saved)
            {
                result.data.insert(*saved);
                saved.reset();
            }

            /* insert char class */
            if (const auto& [ncc, ncc_negated] = get<named_cc>(tok); ncc_negated)
                result.data.insert(ncc, negated_cc_tag);
            else
                result.data.insert(ncc);
            break;

        case tok_index<space>:
            if (extended_more)
                continue;
            insert(underlying_t{ ' ' });
            break;

        case tok_index<tab>:
            if (extended_more)
                continue;
            insert(underlying_t{ '\t' });
            break;

        case tok_index<underlying_t>:
            insert(get<underlying_t>(tok));
            break;
        }

        ++count;
    }

    if (saved)
        result.data.insert(*saved);
    if (saved and is_range)
        result.data.insert(underlying_t{ '-' });
    if (cc_negated)
        result.data.negate();

    return result;
}


/* lexer class implementation */

namespace lexer {

template<typename CharT>
constexpr generated<CharT>::token_t generated<CharT>::nexttok()
{
    using namespace tok;

    using lex_impl    = p1306dfa<(^^amlex<CharT>)>;
    using lex_ls_impl = p1306dfa<(^^alslex<CharT>)>;

    constexpr lex_impl main_lexer;
    constexpr lex_ls_impl literal_string_lexer;

    token_t token;

    static constexpr auto is_whitespace = [](CharT c) static {
        switch (c)
        {
        case '\v':
        case '\t':
        case '\n':
        case '\f':
        case '\r':
        case ' ':
            return true;
        default:
            return false;
        }
    };

    while (true) /* disguised label */
    {
        if (literal_string_mode_)
            token = literal_string_lexer(it_, end_);
        else
            token = main_lexer(it_, end_);

        if (holds_alternative<char_str<CharT>>(token))
        {
            auto& tok_ref = std::get<char_str<CharT>>(token);

            if (tok_ref.data.size() >= 2 and tok_ref.data[0] == '\\')
            {
                if (tok_ref.data[1] == 'E')
                {
                    /* we use "\E" as a prefix for whitespace characters since it cannot be
                    encountered normally -- the lexer always returns 1 character at a time */
                    tok_ref.data.erase(0, 2);

                    if (extended_mode_ >= 1 and tok_ref.data.size() == 1)
                    {
                        if (is_whitespace(tok_ref.data.front()))
                        {
                            it_ += tok_ref.data.size();
                            continue; /* disguised goto */
                        }
                        else if (tok_ref.data.front() == '#')
                        {
                            while (it_ != end_)
                                if (*it_++ == '\n')
                                    break;

                            continue; /* disguised goto */
                        }
                    }
                }
                if (tok_ref.data[1] == 'Q')
                {
                    /* similarly, we use \Q to toggle between main and literal string lexer */
                    literal_string_mode_ = not literal_string_mode_;
                    continue; /* disguised goto */
                }
            }
        }
        else if (holds_alternative<char_class<CharT>>(token))
        {
            if (get<char_class<CharT>>(token).data.empty())
                token = cc_parser<CharT>::parse(it_, end_, extended_mode_ >= 2);
        }

        return token;
    }
}

} // namespace lexer
} // namespace detail
} // namespace srx
