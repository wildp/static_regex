// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

#include "rx/api/submatch.hpp"
#include "rx/etc/string_literal.hpp"
#include "rx/etc/util.hpp"
#include "rx/fsm/flags.hpp"
#include "rx/gen/compile.hpp"


namespace rx::detail
{
    template<string_literal Pattern, fsm_flags Flags>
    struct p1306_matcher;
}

namespace rx
{
    template<std::bidirectional_iterator I, string_literal Pattern, detail::fsm_flags Flags>
    requires std::default_initializable<I> and std::copyable<I> 
    class static_regex_match_result
    {
        using factory = detail::submatch_factory<I>;
        using dfa_t   = detail::compiled_dfa<Pattern, Flags>;

        template<bool Const>
        class proxy_iterator;

    public:
        using size_type              = std::size_t;
        using char_type              = std::remove_cv_t<std::iter_value_t<I>>;
        using submatch_type          = submatch<I>;
        using iterator               = proxy_iterator<false>;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_iterator         = proxy_iterator<true>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        static constexpr size_type submatch_count{ dfa_t::value.captures.capture_count() };

        constexpr static_regex_match_result() noexcept
        {
            if constexpr (has_registers and not has_enabled)
                reg_.fill(I{});
            if constexpr (has_enabled)
                enabled_.fill(false);
        }

        /* observers */

        [[nodiscard]] constexpr bool has_value() const
        {
            if constexpr (std::contiguous_iterator<I>)
                return std::to_address(match_end_) != std::to_address(I{});
            else
                return match_success_;
        }

        [[nodiscard]] constexpr explicit(false) operator bool() const
        {
            return this->has_value();
        }
        
        [[nodiscard]] constexpr size_type size() const
        {
            return (this->has_value()) ? submatch_count : 0;
        }

        /* array-like access */

        [[nodiscard]] constexpr submatch_type operator[](size_type n) const noexcept
        {
            template for (constexpr size_type N : std::views::iota(0uz, submatch_count))
            {
                if (n == N)
                    return this->get<N>();
            }
            std::unreachable();
        }

        [[nodiscard]] constexpr submatch_type at(size_type i) const
        {
            this->range_check(i);
            return this->operator[](i);
        }

        /* iterator support */

        [[nodiscard]] constexpr iterator begin() const
        {
            return this->has_value()
                   ? iterator{ this, 0 }
                   : this->end();
        }

        [[nodiscard]] constexpr iterator end() const
        {
            return { this, this->size() };
        }

        [[nodiscard]] constexpr reverse_iterator rbegin() const
        {
            return std::make_reverse_iterator(this->end());
        }

        [[nodiscard]] constexpr reverse_iterator rend() const
        {
            return std::make_reverse_iterator(this->begin());
        }

        [[nodiscard]] constexpr const_iterator cbegin() const
        {
            return this->has_value()
                   ? const_iterator{ this, 0 }
                   : this->end();
        }

        [[nodiscard]] constexpr const_iterator cend() const
        {
            return { this, this->size() };
        }

        [[nodiscard]] constexpr const_reverse_iterator crbegin() const
        {
            return std::make_reverse_iterator(this->cend());
        }

        [[nodiscard]] constexpr const_reverse_iterator crend() const
        {
            return std::make_reverse_iterator(this->cbegin());
        }

        /* tuple support */

        template<size_type N>
        requires (N < submatch_count)
        [[nodiscard]] constexpr submatch_type get() const noexcept
        {
            using namespace detail;

            static constexpr auto current{ dfa_t::value.captures.captures[N] };

            if (not this->has_value())
                return {};

            if constexpr (current.first.tag_number == end_of_input_tag and current.second.tag_number == start_of_input_tag)
            {
                // TODO: make multi tag
                throw std::logic_error("Branch reset not implemented");
            }
            else
            {
                if constexpr (current.first.tag_number == current.second.tag_number)
                {
                    if (this->tag_enabled<current.first.tag_number>())
                    {
                        return factory::make_submatch(
                            std::next(this->get_tag<current.first.tag_number>(), current.first.offset),
                            std::next(this->get_tag<current.second.tag_number>(), current.second.offset)
                        );
                    }
                }
                else
                {
                    if (this->tag_enabled<current.first.tag_number>() and this->tag_enabled<current.second.tag_number>())
                    {
                        return factory::make_submatch(
                            std::next(this->get_tag<current.first.tag_number>(), current.first.offset),
                            std::next(this->get_tag<current.second.tag_number>(), current.second.offset)
                        );
                    }
                }
            }

            return {};
        }

        friend struct detail::p1306_matcher<Pattern, Flags>;

        template<std::ranges::bidirectional_range V, typename Regex>
        requires std::ranges::view<V>
        friend class regex_match_view;

    private:
        /* implementation helpers */

        static constexpr bool has_registers{ dfa_t::value.register_count != 0 };
        static constexpr bool has_success{ not std::contiguous_iterator<I> };
        static constexpr bool has_enabled{ has_registers and has_success };
        static constexpr bool has_match_start{ dfa_t::value.captures.has_match_start() };

        explicit constexpr static_regex_match_result(I start)
            : match_start_{ start }
        {
            if constexpr (has_registers and not has_enabled)
                reg_.fill(I{});
            if constexpr (has_enabled)
                enabled_.fill(false);
        }

