// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <ranges>
#include <string_view>
#include <vector>

#include <srx/api/regex_error.hpp>
#include <srx/ast/tree.hpp>

#include "cdarray.hpp"


namespace srx {
namespace testing {

template<typename CharT>
class tree_matcher : public detail::expr_tree<CharT>
{
public:
    using detail::expr_tree<CharT>::expr_tree;
    using tag_result = std::vector<std::size_t>;

    template<std::random_access_iterator I>
        requires std::convertible_to<std::iter_value_t<I>, CharT>
    [[nodiscard]] constexpr std::optional<tag_result> match(I first, I last) const;

    template<std::ranges::random_access_range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, CharT>
    [[nodiscard]] constexpr std::optional<tag_result> match(R&& r) const
    {
        return match(std::ranges::begin(r), std::ranges::end(r));
    }

    [[nodiscard]] constexpr std::optional<tag_result> match(const CharT* cstr) const
    {
        return match(std::basic_string_view{ cstr });
    }
};

template<typename CharT>
tree_matcher(std::basic_string_view<CharT>) -> tree_matcher<CharT>;

template<typename CharT>
tree_matcher(std::basic_string_view<CharT>, detail::parser_flags) -> tree_matcher<CharT>;

template<typename CharT>
tree_matcher(const CharT*) -> tree_matcher<CharT>;

template<typename CharT>
tree_matcher(const CharT*, detail::parser_flags) -> tree_matcher<CharT>;


/* helper for tree matcher */

class cont_val
{
public:
    using rep_t = std::int_least16_t;

    constexpr explicit(false) cont_val(std::size_t i) : pos_{ i } {}
    constexpr explicit cont_val(std::size_t i, rep_t r) : pos_{ i }, reps_{ r } {}
    constexpr explicit cont_val(std::size_t i, rep_t r, rep_t amt) : pos_{ i }, reps_{ std::saturating_add(r, amt) } {}

    [[nodiscard]] constexpr std::size_t pos() const noexcept { return pos_; }
    [[nodiscard]] constexpr rep_t reps() const noexcept { return reps_; }

private:
    std::uint_least64_t pos_ : 48;
    rep_t reps_              : 16 { 0 };
};

template<typename I>
struct matcher_state
{
    using tags_t         = cdarray<std::optional<I>>;
    using continuation_t = std::vector<cont_val>;

    I              it;
    continuation_t cont;
    tags_t         tags;

    constexpr matcher_state(I first, std::size_t tag_count, std::size_t start_state)
        : it{ first }, cont{ start_state }, tags(tag_count) {}

    constexpr matcher_state(I first, std::size_t tag_count, std::size_t start_state, std::size_t end_state)
        : it{ first }, cont{ end_state, start_state }, tags(tag_count) {}

