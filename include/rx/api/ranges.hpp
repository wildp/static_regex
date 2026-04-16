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
#include "rx/api/static_regex.hpp"
#include "rx/api/replace.hpp"
#include "rx/etc/string_literal.hpp"
#include "rx/etc/util.hpp"
#include "rx/fsm/flags.hpp"


namespace rx
{
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
        class iterator;

    public:
        regex_match_view() requires std::default_initializable<V> = default;
        constexpr explicit regex_match_view(V base, static_regex<Pattern, Mode> /* regex */) : base_{ std::move(base) } {}

        [[nodiscard]] constexpr V base() const& requires std::copy_constructible<V> { return base_; }
        [[nodiscard]] constexpr V base() && { return std::move(base_); }

        [[nodiscard]] constexpr iterator begin()
        {
            /* since regex_match_view is an input range, there
               is no need to cache future calls to begin() */
            auto current = std::ranges::begin(base_);
            find_first(current);
            return iterator{ *this, std::move(current) };
        }

        [[nodiscard]] constexpr std::default_sentinel_t end()
        {
            return std::default_sentinel;
        }

    private:
        using matcher_type = [: detail::get_matcher_refl(Mode, true) :]<Pattern, detail::default_fsm_flags::search_all>;
        using result_type = matcher_type::template result<std::ranges::iterator_t<V>>;

        template<bool MatchNonEmpty = false>
        constexpr void find_first(std::ranges::iterator_t<V> current)
        {
            if constexpr (MatchNonEmpty)
                cached_result_ = matcher(current, std::ranges::end(base_), detail::match_non_empty);
            else
                cached_result_ = matcher(current, std::ranges::end(base_));
        }

        template<bool MatchNonEmpty = false>
        constexpr void find_next(std::ranges::iterator_t<V> current)
        {
            if constexpr (MatchNonEmpty)
            {
                if constexpr (result_type::continue_from_it)
                    cached_result_ = matcher(std::ranges::begin(base_), std::ranges::end(base_), current, detail::match_non_empty);
                else if constexpr (result_type::has_continue)
                    cached_result_ = matcher(current, std::ranges::end(base_), cached_result_.continue_at_, detail::match_non_empty);
                else
                    cached_result_ = matcher(current, std::ranges::end(base_), detail::match_non_empty);
            }
            else
            {
                if constexpr (result_type::continue_from_it)
                    cached_result_ = matcher(std::ranges::begin(base_), std::ranges::end(base_), current);
                else if constexpr (result_type::has_continue)
                    cached_result_ = matcher(current, std::ranges::end(base_), cached_result_.continue_at_);
                else
                    cached_result_ = matcher(current, std::ranges::end(base_));
            }
        }

        static constexpr matcher_type matcher{};

