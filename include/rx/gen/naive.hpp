// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <iterator>
#include <limits>
#include <meta>
#include <numeric>
#include <utility>

#include "rx/ast/tree.hpp"
#include "rx/etc/captures.hpp"
#include "rx/etc/string_literal.hpp"
#include "rx/etc/static_span.hpp"
#include "rx/etc/util.hpp"
#include "rx/gen/compile.hpp"
#include "rx/gen/result.hpp"


namespace rx::detail
{
    template<typename CharT>
    struct tree_info
    {
        using old_type = expr_tree<CharT>::type;

        struct transition
        {
            static constexpr bool is_narrow{ std::same_as<CharT, char> };
            using underlying_char_type = std::conditional_t<is_narrow, char, char32_t>;

            consteval explicit(false) transition(const char_class_impl<is_narrow>& cci)
                : cs{ cci.intervals() } {}

            static_span<std::pair<underlying_char_type, underlying_char_type>> cs;
        };

        template<typename U>
        struct nttp_of;

        consteval
        {
            for (const auto expr_type : template_arguments_of(dealias(^^old_type)))
            {
                if (is_aggregate_type(expr_type) and is_trivially_copyable_type(expr_type))
                    continue;

                auto spec{ nonstatic_data_members_of(expr_type, std::meta::access_context::current()) };

                for (auto& info : spec)
                {
                    std::meta::data_member_options options{ .name = identifier_of(info) };
                    const auto type{ type_of(info) };

                    if (has_template_arguments(type) and (template_of(type) == ^^std::vector or template_of(type) == ^^std::basic_string))
                        info = data_member_spec(substitute(^^static_span, { template_arguments_of(type)[0] }), options);
                    else if (has_template_arguments(type) and template_of(type) == ^^char_class_impl)
                        info = data_member_spec(^^transition, options);
                    else if (is_integral_type(type) or is_enum_type(type) or (is_aggregate_type(type) and is_trivially_copyable_type(type)))
                        info = data_member_spec(type, options);
                    else
                        throw regex_error("unsupported variant type ");
                }

                define_aggregate(substitute(^^nttp_of, { dealias(expr_type) }), spec);
            }
        }

        struct type
        {
            struct empty {};

            union value_t;

            consteval
            {
                auto mems{ template_arguments_of(dealias(^^old_type)) };
                for (auto& member : mems)
                {
                    std::string_view name;

                    if (has_template_arguments(dealias(member)))
                        name = display_string_of(template_of(dealias(member)));
                    else
                        name = display_string_of(member);

                    member = dealias(member);

                    if (not (is_aggregate_type(member) and is_trivially_copyable_type(member)))
                        member = substitute(^^nttp_of, { member });

                    member = data_member_spec(member, { .name = name });
                }
                define_aggregate(^^value_t, mems);
            }

            static constexpr auto nsdms{ define_static_array(nonstatic_data_members_of(^^value_t, std::meta::access_context::unchecked())) };

            using index_t = unsigned char;

            value_t value;
            index_t index;
        };

