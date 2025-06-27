#pragma once

#include <cstdint>
#include <ranges>

#include <rx/cdarray.hpp>
#include <rx/error.hpp>
#include <rx/util.hpp>
#include <rx/tree.hpp>

namespace rx::testing
{
    template<typename CharT>
    class tree_matcher : public detail::expr_tree<CharT>
    {
    public:
        using detail::expr_tree<CharT>::expr_tree;
        using tag_result = std::vector<std::size_t>;

        template<std::input_iterator I, std::sentinel_for<I> S>
        requires (std::convertible_to<std::iter_value_t<I>, CharT>)
        [[nodiscard]] constexpr std::optional<tag_result> submatches(I first, S last) const;

        template<std::ranges::input_range R>
        requires (std::convertible_to<std::ranges::range_value_t<R>, CharT>)
        [[nodiscard]] constexpr std::optional<tag_result> submatches(R&& r) const
        {
            return submatches(std::ranges::begin(r), std::ranges::end(r));
        }

        template<std::input_iterator I, std::sentinel_for<I> S>
        requires (std::convertible_to<std::iter_value_t<I>, CharT>)
        [[nodiscard]] constexpr bool match(I first, S last) const
        {
            return submatches(first, last).has_value();
        }

        template<std::ranges::input_range R>
        requires (std::convertible_to<std::ranges::range_value_t<R>, CharT>)
        [[nodiscard]] constexpr bool match(R&& r) const
        {
            return match(std::ranges::begin(r), std::ranges::end(r));
        }
    };

    /* tree matcher */