    constexpr matcher_state(I first, const tags_t& tags, std::size_t start_state)
        : it{ first }, cont{ start_state }, tags(tags) {}
};


/* tree matcher */

template<typename CharT>
template<std::random_access_iterator I>
    requires std::convertible_to<std::iter_value_t<I>, CharT>
[[nodiscard]] constexpr auto tree_matcher<CharT>::match(const I first, const I last) const -> std::optional<tag_result>
{
    using state_t = matcher_state<I>;

    const auto& ci = this->get_capture_info();
    const std::size_t capture_count{ ci.capture_count() };
    const std::size_t pos_end{ this->get_all_exprs().size() };

    auto visitor = [&, this](this const auto& rec, state_t& s) -> bool {
        while (not s.cont.empty())
        {
            const auto raw_data = s.cont.back();
            s.cont.pop_back();

            const std::size_t pos{ raw_data.pos() };

            static constexpr auto is_ascii_word_character = [](CharT c){
                return ('0' <= c and c <= '9') or ('A' <= c and c <= 'Z') or ('a' <= c and c <= 'z') or (c == '_');
            };

            if (pos >= pos_end)
                return s.it == last;

            const bool result = this->get_expr(pos).visit(detail::overloads{
                [&](const typename tree_matcher::assertion& asr) -> bool {
                    using detail::assert_type;
                    switch (asr.type)
                    {
                    case assert_type::text_start:
                        return s.it == first;
                    case assert_type::text_end:
                        return s.it == last;
                    case assert_type::line_start:
                        return s.it == first or *std::ranges::prev(s.it) == '\n';
                    case assert_type::line_end:
                        return s.it == last or *s.it == '\n';
                    case assert_type::ascii_word_boundary:
                        if (s.it == first or not is_ascii_word_character(*std::ranges::prev(s.it)))
                            return s.it != last and is_ascii_word_character(*s.it);
                        else /* if (s.it != first and is_ascii_word_character(*std::ranges::prev(s.it))) */
                            return s.it == last or not is_ascii_word_character(*s.it);
                    case assert_type::not_ascii_word_boundary:
                        if (s.it == first or not is_ascii_word_character(*std::ranges::prev(s.it)))
                            return s.it == last or not is_ascii_word_character(*s.it);
                        else /* if (s.it != first and is_ascii_word_character(*std::ranges::prev(s.it))) */
                            return s.it != last and is_ascii_word_character(*s.it);
                    default:
                        throw tree_error("Encountered unimplemented assertion while matching");
                    }
                    return false;
                },
                [&](const typename tree_matcher::concat& cat) -> bool {
                    s.cont.append_range(cat.idxs | std::views::reverse);
                    return true;
                },
                [&](const typename tree_matcher::alt& alt) -> bool {
                    if (alt.idxs.empty())
                        return false;

                    for (std::size_t i{ 0 }, i_end{ alt.idxs.size() }; i + 1 < i_end; ++i)
                    {
                        state_t s_copy{ s };
                        s_copy.cont.push_back(alt.idxs.at(i));

                        if (rec(s_copy))
                        {
                            s = std::move(s_copy);
                            return true;
                        }
                    }

                    s.cont.push_back(alt.idxs.back());
                    return true;
                },
                [&](const typename tree_matcher::tag& tag) -> bool {
                    s.tags.at(tag.number) = s.it;
                    return true;
                },
                [&](const typename tree_matcher::backref& bref) -> bool {
                    if (bref.number >= capture_count)
                        throw pattern_error("Backreference to non-existent submatch");

                    auto f = [&](const detail::capture_info::tag_pair_t& p) -> bool {
                        return not ((p.first.tag_number >= 0 and not s.tags.at(p.first.tag_number).has_value())
                                    or (p.second.tag_number >= 0 and not s.tags.at(p.second.tag_number).has_value()));
                    };

                    auto t = [&](const detail::capture_info::tag_pair_t& p) -> std::pair<I, I> {
                        return {
                            std::ranges::next((p.first.tag_number >= 0)
                                              ? *s.tags.at(p.first.tag_number)
                                              : ((p.first.tag_number == detail::start_of_input_tag) ? first : last), p.first.offset),
                            std::ranges::next((p.second.tag_number >= 0)
                                              ? *s.tags.at(p.second.tag_number)
                                              : ((p.second.tag_number == detail::start_of_input_tag) ? first : last), p.second.offset)
                        };
                    };

                    const auto [beg, end] = ci.lookup(bref.number);

                    auto rng = std::ranges::subrange(beg, end)
                               | std::views::filter(f)
                               | std::views::transform(t)
                               | std::ranges::to<std::vector>();

                    if (std::ranges::size(rng) == 0)
                        return false; /* capture doesn't exist */

                    auto [bit, blast] = std::ranges::max(rng, std::ranges::less{}, &std::pair<I, I>::first);

                    for (; bit != blast; ++s.it, ++bit)
                        if (s.it == last or *s.it != *bit)
                            return false;

                    return true;
                },
                [&](const typename tree_matcher::repeat& rep) -> bool {
                    using detail::repeater_mode;

                    const auto& [min, max] = rep.reps;

                    if (min == max)
                    {
                        /* fixed number of repetitions - don't insert repeated */
                        s.cont.insert(s.cont.end(), /* count = */ min, /* value = */ rep.idx);
                        return true;
                    }

                    const auto rep_count = raw_data.reps();

                    if (std::cmp_less(rep_count, min))
                    {
                        /* perform fixed number of repetitions and then revisit  */
                        s.cont.emplace_back(pos, min);
                        s.cont.insert(s.cont.end(), /* count = */ min - rep_count, /* value = */ rep.idx);
                        return true;
                    }

                    if (rep.mode == repeater_mode::greedy)
                    {
                        /* try to match repeated pattern first */
                        state_t s_copy{ s };

                        if (max < min or rep_count < max - 1)
                            s_copy.cont.emplace_back(pos, rep_count, 1);
                        s_copy.cont.push_back(rep.idx);

                        if (rec(s_copy))
                        {
                            s = std::move(s_copy);
                            return true;
                        }
                    }
                    else if (rep.mode == repeater_mode::lazy)
                    {
                        /* try to match pattern after rep first */
                        state_t s_copy{ s };

                        if (rec(s_copy))
                        {
                            s = std::move(s_copy);
                            return true;
                        }

                        if (max < min or rep_count < max - 1)
                            s.cont.emplace_back(pos, rep_count, 1);
                        s.cont.push_back(rep.idx);
                    }
                    else /* if (rep.mode == repeater_mode::possessive) */
                    {
                        /* check rep in a copy of state */
                        if (max < min)
                        {
                            while (true)
                            {
                                state_t s_tmp{ s.it, s.tags, rep.idx };
                                if (not rec(s_tmp))
                                    break;
                                s.it = std::move(s_tmp.it);
                                s.tags = std::move(s_tmp.tags);
                            }
                        }
                        else
                        {
                            for (auto count = rep_count; count < max; ++count)
                            {
                                state_t s_tmp{ s.it, s.tags, rep.idx };
                                if (not rec(s_tmp))
                                    break;
                                s.it = std::move(s_tmp.it);
                                s.tags = std::move(s_tmp.tags);
                            }
                        }
                    }

                    return true;
                },
                [&](const typename tree_matcher::char_str& lit) -> bool {
                    for (const auto c : lit.data)
                    {
                        if (s.it == last or *s.it != c)
                            return false; /* unsuccessful match */
                        ++s.it;
                    }

                    return true;
                },
                [&](const typename tree_matcher::char_class& cla) -> bool {
                    if (s.it == last)
                        return false;

                    using uct = tree_matcher::char_class::underlying_char_type;

                    uct input{};

                    if constexpr (detail::char_is_utf8<CharT>)
                    {
                        // TODO: Implement
                        throw tree_error("UTF-8 char class matching unimplemented");
                    }
                    else if constexpr (detail::char_is_utf16<CharT>)
                    {
                        // TODO: Implement
                        throw tree_error("UTF-16 char class matching unimplemented");
                    }
                    else
                    {
                        input = *s.it++;
                    }

                    return cla.data.get().contains(input);
                }
            });

            if (not result)
                return false;
        }

        return s.cont.empty();
    };

    state_t s(first, this->tag_count(), this->root_idx(), pos_end);
    auto rv = visitor(s);

    if (not rv)
        return {};

    /* construct submatch results from tags */

    tag_result res{};

    auto f = [&](const detail::capture_info::tag_pair_t& p) -> bool {
        return not ((p.first.tag_number >= 0 and not s.tags.at(p.first.tag_number).has_value())
                    or (p.second.tag_number >= 0 and not s.tags.at(p.second.tag_number).has_value()));
    };

    auto t = [&](const detail::capture_info::tag_pair_t& p) -> std::pair<I, I> {
        return {
            std::ranges::next((p.first.tag_number >= 0)
                              ? *s.tags.at(p.first.tag_number)
                              : ((p.first.tag_number == detail::start_of_input_tag) ? first : last), p.first.offset),
            std::ranges::next((p.second.tag_number >= 0)
                              ? *s.tags.at(p.second.tag_number)
                              : ((p.second.tag_number == detail::start_of_input_tag) ? first : last), p.second.offset)
        };
    };

    for (std::size_t i{ 0 }; i < capture_count; ++i)
    {
        const auto [beg, end] = ci.lookup(i);

        auto rng = std::ranges::subrange(beg, end) | std::views::filter(f)
                    | std::views::transform(t)
                    | std::ranges::to<std::vector>();

        if (std::ranges::size(rng) == 0)
        {
            res.insert(res.end(), { detail::no_tag, detail::no_tag });
            continue;
        }

        auto max_elem = std::ranges::max_element(rng, std::ranges::less{}, &std::pair<I, I>::first);
        auto [bit, blast] = *max_elem;

        res.push_back(std::ranges::distance(first, bit));
        res.push_back(std::ranges::distance(first, blast));
    }

    return res;
}

} // namespace testing
} // namespace srx
