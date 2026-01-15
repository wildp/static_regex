// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <type_traits>

#include "etc/string_literal.hpp"
#include "etc/util.hpp"
#include "fsm/flags.hpp"
#include "gen/p1306.hpp"
#include "gen/result.hpp"


namespace rx
{
    template<string_literal Pattern>
    requires std::same_as<char, typename decltype(Pattern)::char_type> /* temporary: remove later */
    struct static_regex
    {
        using char_type = decltype(Pattern)::char_type;

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto match(const I first, const S last)
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::full_match> matcher;
            return matcher(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] static constexpr auto match(R&& r)
        {
            return match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr auto match(const CharT* cstr)
        {
            return match(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto starts_with(const I first, const S last)
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::partial_match> matcher;
            return matcher(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] static constexpr auto starts_with(R&& r)
        {
            return starts_with(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr auto starts_with(const CharT* cstr)
        {
            return starts_with(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto search(const I first, const S last)
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::search_single> matcher;
            return matcher(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] static constexpr auto search(R&& r)
        {
            return search(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr auto search(const CharT* cstr)
        {
            return search(cstr, detail::cstr_sentinel);
        }
    };


    template<string_literal, typename, typename>
    class static_regex_iterator
    {
    public:
        constexpr static_regex_iterator() noexcept = default;
        friend constexpr bool operator==(static_regex_iterator, static_regex_iterator) noexcept = default;
    };

    using static_regex_sentinel_t = static_regex_iterator<"", void, void>;
    inline constexpr static_regex_sentinel_t static_regex_sentinel;


    template<string_literal, typename, typename, int...>
    class static_regex_token_iterator
    {
    public:
        constexpr static_regex_token_iterator() noexcept = default;
        friend constexpr bool operator==(static_regex_token_iterator, static_regex_token_iterator) noexcept = default;
    };

    using static_regex_token_sentinel_t = static_regex_token_iterator<"", void, void>;
    inline constexpr static_regex_token_sentinel_t static_regex_token_sentinel;


    template<string_literal Pattern, std::bidirectional_iterator I, std::sentinel_for<I> S>
    requires std::same_as<std::iter_value_t<I>, typename static_regex<Pattern>::char_type> and std::default_initializable<I>
    class static_regex_iterator<Pattern, I, S>
    {
        using matcher_type      = detail::p1306_matcher<Pattern, detail::default_fsm_flags::search_all>;

    public:
        using iterator_concept  = std::input_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using value_type        = matcher_type::template result_type<I>;
        using difference_type   = std::ptrdiff_t;
        using regex_type        = static_regex<Pattern>;

        static_regex_iterator() = default;

        constexpr static_regex_iterator(const I first, const S last, regex_type)
            : static_regex_iterator(first, last) {}

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, typename regex_type::char_type>
                 and std::same_as<std::ranges::iterator_t<R>, I> and std::same_as<std::ranges::sentinel_t<R>, S>
        constexpr static_regex_iterator(R&& r, regex_type)
            : static_regex_iterator(std::ranges::begin(r), std::ranges::end(r)) {}

        template<detail::character CharT>
        requires std::same_as<const CharT*, I>
        constexpr static_regex_iterator(const CharT* str, regex_type)
            : static_regex_iterator(str, detail::cstr_sentinel) {}

        constexpr static_regex_iterator& operator++()
        {
            if (not result_.has_value())
                return *this;

            if (current_ == result_.match_end_)
            {
                if (current_ == end_)
                {
                    result_ = value_type{};
                    return *this;
                }
                else
                {
                    ++current_;
                }
            }
            else
            {
                current_ = result_.match_end_;
            }

            result_ = matcher_(current_, end_, result_.continue_at_);
            return *this;
        }

        constexpr void operator++(int)
        {
            +*this;
        }

        [[nodiscard]] constexpr const value_type& operator*() const noexcept
        {
            return result_;
        }

        [[nodiscard]] constexpr const value_type* operator->() const noexcept
        {
            return &result_;
        }

        friend constexpr bool operator==(const static_regex_iterator& x, const static_regex_iterator& y) noexcept
        {
            return x.current_ == y.current_;
        }

        friend constexpr bool operator==(const static_regex_iterator& x, static_regex_sentinel_t) noexcept
        {
            return not x.result_.has_value();
        }

        template<string_literal, typename, typename, int...>
        friend class static_regex_token_iterator;

    private:
        constexpr static_regex_iterator(const I first, const S last)
            : current_{ first }, end_{ last }, result_(matcher_(current_, end_))
        {
            if (not result_.has_value())
                current_ = I{};
        }

        I current_{};
        [[no_unique_address]] S end_{};
        [[no_unique_address]] matcher_type matcher_;
        value_type result_;
    };


    static_regex_iterator() -> static_regex_iterator<"", void, void>;

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, string_literal Pattern>
    static_regex_iterator(I, S, static_regex<Pattern>) -> static_regex_iterator<Pattern, I, S>;

    template<std::ranges::bidirectional_range R, string_literal Pattern>
    static_regex_iterator(R&&, static_regex<Pattern>) -> static_regex_iterator<Pattern, std::ranges::iterator_t<R>, std::ranges::sentinel_t<R>>;

    template<detail::character CharT, string_literal Pattern>
    static_regex_iterator(const CharT*, static_regex<Pattern>) -> static_regex_iterator<Pattern, const CharT*, detail::cstr_sentinel_t>;


    template<string_literal Pattern, std::bidirectional_iterator I, std::sentinel_for<I> S, int... Submatches> 
    class static_regex_token_iterator<Pattern, I, S, Submatches...>
    {
        using parent_type = static_regex_iterator<Pattern, I, S>;

        template<int Submatch>
        static constexpr bool submatch_is_valid{ ((-1 == Submatch) or (Submatch < parent_type::value_type::submatch_count)) };

        static_assert(sizeof...(Submatches) > 0);
        static_assert((submatch_is_valid<Submatches> and ...));

    public:
        using iterator_concept  = std::input_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using value_type        = parent_type::value_type::submatch_type;
        using difference_type   = std::ptrdiff_t;
        using regex_type        = static_regex<Pattern>;

        static_regex_token_iterator() = default;

        constexpr static_regex_token_iterator(const I first, const S last, regex_type, std::integer_sequence<int, Submatches...>)
            : static_regex_token_iterator(first, last) {}

        constexpr static_regex_token_iterator(const I first, const S last, regex_type, std::integral_constant<int, Submatches>...)
        requires (sizeof...(Submatches) == 1)
            : static_regex_token_iterator(first, last) {}

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, typename regex_type::char_type>
                 and std::same_as<std::ranges::iterator_t<R>, I> and std::same_as<std::ranges::sentinel_t<R>, S>
        constexpr static_regex_token_iterator(R&& r, regex_type, std::integer_sequence<int, Submatches...>)
            : static_regex_token_iterator(std::ranges::begin(r), std::ranges::end(r)) {}

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, typename regex_type::char_type>
                 and std::same_as<std::ranges::iterator_t<R>, I> and std::same_as<std::ranges::sentinel_t<R>, S>
        constexpr static_regex_token_iterator(R&& r, regex_type, std::integral_constant<int, Submatches>...)
        requires (sizeof...(Submatches) == 1)
            : static_regex_token_iterator(std::ranges::begin(r), std::ranges::end(r)) {}

        template<detail::character CharT>
        requires std::same_as<const CharT*, I>
        constexpr static_regex_token_iterator(const CharT* str, regex_type, std::integer_sequence<int, Submatches...>)
            : static_regex_token_iterator(str, detail::cstr_sentinel) {}

        template<detail::character CharT>
        requires std::same_as<const CharT*, I>
        constexpr static_regex_token_iterator(const CharT* str, regex_type, std::integral_constant<int, Submatches>...)
        requires (sizeof...(Submatches) == 1)
            : static_regex_token_iterator(str, detail::cstr_sentinel) {}

        constexpr static_regex_token_iterator& operator++()
        {
            ++index_;

            if (index_ == submatches.size() and iterator_ != static_regex_sentinel)
            {
                ++iterator_;
                index_ = 0;

                if constexpr (is_suffix_iterator)
                    suffix_start_ = iterator_.current_;
            }

            if constexpr (is_suffix_iterator)
            {
                if (iterator_ == static_regex_sentinel)
                {
                    while (index_ < submatches.size() and submatches[index_] != -1)
                        ++index_;

                    if (index_ == submatches.size())
                        return *this;
                }
            }

            this->stash_result();
            return *this;
        }

        constexpr void operator++(int)
        {
            *this;
        }

        [[nodiscard]] constexpr const value_type& operator*() const noexcept
        {
            return result_;
        }

        [[nodiscard]] constexpr const value_type* operator->() const noexcept
        {
            return &result_;
        }

        friend constexpr bool operator==(const static_regex_token_iterator& x, const static_regex_token_iterator& y) noexcept
        {
            return x.iterator_ == y.iterator_ and x.index_ == y.index_;
        }

        friend constexpr bool operator==(const static_regex_token_iterator& x, static_regex_token_sentinel_t) noexcept
        {
            if constexpr (is_suffix_iterator)
                return x.iterator_ == static_regex_sentinel and x.index_ == submatches.size();
            else
                return x.iterator_ == static_regex_sentinel;
        }

    private:
        static constexpr bool is_suffix_iterator{ ((Submatches == -1) or ...) };
        static constexpr std::array submatches{ Submatches... };

        using iter_type = value_type::iterator;
        using suffix_it = detail::maybe_type_t<is_suffix_iterator, iter_type>;

        constexpr static_regex_token_iterator(const I first, const S last)
            : iterator_{ first, last, regex_type{} }, suffix_start_{ detail::maybe_type_init<is_suffix_iterator>(first) }
        {
            this->stash_result();
        }

        constexpr void stash_result()
        {
            if constexpr (is_suffix_iterator)
            {
                if (submatches[index_] == -1)
                {
                    using sf = detail::submatch_factory<iter_type>;
                    
                    if (iterator_->has_value())
                    {
                        result_ = sf::make_submatch(suffix_start_, iterator_->template get<0>().begin());
                    }
                    else
                    {
                        if constexpr (std::same_as<S, I>)
                        {
                            result_ = sf::make_submatch(suffix_start_, iterator_.end_);
                        }
                        else if constexpr (std::random_access_iterator<I> and std::sized_sentinel_for<S, I>)
                        {
                            result_ = sf::make_submatch(suffix_start_, suffix_start_ + (iterator_.end_ - suffix_start_));
                        }
                        else
                        {
                            I suffix_end{ suffix_start_ };
                            while (suffix_end != iterator_.end_)
                                ++suffix_end;

                            result_ = sf::make_submatch(suffix_start_, suffix_end);
                        }

                        if (result_.begin() == result_.end())
                            index_ = submatches.size();
                    }

                    return;
                }
            }

            if (iterator_->has_value())
                result_ = iterator_->operator[](submatches[index_]);
        }


        parent_type iterator_{};
        std::size_t index_{ 0 };
        value_type  result_;

        [[no_unique_address]] suffix_it suffix_start_;
    };


    static_regex_token_iterator() -> static_regex_token_iterator<"", void, void>;

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, string_literal Pattern, int Int>
    static_regex_token_iterator(I, S, static_regex<Pattern>, std::integral_constant<int, Int>)
        -> static_regex_token_iterator<Pattern, I, S, Int>;

    template<std::bidirectional_iterator I, std::sentinel_for<I> S, string_literal Pattern, int... Ints>
    static_regex_token_iterator(I, S, static_regex<Pattern>, std::integer_sequence<int, Ints...>)
        -> static_regex_token_iterator<Pattern, I, S, Ints...>;

    template<std::ranges::bidirectional_range R, string_literal Pattern, int Int>
    static_regex_token_iterator(R&&, static_regex<Pattern>, std::integral_constant<int, Int>)
        -> static_regex_token_iterator<Pattern, std::ranges::iterator_t<R>, std::ranges::sentinel_t<R>, Int>;

    template<std::ranges::bidirectional_range R, string_literal Pattern, int... Ints>
    static_regex_token_iterator(R&&, static_regex<Pattern>, std::integer_sequence<int, Ints...>)
        -> static_regex_token_iterator<Pattern, std::ranges::iterator_t<R>, std::ranges::sentinel_t<R>, Ints...>;

    template<typename CharT, string_literal Pattern, int Int>
    static_regex_token_iterator(const CharT*, static_regex<Pattern>, std::integral_constant<int, Int>)
        -> static_regex_token_iterator<Pattern, const CharT*, detail::cstr_sentinel_t, Int>;

    template<typename CharT, string_literal Pattern, int... Ints>
    static_regex_token_iterator(const CharT*, static_regex<Pattern>, std::integer_sequence<int, Ints...>)
        -> static_regex_token_iterator<Pattern, const CharT*, detail::cstr_sentinel_t, Ints...>;


    namespace literals
    {
        template<string_literal P>
        consteval auto operator ""_rx()
        {
            return static_regex<P>();
        }
    }
}

