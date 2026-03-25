// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <deque>
#include <format>
#include <limits>
#include <print>
#include <ranges>
#include <string>
#include <variant>

#include <rx/etc/util.hpp>
#include <rx/fsm/tdfa.hpp>
#include <rx/fsm/tnfa.hpp>


namespace rx::testing
{
    template<typename T>
    concept print_destination = requires (T t) { std::println(t); };

    template<typename CharSetType>
    constexpr std::string make_pretty_charset_string(const CharSetType& cs)
    {
        using char_type = CharSetType::char_type;
        static_assert(std::same_as<char_type, char>); // Unicode transcoding unsupported

        static constexpr std::size_t byte_bits{ std::numeric_limits<unsigned char>::digits };
        static constexpr std::size_t max_count{ 0b1uz << (sizeof(char_type) * byte_bits) };

        const bool is_negated{ cs.count() > max_count / 2 };
        const auto negated_cs = ~cs;

        const auto intervals = ((is_negated) ? negated_cs : cs).get_intervals();

        if (is_negated and intervals.empty())
            return ".";

        if (not is_negated and intervals.size() == 1)
            if (const auto& [lower, upper] = intervals.back(); lower == upper)
                if (('0' <= lower and lower <= '9') or ('A' <= lower and lower <= 'Z') or ('a' <= lower and lower <= 'z'))
                    return std::string{ lower };

        std::string result;
        result += '[';

        if (is_negated)
            result += '^';

        static constexpr auto get_char = [](char_type c) {
            if (c == '-' or c == '[' or c == ']' or c == '^')
                return std::format("\\{}", c);

            std::string str{ std::format("{:?}", c) };
            str.pop_back();
            str.erase(str.begin());
            return str;
        };

        for (const auto [lower, upper] : intervals)
        {
            result += get_char(lower);
            if (lower == upper)
                continue;
            result += '-';
            result += get_char(upper);
        }

        result += ']';
        return result;
    }

    template<print_destination T, typename CharT>
    constexpr void graph_export(T target, const detail::expr_tree<CharT>& ast)
    {
        static_assert(std::same_as<CharT, char>); // Unicode transcoding unsupported

        using ast_t = detail::expr_tree<CharT>;

        std::println(target, "digraph {{");
        std::println(target, "    rankdir=\"TB\";");
        std::println(target, "    fontsize=10;");
        std::println(target);

        std::deque<std::size_t> to_visit{ ast.root_idx() };
        const auto& ci = ast.get_capture_info();
        std::vector<std::pair<std::size_t, std::size_t>> edges;

        while (not to_visit.empty())
        {
            const std::size_t current_idx{ to_visit.front() };
            to_visit.pop_front();

            std::string label = ast.get_expr(current_idx).visit(detail::overloads{
                [&](const ast_t::assertion& asr) -> std::string {
                    switch (asr.type)
                    {
                    case detail::assert_type::text_start:
                    case detail::assert_type::line_start:
                        return "^";
                    case detail::assert_type::text_end:
                    case detail::assert_type::line_end:
                        return "$";
                    default:
                        throw std::runtime_error("unrecognised assert_type");
                    }
                },
                [&](const ast_t::concat& cat) -> std::string {
                    to_visit.append_range(cat.idxs);
                    edges.append_range(cat.idxs | std::views::transform([&](std::size_t dst) -> std::pair<std::size_t, std::size_t> { return { current_idx, dst }; }));
                    return "";
                },
                [&](const ast_t::alt& alt) -> std::string {
                    to_visit.append_range(alt.idxs);
                    edges.append_range(alt.idxs | std::views::transform([&](std::size_t dst) -> std::pair<std::size_t, std::size_t> { return { current_idx, dst }; }));
                    return "|";
                },
                [&](const ast_t::tag& tag) -> std::string {
                    const auto [is_lhs, is_rhs] = ci.capture_side(tag.number);
                    if (is_lhs) return "(";
                    if (is_rhs) return ")";
                    return "()";
                },
                [&](const ast_t::backref& bref) -> std::string {
                    return std::format("\\{}", bref.number);
                },
                [&](const ast_t::repeat& rep) -> std::string {
                    to_visit.emplace_back(rep.idx);
                    edges.emplace_back(current_idx, rep.idx);

                    std::string qualifier;
                    if (rep.mode == detail::repeater_mode::lazy) qualifier = "?";
                    else if (rep.mode == detail::repeater_mode::possessive) qualifier = "+";

                    if (rep.min == 0 and rep.max < 0)
                        return std::format("*{}", qualifier);
                    else if (rep.min == 1 and rep.max < 1)
                        return std::format("+{}", qualifier);
                    else if ((rep.min == 0 and rep.max == 1))
                        return std::format("?{}", qualifier);
                    else if (rep.max == rep.min)
                        return std::format("{{{}}}{}", rep.min, qualifier);
                    else if (rep.max < rep.min)
                        return std::format("{{{},}}{}", rep.min, qualifier);
                    else
                        return std::format("{{{},{}}}{}", rep.min, rep.max, qualifier);
                },
                [&](const ast_t::char_str& lit) -> std::string {
                    return std::format("{:?}", lit.data);
                },
                [&](const ast_t::char_class& cla) -> std::string {
                    return make_pretty_charset_string(cla.data.get());
                },
            });

            std::println(target, "    n{} [label={:?}];", current_idx, label);
        }

        std::println(target);

        for (const auto [src, dst] : edges)
            std::println(target, "    n{} -> n{}", src, dst);

        std::println(target);
        std::println(target, "    {{ rank=min; n{} }}", ast.root_idx());

        std::println(target, "}}");
    }

