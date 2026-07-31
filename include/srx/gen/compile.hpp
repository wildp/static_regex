// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <meta>
#include <ranges>
#include <utility>
#include <vector>

#include "srx/ast/tree.hpp"
#include "srx/etc/static_charset.hpp"
#include "srx/etc/static_span.hpp"
#include "srx/fsm/flags.hpp"
#include "srx/fsm/tdfa.hpp"
#include "srx/fsm/tnfa.hpp"


namespace srx {
namespace detail {

struct final_capture_info
{
    explicit consteval final_capture_info(const capture_info& ci)
        : captures{ ci.get_values_branchfree() } {}

    consteval final_capture_info() = default;

    [[nodiscard]] consteval std::size_t capture_count() const noexcept
    {
        return captures.size();
    }

    [[nodiscard]] consteval bool has_match_start() const
    {
        static constexpr auto pred = [](const capture_info::tag_pair_t& pair){
            const auto& [fst, snd] = pair;
            return (fst.tag_number == start_of_input_tag or snd.tag_number == start_of_input_tag);
        };

        return std::ranges::any_of(captures, pred);
    }

    /* data members (public so that final_capture_info is structural) */
    static_span<capture_info::tag_pair_t> captures;
};

struct register_operation
{
    tdfa::reg_t dst;
    tdfa::reg_t cpy_src;
    bool        set_val;
    bool        is_copy; /* true if cpy, false if set */
};

struct static_backlink
{
    tdfa::backlink_index_t   prev_index{};
    static_span<tdfa::tag_t> tag_seq;
};

template<typename CharT>
struct static_transition
{
    std::size_t next;
    std::size_t op_index;
    static_charset<CharT> cs;
};

template<typename CharT>
struct tdfa_info
{
    using char_type = CharT;

private:
    static consteval auto make_static_transition(const tdfa::transition<char_type>& tr)
    {
        return static_transition{ tr.next, tr.op_index, static_charset{ tr.cs } };
    }

    static consteval auto make_static_outer_transition(const std::pair<std::size_t, tnfa::charset_t<char_type>>& otr)
    {
        return static_transition{ otr.first, tdfa::no_transition_regops, static_charset{ otr.second } };
    }

    static consteval auto make_node_transitions(const tdfa::node<char_type>& n)
    {
        return static_span{ n.tr | std::views::transform(make_static_transition) };
    }

    static consteval auto make_register_operations(const tdfa::regops_t& o)
    {
        return static_span{ o | std::views::transform(
            [](const tdfa::regop& op) consteval -> register_operation {
                if (const auto* set = get_if<tdfa::regop::set>(&op.op); set != nullptr)
                    return { .dst = op.dst, .cpy_src = 0, .set_val = set->val, .is_copy = false };
                else if (const auto* cpy = get_if<tdfa::regop::copy>(&op.op); cpy != nullptr)
                    return { .dst = op.dst, .cpy_src = cpy->src, .set_val = false, .is_copy = true };
                else
                    std::unreachable();
            })
        };
    }

    static consteval auto make_backlink(const tdfa::backlink& bl)
    {
        return static_backlink{ .prev_index = bl.prev_index, .tag_seq{ bl.tags_seq } };
    }

    static consteval auto make_backlinks(const tdfa::backlinks_t& b)
    {
        return static_span{ b | std::views::transform(make_backlink) };
    }

    static consteval auto make_continue_info(const tagged_dfa<char_type>& dfa, const tagged_nfa<char_type>& nfa)
    {
        /* adapted from tagged_dfa::minimise_transition_edges */
        using tr_type = std::pair<std::size_t, tdfa::charset_t<char_type>>;

        std::vector<tr_type> vec;

        for (const auto& [nfa_cont, dfa_cont] : std::views::zip(nfa.get_cont_info(), dfa.continue_nodes()))
            vec.emplace_back(dfa_cont, std::cref(nfa_cont.cs));

        if (not vec.empty())
        {
            auto scored_pairs = vec
                                | std::views::transform([](const auto& t){ return t.second.score_intervals(); })
                                | std::views::enumerate
                                | std::ranges::to<std::vector>();

            std::ranges::sort(scored_pairs, {}, [](const auto& x){ return get<1>(x); });

            std::vector<tr_type> new_tr;
            std::vector<tdfa::charset_t<char_type>> dont_cares;
            tdfa::charset_t<char_type> acc;

            for (const auto& [i, _] : scored_pairs)
            {
                auto& tr = vec.at(i);
                dont_cares.emplace_back(acc);
                acc |= tr.second;
                new_tr.emplace_back(std::move(tr));
            }

            if (acc.full())
            {
                new_tr.back().second = std::move(acc);
                dont_cares.back() = tdfa::charset_t<char_type>{};
            }

            /* fill gaps where possible */

            for (const auto& [tr_ref, dont_cares] : std::views::zip(std::ranges::ref_view(new_tr), dont_cares))
            {
                tr_type& tr{ tr_ref };

                if (dont_cares.empty())
                    continue;

                using interval_t = tdfa::charset_t<char_type>::char_interval;
                std::vector<interval_t> to_insert;

                std::ranges::set_intersection((~tr.second).get_intervals(), dont_cares.get_intervals(), std::back_inserter(to_insert));

                for (const auto [beg, end] : to_insert)
                {
                    if (beg == end)
                        tr.second.insert(beg);
                    else
                        tr.second.insert(beg, end);
                }
            }

            vec = std::move(new_tr);
        }

        return static_span{ vec | std::views::transform(make_static_outer_transition) };
    }

public:
    explicit consteval tdfa_info(const tagged_dfa<char_type>& dfa, const tagged_nfa<char_type>& nfa,
                                 const std::pair<std::size_t, std::size_t>& mml, fsm_flags f, bool alt_mode)
        : nodes{ dfa.nodes_ | std::views::transform(make_node_transitions) }
        , regops{ dfa.regops_ | std::views::transform(make_register_operations) }
        , backlink_arrays{ dfa.backlink_arrays_ | std::views::transform(make_backlinks) }
        , continue_nodes{ dfa.continue_nodes() }
        , additional_continue_nodes{ dfa.additional_continue_nodes() }
        , final_nodes{ dfa.final_nodes() }
        , fallback_nodes{ dfa.fallback_nodes() }
        , final_registers{ dfa.final_registers() }
        , tag_count{ dfa.tag_count() }
        , register_count{ dfa.reg_count() }
        , match_start{ dfa.match_start }
        , captures{ dfa.get_capture_info() }
        , outer_transitions{ make_continue_info(dfa, nfa) }
        , min_max_lengths{ mml }
        , flags{ f }
        , alt_mode{ alt_mode }
        , onepass{ dfa.is_onepass() } {}