        V base_{};
        result_type cached_result_;
    };

    template<std::ranges::bidirectional_range V, string_literal Pattern, mode Mode>
        requires std::ranges::view<V>
    class regex_match_view<V, static_regex<Pattern, Mode>>::iterator
    {
    public:
        using iterator_concept  = std::input_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using value_type        = result_type;
        using difference_type   = std::ranges::range_difference_t<V>;

        iterator() requires std::default_initializable<std::ranges::iterator_t<V>> = default;

        constexpr explicit iterator(regex_match_view& parent, std::ranges::iterator_t<V> current)
            : parent_{ std::addressof(parent) }, current_{ std::move(current) } {}

        constexpr std::ranges::iterator_t<V> base() const
        {
            return current_;
        }

        constexpr const value_type& operator*() const noexcept
        {
            return parent_->cached_result_;
        }

        constexpr iterator& operator++()
        {
            if (not parent_->cached_result_.has_value())
                return *this;

            const auto& [mfirst, mlast] = parent_->cached_result_.template force_get<0>();
            current_ = mlast;

            if constexpr (not matcher_type::never_empty)
            {
                if (mfirst == mlast)
                {
                    if (current_ == end())
                    {
                        parent_->cached_result_.clear_match();
                        return *this;
                    }

                    if (current_ == begin())
                        parent_->template find_first<true>(current_);
                    else
                        parent_->template find_next<true>(current_);

                    return *this;
                }
            }

            parent_->find_next(current_);
            return *this;
        }

        constexpr void operator++(int)
        {
            ++*this;
        }

        friend constexpr bool operator==(const iterator& x, std::default_sentinel_t)
        {
            return not x.parent_->cached_result_.has_value();
        }

        template<std::ranges::input_range W, int...>
            requires std::ranges::view<W>
        friend class submatches_view;

        template<std::ranges::input_range W, typename>
            requires std::ranges::view<W>
        friend class replace_view;

    private:
        constexpr std::ranges::iterator_t<V> begin() const
        {
            return std::ranges::begin(parent_->base_);
        }

        constexpr std::ranges::sentinel_t<V> end() const
        {
            return std::ranges::end(parent_->base_);
        }

        regex_match_view* parent_{ nullptr };
        std::ranges::iterator_t<V> current_{};
    };

    template<typename R, string_literal Pattern, mode Mode>
    regex_match_view(R&&, static_regex<Pattern, Mode>) -> regex_match_view<std::views::all_t<R>, static_regex<Pattern, Mode>>;


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
        using match_result_type = std::ranges::range_value_t<V>;
        static constexpr int submatch_limit{ match_result_type::submatch_count };

        template<int Submatch>
        static constexpr bool submatch_is_valid{ ((-1 == Submatch) or (Submatch < match_result_type::submatch_count)) };
        static_assert((submatch_is_valid<Submatches> and ... and true));

        struct iterator;

    public:
        submatches_view() requires std::default_initializable<V> = default;

        constexpr explicit submatches_view(V base, std::integer_sequence<int, Submatches...> /* submatches */)
            requires (sizeof...(Submatches) > 0)
            : base_{ std::move(base) } {}

        template<std::ranges::input_range R>
            requires std::same_as<std::ranges::range_value_t<R>, int>
        constexpr explicit submatches_view(V base, R&& submatches)
            requires (sizeof...(Submatches) == 0)
            : base_{ std::move(base) }, dynamic_submatches_(std::from_range, std::forward<R>(submatches))
        {
            if (dynamic_submatches_.empty())
                throw std::invalid_argument("submatches_view::submatches_view: no submatches specified");

            if (not std::ranges::all_of(dynamic_submatches_, [](int s) { return (-1 == s) or (s < submatch_limit); }))
                throw std::out_of_range("submatches_view::submatches_view: invalid submatch index");
        }

        [[nodiscard]] constexpr V base() const& requires std::copy_constructible<V> { return base_; }
        [[nodiscard]] constexpr V base() && { return std::move(base_); }

        [[nodiscard]] constexpr iterator begin()
        {
            return iterator{ *this, std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr std::default_sentinel_t end()
        {
            return std::default_sentinel;
        }

    private:
        using dynamic_submatches_t = detail::maybe_type_t<sizeof...(Submatches) == 0, std::vector<int>>;
        using underlying_type = std::remove_cvref_t<decltype(std::declval<V>().base().base())>;
        using submatch_type = submatch<std::ranges::iterator_t<underlying_type>>;

        static constexpr bool maybe_has_suffix_iterator{ ((sizeof...(Submatches) == 0) or ... or (Submatches == -1)) };
        static constexpr std::size_t suffix_index{ -1uz };

        V base_{};
        [[no_unique_address]] dynamic_submatches_t dynamic_submatches_;
    };

    template<std::ranges::input_range V, int... Submatches>
        requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
    struct submatches_view<V, Submatches...>::iterator
    {
    public:
        using iterator_concept  = std::input_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using value_type        = submatch_type;
        using difference_type   = std::ranges::range_difference_t<V>;

        iterator() requires std::default_initializable<std::ranges::iterator_t<V>>
                            and std::default_initializable<std::ranges::sentinel_t<V>>
                            and std::default_initializable<std::ranges::iterator_t<underlying_type>> = default;

        constexpr iterator(submatches_view& parent, std::ranges::iterator_t<V> current, std::ranges::sentinel_t<V> end)
            : parent_{ std::addressof(parent) }, current_{ std::move(current) }, end_{ std::move(end) }
        {
            if constexpr (maybe_has_suffix_iterator)
                set_suffix_iterator_if_needed();
        }

        constexpr const std::ranges::iterator_t<V>& base() const& noexcept
        {
            return current_;
        }

        constexpr std::ranges::iterator_t<V>& base() &&
        {
            return std::move(current_);
        }

        constexpr value_type operator*() const
        {
            using sf = detail::submatch_factory<std::ranges::iterator_t<underlying_type>>;

            if constexpr (maybe_has_suffix_iterator)
                if (index_ == suffix_index)
                    return sf::make_submatch(current_.base(), current_.end());

            const auto submatch_index = get_submatch(index_);

            if constexpr (maybe_has_suffix_iterator)
                if (submatch_index == -1)
                    return sf::make_submatch(current_.base(), get<0>(*current_).begin());

            return (*current_)[submatch_index];
        }

        constexpr iterator& operator++()
        {
            ++index_;

            if (index_ == submatch_count() and current_ != end_)
            {
                ++current_;
                index_ = 0;

                if constexpr (maybe_has_suffix_iterator)
                    set_suffix_iterator_if_needed();
            }

            return *this;
        }

        constexpr void operator++(int)
        {
            ++*this;
        }

        friend constexpr bool operator==(const iterator& x, std::default_sentinel_t)
        {
            if constexpr (maybe_has_suffix_iterator)
                return x.current_ == x.end_ and x.index_ != suffix_index;
            else
                return x.current_ == x.end_;
        }

    private:
        using maybe_parent_ptr = detail::maybe_type_t<sizeof...(Submatches) == 0, submatches_view*>;

        constexpr void set_suffix_iterator_if_needed()
        {
            if (current_ == end_ and current_.base() != current_.end())
            {
                if constexpr (sizeof...(Submatches) == 0)
                {
                    if (std::ranges::contains(parent_->dynamic_submatches_, -1))
                        index_ = suffix_index;
                }
                else
                {
                    index_ = suffix_index;
                }
            }
        }

        [[nodiscard]] static consteval std::size_t submatch_count()
            requires (sizeof...(Submatches) > 0)
        {
            return sizeof...(Submatches);
        }

        [[nodiscard]] constexpr std::size_t submatch_count() const
            requires (sizeof...(Submatches) == 0)
        {
            return parent_->dynamic_submatches_.size();
        }

        [[nodiscard]] static constexpr int get_submatch(std::size_t i)
            requires (sizeof...(Submatches) > 0)
        {
            static constexpr std::array static_submatches{ Submatches... };
            return static_submatches[i];
        }

        [[nodiscard]] constexpr int get_submatch(std::size_t i) const
            requires (sizeof...(Submatches) == 0)
        {
            return parent_->dynamic_submatches_[i];
        }

        [[no_unique_address]] maybe_parent_ptr parent_{ nullptr };
        std::ranges::iterator_t<V> current_{};
        [[no_unique_address]] std::ranges::sentinel_t<V> end_{};
        std::size_t index_{ 0 };
    };

    template<typename R, int... Submatches>
    submatches_view(R&&, std::integer_sequence<int, Submatches...>) -> submatches_view<std::views::all_t<R>, Submatches...>;

    template<typename R, typename Submatches>
    submatches_view(R&&, Submatches&&) -> submatches_view<std::views::all_t<R>>;


    template<std::ranges::input_range V, typename Fmt>
        requires std::ranges::view<V>
    class replace_view
    {
        static_assert("replace_view: invalid range");
    };


    template<std::ranges::input_range V, string_literal Fmt>
        requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
    class replace_view<V, fmt_t<Fmt>> : std::ranges::view_interface<replace_view<V, fmt_t<Fmt>>>
    {
        using match_result_type = std::ranges::range_value_t<V>;
        static constexpr int submatch_limit{ match_result_type::submatch_count };

        struct iterator;

    public:
        replace_view() requires std::default_initializable<V> = default;

        constexpr explicit replace_view(V base, fmt_t<Fmt> /* fmt */)
            : base_{ std::move(base) } {}

        [[nodiscard]] constexpr V base() const& requires std::copy_constructible<V> { return base_; }
        [[nodiscard]] constexpr V base() && { return std::move(base_); }

        [[nodiscard]] constexpr iterator begin()
        {
            return iterator{ std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr std::default_sentinel_t end()
        {
            return std::default_sentinel;
        }

    private:
        using underlying_type = std::remove_cvref_t<decltype(std::declval<V>().base().base())>;
        static_assert(std::same_as<std::ranges::range_value_t<underlying_type>, typename decltype(Fmt)::value_type>);

        static constexpr detail::static_replace_fmt fmt{ Fmt.view() };
        static constexpr std::size_t max_index{ fmt.subranges().size() + fmt.captures().size() };
        static constexpr std::size_t suffix_index{ -1uz };

        using char_type = std::ranges::range_value_t<underlying_type>;
        using base_subrange = std::ranges::subrange<std::ranges::iterator_t<underlying_type>>;
        using fmt_subrange = std::ranges::subrange<std::ranges::iterator_t<typename decltype(fmt)::subrange_type>>;
        using next_type = std::variant<base_subrange, fmt_subrange>;

        static constexpr std::size_t base_subrange_index{ 0 };
        static constexpr std::size_t fmt_subrange_index{ 1 };

        V base_{};
    };

    template<std::ranges::input_range V, string_literal Fmt>
        requires std::ranges::view<V> and detail::static_regex_match_view_like<V>
    struct replace_view<V, fmt_t<Fmt>>::iterator
    {
    public:
        using iterator_concept  = std::input_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using value_type        = std::ranges::range_value_t<underlying_type>;
        using difference_type   = std::ranges::range_difference_t<V>;

        iterator() requires std::default_initializable<std::ranges::iterator_t<V>>
                            and std::default_initializable<std::ranges::sentinel_t<V>>
                            and std::default_initializable<std::ranges::iterator_t<underlying_type>> = default;

        constexpr iterator(std::ranges::iterator_t<V> current, std::ranges::sentinel_t<V> end)
            : current_{ std::move(current) }, end_{ std::move(end) }
            , subrange_{ std::in_place_index<base_subrange_index>, current_.base(), get<0>(*current_).begin() }
        {
            if (subrange_.visit(detail::overloads([](const auto& sub) { return sub.empty(); })))
                find_next();
        }

        constexpr const std::ranges::iterator_t<V>& base() const& noexcept
        {
            return current_;
        }

        constexpr std::ranges::iterator_t<V>& base() &&
        {
            return std::move(current_);
        }

        constexpr value_type operator*() const
        {
            return subrange_.visit(detail::overloads([](const auto& sub) { return *sub.begin(); }));
        }

        constexpr iterator& operator++()
        {
            if (subrange_.visit(detail::overloads([](auto& sub) { sub.advance(1); return sub.empty(); })))
                find_next();

            return *this;
        }

        constexpr void operator++(int)
        {
            ++*this;
        }

        friend constexpr bool operator==(const iterator& x, std::default_sentinel_t)
        {
            return x.subrange_.visit(detail::overloads([](const auto& sub) { return sub.empty(); }));
        }

    private:
        constexpr void find_next()
        {
            while (current_ != end_)
            {
                ++index_;

                if (index_ == max_index)
                {
                    index_ = suffix_index;
                    ++current_;

                    if (current_ == end_)
                    {
                        subrange_.template emplace<base_subrange_index>(current_.base(), current_.end());
                        return;
                    }

                    const auto& subrange_ref = subrange_.template emplace<base_subrange_index>(current_.base(), get<0>(*current_).begin());

                    if (not subrange_ref.empty())
                        return;

                    ++index_;
                }

                template for (constexpr std::size_t I : std::views::iota(0uz, max_index))
                {
                    if (index_ == I)
                    {
                        if constexpr (I % 2 == 0)
                        {
                            static constexpr fmt_subrange format{ fmt.subranges()[I / 2] };

                            if constexpr (not format.empty())
                            {
                                subrange_.template emplace<fmt_subrange_index>(format);
                                return;
                            }
                        }
                        else
                        {
                            submatch capture{ get<fmt.captures()[I / 2]>(*current_) };

                            if (not capture.empty())
                            {
                                subrange_.template emplace<base_subrange_index>(capture.begin(), capture.end());
                                return;
                            }
                        }
                    }
                }
            }
        }

        std::ranges::iterator_t<V> current_{};
        [[no_unique_address]] std::ranges::sentinel_t<V> end_{};
        next_type subrange_{};
        std::size_t index_{ suffix_index };
    };

    template<typename R, string_literal Fmt>
    replace_view(R&&, fmt_t<Fmt>) -> replace_view<std::views::all_t<R>, fmt_t<Fmt>>;


    template<std::ranges::input_range V, std::ranges::bidirectional_range Fmt>
        requires std::ranges::view<V> and detail::static_regex_match_view_like<V> and std::ranges::view<Fmt>
    class replace_view<V, Fmt> : std::ranges::view_interface<replace_view<V, Fmt>>
    {
        using match_result_type = std::ranges::range_value_t<V>;
        static constexpr int submatch_limit{ match_result_type::submatch_count };

        struct iterator;

    public:
        replace_view() requires std::default_initializable<V> and std::default_initializable<Fmt> = default;

        constexpr explicit replace_view(V base, Fmt fmt)
            : base_{ std::move(base) }, format_input_{ std::move(fmt) }
            , fmt_{ std::ranges::begin(format_input_), std::ranges::end(format_input_) }
            , max_index_{ fmt_.captures().size() + fmt_.subranges().size() }
        {
            fmt_.range_check(submatch_limit);
        }

        [[nodiscard]] constexpr V base() const& requires std::copy_constructible<V> { return base_; }
        [[nodiscard]] constexpr V base() && { return std::move(base_); }

        [[nodiscard]] constexpr iterator begin()
        {
            return iterator{ *this, std::ranges::begin(base_), std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr std::default_sentinel_t end()
        {
            return std::default_sentinel;
        }

    private:
        using underlying_type = std::remove_cvref_t<decltype(std::declval<V>().base().base())>;
        static_assert(std::same_as<std::ranges::range_value_t<underlying_type>, std::ranges::range_value_t<Fmt>>);

        using format_type = detail::replace_fmt<std::ranges::iterator_t<Fmt>>;
        static constexpr std::size_t suffix_index{ -1uz };

        using char_type = std::ranges::range_value_t<underlying_type>;
        using base_subrange = std::ranges::subrange<std::ranges::iterator_t<underlying_type>>;
        using fmt_subrange = format_type::subrange_type;
        using next_type = std::variant<base_subrange, fmt_subrange>;

        static constexpr std::size_t base_subrange_index{ 0 };
        static constexpr std::size_t fmt_subrange_index{ 1 };

        V base_{};
        Fmt format_input_{};
        format_type fmt_{};
        std::size_t max_index_{ 0 };
    };

    template<std::ranges::input_range V, std::ranges::bidirectional_range Fmt>
        requires std::ranges::view<V> and detail::static_regex_match_view_like<V> and std::ranges::view<Fmt>
    struct replace_view<V, Fmt>::iterator
    {
    public:
        using iterator_concept  = std::input_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using value_type        = std::ranges::range_value_t<underlying_type>;
        using difference_type   = std::ranges::range_difference_t<V>;

        iterator() requires std::default_initializable<std::ranges::iterator_t<V>>
                            and std::default_initializable<std::ranges::sentinel_t<V>>
                            and std::default_initializable<std::ranges::iterator_t<underlying_type>> = default;

        constexpr iterator(replace_view& parent, std::ranges::iterator_t<V> current, std::ranges::sentinel_t<V> end)
            : parent_{ std::addressof(parent) }, current_{ std::move(current) }, end_{ std::move(end) }
            , subrange_{ std::in_place_index<base_subrange_index>, current_.base(), get<0>(*current_).begin() }
        {
            if (subrange_.visit(detail::overloads([](const auto& sub) { return sub.empty(); })))
                find_next();
        }

        constexpr const std::ranges::iterator_t<V>& base() const& noexcept
        {
            return current_;
        }

        constexpr std::ranges::iterator_t<V>& base() &&
        {
            return std::move(current_);
        }

        constexpr value_type operator*() const
        {
            return subrange_.visit(detail::overloads([](const auto& sub) { return *sub.begin(); }));
        }

        constexpr iterator& operator++()
        {
            if (subrange_.visit(detail::overloads([](auto& sub) { sub.advance(1); return sub.empty(); })))
                find_next();

            return *this;
        }

        constexpr void operator++(int)
        {
            ++*this;
        }

        friend constexpr bool operator==(const iterator& x, std::default_sentinel_t)
        {
            return x.subrange_.visit(detail::overloads([](const auto& sub) { return sub.empty(); }));
        }

    private:
        constexpr void find_next()
        {
            while (current_ != end_)
            {
                ++index_;

                if (index_ == parent_->max_index_)
                {
                    index_ = suffix_index;
                    ++current_;

                    if (current_ == end_)
                    {
                        subrange_.template emplace<base_subrange_index>(current_.base(), current_.end());
                        return;
                    }

                    const auto& subrange_ref = subrange_.template emplace<base_subrange_index>(current_.base(), get<0>(*current_).begin());

                    if (not subrange_ref.empty())
                        return;

                    ++index_;
                }

                if (index_ % 2 == 0)
                {
                    const auto& format = parent_->fmt_.subranges().at(index_ / 2);

                    if (not format.empty())
                    {
                        subrange_.template emplace<fmt_subrange_index>(format);
                        return;
                    }
                }
                else
                {
                    const auto match_index = parent_->fmt_.captures().at(index_ / 2);
                    submatch capture{ (*current_).at(match_index) };

                    if (not capture.empty())
                    {
                        subrange_.template emplace<base_subrange_index>(capture.begin(), capture.end());
                        return;
                    }
                }
            }
        }

        replace_view* parent_{ nullptr };
        std::ranges::iterator_t<V> current_{};
        [[no_unique_address]] std::ranges::sentinel_t<V> end_{};
        next_type subrange_{};
        std::size_t index_{ suffix_index };
    };


    template<typename R, std::ranges::bidirectional_range Fmt>
    replace_view(R&&, Fmt&&) -> replace_view<std::views::all_t<R>, std::views::all_t<Fmt>>;


    template<std::ranges::bidirectional_range V, typename Regex>
        requires std::ranges::view<V>
    class regex_split_view
    {
        static_assert("regex_split_view: invalid regex");
    };


    template<std::ranges::bidirectional_range V, string_literal Pattern, mode Mode>
        requires std::ranges::view<V>
    class regex_split_view<V, static_regex<Pattern, Mode>> : std::ranges::view_interface<regex_split_view<V, static_regex<Pattern, Mode>>>
    {
        class iterator;
        class sentinel;

    public:
        regex_split_view() requires std::default_initializable<V> = default;
        constexpr explicit regex_split_view(V base, static_regex<Pattern, Mode> /* regex */) : base_{ std::move(base) } {}

        [[nodiscard]] V base() const& requires std::copy_constructible<V> { return base_; }
        [[nodiscard]] V base() && requires std::copy_constructible<V> { return std::move(base_); }

        [[nodiscard]] constexpr iterator begin()
        {
            if (not cache_engaged_)
            {
                const auto beg = std::ranges::begin(base_);
                const auto end = std::ranges::end(base_);
                auto result = matcher(beg, end, detail::match_non_empty);

                if (result.has_value())
                {
                    if constexpr (result_type::has_continue)
                        cached_begin_continue_at_ = result.continue_at_;
                    auto [mfirst, mlast] = result.template force_get<0>();
                    cached_begin_next_ = { std::move(mfirst), std::move(mlast) };
                }
                else
                {
                    if constexpr (std::ranges::common_range<V>)
                    {
                        cached_begin_next_ = { end, end };
                    }
                    else
                    {
                        const auto it = std::ranges::next(beg, end);
                        cached_begin_next_ = { it, it };
                    }
                }

                cache_engaged_ = true;
            }

            if constexpr (result_type::has_continue)
                return iterator{ *this, std::ranges::begin(base_), cached_begin_next_, cached_begin_continue_at_ };
            else
                return iterator{ *this, std::ranges::begin(base_), cached_begin_next_ };
        }

        [[nodiscard]] constexpr sentinel end()
        {
            return sentinel{ std::ranges::end(base_) };
        }

        [[nodiscard]] constexpr iterator end()
            requires std::ranges::common_range<V>
        {
            return iterator{ *this, std::ranges::end(base_), {} };
        }

        /* workaround to avoid implementing non-propagating-cache<T> */
        constexpr regex_split_view(const regex_split_view& other) : base_{ other.base_ } {}
        constexpr regex_split_view(regex_split_view&& other) noexcept : base_{ std::move(other).base_ } {}
        constexpr regex_split_view& operator=(const regex_split_view& other) { base_ = other; }
        constexpr regex_split_view& operator=(regex_split_view&& other) noexcept { base_ = std::move(other).base_; cache_engaged_ = false; }
        constexpr ~regex_split_view() = default;

    private:
        using matcher_type   = [: detail::get_matcher_refl(Mode, true) :]<Pattern, detail::default_fsm_flags::search_all>;
        using result_type    = matcher_type::template result<std::ranges::iterator_t<V>>;
        using next_type      = std::ranges::subrange<std::ranges::iterator_t<V>>;
        using continue_type  = detail::tdfa::continue_at_t;
        using maybe_continue = detail::maybe_type_t<result_type::has_continue, continue_type>;

        static constexpr matcher_type matcher{};

        V base_{};
        next_type cached_begin_next_;
        [[no_unique_address]] maybe_continue cached_begin_continue_at_{ 0 };
        bool cache_engaged_{ false };
    };

    template<std::ranges::bidirectional_range V, string_literal Pattern, mode Mode>
        requires std::ranges::view<V>
    class regex_split_view<V, static_regex<Pattern, Mode>>::iterator
    {
    public:
        using iterator_concept  = std::forward_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using value_type        = std::ranges::subrange<std::ranges::iterator_t<V>>;
        using difference_type   = std::ranges::range_difference_t<V>;

        iterator() requires std::default_initializable<std::ranges::iterator_t<V>> = default;

        explicit constexpr iterator(regex_split_view& parent, std::ranges::iterator_t<V> current, next_type next)
            : parent_{ std::addressof(parent) }, current_{ std::move(current) }, next_{ std::move(next) } {}

        explicit constexpr iterator(regex_split_view& parent, std::ranges::iterator_t<V> current, next_type next, continue_type cont)
            requires result_type::has_continue
            : parent_{ std::addressof(parent) }, current_{ std::move(current) }, next_{ std::move(next) }, continue_at_{ cont } {}

        constexpr std::ranges::iterator_t<V>& base() const
        {
            return current_;
        }

        constexpr value_type operator*() const
        {
            return { current_, next_.begin() };
        }

        constexpr iterator& operator++()
        {
            current_ = next_.begin();

            if (const auto end = std::ranges::end(parent_->base_); current_ != end)
            {
                current_ = next_.end();

                if (current_ == end)
                {
                    trailing_empty_ = true;
                    next_ = { current_, current_ };
                }
                else
                {
                    const auto result = [&]() {
                        if constexpr (not matcher_type::never_empty)
                        {
                            if (next_.begin() == next_.end())
                            {
                                if constexpr (result_type::continue_from_it)
                                    return matcher(std::ranges::begin(parent_->base_), end, current_, detail::match_non_empty);
                                else if constexpr (result_type::has_continue)
                                    return matcher(current_, end, continue_at_, detail::match_non_empty);
                                else
                                    return matcher(current_, end, detail::match_non_empty);
                            }
                        }

                        if constexpr (result_type::continue_from_it)
                            return matcher(std::ranges::begin(parent_->base_), end, current_);
                        else if constexpr (result_type::has_continue)
                            return matcher(current_, end, continue_at_);
                        else
                            return matcher(current_, end);
                    }();

                    if (result.has_value())
                    {
                        if constexpr (result_type::has_continue)
                            continue_at_ = result.continue_at_;
                        auto [mfirst, mlast] = result.template force_get<0>();
                        next_ = { std::move(mfirst), std::move(mlast) };
                    }
                    else
                    {
                        if constexpr (std::ranges::common_range<V>)
                        {
                            next_ = { end, end };
                        }
                        else
                        {
                            const auto it = std::ranges::next(current_, end);
                            next_ = { it, it };
                        }
                    }
                }
            }
            else
            {
                trailing_empty_ = false;
            }

            return *this;
        }

        constexpr iterator operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        friend constexpr bool operator==(const iterator& x, const iterator& y)
            requires std::equality_comparable<std::ranges::iterator_t<V>>
        {
            return x.current_ == y.current_ and x.trailing_empty_ == y.trailing_empty_;
        }

        friend class sentinel;

    private:
        regex_split_view* parent_{ nullptr };
        std::ranges::iterator_t<V> current_{};
        next_type next_{};
        [[no_unique_address]] maybe_continue continue_at_{ 0 };
        bool trailing_empty_{ false };
    };

    template<std::ranges::bidirectional_range V, string_literal Pattern, mode Mode>
        requires std::ranges::view<V>
    class regex_split_view<V, static_regex<Pattern, Mode>>::sentinel
    {
    public:
        sentinel() = default;

        constexpr explicit sentinel(std::ranges::sentinel_t<V> end)
            : end_{ end } {}

        friend constexpr bool operator==(const iterator& x, const sentinel& y)
        {
            return x.current_ == y.end_ and not x.trailing_empty_;
        }

    private:
        [[no_unique_address]] std::ranges::sentinel_t<V> end_{};
    };

    template<typename R, string_literal Pattern, mode Mode>
    regex_split_view(R&&, static_regex<Pattern, Mode>) -> regex_split_view<std::views::all_t<R>, static_regex<Pattern, Mode>>;


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

            template<typename Range, typename T>
            concept can_replace_view = requires
            {
                replace_view(std::declval<Range>(), std::declval<T>());
            };

            template<typename Range, typename T>
            concept can_regex_split_view = requires
            {
                regex_split_view(std::declval<Range>(), std::declval<T>());
            };

            template<typename Regex>
            struct static_regex_match_adaptor_closure : std::ranges::range_adaptor_closure<static_regex_match_adaptor_closure<Regex>>
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
                    return static_regex_match_adaptor_closure<Regex>();
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

                constexpr explicit dynamic_submatches_adaptor_closure(T submatches)
                    : sub_{ std::forward<T>(submatches) } {}

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

            template<string_literal Fmt>
            struct static_replace_adaptor_closure : std::ranges::range_adaptor_closure<static_replace_adaptor_closure<Fmt>>
            {
                template<std::ranges::viewable_range Range>
                    requires detail::can_replace_view<Range, fmt_t<Fmt>>
                [[nodiscard]] constexpr auto operator()(Range&& r) const
                {
                    return replace_view(std::forward<Range>(r), fmt<Fmt>);
                }
            };

            template<typename T>
            struct dynamic_replace_adaptor_closure : std::ranges::range_adaptor_closure<dynamic_replace_adaptor_closure<T>>
            {
                template<std::ranges::viewable_range Range>
                    requires detail::can_replace_view<Range, std::views::all_t<T>>
                [[nodiscard]] constexpr auto operator()(Range&& r) const
                {
                    return replace_view(std::forward<Range>(r), fmt_);
                }

                constexpr explicit dynamic_replace_adaptor_closure(T submatches)
                    : fmt_{ std::forward<T>(submatches) } {}

            private:
                std::views::all_t<T> fmt_;
            };

            struct replace_adaptor
            {
                template<std::ranges::viewable_range Range, typename Fmt>
                    requires detail::can_replace_view<Range, Fmt>
                [[nodiscard]] constexpr auto operator()(Range&& r, Fmt&& fmt) const
                {
                    return replace_view(std::forward<Range>(r), std::forward<Fmt>(fmt));
                }

                template<std::ranges::viewable_range Range, typename CharT>
                    requires detail::can_replace_view<Range, std::ranges::subrange<const CharT*, rx::detail::cstr_sentinel_t>>
                [[nodiscard]] constexpr auto operator()(Range&& r, const CharT* fmtstr) const
                {
                    return replace_view(std::forward<Range>(r), std::ranges::subrange(fmtstr, rx::detail::cstr_sentinel));
                }

                template<typename Fmt>
                [[nodiscard]] constexpr auto operator()(Fmt&& fmt) const
                {
                    return dynamic_replace_adaptor_closure{ std::forward<Fmt>(fmt) };
                }

                template<typename CharT>
                [[nodiscard]] constexpr auto operator()(const CharT* fmtstr) const
                {
                    return dynamic_replace_adaptor_closure{ std::ranges::subrange(fmtstr, rx::detail::cstr_sentinel) };
                }

                template<rx::string_literal Fmt>
                [[nodiscard]] consteval auto operator()(fmt_t<Fmt>) const
                {
                    return static_replace_adaptor_closure<Fmt>();
                }
            };

            template<typename Regex>
            struct static_regex_split_adaptor_closure : std::ranges::range_adaptor_closure<static_regex_split_adaptor_closure<Regex>>
            {
                template<std::ranges::viewable_range Range>
                    requires detail::can_regex_split_view<Range, Regex>
                [[nodiscard]] constexpr auto operator()(Range&& r) const
                {
                    return regex_split_view(std::forward<Range>(r), Regex{});
                }
            };

            struct regex_split_adaptor
            {
                template<std::ranges::viewable_range Range, typename Regex>
                    requires detail::can_regex_split_view<Range, Regex>
                [[nodiscard]] constexpr auto operator()(Range&& r, Regex&& x) const
                {
                    return regex_split_view(std::forward<Range>(r), std::forward<Regex>(x));
                }

                template<typename Regex>
                    requires rx::detail::static_regex_like<Regex>
                [[nodiscard]] consteval auto operator()(Regex /* x */) const
                {
                    return static_regex_split_adaptor_closure<Regex>();
                }
            };
        }

        inline constexpr detail::regex_match_adaptor regex_match;
        inline constexpr detail::submatches_adaptor submatches;
        inline constexpr detail::replace_adaptor replace;
        inline constexpr detail::regex_split_adaptor regex_split;

        template<string_literal Pattern, mode Mode = mode::standard>
        inline constexpr detail::static_regex_match_adaptor_closure<static_regex<Pattern, Mode>> static_regex_match;

        template<int... Submatches> requires (sizeof...(Submatches) > 0)
        inline constexpr detail::static_submatches_adaptor_closure<Submatches...> static_submatches;

        template<string_literal Fmt>
        inline constexpr detail::static_replace_adaptor_closure<Fmt> static_replace;

        template<string_literal Pattern, mode Mode = mode::standard>
        inline constexpr detail::static_regex_split_adaptor_closure<static_regex<Pattern, Mode>> static_regex_split;
    }


    template<string_literal Pattern, mode Mode>
    template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, typename static_regex<Pattern, Mode>::char_type>
    constexpr auto static_regex<Pattern, Mode>::range(R&& r)
    {
        return views::regex_match(std::forward<R>(r), static_regex<Pattern, Mode>{});
    }
}