    template<print_destination T, typename CharT>
    void graph_export(T target, const detail::tagged_nfa<CharT>& nfa)
    {
        static_assert(std::same_as<CharT, char>); // Unicode transcoding unsupported

        namespace tnfa = detail::tnfa;

        std::println(target, "digraph {{");
        std::println(target, "    rankdir=\"LR\";");
        std::println(target, "    fontsize=10;");
        std::println(target, "    node [shape=circle,width=0.4,fixedsize=true];");
        std::println(target);

        std::vector<tnfa::state_t> final_nodes;

        for (tnfa::state_t q{ 0 }, q_end{ nfa.node_count() }; q < q_end; ++q)
        {
            const auto& node = nfa.get_node(q);

            if (node.in_tr.empty() and node.out_tr.empty())
                continue;

            /* do stuff like labels, etc */
            if (node.is_final)
            {
                std::println(target, "    q{} [shape=doublecircle];", q);
                final_nodes.emplace_back(q);
            }
        }

        std::println();

        for (tnfa::tr_index i{ 0 }, i_end{ nfa.transition_count() }; i < i_end; ++i)
        {
            const auto& tr = nfa.get_tr(i);

            if (holds_alternative<std::monostate>(tr.type))
                continue;

            std::string label{
                tr.type.visit(detail::overloads(
                [](std::monostate) {
                    return std::string{};
                },
                [](const tnfa::normal_tr<CharT>& t) {
                    return make_pretty_charset_string(t.cs);
                },
                [](const tnfa::epsilon_tr& t) {
                    if (t.tag == 0)
                        return std::format("{}/ϵ", t.priority);
                    else if (t.tag > 0)
                        return std::format("{}/t{}", t.priority, t.tag);
                    else
                        return std::format("{}/-t{}", t.priority, -t.tag);
                },
                [](const tnfa::sof_anchor_tr&) {
                    return std::string{ "^" };
                },
                [](const tnfa::eof_anchor_tr&) {
                    return std::string{ "$" };
                },
                [](const tnfa::lookahead_1_tr<CharT>& t) {
                    return std::format("(?={})", make_pretty_charset_string(t.cs));
                },
                [](const tnfa::lookbehind_1_tr<CharT>& t) {
                    return std::format("(?<={})", make_pretty_charset_string(t.cs));
                }
               ))
            };

            std::println(target, "    q{} -> q{} [label={:?}];", tr.src, tr.dst, label);
        }

        std::println(target);

        std::print(target, "    {{ rank=min; q{};", nfa.start_node());
        for (const auto& cont : nfa.get_cont_info())
            if (cont.value != nfa.start_node())
                std::print(target, " q{};", cont.value);
        std::println(target, " }}");

        std::print(target, "    {{ rank=max;");
        for (const tnfa::state_t q : final_nodes)
            std::print(target, " q{};", q);
        std::println(target, " }}");

        std::println(target, "}}");
    }


    template<print_destination T, typename CharT>
    void graph_export(T target, const detail::tagged_dfa<CharT>& dfa)
    {
        static_assert(std::same_as<CharT, char>); // Unicode transcoding unsupported

        namespace tdfa = detail::tdfa;

        std::println(target, "digraph {{");
        std::println(target, "    rankdir=\"LR\";");
        std::println(target, "    fontsize=10;");
        std::println(target, "    node [shape=circle,width=0.4,fixedsize=true];");
        std::println(target);

        // TODO: show regops instead of op_index

        for (const auto& [i, info] : dfa.final_nodes())
        {
            std::string label{ std::format("q{}", i) };
            std::string xlabel;

            if (info.op_index != tdfa::no_transition_regops)
                xlabel += std::format("/{}", info.op_index);

            if (const auto it = dfa.fallback_nodes().find(i); it != dfa.fallback_nodes().end())
            {
                label += 'f';
                if (it->second.op_index != tdfa::no_transition_regops)
                    xlabel += std::format("{}({})", (xlabel.empty() ? "/" : ""), it->second.op_index);
            }

            std::println(target, "     q{} [shape=doublecircle,label={:?},xlabel={:?}];", i, label, xlabel);
        }

        std::println(target);

        for (std::size_t i{ 0 }, i_end{ dfa.node_count() }; i < i_end; ++i)
        {
            const auto& node = dfa.get_node(i);

            for (const auto& tr : node.tr)
            {
                std::string label{ make_pretty_charset_string(tr.cs) };
                if (tr.op_index != tdfa::no_transition_regops)
                    label += std::format(" /{}", tr.op_index);

                std::println(target, "     q{} -> q{} [label={:?}];", i, tr.next, label);
            }

            if (node.default_tr.has_value())
            {
                if (const auto op_index = node.default_tr->op_index; op_index == detail::tdfa::default_transition_is_not_state)
                {
                    std::println(target, "     q{} -> q{} [style=\"dotted\"];", i, node.default_tr->next);
                }
                else
                {
                    std::string label{ "." };
                    if (op_index != tdfa::no_transition_regops)
                        label += std::format(" /{}", op_index);
                    std::println(target, "     q{} -> q{} [label={:?}];", i, node.default_tr->next, label);
                }
            }
        }

        std::println(target, "}}");
    }
}
