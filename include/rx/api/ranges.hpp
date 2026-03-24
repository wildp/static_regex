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
            auto current{ std::ranges::begin(base_) };
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
                cached_result_ = matcher_(current, std::ranges::end(base_), detail::match_non_empty);
            else
                cached_result_ = matcher_(current, std::ranges::end(base_));
        }

        template<bool MatchNonEmpty = false>
        constexpr void find_next(std::ranges::iterator_t<V> current)
        {
            if constexpr (MatchNonEmpty)
            {
                if constexpr (Mode == mode::naive)
                    cached_result_ = matcher_(std::ranges::begin(base_), std::ranges::end(base_), current, detail::match_non_empty);
                else if constexpr (result_type::has_continue)
                    cached_result_ = matcher_(current, std::ranges::end(base_), cached_result_.continue_at_, detail::match_non_empty);
                else
                    cached_result_ = matcher_(current, std::ranges::end(base_), detail::match_non_empty);
            }
            else
            {

                if constexpr (Mode == mode::naive)
                    cached_result_ = matcher_(std::ranges::begin(base_), std::ranges::end(base_), current);
                else if constexpr (result_type::has_continue)
                    cached_result_ = matcher_(current, std::ranges::end(base_), cached_result_.continue_at_);
                else
                    cached_result_ = matcher_(current, std::ranges::end(base_));
            }
        }

        V base_{};
        result_type cached_result_;
        [[no_unique_address]] matcher_type matcher_;
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
            : current_{ std::move(current) }, parent_{ std::addressof(parent) } {}

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

            const auto& [mfirst, mlast]{ force_get<0>(parent_->cached_result_) };
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

    private:
        constexpr std::ranges::iterator_t<V> begin() const
        {
            return std::ranges::begin(parent_->base_);
        }

        constexpr std::ranges::sentinel_t<V> end() const
        {
            return std::ranges::end(parent_->base_);
        }

        std::ranges::iterator_t<V> current_{};
        regex_match_view* parent_{ nullptr };
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
            : base_{ std::move(base) }, dynamic_submatches_{ std::from_range, std::forward<R>(submatches) }
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
            return iterator{ *this };
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
                            and std::default_initializable<std::ranges::iterator_t<underlying_type>> = default;

        constexpr iterator(submatches_view& parent)
            : current_{ std::ranges::begin(parent.base_) }, parent_{ std::addressof(parent) }
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

            const auto submatch_index{ get_submatch(index_) };

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

        std::ranges::iterator_t<V> current_{};
        [[no_unique_address]] std::ranges::sentinel_t<V> end_{};
        std::size_t index_{ 0 };
        submatches_view* parent_{ nullptr };
    };

    template<typename R, int... Submatches>
    submatches_view(R&&, std::integer_sequence<int, Submatches...>) -> submatches_view<std::views::all_t<R>, Submatches...>;

    template<typename R, typename Submatches>
    submatches_view(R&&, Submatches&&) -> submatches_view<std::views::all_t<R>>;


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
                const auto beg{ std::ranges::begin(base_) };
                const auto end{ std::ranges::end(base_) };
                auto result{ matcher_(beg, end, detail::match_non_empty) };

                if (result.has_value())
                {
                    if constexpr (result_type::has_continue)
                        cached_begin_continue_at_ = result.continue_at_;
                    auto [mfirst, mlast]{ force_get<0>(result) };
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
                        const auto it{ std::ranges::next(beg, end) };
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

        V base_{};
        [[no_unique_address]] matcher_type matcher_;
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
            : current_{ std::move(current) }, parent_{ std::addressof(parent) }, next_{ std::move(next) } {}

        explicit constexpr iterator(regex_split_view& parent, std::ranges::iterator_t<V> current, next_type next, continue_type cont)
        requires result_type::has_continue
            : current_{ std::move(current) }, parent_{ std::addressof(parent) }, next_{ std::move(next) }, continue_at_{ cont } {}

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

            if (const auto end{ std::ranges::end(parent_->base_) }; current_ != end)
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
                                if constexpr (Mode == mode::naive)
                                    return parent_->matcher_(std::ranges::begin(parent_->base_), end, current_, detail::match_non_empty);
                                else if constexpr (result_type::has_continue)
                                    return parent_->matcher_(current_, end, continue_at_, detail::match_non_empty);
                                else
                                    return parent_->matcher_(current_, end, detail::match_non_empty);
                            }
                        }

                        if constexpr (Mode == mode::naive)
                            return parent_->matcher_(std::ranges::begin(parent_->base_), end, current_);
                        else if constexpr (result_type::has_continue)
                            return parent_->matcher_(current_, end, continue_at_);
                        else
                            return parent_->matcher_(current_, end);
                    }();

                    if (result.has_value())
                    {
                        if constexpr (result_type::has_continue)
                            continue_at_ = result.continue_at_;
                        auto [mfirst, mlast]{ force_get<0>(result) };
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
                            const auto it{ std::ranges::next(current_, end) };
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
            auto tmp{ *this };
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
        std::ranges::iterator_t<V> current_{};
        regex_split_view* parent_{ nullptr };
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
        inline constexpr detail::regex_split_adaptor regex_split;

        template<int... Submatches> requires (sizeof...(Submatches) > 0)
        inline constexpr detail::static_submatches_adaptor_closure<Submatches...> static_submatches;
    }


    template<string_literal Pattern, mode Mode>
    template<std::ranges::bidirectional_range R>
    requires std::same_as<std::ranges::range_value_t<R>, typename static_regex<Pattern, Mode>::char_type>
    constexpr auto static_regex<Pattern, Mode>::range(R&& r)
    {
        return views::regex_match(std::forward<R>(r), static_regex<Pattern, Mode>{});
    }
}