    template<typename CharT>
    template<std::input_iterator I, std::sentinel_for<I> S>
    requires (std::convertible_to<std::iter_value_t<I>, CharT>)
    [[nodiscard]] constexpr auto tree_matcher<CharT>::submatches(const I first, const S last) const -> std::optional<tag_result>
    {
        using expr_tree_t = tree_matcher<CharT>;

        class cont_val
        {
        public:
            using rep_t = std::int_least16_t;

            constexpr cont_val(std::size_t i) : pos_{ i } {}
            constexpr explicit cont_val(std::size_t i, rep_t r) : pos_{ i }, reps_{ r } {}
            constexpr explicit cont_val(std::size_t i, rep_t r, rep_t amt) : pos_{ i }, reps_{ std::add_sat(r, amt) } {}

            [[nodiscard]] constexpr std::size_t pos() const noexcept { return static_cast<std::size_t>(pos_); }
            [[nodiscard]] constexpr rep_t reps() const noexcept { return reps_; }

        private:
            std::uint_least64_t pos_: 48;
            rep_t               reps_: 16 { 0 };
        };

        using tags_t = detail::cdarray<std::optional<std::pair<I, I>>>;
        using tag_begs_t = detail::cdarray<std::optional<I>>;
        using continuation_t = std::vector<cont_val>;

        struct state
        {
            I                   it;
            continuation_t      cont;
            tags_t              tags;
            tag_begs_t          tag_begs;

            constexpr state(I first, std::size_t tag_count, std::size_t start_state) :
                it{ first },
                cont{ start_state },
                tags(tag_count),
                tag_begs(tag_count)
            {
            }
        };

        // TODO: maybe change continuation_t to also contain tags, and make capture produce a re-visit to itself?

        enum class rc : uint_least8_t
        {
            match_failure,
            match_success,
            match_continue,
        };

        auto visitor = [&, this](this const auto& rec, state& s) -> bool
        {
            while (not s.cont.empty())
            {
                const auto raw_data{ s.cont.back() };
                s.cont.pop_back();

                const std::size_t pos{ raw_data.pos() };

                const rc result = this->get_expr(pos).visit(detail::overloads{
                    [&](const expr_tree_t::assertion& asr) -> rc
                    {
                        using detail::assert_type;
                        switch (asr.type)
                        {
                        case assert_type::text_start:
                            if (s.it == first)
                                return rc::match_continue;
                        case assert_type::text_end:
                            if (s.it == last)
                                return rc::match_continue;
                        case assert_type::line_start:
                            if (s.it == first or *std::ranges::prev(s.it) == '\n')
                                return rc::match_continue;
                        case assert_type::line_end:
                            if (s.it == last or *s.it == '\n')
                                return rc::match_continue; 
                        default:
                            throw tree_error("Encountered unimplemented assertion while matching");
                        }
                        return rc::match_failure;
                    },
                    [&](const expr_tree_t::concat& cat) -> rc
                    {
                        s.cont.append_range(cat.idxs | std::views::reverse);
                        return rc::match_continue;
                    },
                    [&](const expr_tree_t::alt& alt) -> rc
                    {
                        if (alt.idxs.empty())
                            return rc::match_failure;

                        for (std::size_t i{ 0 }; i + 1 < alt.idxs.size(); ++i)
                        {
                            state s_copy{ s };
                            s_copy.cont.push_back(alt.idxs.at(i));

                            if (rec(s_copy))
                            {
                                s = std::move(s_copy); 
                                return rc::match_success;
                            }
                        }

                        s.cont.push_back(alt.idxs.back());
                        return rc::match_continue;
                    },
                    [&](const expr_tree_t::capture& cap) -> rc
                    {
                        if (not s.tag_begs.at(cap.number).has_value())
                        {
                            /* lhs of capture */
                            s.tag_begs.at(cap.number) = s.it;
                            s.cont.push_back(pos);
                            s.cont.push_back(cap.idx);
                        }
                        else
                        {
                            /* rhs of capture */
                            s.tags.at(cap.number) = std::pair{ *s.tag_begs.at(cap.number), s.it };
                            s.tag_begs.at(cap.number).reset();
                        }

                        return rc::match_continue;
                    },
                    [&](const expr_tree_t::backref& bref) -> rc
                    {
                        if (bref.number >= s.tags.size())
                            throw pattern_error("Backreference to non-existent submatch");

                        if (const auto& range{ s.tags.cat(bref.number) }; range.has_value())
                        {
                            for (auto [bit, blast] { *range }; bit != blast; std::ranges::advance(s.it, 1), std::ranges::advance(bit, 1))
                                if (s.it == last or *s.it != *bit)
                                    return rc::match_failure;
                        }
                        else
                        {
                            /* capture doesn't exist */
                            return rc::match_failure;
                        }

                        return rc::match_continue;
                    },
                    [&](const expr_tree_t::repeat& rep) -> rc
                    {
                        using detail::repeater_mode;

                        if (rep.min == rep.max)
                        {
                            /* fixed number of repetitions - don't insert repeated */
                            s.cont.insert(s.cont.end(), /* count = */ rep.min, /* value = */ rep.idx);
                            return rc::match_continue;
                        }

                        const auto rep_count{ raw_data.reps() };

                        if (std::cmp_less(rep_count, rep.min))
                        {
                            /* perform fixed number of repetitions and then revisit  */
                            s.cont.emplace_back(pos, rep.min);
                            s.cont.insert(s.cont.end(), /* count = */ rep.min - rep_count, /* value = */ rep.idx);
                            return rc::match_continue;
                        }
                    
                        if (rep.mode == repeater_mode::greedy)
                        {
                            /* try to match repeated pattern first */
                            state s_copy{ s };

                            if (rep.max < rep.min or rep_count < rep.max - 1)
                                s_copy.cont.emplace_back(pos, rep_count, 1);
                            s_copy.cont.push_back(rep.idx);

                            if (rec(s_copy))
                            {
                                s = std::move(s_copy); 
                                return rc::match_success;
                            }
                        }
                        else if (rep.mode == repeater_mode::lazy)
                        {
                            /* try to match pattern after rep first */
                            state s_copy{ s };

                            if (rec(s_copy))
                            {
                                s = std::move(s_copy); 
                                return rc::match_success;
                            }

                            if (rep.max < rep.min or rep_count < rep.max - 1)
                                s.cont.emplace_back(pos, rep_count, 1);
                            s.cont.push_back(rep.idx);
                        }
                        else /* if (rep.mode == repeater_mode::possessive) */
                        {
                            /* unoptimised possessive matching */

                             state s_copy{ s };

                             if (rep.max < rep.min or rep_count < rep.max - 1)
                                 s_copy.cont.emplace_back(pos, rep_count, 1);
                             s_copy.cont.push_back(rep.idx);
 
                             if (rec(s_copy))
                             {
                                 s = std::move(s_copy); 
                                 return rc::match_success;
                             }
                             else if (rec(s))
                             {
                                return rc::match_success;
                             }
                             else
                             {
                                return rc::match_failure;
                             }
                        }

                        return rc::match_continue;
                    },
                    [&](const expr_tree_t::char_str& lit) -> rc
                    {
                        for (const auto c: lit.data)
                        {
                            if (s.it == last or *s.it != c)
                                return rc::match_failure; /* unsuccessful match */
                            std::ranges::advance(s.it, 1);
                        }

                        return rc::match_continue;
                    },
                    [&](const expr_tree_t::char_class& cla) -> rc
                    {
                        if (s.it == last)
                        {
                            return rc::match_failure;
                        }

                        using uct = typename expr_tree_t::char_class::underlying_char_type;

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

                        
                        if (cla.data.is_negated())
                        {
                            for (const auto& [lower, upper] : cla.data.get())
                                if (lower <= input and input <= upper)
                                    return rc::match_failure;
                            return rc::match_continue;
                        }
                        else
                        {
                            for (const auto& [lower, upper] : cla.data.get())
                                if (lower <= input and input <= upper)
                                    return rc::match_continue;
                            return rc::match_failure;
                        }
                    }
                });

                if (result != rc::match_continue)
                    return (result == rc::match_success);
            }

            return (s.it == last);
        };

        state s(first, this->capture_count(), this->root_idx());
        auto rv{ visitor(s) };

        if (not rv)
            return {};

        tag_result res{ 0 };
        for (const auto& opt: s.tags)
        {
            if (opt.has_value())
                res.push_back(std::distance(first, opt->first)), res.push_back(std::distance(first, opt->second));
            else
                res.insert(res.end(), { detail::no_tag, detail::no_tag });
        }

        return res;
    }
}