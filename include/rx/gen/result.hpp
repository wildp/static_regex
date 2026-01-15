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

#include "../api/submatch.hpp"
#include "../etc/string_literal.hpp"
#include "../etc/util.hpp"
#include "../fsm/flags.hpp"
#include "../gen/compile.hpp"


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

        class proxy_iterator;

    public:
        using size_type              = std::size_t;
        using char_type              = std::remove_cv_t<std::iter_value_t<I>>;
        using submatch_type          = submatch<I>;
        using iterator               = proxy_iterator;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        // using const_iterator         = proxy_iterator;
        // using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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

        // /* TODO: Implement const iterators with correct semantics */
        // [[nodiscard]] constexpr const_iterator cbegin() const = delete;
        // [[nodiscard]] constexpr const_iterator cend() const = delete;
        // [[nodiscard]] constexpr const_reverse_iterator crbegin() const = delete;
        // [[nodiscard]] constexpr const_reverse_iterator crend() const = delete;

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

        template<string_literal, typename, typename>
        friend class static_regex_iterator;

    private:
        /* iterator implementation */

        class proxy_iterator
        {
            friend class static_regex_match_result;

            using it          = proxy_iterator;
            using parent_type = static_regex_match_result;

        public:
            using iterator_concept  = std::random_access_iterator_tag;
            using iterator_category = std::input_iterator_tag;
            using value_type        = submatch_type;
            using difference_type   = std::ptrdiff_t;

            proxy_iterator() = default;

            constexpr value_type operator*() const { return (*ptr_)[pos_]; }
            constexpr value_type operator[](difference_type n) const { return (*ptr_)[pos_ + n]; }

            constexpr it& operator++() { ++pos_; return *this; }
            constexpr it  operator++(int) { auto tmp{ *this }; ++(*this); return tmp; }
            constexpr it& operator--() { --pos_; return *this; }
            constexpr it  operator--(int) { auto tmp{ *this }; --(*this); return tmp; }
            constexpr it& operator+=(difference_type n) { pos_ += n; return *this; }
            constexpr it& operator-=(difference_type n) { pos_ -= n; return *this; }

            constexpr friend bool operator==(const it&, const it&) = default;
            constexpr friend auto operator<=>(const it&, const it&) = default;
            constexpr friend it operator+(const it& lhs, difference_type rhs) { return { lhs.ptr_, lhs.pos_ + rhs }; }
            constexpr friend it operator+(difference_type lhs, const it& rhs) { return { rhs.ptr_, lhs + rhs.pos_ }; }
            constexpr friend it operator-(const it& lhs, difference_type rhs) { return { lhs.ptr_, lhs.pos_ - rhs }; }
            constexpr friend difference_type operator-(const it& lhs, const it& rhs) { return rhs.pos_ - lhs.pos_; }

        private:
            constexpr proxy_iterator(const parent_type* ptr, std::size_t pos) : ptr_{ ptr }, pos_{ pos } {}

            const parent_type* ptr_{ nullptr };
            std::size_t pos_{ 0 };
        };


        /* implementation helpers */

        static constexpr bool has_registers{ dfa_t::value.register_count != 0 };
        static constexpr bool has_success{ not std::contiguous_iterator<I> };
        static constexpr bool has_enabled{ has_registers and has_success };
        static constexpr bool has_match_start{ dfa_t::value.captures.has_match_start() };

        explicit constexpr static_regex_match_result(I start)
            : match_start_{ detail::maybe_type_init<has_match_start>(start)}
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
        [[no_unique_address]] continue_type continue_at_{ detail::maybe_type_init<Flags.is_iterator>(detail::tdfa::no_continue) };
        [[no_unique_address]] success_type match_success_{ detail::maybe_type_init<has_success>(false) };
    };
}


/* structured binding support for compile_time_match_result */

template<std::bidirectional_iterator Iter, rx::string_literal Pattern, rx::detail::fsm_flags Flags>
struct std::tuple_size<rx::static_regex_match_result<Iter, Pattern, Flags>>
    : integral_constant<std::size_t, rx::static_regex_match_result<Iter, Pattern, Flags>::submatch_count> {};

template<std::size_t N, std::bidirectional_iterator Iter, rx::string_literal Pattern, rx::detail::fsm_flags Flags>
requires (N < rx::static_regex_match_result<Iter, Pattern, Flags>::submatch_count)
struct std::tuple_element<N, rx::static_regex_match_result<Iter, Pattern, Flags>>
{
    using type = rx::static_regex_match_result<Iter, Pattern, Flags>::submatch_type;
};