    [[nodiscard]] consteval bool has_continue() const
    {
        return flags.is_iterator and not (continue_nodes.size() == 1 and continue_nodes[0] == match_start);
    }

    [[nodiscard]] consteval std::uint_least16_t largest_offset() const
    {
        if (final_nodes.empty())
            return 0;
        return std::ranges::max(final_nodes | std::views::transform([](const auto& x){ return x.second.offset; }));
    }

    /* data members (public so that tdfa_info is structural) */
    static_span<static_span<static_transition<char_type>>> nodes;
    static_span<static_span<register_operation>> regops;
    static_span<static_span<static_backlink>> backlink_arrays;
    static_span<std::size_t> continue_nodes;
    static_span<std::size_t> additional_continue_nodes;
    static_map<std::size_t, tdfa::final_node_info> final_nodes;
    static_map<std::size_t, tdfa::fallback_node_info> fallback_nodes;
    static_span<tdfa::reg_t> final_registers;

    std::size_t tag_count{ 0 };
    std::size_t register_count{ 0 };
    std::size_t match_start{ 0 };
    final_capture_info captures;

    static_span<static_transition<char_type>> outer_transitions;
    std::pair<std::size_t, std::size_t> min_max_lengths;

    fsm_flags flags;
    bool alt_mode;
    bool onepass;
};

template<typename CharT>
struct lexer_info
{
    using char_type = CharT;

    lexer_info() = delete;

    template<std::ranges::range R>
        requires std::same_as<std::ranges::range_value_t<R>, std::meta::info>
    consteval lexer_info(tdfa_info<CharT>&& dfa, R&& actions, std::meta::info eof_act, std::meta::info err_act, std::meta::info ret_ty)
        : dfa{ std::move(dfa) }
        , actions{ std::forward<R>(actions) }
        , eof_action{ eof_act }
        , error_action{ err_act }
        , return_type{ ret_ty } {}


    /* data members (public so that tdfa_info is structural) */
    tdfa_info<CharT> dfa;
    static_span<std::meta::info> actions;
    std::meta::info eof_action;
    std::meta::info error_action;
    std::meta::info return_type;
};


template<typename CharT>
consteval tdfa_info<CharT> compile_pattern(std::basic_string_view<CharT> pattern, fsm_flags f, bool onepass)
{
    /* set parser flags as appropriate */
    parser_flags p{};
    if (f.no_captures)
        p.enable_captures = false;
    if (f.return_bool)
        p.enable_start_tag = false;

    /* parse pattern string into tree */
    expr_tree ast{ pattern, p };
    if (f.is_search)
        ast.insert_search_prefix();
    ast.optimise_tags();
    const auto mml = ast.min_max_length();

    /* convert to tnfa */
    tagged_nfa nfa{ ast, f };
    nfa.rewrite_assertions();
    if (f.maybe_no_empty and mml.first == 0)
        nfa.add_non_empty_match_pathway();

    /* convert to tdfa */
    tagged_dfa dfa{ nfa, onepass };
    dfa.optimise_registers();
    // dfa.minimise_states();

    /* optimise transition edges and their order to produce fewest comparisons */
    /* (if using tables, do `dfa.make_default_tr_if_possible()` instead) */
    dfa.minimise_transition_edges();
    dfa.de_default_edges();

    return tdfa_info{ dfa, nfa, mml, f, ast.is_alt_mode() };
}

template<std::meta::info P, ff F>
inline constexpr auto re = compile_pattern(std::basic_string_view{ [: P :] }, unpack_flags(F), true);

template<std::meta::info P, ff F>
inline constexpr auto rf = compile_pattern(std::basic_string_view{ [: P :] }, unpack_flags(F), false);


struct match_non_empty_t {};
inline constexpr match_non_empty_t match_non_empty;

} // namespace detail
} // namespace srx