        template<detail::tag_number_t N>
        [[nodiscard]] constexpr bool tag_enabled() const
        {
            if constexpr (N == detail::start_of_input_tag or N == detail::end_of_input_tag)
                return true;
            else if constexpr (std::contiguous_iterator<I>)
                return std::to_address(reg_[dfa_t::value.final_registers[N]]) != std::to_address(I{});
            else
                return enabled_[dfa_t::value.final_registers[N]];
        }

        template<detail::tag_number_t N>
        [[nodiscard]] constexpr I get_tag() const
        {
            if constexpr (N == detail::start_of_input_tag)
                return match_start_;
            else if constexpr (N == detail::end_of_input_tag)
                return match_end_;
            else
                return reg_[dfa_t::value.final_registers[N]];
        }

        constexpr void range_check(size_type n) const
        {
            if (n >= this->size())
                throw std::out_of_range("static_regex_match_result::range_check: n >= this->size()");
        }


        /* data members and protected trivial accessors */

        using registers_type   = detail::maybe_type_t<has_registers, std::array<I, dfa_t::value.register_count>>;
        using enabled_type     = detail::maybe_type_t<has_enabled, std::array<bool, dfa_t::value.register_count>>;
        using match_start_type = detail::maybe_type_t<has_match_start, I>;
        using continue_type    = detail::maybe_type_t<Flags.is_iterator, detail::tdfa::continue_at_t>;
        using success_type     = detail::maybe_type_t<has_success, bool>;

        [[no_unique_address]] registers_type reg_{};
        [[no_unique_address]] match_start_type match_start_{};
        I match_end_{};
        [[no_unique_address]] enabled_type enabled_{};
        [[no_unique_address]] continue_type continue_at_{ detail::tdfa::no_continue };
        [[no_unique_address]] success_type match_success_{ false };
    };


    /* iterator implementation */

    template<std::bidirectional_iterator I, string_literal Pattern, detail::fsm_flags Flags>
    requires std::default_initializable<I> and std::copyable<I>
    template<bool Const>
    class static_regex_match_result<I, Pattern, Flags>::proxy_iterator
    {
    public:
        using iterator_concept  = std::random_access_iterator_tag;
        using iterator_category = std::input_iterator_tag;
#if __cpp_lib_ranges_as_const >= 202311L
        using value_type        = submatch<std::conditional_t<Const, std::const_iterator<I>, I>>;
#else
        using value_type        = submatch<I>; /* semantically incorrect workaround */
#endif
        using difference_type   = std::ptrdiff_t;

        proxy_iterator() = default;

        constexpr proxy_iterator(const static_regex_match_result* ptr, std::size_t pos)
            : ptr_{ ptr }, pos_{ pos } {}

        constexpr explicit(false) proxy_iterator(proxy_iterator<not Const> i) requires Const
            : ptr_{ i.ptr_ }, pos_{ i.pos_ } {}

        constexpr value_type operator*() const
        {
            return (*ptr_)[pos_];
        }

        constexpr value_type operator[](difference_type n) const
        {
            return (*ptr_)[pos_ + n];
        }

        constexpr proxy_iterator& operator++()
        {
            ++pos_;
            return *this;
        }

        constexpr proxy_iterator operator++(int)
        {
            auto tmp{ *this };
            ++*this;
            return tmp;
        }

        constexpr proxy_iterator& operator--()
        {
            --pos_;
            return *this;
        }

        constexpr proxy_iterator operator--(int)
        {
            auto tmp{ *this };
            --*this;
            return tmp;
        }

        constexpr proxy_iterator& operator+=(difference_type n)
        {
            pos_ += n;
            return *this;
        }

        constexpr proxy_iterator& operator-=(difference_type n)
        {
            pos_ -= n;
            return *this;
        }

        friend constexpr bool operator==(const proxy_iterator&, const proxy_iterator&) = default;

        friend constexpr auto operator<=>(const proxy_iterator&, const proxy_iterator&) = default;

        friend constexpr proxy_iterator operator+(const proxy_iterator& i, difference_type n)
        {
            return { i.ptr_, i.pos_ + n };
        }

        friend constexpr proxy_iterator operator+(difference_type n, const proxy_iterator& i)
        {
            return { i.ptr_, n + i.pos_ };
        }

        friend constexpr proxy_iterator operator-(const proxy_iterator& i, difference_type n)
        {
            return { i.ptr_, i.pos_ - n };
        }

        friend constexpr difference_type operator-(const proxy_iterator& x, const proxy_iterator& y)
        {
            return y.pos_ - x.pos_;
        }

    private:
        const static_regex_match_result* ptr_{ nullptr };
        std::size_t pos_{ 0 };
    };
}


/* structured binding support for static_regex_match_result */

template<std::bidirectional_iterator Iter, rx::string_literal Pattern, rx::detail::fsm_flags Flags>
struct std::tuple_size<rx::static_regex_match_result<Iter, Pattern, Flags>>
    : integral_constant<std::size_t, rx::static_regex_match_result<Iter, Pattern, Flags>::submatch_count> {};

template<std::size_t N, std::bidirectional_iterator Iter, rx::string_literal Pattern, rx::detail::fsm_flags Flags>
requires (N < rx::static_regex_match_result<Iter, Pattern, Flags>::submatch_count)
struct std::tuple_element<N, rx::static_regex_match_result<Iter, Pattern, Flags>>
{
    using type = rx::static_regex_match_result<Iter, Pattern, Flags>::submatch_type;
};