    private:
        static consteval type to_static_expression(const old_type& expr)
        {
            constexpr auto original_type{ dealias(^^old_type) };

            /* splicing designated initializers is unsupported in C++26; if it was, we could do something like this: */

            // template for (constexpr std::size_t I : std::views::iota(0uz, variant_size(original_type)))
            // {
            //     if (expr.index() == I)
            //     {
            //         const auto& [...mems]{ get<I>(expr) };
            //         return { .value{ .[: type::nsdms[I] :]{ mems... } }, .index = I };
            //     }
            // }

            // throw std::bad_variant_access("tree_info::to_static_expression: argument has invalid variant alternative")

            /* we do this instead as a temporary workaround */

            using index_t = type::index_t;

            return expr.visit(overloads{
                [](const typename expr_tree<CharT>::assertion& e) -> type
                {
                    const auto& [...mems]{ e };
                    return { .value{ .assertion{ mems... } }, .index = std::saturate_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::assertion, original_type)) };
                },
                [](const typename expr_tree<CharT>::char_str& e) -> type
                {
                     const auto& [...mems]{ e };
                     return { .value{ .char_str{ mems... } }, .index = std::saturate_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::char_str, original_type)) };
                },
                [](const typename expr_tree<CharT>::char_class& e) -> type
                {
                     const auto& [...mems]{ e };
                     return { .value{ .char_class{ mems... } }, .index = std::saturate_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::char_class, original_type)) };
                },
                [](const typename expr_tree<CharT>::backref& e) -> type
                {
                    const auto& [...mems]{ e };
                    return { .value{ .backref{ mems... } }, .index = std::saturate_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::backref, original_type)) };
                },
                [](const typename expr_tree<CharT>::alt& e) -> type
                {
                    const auto& [...mems]{ e };
                    return { .value{ .alt{ mems... } }, .index = std::saturate_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::alt, original_type)) };
                },
                [](const typename expr_tree<CharT>::concat& e) -> type
                {
                    const auto& [...mems]{ e };
                    return { .value{ .concat{ mems... } }, .index = std::saturate_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::concat, ^^old_type)) };
                },
                [](const typename expr_tree<CharT>::tag& e) -> type
                {
                    const auto& [...mems]{ e };
                    return { .value{ .tag{  mems... } }, .index = std::saturate_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::tag, original_type)) };

                },
                [](const typename expr_tree<CharT>::repeat& e) -> type
                {
                    const auto& [...mems]{ e };
                    return { .value{ .repeat{ mems... } }, .index = std::saturate_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::repeat, original_type)) };
                }
                // , [](const typename expr_tree<CharT>::special& e) -> type
                // {
                //     const auto& [...mems]{ e };
                //     return { .value{ .special{ mems... } }, .index = std::saturate_cast<index_t>(index_in_variant(^^typename expr_tree<CharT>::special, original_type)) };
                // }
            });
        }

    public:
        consteval tree_info(const expr_tree<CharT>& ast, const capture_info::staging_info_t& tag_info)
            : root_idx{ ast.root_idx() },
              tag_count{ ast.tag_count() },
              exprs{ ast.get_all_exprs() | std::views::transform(to_static_expression) },
              staging{ tag_info },
              fci{ ast.get_capture_info() },
              empty_match_possible{ ast.empty_match_possible() } {}

        [[nodiscard]] consteval static_match_result_info make_match_result_info() const
        {
            static_span regs{ std::views::iota(0u, static_cast<tdfa::reg_t>(tag_count)) };
            return { .fci = fci, .final_registers = regs, .register_count = tag_count, .has_continue = false };
        }

        std::size_t root_idx;
        std::size_t tag_count;
        static_span<type> exprs;
        static_map<tag_number_t, tag_number_t> staging;

        final_capture_info fci;

        bool empty_match_possible;
    };


    template<typename CharT>
    consteval tree_info<CharT> parse_pattern(std::basic_string_view<CharT> pattern)
    {
        /* set parser flags as appropriate */
        parser_flags p{
            .enable_possessive = true,
            .enable_backrefs = true,
            .enable_branchreset = true
        };

        /* parse pattern string into tree */
        expr_tree ast{ pattern, p };
        ast.simplify_counted_repeat();
        ast.optimise_exact_repeat();
        auto tag_info{ ast.tag_to_register() };

        return tree_info{ ast, tag_info };
    }

    template<static_span Intervals, typename CharT>
    [[gnu::always_inline]] constexpr bool cs_possible(CharT c)
    {
        // TODO: change to use structured binding when supported
        template for (constexpr auto pair : Intervals)
        {
            if (pair.first <= c and c <= pair.second)
                return true;
        }

        return false;
    }

    template<string_literal Pattern>
    struct p1306_naive_impl
    {
        using char_type = decltype(Pattern)::value_type;

    private:
        using ast_t = expr_tree<char_type>;
        using info_t = tree_info<char_type>;

        template<typename T>
        static constexpr std::size_t ast_index{ index_in_variant(^^T, ^^typename expr_tree<char_type>::type) };

        static constexpr info_t ast{ parse_pattern(Pattern.view()) };

        static constexpr std::size_t require_full_match{ std::numeric_limits<std::size_t>::max() };
        static constexpr std::size_t require_non_empty_match{ std::numeric_limits<std::size_t>::max() - 1 };

        template<typename I>
        using result = static_regex_match_result<I, ast.make_match_result_info()>;

        template<typename I>
        struct result_helper
        {
            std::array<I, ast.staging.size()> reg;
        };

        template<typename I>
        using staging_info = maybe_type_t<not ast.staging.empty(), result_helper<I>>;

        static consteval std::size_t get_staging_index(tag_number_t tagnum)
        {
            return static_cast<std::size_t>(std::ranges::distance(ast.staging.begin(), ast.staging.find(tagnum)));
        }

        template<typename I>
        [[gnu::always_inline]] static constexpr void apply_final_staging_info(result<I>& res, staging_info<I>& si)
        {
            if constexpr (not ast.staging.empty())
            {
                static constexpr auto staged = [] consteval {
                    std::vector<tag_number_t> vec;
                    for (const auto& [k, v] : ast.staging)
                        if (v == end_of_input_tag)
                            vec.emplace_back(k);
                    std::ranges::sort(vec);
                    auto [it, _]{ std::ranges::unique(vec) };
                    vec.erase(it, vec.end());
                    return std::define_static_array(vec);
                }();

                template for (constexpr auto tagnum : staged)
                    res.reg_[tagnum] = si.reg[get_staging_index(tagnum)];

                if constexpr (not std::contiguous_iterator<I>)
                    template for (constexpr auto tagnum : staged)
                        res.enabled_[tagnum] = true;
            }
        }

        template<bool Fwd, std::size_t... Ints>
        struct state {};

        template<bool Fwd>
        struct state<Fwd>
        {
            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator()(result<I>& /* res */, staging_info<I>& /* si */, const I /* first */, const S /* last */, I& /* it */)
            {
                return true;
            }
        };

        template<bool Fwd>
        struct state<Fwd, require_full_match>
        {
            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator()(result<I>& /* res */, staging_info<I>& /* si */, const I /* first */, const S last, I& it)
            {
                return (it == last);
            }
        };

        template<bool Fwd>
        struct state<Fwd, require_non_empty_match>
        {
            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator()(result<I>& res , staging_info<I>& /* si */, const I /* first */, const S /* last */, I& it)
            {
                return (it != res.match_start_);
            }
        };


        template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::char_str>)
        struct state<Fwd, Expr, Cont...>
        {
            static constexpr auto str{ ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::char_str>] :] };

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            {
                if constexpr (Fwd)
                {
                    if constexpr (std::random_access_iterator<I> and std::sized_sentinel_for<S, I>)
                    {
                        if (static_cast<std::size_t>(std::ranges::distance(it, last)) < str.data.size())
                            return false;

                        int mismatch{ false };  /* encourage vectorisation */
                        for (std::size_t i{ 0 }, i_end{ str.data.size() }; i < i_end; ++i)
                            mismatch |= (it[i] != str.data[i]);

                        if (mismatch)
                            return false;

                        std::ranges::advance(it, static_cast<std::ptrdiff_t>(str.data.size()));
                    }
                    else
                    {
                        template for (constexpr char_type c : str.data)
                        {
                            if (it == last or *it != c)
                                return false;

                            ++it;
                        }
                    }
                }
                else
                {
                    if constexpr (std::random_access_iterator<I> and std::sized_sentinel_for<S, I>)
                    {
                        if (static_cast<std::size_t>(std::ranges::distance(first, it)) < str.data.size())
                            return false;

                        std::ranges::advance(it, -static_cast<std::ptrdiff_t>(str.data.size()));

                        int mismatch{ false };  /* encourage vectorisation */
                        for (std::size_t i{ 0 }, i_end{ str.data.size() }; i < i_end; ++i)
                            mismatch |= (it[i] != str.data[i]);

                        if (mismatch)
                            return false;
                    }
                    else
                    {
                        template for (constexpr char_type c : str.data | std::views::reverse)
                        {
                            if (it == first)
                                return false;

                            --it;

                            if (*it != c)
                                return false;
                        }
                    }
                }

                [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
            }
        };

        template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::char_class>)
        struct state<Fwd, Expr, Cont...>
        {
            static constexpr auto cla{ ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::char_class>] :] };

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (std::same_as<char_type, char> or std::same_as<char_type, char32_t>)
            {
                if constexpr (Fwd)
                {
                    if (it == last)
                        return false;
                }
                else
                {
                    if (it == first)
                        return false;

                    --it;
                }

                if (not cs_possible<cla.data.cs>(*it))
                    return false;

                if constexpr (Fwd)
                    ++it;

                [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator()(result<I>& /* res */, staging_info<I>& /* si */, const I /* first */, const S /* last */, I& /* it */)
            requires (not std::same_as<char_type, char> and not std::same_as<char_type, char32_t>)
            {
                // using uct = decltype(cla)::underlying_char_type;

                // if constexpr (Fwd)
                // {
                //     // TODO:
                //     // - extract char32_t from input
                //     // - compare against cla.data.cs
                //     // - advance it as necessary
                // }
                // else
                // {
                //     // TODO:
                //     // - extract char32_t from input (backwards)
                //     // - compare against cla.data.cs
                //     // - advance it as necessary
                // }

                static_assert("Unimplemented");
                return false;
            }
        };

        template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::alt>)
        struct state<Fwd, Expr, Cont...>
        {
            static constexpr auto alt{ ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::alt>] :] };

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator() (result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (not alt.idxs.empty())
            {
                template for (constexpr std::size_t Next : alt.idxs | std::views::take(alt.idxs.size() - 1))
                {
                    if (auto saved_it{ it }; state<Fwd, Next, Cont...>::operator()(res, si, first, last, it))
                        return true;
                    else
                        it = saved_it;
                }

                [[clang::musttail]] return state<Fwd, alt.idxs.back(), Cont...>::operator()(res, si, first, last, it);
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator() (result<I>& /* res */, const I /* first */, const S /* last */, I& /* it */)
            requires (alt.idxs.empty())
            {
                return false;
            }
        };

        template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::concat>)
        struct state<Fwd, Expr, Cont...>
        {
            static constexpr auto cat{ ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::concat>] :] };

            template<typename T> struct helper {};
            template<std::size_t... Next> struct helper<std::index_sequence<Next...>> { using type = state<Fwd, Next..., Cont...>; };

            // END TEMPORARY: TODO: REMOVE

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            {
                using next_sequence_t = [: [] consteval {
                    std::vector<std::meta::info> vec{};
                    vec.reserve(cat.idxs.size());
                    if (Fwd)
                    {
                        for (const auto idx: cat.idxs)
                            vec.push_back(std::meta::reflect_constant(idx));
                    }
                    else
                    {
                        for (const auto idx: cat.idxs | std::views::reverse)
                            vec.push_back(std::meta::reflect_constant(idx));
                    }
                    return substitute(^^std::index_sequence, vec);
                }() :];

                // constexpr auto [...Next]{ next_sequence_t{} };

                // [[clang::musttail]] return state<Fwd, Next..., Cont...>::operator()(res, si, first, last, it);

                using next_state = helper<next_sequence_t>::type;
                [[clang::musttail]] return next_state::operator()(res, si, first, last, it);
            }
        };

        template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::repeat>)
        struct state<Fwd, Expr, Cont...>
        {
            static constexpr auto rep{ ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::repeat>] :] };

            // TEMPORARY: TODO: REMOVE
            template<typename T> struct helper {};
            template<std::size_t... Next> struct helper<std::index_sequence<Next...>> { using type = state<Fwd, Next..., Cont...>; };
            // END TEMPORARY: TODO: REMOVE

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (rep.min == rep.max)
            {
                using next_sequence_t = [: [] consteval {
                    std::vector vec(rep.min, std::meta::reflect_constant(rep.idx));
                    return substitute(^^std::index_sequence, vec);
                }() :];

                // constexpr auto [...Next]{ next_sequence_t{} };

                // [[clang::musttail]] return state<Fwd, Next..., Cont...>::operator()(res, si, first, last, it);

                /* workaround for the above */

                using next_state = helper<next_sequence_t>::type;

                [[clang::musttail]] return next_state::operator()(res, si, first, last, it);
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (rep.min == 0 and rep.max < 0 and rep.mode == repeater_mode::greedy)
            {
                if (auto saved_it{ it }; state<Fwd, rep.idx, Expr, Cont...>::operator()(res, si, first, last, it))
                    return true;
                else
                    it = saved_it;

                [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (rep.min == 0 and rep.max < 0 and rep.mode == repeater_mode::lazy)
            {
                if (auto saved_it{ it }; state<Fwd, Cont...>::operator()(res, si, first, last, it))
                    return true;
                else
                    it = saved_it;

                [[clang::musttail]] return state<Fwd, rep.idx, Expr, Cont...>::operator()(res, si, first, last, it);
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (rep.min == 0 and rep.max < 0 and rep.mode == repeater_mode::possessive)
            {
                while (true)
                {
                    if (auto saved_it{ it }; not state<Fwd, rep.idx>::operator()(res, si, first, last, it))
                    {
                        it = saved_it;
                        break;
                    }
                }

                [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (rep.min == 0 and rep.max == 1 and rep.mode == repeater_mode::greedy)
            {
                if (auto saved_it{ it }; state<Fwd, rep.idx, Cont...>::operator()(res, si, first, last, it))
                    return true;
                else
                    it = saved_it;

                [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (rep.min == 0 and rep.max == 1 and rep.mode == repeater_mode::lazy)
            {
                if (auto saved_it{ it }; state<Fwd, Cont...>::operator()(res, si, first, last, it))
                    return true;
                else
                    it = saved_it;

                [[clang::musttail]] return state<Fwd, rep.idx, Cont...>::operator()(res, si, first, last, it);
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (rep.min == 0 and rep.max == 1 and rep.mode == repeater_mode::possessive)
            {
                if (auto saved_it{ it }; not state<Fwd, rep.idx>::operator()(res, si, first, last, it))
                    it = saved_it;

                [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
            }
        };

        template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::assertion>)
        struct state<Fwd, Expr, Cont...>
        {
            static constexpr auto asr{ ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::assertion>] :] };

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool check_impl(const I first, const S  /* last */, const I it)
            requires (asr.type == assert_type::text_start)
            {
                return (it == first);
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool check_impl(const I  /* first */, const S last, const I it)
            requires (asr.type == assert_type::text_end)
            {
                return (it == last);
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool check_impl(const I first, const S  /* last */, const I it)
            requires (asr.type == assert_type::line_start)
            {
                return (it == first or *std::ranges::prev(it) == '\n');
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool check_impl(const I  /* first */, const S last, const I it)
            requires (asr.type == assert_type::line_end)
            {
                return (it == last or *it == '\n');
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool check_impl(const I first, const S last, const I it)
            requires (asr.type == assert_type::ascii_word_boundary)
            {
                static constexpr auto is_ascii_word_character = [](std::iter_value_t<I> c) {
                    return ('0' <= c and c <= '9') or ('A' <= c and c <= 'Z') or ('a' <= c and c <= 'z') or (c == '_');
                };

                if (it == first or not is_ascii_word_character(*std::ranges::prev(it)))
                    return (it != last and is_ascii_word_character(*it));
                else /* if (it != first and is_ascii_word_character(*std::ranges::prev(it))) */
                    return (it == last or not is_ascii_word_character(*it));
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool check_impl(const I first, const S last, const I it)
            requires (asr.type == assert_type::not_ascii_word_boundary)
            {
                static constexpr auto is_ascii_word_character = [](std::iter_value_t<I> c) {
                    return ('0' <= c and c <= '9') or ('A' <= c and c <= 'Z') or ('a' <= c and c <= 'z') or (c == '_');
                };

                if (it == first or not is_ascii_word_character(*std::ranges::prev(it)))
                    return (it == last or not is_ascii_word_character(*it));
                else /* if (it != first and is_ascii_word_character(*std::ranges::prev(it))) */
                    return (it != last and is_ascii_word_character(*it));
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            {
                // Warning: this is likely to be buggy
                if (check_impl(first, last, it))
                    [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);

                return false;
            }
        };

        template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::backref>)
        struct state<Fwd, Expr, Cont...>
        {
            static constexpr auto bref{ ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::backref>] :] };

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            {
                static_assert(bref.number < result<I>::submatch_count, "Backreference to non-existent submatch");

                const auto submatch{ force_get<bref.number>(res) };

                if (not submatch.matched())
                    return false;

                if constexpr (Fwd)
                {
                    if constexpr (std::random_access_iterator<I> and std::sized_sentinel_for<S, I>)
                    {
                        if (static_cast<std::size_t>(std::ranges::distance(it, last)) < submatch.size())
                            return false;

                        auto submatch_it{ submatch.begin() };
                        int mismatch{ false };  /* encourage vectorisation */
                        for (std::size_t i{ 0 }, i_end{ submatch.size() }; i < i_end; ++i)
                            mismatch |= (it[i] != submatch_it[i]);

                        if (mismatch)
                            return false;

                        std::ranges::advance(it, static_cast<std::ptrdiff_t>(submatch.size()));
                    }
                    else
                    {
                        for (char_type c : submatch)
                        {
                            if (it == last or *it != c)
                                return false;

                            ++it;
                        }
                    }
                }
                else
                {
                    if constexpr (std::random_access_iterator<I> and std::sized_sentinel_for<S, I>)
                    {
                        if (static_cast<std::size_t>(std::ranges::distance(first, it)) < submatch.size())
                            return false;

                        std::ranges::advance(it, -static_cast<std::ptrdiff_t>(submatch.size()));

                        auto submatch_it{ submatch.begin() };
                        int mismatch{ false };  /* encourage vectorisation */
                        for (std::size_t i{ 0 }, i_end{ submatch.size() }; i < i_end; ++i)
                            mismatch |= (it[i] != submatch_it[i]);

                        if (mismatch)
                            return false;
                    }
                    else
                    {
                        for (char_type c : submatch | std::views::reverse)
                        {
                            if (it == first)
                                return false;

                            --it;

                            if (*it != c)
                                return false;
                        }
                    }
                }

                [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
            }
        };

        template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::tag>)
        struct state<Fwd, Expr, Cont...>
        {
            static constexpr auto tag{ ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::tag>] :] };

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (ast.staging.contains(tag.number))
            {
                si.reg[get_staging_index(tag.number)] = it;

                [[clang::musttail]] return state<Fwd, Cont...>::operator()(res, si, first, last, it);
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
            requires (not ast.staging.contains(tag.number))
            {
                static_assert(tag.number < res.reg_.size());

                static constexpr auto staged = [] consteval {
                    std::vector<tag_number_t> vec;
                    for (const auto& [k, v] : ast.staging)
                        if (v == tag.number)
                            vec.emplace_back(k);
                    std::ranges::sort(vec);
                    auto [it, _]{ std::ranges::unique(vec) };
                    vec.erase(it, vec.end());
                    return std::define_static_array(vec);
                }();


                using saved_t = std::conditional_t<std::contiguous_iterator<I>, I, bool>;
                std::array<saved_t, 1 + staged.size()> saved;

                if constexpr (std::contiguous_iterator<I>)
                {
                    saved.back() = res.reg_[tag.number];

                    if constexpr (not ast.staging.empty())
                        template for (constexpr auto i : std::views::iota(0uz, staged.size()))
                            saved[i] = res.reg_[staged[i]];
                }
                else
                {
                    saved.back() = res.enabled_[tag.number];

                    if constexpr (not ast.staging.empty())
                        template for (constexpr auto i : std::views::iota(0uz, staged.size()))
                            saved[i] = res.enabled_[staged[i]];
                }

                res.reg_[tag.number] = it;

                if constexpr (not ast.staging.empty())
                    template for (constexpr auto tagnum : staged)
                        res.reg_[tagnum] = si.reg[get_staging_index(tagnum)];

                if constexpr (not std::contiguous_iterator<I>)
                {
                    res.enabled_[tag.number] = true;

                    if constexpr (not ast.staging.empty())
                        template for (constexpr auto tagnum : staged)
                            res.enabled_[tagnum] = true;
                }

                if (auto saved_it{ it }; state<Fwd, Cont...>::operator()(res, si, first, last, it))
                    return true;
                else
                    it = saved_it;

                if constexpr (std::contiguous_iterator<I>)
                {
                    res.reg_[tag.number] = saved.back();

                    if constexpr (not ast.staging.empty())
                        template for (constexpr auto i : std::views::iota(0uz, staged.size()))
                            res.reg_[staged[i]] = saved[i];
                }
                else
                {
                    res.enabled_[tag.number] = saved.back();

                    if constexpr (not ast.staging.empty())
                        template for (constexpr auto i : std::views::iota(0uz, staged.size()))
                            res.enabled_[staged[i]] = saved[i];
                }

                return false;
            }
        };

        // template<bool Fwd, std::size_t Expr, std::size_t... Cont>
        // requires (Expr < ast.exprs.size() and ast.exprs[Expr].index == ast_index<typename ast_t::special>)
        // struct state<Fwd, Expr, Cont...>
        // {
        //     static constexpr auto spg{ ast.exprs[Expr].value.[: info_t::type::nsdms[ast_index<typename ast_t::special>] :] };

        //     template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        //     [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
        //     requires (spg.mode == special_group_mode::atomic_group)
        //     {
        //         if (not state<Fwd, spg.idx>(res, first, last, it))
        //             return false;

        //         [[clang::musttail]] return state<Fwd, Cont...>(res, first, last, it);
        //     }

        //     template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        //     [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
        //     requires (spg.mode == special_group_mode::positive_lookahead or spg.mode == special_group_mode::negative_lookahead)
        //     {
        //         static constexpr bool required_result{ spg.mode == special_group_mode::negative_lookahead };

        //         if (auto it_copy{ it }; state<true, spg.idx>(res, first, last, it_copy) == required_result)
        //             return false;

        //         [[clang::musttail]] return state<Fwd, Cont...>(res, first, last, it);
        //     }

        //     template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        //     [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
        //     requires (spg.mode == special_group_mode::positive_lookbehind or spg.mode == special_group_mode::negative_lookbehind)
        //     {
        //         static constexpr bool required_result{ spg.mode == special_group_mode::negative_lookbehind };

        //         if (auto it_copy{ it }; state<false, spg.idx>(res, first, last, it_copy) == required_result)
        //             return false;

        //         [[clang::musttail]] return state<Fwd, Cont...>(res, first, last, it);
        //     }

        //     template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        //     [[gnu::always_inline]] static constexpr bool operator()(result<I>& /* res */, staging_info<I>& /* si */, const I /* first */, const S /* last */, I& /* it */)
        //     requires (spg.mode == special_group_mode::backtrack_accept or spg.mode == special_group_mode::backtrack_fail)
        //     {
        //         static constexpr bool return_value{ spg.mode == special_group_mode::backtrack_accept };

        //         return return_value;
        //     }

        //     template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        //     [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
        //     requires (spg.mode == special_group_mode::backtrack_mark)
        //     {
        //         static_assert("Unimplemented");
        //     }

        //     template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        //     [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
        //     requires (spg.mode == special_group_mode::backtrack_commit)
        //     {
        //         static_assert("Unimplemented");
        //     }

        //     template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        //     [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
        //     requires (spg.mode == special_group_mode::backtrack_prune)
        //     {
        //         static_assert("Unimplemented");
        //     }

        //     template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        //     [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
        //     requires (spg.mode == special_group_mode::backtrack_skip)
        //     {
        //         static_assert("Unimplemented");
        //     }

        //     template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        //     [[gnu::always_inline]] static constexpr bool operator()(result<I>& res, staging_info<I>& si, const I first, const S last, I& it)
        //     requires (spg.mode == special_group_mode::backtrack_then)
        //     {
        //         static_assert("Unimplemented");
        //     }
        // };

    public:
        template<bool Full>
        struct match
        {
            static constexpr bool never_empty{ not ast.empty_match_possible };

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            static constexpr auto operator()(const I first, const S last)
            requires (Full)
            {
                result<I> res{ first };
                staging_info<I> si{};
                if (I it{ first }; state<true, ast.root_idx, require_full_match>::operator()(res, si, first, last, it))
                {
                    apply_final_staging_info(res, si);
                    res.match_end_ = it;

                    if constexpr (not std::contiguous_iterator<I>)
                        res.match_success_ = true;
                }
                return res;
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            static constexpr auto operator()(const I first, const S last)
            requires (not Full)
            {
                result<I> res{ first };
                staging_info<I> si{};
                if (I it{ first }; state<true, ast.root_idx>::operator()(res, si, first, last, it))
                {
                    apply_final_staging_info(res, si);
                    res.match_end_ = it;

                    if constexpr (not std::contiguous_iterator<I>)
                        res.match_success_ = true;
                }
                return res;
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            static constexpr auto operator()(const I first, const S last, match_non_empty_t) = delete;
        };

        template<bool Single>
        struct search
        {
        private:
            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr auto outer_state(const I first, const S last, I continue_from)
            {
                result<I> res{};
                staging_info<I> si{};
                while (true)
                {
                    if (I it{ continue_from }; state<true, ast.root_idx>::operator()(res, si, first, last, it))
                    {
                        apply_final_staging_info(res, si);
                        res.match_end_ = it;
                        res.match_start_ = continue_from;

                        if constexpr (not std::contiguous_iterator<I>)
                            res.match_success_ = true;
                        return res;
                    }

                    res.clear_match();

                    if (continue_from == last)
                        return res;

                    ++continue_from;
                }
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            [[gnu::always_inline]] static constexpr auto non_empty_outer_state(const I first, const S last, I continue_from)
            {
                result<I> res{ continue_from };
                staging_info<I> si{};

                if (I it{ continue_from }; state<true, ast.root_idx, require_non_empty_match>::operator()(res, si, first, last, it))
                {
                    apply_final_staging_info(res, si);
                    res.match_end_ = it;

                    if constexpr (not std::contiguous_iterator<I>)
                        res.match_success_ = true;

                    return res;
                }

                if (continue_from == last)
                    return res;

                ++continue_from;

                [[clang::musttail]] return outer_state(first, last, continue_from);
            }

        public:
            static constexpr bool never_empty{ not ast.empty_match_possible };

            template<std::bidirectional_iterator I>
            using result = p1306_naive_impl::result<I>;

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            static constexpr auto operator()(const I first, const S last)
            {
                return outer_state(first, last, first);
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            static constexpr auto operator()(const I first, const S last, I continue_from)
            requires (not Single)
            {
                return outer_state(first, last, continue_from);
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            static constexpr auto operator()(const I first, const S last, match_non_empty_t)
            {
                if constexpr (ast.empty_match_possible)
                    return non_empty_outer_state(first, last, first);
                else
                    return outer_state(first, last, first);
            }

            template<std::bidirectional_iterator I, std::sentinel_for<I> S>
            static constexpr auto operator()(const I first, const S last, I continue_from, match_non_empty_t)
            requires (not Single)
            {
                if constexpr (ast.empty_match_possible)
                    return non_empty_outer_state(first, last, continue_from);
                else
                    return outer_state(first, last, continue_from);
            }
        };

        static consteval auto get_matcher(fsm_flags f)
        {
            if (f.longest_match)
            {
                if (not f.is_search and not f.enable_fallback and not f.is_iterator)
                    return ^^match<true>;
            }
            else if (f.enable_fallback)
            {
                if (not f.is_search)
                {
                    if (not f.is_iterator)
                        return ^^match<false>;
                }
                else if (not f.is_iterator)
                {
                    return ^^search<true>;
                }
                else
                {
                    return ^^search<false>;
                }
            }

            /* invalid flag combination */
            throw regex_error("p1306_naive_adaptor: invalid fsm flag combination");
        }
    };



    template<string_literal Pattern, fsm_flags Flags>
    struct p1306_naive_adaptor : public [: p1306_naive_impl<Pattern>::get_matcher(Flags) :] {};

    template<string_literal Pattern, fsm_flags Flags>
    requires (Flags.return_bool)
    struct p1306_naive_adaptor<Pattern, Flags>
    {
        using underlying = p1306_naive_adaptor<Pattern, [](fsm_flags f) consteval { f.return_bool = false; return f; }(Flags)>;

        template<typename... Ts>
        static constexpr bool operator()(Ts&&... args)
        {
            return underlying::operator()(std::forward<Ts>(args)...);
        }
    };


}