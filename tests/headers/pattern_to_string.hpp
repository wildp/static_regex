// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <concepts>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

#include <rx/ast/tree.hpp>
#include <rx/etc/util.hpp>


// TODO: replace with constexpr std::format specialization
// TODO: add unicode transcoding support

namespace rx::testing
{
    template<typename CharT>
    [[nodiscard]] constexpr std::basic_string<CharT> to_basic_string(const detail::expr_tree<CharT>& ast)
    {
        using ast_t = detail::expr_tree<CharT>;
        using char_type = ast_t::char_type;
        using string_type = std::basic_string<char_type>;

        // Unicode transcoding unsupported -- TODO: change later
        static_assert(std::same_as<char_type, CharT>);
        static_assert(std::same_as<char_type, typename ast_t::char_class::underlying_char_type>);

        static constexpr auto append_int = [](string_type& result, const std::integral auto value) {
            static constexpr decltype(value) base{ 10 };
            std::vector<char> buf;
            if (value == 0)
                buf.emplace_back('0');
            else
                for (auto v = value; v > 0; v /= base)
                    buf.emplace_back('0' + (v % base));
            if (value < 0)
                buf.emplace_back('-');
            std::ranges::move(buf.rbegin(), buf.rend(), std::back_insert_iterator(result));
        };

        static constexpr auto escape = [](string_type& result, char_type c) {
            if (c == '\177' or (c >= '\0' and c < '\40'))
            {
                result += '\\';
                switch (c)
                {
                case '\0': result += '0'; break;
                case '\a': result += 'a'; break;
                case '\b': result += 'b'; break;
                case '\f': result += 'f'; break;
                case '\t': result += 't'; break;
                case '\n': result += 'n'; break;
                case '\r': result += 'r'; break;
                case '\v': result += 'v'; break;
                default:
                {
                    static constexpr char octal_base{ 010 };
                    string_type tmp;
                    for (; c != 0; c /= octal_base)
                        tmp.push_back('0' + (c % octal_base));
                    result.append(tmp.rbegin(), tmp.rend());
                    break;
                }
                }
            }
            else
            {
                if (c == '-' or c == '[' or c == ']' or c == '^') result += '\\';
                result += c;
            }
        };

        string_type res;

        auto visitor = [&ast](this const auto& rec, string_type& result, std::size_t pos) -> void {
            ast.get_expr(pos).visit(detail::overloads{
                [&](const ast_t::assertion& asr) {
                    switch (asr.type)
                    {
                    case detail::assert_type::text_start:
                    case detail::assert_type::line_start:
                        result += '^';
                        break;
                    case detail::assert_type::text_end:
                    case detail::assert_type::line_end:
                        result += '$';
                        break;
                    default:
                        throw std::runtime_error("unrecognised assert_type");
                    }
                },
                [&](const ast_t::concat& cat) {
                    for (const auto idx : cat.idxs)
                        rec(result, idx);
                },
                [&](const ast_t::alt& alt) {
                    for (const auto idx : alt.idxs)
                    {
                        rec(result, idx);
                        result += '|';
                    }

                    if (alt.idxs.size() > 0)
                        result.pop_back();
                },
                [&](const ast_t::tag& tag) {
                    auto [is_lhs, is_rhs] = ast.get_capture_info().capture_side(tag.number);
                    if (is_lhs)
                        result += '(';
                    if (is_rhs)
                        result += ')';
                },
                [&](const ast_t::backref& bref) {
                    result += '\\';
                    append_int(result, bref.number);
                },
                [&](const ast_t::repeat& rep) {
                    rec(result, rep.idx);

                    if (rep.min == 0 and rep.max < 0)
                    {
                        result += '*';
                    }
                    else if (rep.min == 1 and rep.max < 1)
                    {
                        result += '+';
                    }
                    else if ((rep.min == 0 and rep.max == 1))
                    {
                        result += '?';
                    }
                    else
                    {
                        result += '{';
                        append_int(result, rep.min);
                        if (rep.max != rep.min)
                            result += ',';
                        if (rep.max > rep.min)
                            append_int(result, rep.max);
                        result += '}';
                    }

                    if (rep.mode == detail::repeater_mode::lazy)
                        result += '?';
                    else if (rep.mode == detail::repeater_mode::possessive)
                        result += '+';
                },
                [&](const ast_t::char_str& lit) {
                    // for (const auto c: lit.data)
                    //     escape(result, c);
                    result += lit.data;
                },
                [&](const ast_t::char_class& cla) {
                    using pair_t = typename ast_t::char_class::impl_type::underlying_type::char_interval;
                    const auto dn = cla.data.denormalise();
                    const auto ci = dn.intervals();

                    if (dn.is_negated() and ((ci.empty()) or (ci.size() == 1 and ci.front() == pair_t{ '\n', '\n' })))
                    {
                        result += '.';
                        return;
                    }

                    result += '[';
                    if (dn.is_negated())
                        result += '^';

                    for (const auto& [lower, upper] : ci)
                    {
                        escape(result, lower);
                        if (upper == lower)
                            continue;
                        result += '-';
                        escape(result, upper);
                    }
                    result += ']';
                },
            });
        };

        visitor(res, ast.root_idx());

        return res;
    }

    [[nodiscard]] constexpr std::string to_string(const detail::expr_tree<char>& ast)
    {
        return to_basic_string(ast);
    }
}
