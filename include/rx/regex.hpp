// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "rx/api/submatch.hpp"
#include "rx/etc/string_literal.hpp"
#include "rx/etc/util.hpp"
#include "rx/fsm/flags.hpp"
#include "rx/gen/p1306.hpp"
#include "rx/gen/result.hpp"


namespace rx
{
    enum class mode : unsigned char
    {
        standard,
        fast,
        naive
    };

    namespace detail
    {
        consteval auto get_matcher_refl(mode i, bool is_search = false)
        {
            if (i == mode::naive)
                return ^^detail::p1306_matcher; /* todo: implement */
            else if (i == mode::standard and is_search)
                return ^^detail::p1306_searcher;
            else
                return ^^detail::p1306_matcher;
        }
    }

    template<string_literal Pattern, mode Mode = mode::standard>
    struct static_regex
    {
        using char_type = decltype(Pattern)::char_type;
        static_assert(std::same_as<char, char_type>); /* temporary: remove later */

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto match(const I first, const S last)
        {
            using namespace detail::default_fsm_flags;
            using matcher_t = [: detail::get_matcher_refl(Mode) :]<Pattern, full_match>;
            return matcher_t{}(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type> and std::ranges::borrowed_range<R>
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
        [[nodiscard]] static constexpr auto prefix_match(const I first, const S last)
        {
            using namespace detail::default_fsm_flags;
            using matcher_t = [: detail::get_matcher_refl(Mode) :]<Pattern, partial_match>;
            return matcher_t{}(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type> and std::ranges::borrowed_range<R>
        [[nodiscard]] static constexpr auto prefix_match(R&& r)
        {
            return prefix_match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr auto prefix_match(const CharT* cstr)
        {
            return prefix_match(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto search(const I first, const S last)
        {
            using namespace detail::default_fsm_flags;
            using matcher_t = [: detail::get_matcher_refl(Mode, true) :]<Pattern, search_single>;
            return matcher_t{}(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type> and std::ranges::borrowed_range<R>
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

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr bool is_match(const I first, const S last)
        {
            using namespace detail::default_fsm_flags;
            using matcher_t = [: detail::get_matcher_refl(Mode) :]<Pattern, detail::adapt_flags_return_bool(full_match)>;
            return matcher_t{}(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] static constexpr bool is_match(R&& r)
        {
            return is_match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr bool is_match(const CharT* cstr)
        {
            return is_match(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr bool starts_with_match(const I first, const S last)
        {
            using namespace detail::default_fsm_flags;
            using matcher_t = [: detail::get_matcher_refl(Mode) :]<Pattern, detail::adapt_flags_return_bool(partial_match)>;
            return matcher_t{}(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] static constexpr bool starts_with_match(R&& r)
        {
            return starts_with_match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr bool starts_with_match(const CharT* cstr)
        {
            return starts_with_match(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr bool contains_match(const I first, const S last)
        {
            using namespace detail::default_fsm_flags;
            using matcher_t = [: detail::get_matcher_refl(Mode, true) :]<Pattern, detail::adapt_flags_return_bool(search_single)>;
            return matcher_t{}(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] static constexpr bool contains_match(R&& r)
        {
            return contains_match(std::ranges::begin(r), std::ranges::end(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr bool contains_match(const CharT* cstr)
        {
            return contains_match(cstr, detail::cstr_sentinel);
        }

        template<std::bidirectional_iterator I, std::sentinel_for<I> S>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] static constexpr auto range(const I first, const S last)
        {
            return range(std::ranges::subrange(first, last));
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type> and std::ranges::borrowed_range<R>
        [[nodiscard]] static constexpr auto range(R&& r);

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] static constexpr auto range(const CharT* cstr)
        {
            return range(std::ranges::subrange(cstr, detail::cstr_sentinel));
        }
    };


    template<std::ranges::bidirectional_range V, typename Regex>
    requires std::ranges::view<V>
    class regex_match_view
    {
        static_assert("regex_match_view: invalid regex");
    };


    template<std::ranges::bidirectional_range V, string_literal Pattern, mode Mode>
    requires std::ranges::view<V>
    class regex_match_view<V, static_regex<Pattern, Mode>> : std::ranges::view_interface<regex_match_view<V, static_regex<Pattern, Mode>>>
    {
        template<bool Const>
        struct iterator;

        struct sentinel {};

    public:
        regex_match_view() requires std::default_initializable<V> = default;
        constexpr explicit regex_match_view(V base, static_regex<Pattern, Mode> /* regex */) : base_{ std::move(base) } {}

        [[nodiscard]] constexpr V base() const& requires std::copy_constructible<V> { return base_; }
        [[nodiscard]] constexpr V base() && { return std::move(base_); }

        [[nodiscard]] constexpr iterator<false> begin()
        {
            return iterator<false>{ std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr iterator<true> begin() const
        requires std::ranges::bidirectional_range<const V>
        {
            return iterator<true>{ std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr sentinel end()
        {
            return {};
        }

        [[nodiscard]] constexpr sentinel end() const
        requires std::ranges::bidirectional_range<const V>
        {
            return {};
        }

    private:
        V base_{};
    };

    template<std::ranges::bidirectional_range V, string_literal Pattern, mode Mode>
    requires std::ranges::view<V>
    template<bool Const>
    struct regex_match_view<V, static_regex<Pattern, Mode>>::iterator
    {
    private:
        using Parent  = detail::maybe_const_t<Const, regex_match_view>;
        using Base    = detail::maybe_const_t<Const, V>;
        using Matcher = [: detail::get_matcher_refl(Mode, true) :]<Pattern, detail::default_fsm_flags::search_all>;

    public:
        using iterator_category = std::input_iterator_tag;
        using iterator_concept  = std::input_iterator_tag;
        using value_type        = Matcher::template result<std::ranges::iterator_t<Base>>;
        using difference_type   = std::ranges::range_difference_t<Base>;

        iterator() requires std::default_initializable<std::ranges::iterator_t<Base>> = default;

        constexpr iterator(std::ranges::iterator_t<Base> current, std::ranges::sentinel_t<Base> end)
            : current_{ std::move(current) }, end_{ std::move(end) }, result_{ matcher_(current_, end_) } {}

        constexpr explicit(false) iterator(iterator<not Const> i)
        requires Const and std::convertible_to<std::ranges::iterator_t<V>, std::ranges::iterator_t<Base>>
            : current_{ std::move(i.current_) }, end_{ std::move(i.end_) }, result_{ std::move(i.result_) } {}

        constexpr const std::ranges::iterator_t<Base>& base() const& noexcept
        {
            return current_;
        }

        constexpr std::ranges::iterator_t<Base>& base() &&
        {
            return std::move(current_);
        }

        constexpr const value_type& operator*() const
        {
            return result_;
        }

        constexpr const value_type* operator->() const
        {
            return &result_;
        }

        constexpr iterator& operator++()
        {
            if (not result_.has_value())
                return *this;

            if (current_ == result_.match_end_)
            {
                if (current_ == end_)
                {
                    result_.clear_match();
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

            if constexpr (value_type::has_continue)
                result_ = matcher_(current_, end_, result_.continue_at_);
            else
                result_ = matcher_(current_, end_);
            return *this;
        }

        constexpr void operator++(int)
        {
            ++*this;
        }

        friend constexpr bool operator==(const iterator& x, const iterator& y)
        requires std::equality_comparable<std::ranges::iterator_t<Base>>
        {
            return x.current_ == y.current_;
        }

        friend constexpr auto operator<=>(const iterator& x, const iterator& y)
        requires std::three_way_comparable<std::ranges::iterator_t<Base>>
        {
            return x.current_ <=> y.current_;
        }

        friend constexpr bool operator==(const iterator& x, sentinel /* y */)
        {
            return not x.result_.has_value();
        }

        template<std::ranges::input_range W, int...>
        requires std::ranges::view<W>
        friend class submatches_view;

    private:
        std::ranges::iterator_t<Base> current_{};
        [[no_unique_address]] std::ranges::sentinel_t<Base> end_{};
        [[no_unique_address]] Matcher matcher_;
        value_type result_;
    };

    template<typename Range, string_literal Pattern, mode Mode>
    regex_match_view(Range&&, static_regex<Pattern, Mode>) -> regex_match_view<std::views::all_t<Range>, static_regex<Pattern, Mode>>;

    namespace detail
    {
        template<typename R>
        concept static_regex_match_view_like = template_instantiation_of<std::ranges::range_value_t<R>, ^^static_regex_match_result>;
    }

    template<std::ranges::input_range V, int... Submatches>
    requires std::ranges::view<V>
    class submatches_view
    {
        static_assert("submatches_view: invalid range");
    };

    template<std::ranges::input_range V, int... Submatches>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
    class submatches_view<V, Submatches...> : std::ranges::view_interface<submatches_view<V, Submatches...>>
    {
        using MatchResult = std::ranges::range_value_t<V>;

        template<int Submatch>
        static constexpr bool submatch_is_valid{ ((-1 == Submatch) or (Submatch < MatchResult::submatch_count)) };

        static_assert(sizeof...(Submatches) > 0);
        static_assert((submatch_is_valid<Submatches> and ...));

        template<bool Const>
        struct iterator;

        struct sentinel {};

    public:
        submatches_view() requires std::default_initializable<V> = default;
        constexpr explicit submatches_view(V base, std::integer_sequence<int, Submatches...> /* submatches */) : base_{ std::move(base) } {}

        [[nodiscard]] constexpr V base() const& requires std::copy_constructible<V> { return base_; }
        [[nodiscard]] constexpr V base() && { return std::move(base_); }

        [[nodiscard]] constexpr iterator<false> begin()
        {
            return iterator<false>{ std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr iterator<true> begin() const
        requires std::ranges::bidirectional_range<const V>
        {
            return iterator<true>{ std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr sentinel end()
        {
            return {};
        }

        [[nodiscard]] constexpr sentinel end() const
        requires std::ranges::bidirectional_range<const V>
        {
            return {};
        }

    private:
        V base_{};
    };

    template<std::ranges::input_range V, int... Submatches>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
    template<bool Const>
    struct submatches_view<V, Submatches...>::iterator
    {
    private:
        using Parent   = detail::maybe_const_t<Const, submatches_view>;
        using Base     = detail::maybe_const_t<Const, V>;
        using BaseBase = typename std::ranges::iterator_t<Base>::Base;

    public:
        using iterator_category = std::input_iterator_tag;
        using iterator_concept  = std::input_iterator_tag;
        using value_type        = submatch<std::ranges::iterator_t<BaseBase>>;
        using difference_type   = std::ranges::range_difference_t<Base>;

        iterator()
        requires std::default_initializable<std::ranges::iterator_t<Base>>
                 and std::default_initializable<std::ranges::iterator_t<BaseBase>> = default;

        constexpr iterator(std::ranges::iterator_t<Base> current, std::ranges::sentinel_t<Base> end)
            : current_{ std::move(current) }, end_{ std::move(end) },
              suffix_start_{ std::as_const(current_).base() }
        {
            this->stash_result();
        }

        constexpr explicit(false) iterator(iterator<not Const> i)
        requires Const and std::convertible_to<std::ranges::iterator_t<V>, std::ranges::iterator_t<Base>>
            : current_{ std::move(i.current_) }, end_{ std::move(i.end_) }, index_{ i.index_ },
              result_{ std::move(i.result_) }, suffix_start_{ std::move(i.suffix_start_)} {}

        constexpr const std::ranges::iterator_t<Base>& base() const& noexcept
        {
            return current_;
        }

        constexpr std::ranges::iterator_t<Base>& base() &&
        {
            return std::move(current_);
        }

        constexpr const value_type& operator*() const
        {
            return result_;
        }

        constexpr const value_type* operator->() const
        {
            return &result_;
        }

        constexpr iterator& operator++()
        {
            ++index_;

            if (index_ == submatches.size() and current_ != end_)
            {
                ++current_;
                index_ = 0;

                if constexpr (has_suffix_iterator)
                    suffix_start_ = current_.current_;
            }

            if constexpr (has_suffix_iterator)
            {
                if (current_ == end_)
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
            ++*this;
        }

        friend constexpr bool operator==(const iterator& x, const iterator& y)
        requires std::equality_comparable<std::ranges::iterator_t<Base>>
        {
            return x.current_ == y.current_;
        }

        friend constexpr auto operator<=>(const iterator& x, const iterator& y)
        requires std::three_way_comparable<std::ranges::iterator_t<Base>>
        {
            return x.current_ <=> y.current_;
        }

        friend constexpr bool operator==(const iterator& x, sentinel /* y */)
        {
            if constexpr (has_suffix_iterator)
                return not x.current_->has_value() and x.index_ == submatches.size();
            else
                return not x.current_->has_value();
        }

    private:
        static constexpr bool has_suffix_iterator{ ((Submatches == -1) or ...) };
        static constexpr std::array submatches{ Submatches... };

        using suffix_start_t = detail::maybe_type_t<has_suffix_iterator, std::ranges::iterator_t<BaseBase>>;

        constexpr void stash_result()
        {
            if constexpr (has_suffix_iterator)
            {
                if (submatches[index_] == -1)
                {
                    using sf = detail::submatch_factory<std::ranges::iterator_t<BaseBase>>;

                    if (current_->has_value())
                    {
                        result_ = sf::make_submatch(suffix_start_, current_->template get<0>().begin());
                    }
                    else
                    {
                        if constexpr (std::ranges::common_range<BaseBase>)
                        {
                            result_ = sf::make_submatch(suffix_start_, current_.end_);
                        }
                        else if constexpr (std::ranges::random_access_range<BaseBase> and std::ranges::sized_range<BaseBase>)
                        {
                            result_ = sf::make_submatch(suffix_start_, suffix_start_ + (current_.end_ - suffix_start_));
                        }
                        else
                        {
                            std::ranges::iterator_t<BaseBase> suffix_end{ suffix_start_ };
                            while (suffix_end != current_.end_)
                                ++suffix_end;

                            result_ = sf::make_submatch(suffix_start_, suffix_end);
                        }

                        if (result_.begin() == result_.end())
                            index_ = submatches.size();
                    }

                    return;
                }
            }

            if (current_->has_value())
                result_ = current_->operator[](submatches[index_]);
        }


        std::ranges::iterator_t<Base> current_{};
        [[no_unique_address]] std::ranges::sentinel_t<Base> end_{};
        std::size_t index_{ 0 };
        value_type  result_;

        [[no_unique_address]] suffix_start_t suffix_start_;
    };


    template<std::ranges::input_range V>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
    class submatches_view<V> : std::ranges::view_interface<submatches_view<V>>
    {
        using MatchResult = std::ranges::range_value_t<V>;
        static constexpr int submatch_limit{ MatchResult::submatch_count };

        template<bool Const>
        struct iterator;

        struct sentinel {};

    public:
        submatches_view() requires std::default_initializable<V> = default;

        template<std::ranges::input_range R>
        requires std::same_as<std::ranges::range_value_t<R>, int>
        constexpr explicit submatches_view(V base, R&& submatches)
            : base_{ std::move(base) }, submatches_{ std::from_range, std::forward<R>(submatches) }
        {
            if (submatches_.empty())
                throw std::invalid_argument("submatches_view::submatches_view: no submatches specified");

            if (not std::ranges::all_of(submatches_, [](int s) { return (-1 == s) or (s < submatch_limit); }))
                throw std::out_of_range("submatches_view::submatches_view: invalid submatch index");
        }

        [[nodiscard]] constexpr V base() const& requires std::copy_constructible<V> { return base_; }
        [[nodiscard]] constexpr V base() && { return std::move(base_); }

        [[nodiscard]] constexpr iterator<false> begin()
        {
            return iterator<false>{ this, std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr iterator<true> begin() const
        requires std::ranges::bidirectional_range<const V>
        {
            return iterator<true>{ this, std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr sentinel end()
        {
            return {};
        }

        [[nodiscard]] constexpr sentinel end() const
        requires std::ranges::bidirectional_range<const V>
        {
            return {};
        }

    private:
        V base_{};
        std::vector<int> submatches_{ 0 };
    };

    template<std::ranges::input_range V>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
    template<bool Const>
    struct submatches_view<V>::iterator
    {
    private:
        using Parent   = detail::maybe_const_t<Const, submatches_view>;
        using Base     = detail::maybe_const_t<Const, V>;
        using BaseBase = typename std::ranges::iterator_t<Base>::Base;

    public:
        using iterator_category = std::input_iterator_tag;
        using iterator_concept  = std::input_iterator_tag;
        using value_type        = submatch<std::ranges::iterator_t<BaseBase>>;
        using difference_type   = std::ranges::range_difference_t<Base>;

        iterator()
        requires std::default_initializable<std::ranges::iterator_t<Base>>
                 and std::default_initializable<std::ranges::iterator_t<BaseBase>> = default;

        constexpr iterator(Parent* parent, std::ranges::iterator_t<Base> current, std::ranges::sentinel_t<Base> end)
            : parent_{ parent }, current_{ std::move(current) }, end_{ std::move(end) },
              suffix_start_{ std::as_const(current_).base() }
        {
            this->stash_result();
        }

        constexpr explicit(false) iterator(iterator<not Const> i)
        requires Const and std::convertible_to<std::ranges::iterator_t<V>, std::ranges::iterator_t<Base>>
            : current_{ std::move(i.current_) }, end_{ std::move(i.end_) }, index_{ i.index_ },
              result_{ std::move(i.result_) }, suffix_start_{ std::move(i.suffix_start_)} {}

        constexpr const std::ranges::iterator_t<Base>& base() const& noexcept
        {
            return current_;
        }

        constexpr std::ranges::iterator_t<Base>& base() &&
        {
            return std::move(current_);
        }

        constexpr const value_type& operator*() const
        {
            return result_;
        }

        constexpr const value_type* operator->() const
        {
            return &result_;
        }

        constexpr iterator& operator++()
        {
            ++index_;

            if (index_ == parent_->submatches_.size() and current_ != end_)
            {
                ++current_;
                index_ = 0;
                suffix_start_ = current_.current_;
            }

            if (current_ == end_)
            {
                while (index_ < parent_->submatches_.size() and parent_->submatches_.at(index_) != -1)
                    ++index_;

                if (index_ == parent_->submatches_.size())
                    return *this;
            }

            this->stash_result();
            return *this;
        }

        constexpr void operator++(int)
        {
            ++*this;
        }

        friend constexpr bool operator==(const iterator& x, const iterator& y)
        requires std::equality_comparable<std::ranges::iterator_t<Base>>
        {
            return x.current_ == y.current_;
        }

        friend constexpr auto operator<=>(const iterator& x, const iterator& y)
        requires std::three_way_comparable<std::ranges::iterator_t<Base>>
        {
            return x.current_ <=> y.current_;
        }

        friend constexpr bool operator==(const iterator& x, sentinel /* y */)
        {
            return not x.current_->has_value() and x.index_ == x.parent_->submatches_.size();
        }

    private:
        constexpr void stash_result()
        {
            if (parent_->submatches_.at(index_) == -1)
            {
                using sf = detail::submatch_factory<std::ranges::iterator_t<BaseBase>>;

                if (current_->has_value())
                {
                    result_ = sf::make_submatch(suffix_start_, current_->template get<0>().begin());
                }
                else
                {
                    if constexpr (std::ranges::common_range<BaseBase>)
                    {
                        result_ = sf::make_submatch(suffix_start_, current_.end_);
                    }
                    else if constexpr (std::ranges::random_access_range<BaseBase> and std::ranges::sized_range<BaseBase>)
                    {
                        result_ = sf::make_submatch(suffix_start_, suffix_start_ + (current_.end_ - suffix_start_));
                    }
                    else
                    {
                        std::ranges::iterator_t<BaseBase> suffix_end{ suffix_start_ };
                        while (suffix_end != current_.end_)
                            ++suffix_end;

                        result_ = sf::make_submatch(suffix_start_, suffix_end);
                    }

                    if (result_.begin() == result_.end())
                        index_ = parent_->submatches_.size();
                }

                return;
            }

            if (current_->has_value())
                result_ = current_->operator[](parent_->submatches_.at(index_));
        }


        Parent* parent_;
        std::ranges::iterator_t<Base> current_{};
        [[no_unique_address]] std::ranges::sentinel_t<Base> end_{};
        std::size_t index_{ 0 };
        value_type  result_;

        std::ranges::iterator_t<BaseBase> suffix_start_;
    };


    template<typename Range, int... Submatches>
    submatches_view(Range&&, std::integer_sequence<int, Submatches...>) -> submatches_view<std::views::all_t<Range>, Submatches...>;

    template<typename Range, typename Submatches>
    submatches_view(Range&&, Submatches&&) -> submatches_view<std::views::all_t<Range>>;


    namespace detail
    {
        template<typename Regex>
        concept static_regex_like = template_instantiation_of<Regex, ^^static_regex>;
    }

    namespace views
    {
        namespace detail
        {
            template<typename Range, typename Regex>
            concept can_regex_match_view = requires
            {
                regex_match_view(std::declval<Range>(), std::declval<Regex>());
            };

            template<typename Range, typename T>
            concept can_submatches_view = requires
            {
                submatches_view(std::declval<Range>(), std::declval<T>());
            };

            template<typename Regex>
            struct static_regex_adaptor_closure : std::ranges::range_adaptor_closure<static_regex_adaptor_closure<Regex>>
            {
                template<std::ranges::viewable_range Range>
                requires detail::can_regex_match_view<Range, Regex>
                [[nodiscard]] constexpr auto operator()(Range&& r) const
                {
                    return regex_match_view(std::forward<Range>(r), Regex{});
                }
            };

            struct regex_match_adaptor
            {
                template<std::ranges::viewable_range Range, typename Regex>
                requires detail::can_regex_match_view<Range, Regex>
                [[nodiscard]] constexpr auto operator()(Range&& r, Regex&& x) const
                {
                    return regex_match_view(std::forward<Range>(r), std::forward<Regex>(x));
                }

                template<typename Regex>
                requires rx::detail::static_regex_like<Regex>
                [[nodiscard]] consteval auto operator()(Regex /* x */) const
                {
                    return static_regex_adaptor_closure<Regex>();
                }
            };

            template<int... Submatches>
            struct static_submatches_adaptor_closure : std::ranges::range_adaptor_closure<static_submatches_adaptor_closure<Submatches...>>
            {
                template<std::ranges::viewable_range Range>
                requires detail::can_submatches_view<Range, std::integer_sequence<int, Submatches...>>
                [[nodiscard]] constexpr auto operator()(Range&& r) const
                {
                    return submatches_view(std::forward<Range>(r), std::integer_sequence<int, Submatches...>{});
                }
            };

            template<typename T>
            struct dynamic_submatches_adaptor_closure : std::ranges::range_adaptor_closure<dynamic_submatches_adaptor_closure<T>>
            {
                template<std::ranges::viewable_range Range>
                requires detail::can_submatches_view<Range, std::views::all_t<T>>
                [[nodiscard]] constexpr auto operator()(Range&& r) const
                {
                    return submatches_view(std::forward<Range>(r), sub_);
                }

                constexpr explicit dynamic_submatches_adaptor_closure(T submatches) :
                    sub_{ std::forward<T>(submatches) } {}

            private:
                std::views::all_t<T> sub_;
            };

            struct submatches_adaptor
            {
                template<std::ranges::viewable_range Range, typename Submatches>
                requires detail::can_submatches_view<Range, Submatches>
                [[nodiscard]] constexpr auto operator()(Range&& r, Submatches&& x) const
                {
                    return submatches_view(std::forward<Range>(r), std::forward<Submatches>(x));
                }

                template<typename Submatches>
                [[nodiscard]] constexpr auto operator()(Submatches&& submatches) const
                {
                    return dynamic_submatches_adaptor_closure{ std::forward<Submatches>(submatches) };
                }

                template<int... Submatches>
                [[nodiscard]] consteval auto operator()(std::integer_sequence<int, Submatches...>) const
                {
                    return static_submatches_adaptor_closure<Submatches...>();
                }

                template<int... Submatches>
                [[nodiscard]] consteval auto operator()(std::integral_constant<int, Submatches>...) const
                requires (sizeof...(Submatches) == 1)
                {
                    return static_submatches_adaptor_closure<Submatches...>();
                }
            };
        }

        inline constexpr detail::regex_match_adaptor regex_match;
        inline constexpr detail::submatches_adaptor submatches;
    }


    template<string_literal Pattern, mode Mode>
    template<std::ranges::bidirectional_range R>
    requires std::same_as<std::ranges::range_value_t<R>, typename static_regex<Pattern, Mode>::char_type> and std::ranges::borrowed_range<R>
    constexpr auto static_regex<Pattern, Mode>::range(R&& r)
    {
        return regex_match_view(r, static_regex<Pattern, Mode>{});
    }


    namespace literals
    {
        template<string_literal Pattern>
        consteval static_regex<Pattern> operator ""_rx()
        {
            return {};
        }

        template<string_literal Pattern>
        consteval static_regex<Pattern, mode::fast> operator ""_rxf()
        {
            return {};
        }

        template<string_literal Pattern>
        consteval static_regex<Pattern, mode::fast> operator ""_rx_fast()
        {
            return {};
        }

        template<string_literal Pattern>
        consteval static_regex<Pattern, mode::naive> operator ""_rxn()
        {
            return {};
        }

        template<string_literal Pattern>
        consteval static_regex<Pattern, mode::naive> operator ""_rx_naive()
        {
            return {};
        }
    }
}

