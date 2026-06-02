// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <print>
#include <meta>

#include "graph_export.hpp"
#include "dump_fsm.hpp"


enum class graph_export_option : unsigned char
{
    ast,
    nfa,
    dfa
};

enum class match_type_option : unsigned char
{
    full,
    prefix,
    search,
    searchall
};

int main(int argc, char* argv[])
{
    using namespace std::string_view_literals;

    std::span args(argv, argc);

    if (args.size() < 3)
    {
        std::println(stderr, "usage: graph_export type [flags] pattern");
        return 1;
    }

    auto mode = [](std::string_view sv) -> std::optional<graph_export_option> {
        template for (constexpr std::meta::info r : std::define_static_array(enumerators_of(^^graph_export_option)))
            if (sv == identifier_of(r))
                return [: r :];
        return {};
    }(args[1]);

    if (not mode.has_value())
    {
        std::println(stderr, "error: invalid type option");
        return 1;
    }

    const std::vector<std::string_view> opts(args.begin() + 2, args.end() - 1);

    srx::detail::parser_flags pflags{};
    std::string_view pattern{ args.back() };
    bool optimise_tags{ true };
    bool optimise_registers{ true };
    bool optimise_edges{ true };
    bool linear_search_mode{ false };
    bool dump{ false };
    match_type_option mto{ match_type_option::searchall };


    for (std::string_view sv : std::ranges::subrange(args.begin() + 2, args.end() - 1))
    {
        if (not sv.starts_with("--"))
        {
            std::println(stderr, "error: invalid argument {:?}", sv);
            return 1;
        }

        bool fail{ false };
        const auto sv_copy{ sv };
        sv.remove_prefix(2);

        if (sv == "dump")
        {
            if (*mode == graph_export_option::ast)
            {
                std::println(stderr, "error: option {:?} is incompatible with type=ast", sv_copy);
                return 1;
            }
            dump = true;
        }
        else if (sv == "linear")
        {
            linear_search_mode = true;
        }
        else if (sv.starts_with("no-"))
        {
            sv.remove_prefix(3);

            if (sv == "optimise-tags")
                optimise_tags = false;
            else if (sv == "optimise-registers")
                optimise_registers = false;
            else if (sv == "optimise-edges")
                optimise_edges = false;
            else
                fail = true;
        }
        else if (sv.starts_with("m"))
        {
            [&]{
                template for (constexpr std::meta::info r : std::define_static_array(enumerators_of(^^match_type_option)))
                {
                    if (sv == identifier_of(r))
                    {
                        mto = [: r :];
                        return;
                    }
                }

                fail = true;
            }();
        }
        else
        {
            fail = true;
        }

        if (fail)
        {
            std::println(stderr, "error: unrecognised flag {:?}", sv_copy);
            return 1;
        }
    }

    const bool is_search{ (mto == match_type_option::search or mto == match_type_option::searchall) };

    srx::detail::expr_tree ast{ pattern, pflags };

    if (is_search and linear_search_mode)
        ast.insert_search_prefix();

    if (optimise_tags)
        ast.optimise_tags();

    if (*mode == graph_export_option::ast)
    {
        srx::tools::graph_export(stdout, ast);
        return 0;
    }

    auto flags = [&]{
        auto inner = [&]{
            switch (mto)
            {
            case match_type_option::full:
                return srx::detail::default_fsm_flags::full_match;
            case match_type_option::prefix:
                return srx::detail::default_fsm_flags::partial_match;
            case match_type_option::search:
                return srx::detail::default_fsm_flags::search_single;
            case match_type_option::searchall:
                return srx::detail::default_fsm_flags::search_all;
            }
            return srx::detail::fsm_flags{};
        }();

        if (is_search and not linear_search_mode)
            return srx::detail::adapt_searcher_flags_to_matcher(inner);
        return inner;
    }();

    srx::detail::tagged_nfa nfa{ ast, flags };
    nfa.rewrite_assertions();

    if (flags.maybe_no_empty and ast.empty_match_possible())
        nfa.add_non_empty_match_pathway();

    if (*mode == graph_export_option::nfa)
    {
        if (dump)
        {
            std::println(stderr, "error: dumping nfa is unimplemented");
            return 1;
        }
        else
            srx::tools::graph_export(stdout, nfa);
        return 0;
    }

    srx::detail::tagged_dfa dfa{ nfa };

    if (optimise_registers)
        dfa.optimise_registers();

    // dfa.minimise_states();

    if (optimise_edges)
        dfa.minimise_transition_edges();

    // dfa.make_default_transitions();
    // dfa.make_shared_transitions();

    if (*mode == graph_export_option::dfa)
    {
        if (dump)
            srx::tools::dump_fsm(stdout, dfa);
        else
            srx::tools::graph_export(stdout, dfa);
        return 0;
    }

    return 1;
}


/* Some example patterns:
    "(?m:\n$)+"sv
    "abc|cbd"sv
    "abcdef|bcdef"sv
    "abcdef|bcdef$"sv
    "(?m:^\n*$)"sv
    "(?ms:^abc.)"sv
    "(?m:^abc)"sv
    "(abc)|(^abc)"sv
    "(?m:abc$)"sv
    "(?m)(^\n$)+"
    "(?m)($\n^)+"
    "(?i)Twain"
    "[a-z]shing"
    "(?i)Tom|Sawyer|Huckleberry|Finn"
    R"(Tom.{10,25}river|river.{10,25}Tom)"
    R"(Tom.{1,10}river|river.{1,10}Tom)"
    ".{2,4}(Tom|Sawyer|Huckleberry|Finn)"
    "([a-z])+[a-z]?"
    "a"
    "(?:)|abc"
    "(?:)"
    "\\b"
    "\\b|abc"
    "\\D"
    ".{0,4}Tom"sv
    "(?:az)*ing"sv
    "[A-Za-z]+ing"
    "(?:[A-Z][a-z])+InG"
 */