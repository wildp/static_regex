// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>
#include <cstddef>
#include <flat_map>
#include <functional>
#include <iterator>
#include <meta>
#include <ranges>
#include <span>
#include <stdexcept>
#include <type_traits>


namespace rx::detail
{
    struct non_owning_tag_t {};
    inline constexpr non_owning_tag_t non_owning_tag;

    template<typename T>
    class static_span
    {
    public:
        /* types */
        using element_type           = std::add_const_t<T>;
        using value_type             = std::remove_cv_t<element_type>;
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;
        using pointer                = element_type*;
        using const_pointer          = element_type*;
        using reference              = element_type&;
        using const_reference        = const element_type&;
        using const_iterator         = const_pointer;
        using iterator               = const_iterator;
        using const_reverse_iterator = std::reverse_iterator<iterator>;
        using reverse_iterator       = const_reverse_iterator;

        /* constructors and assignment */
        consteval static_span() noexcept = default;

        template<std::size_t N = std::dynamic_extent>
        consteval static_span(std::span<element_type, N> span) noexcept
            : data_{ span.data() }, size_{ span.size() } {}

        template<typename R>
            requires (std::same_as<std::remove_cvref_t<std::ranges::range_value_t<R>>, value_type> and not std::same_as<R, static_span>)
        consteval static_span(R&& r) noexcept
            : static_span(std::define_static_array(std::forward<R>(r))) {}

        template<std::contiguous_iterator I, std::sentinel_for<I> S>
        constexpr static_span(non_owning_tag_t, I first, S last)
            : data_{ first }, size_{ static_cast<std::size_t>(std::ranges::distance(first, last)) } {}

        /* size observers */
        [[nodiscard]] constexpr size_type size() const noexcept { return size_; }
        [[nodiscard]] constexpr size_type size_bytes() const noexcept { return this->size() * sizeof(element_type); }
        [[nodiscard]] constexpr bool empty() const noexcept { return this->size() == 0; }

        /* element access */
        [[nodiscard]] constexpr const_reference operator[](size_type idx) const { return data_[idx]; }
        [[nodiscard]] constexpr const_reference at(size_type idx) const { this->range_check(idx); return this->operator[](idx); }
        [[nodiscard]] constexpr const_reference front() const { return data_[0]; }
        [[nodiscard]] constexpr const_reference back() const { return data_[this->size() - 1]; }
        [[nodiscard]] constexpr const_pointer data() const noexcept { return data_; }

        /* iterators */
        [[nodiscard]] constexpr const_iterator begin() const noexcept { return data_; }
        [[nodiscard]] constexpr const_iterator end() const noexcept { return data_ + this->size(); }
        [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return this->begin(); }
        [[nodiscard]] constexpr const_iterator cend() const noexcept { return this->end(); }
        [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return std::make_reverse_iterator(this->begin()); }
        [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return std::make_reverse_iterator(this->end()); }
        [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return this->rbegin(); }
        [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return this->rend(); }

    private:
        constexpr void range_check(size_type n) const
        {
            if (n >= this->size())
                throw std::out_of_range("static_span::range_check: n >= this->size()");
        }

    public:
        /* data members (public so that static_span is structural) */
        pointer   data_{ nullptr };
        size_type size_{ 0 };
    };

    template<class R>
    static_span(R&&) -> static_span<std::remove_reference_t<std::ranges::range_reference_t<R>>>;


    template<typename Key, typename T, typename Compare = std::less<std::remove_cv_t<Key>>>
    class static_map
    {
    public:
        /* types */
        using key_type               = Key;
        using mapped_type            = T;
        using element_type           = const std::pair<key_type, mapped_type>;
        using value_type             = std::remove_cv_t<element_type>;
        using key_compare            = Compare;
        using const_reference        = const value_type&;
        using reference              = const_reference;
        using size_type              = size_t;
        using difference_type        = ptrdiff_t;
        using const_iterator         = const value_type*;
        using iterator               = const_iterator;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        /* constructors and assignment */
        consteval static_map() noexcept : static_map(key_compare()) {}
        consteval explicit static_map(const key_compare& comp) noexcept : data_{} , compare_{ comp } {}

        template<typename K, typename V, typename Cmp, typename KeyCont, typename MappedCont>
        consteval explicit static_map(const std::flat_map<K, V, Cmp, KeyCont, MappedCont>& map)
            : data_{ std::vector<value_type>(std::from_range, map) }, compare_{ map.key_comp() } {}

        /* size and other observers */
        [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }
        [[nodiscard]] constexpr size_type size() const noexcept { return data_.size(); }
        [[nodiscard]] constexpr key_compare key_comp() const { return compare_; }

        /* iterators */
        [[nodiscard]] constexpr const_iterator begin() const noexcept { return data_.begin(); }
        [[nodiscard]] constexpr const_iterator end() const noexcept { return data_.end(); }
        [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return this->begin(); }
        [[nodiscard]] constexpr const_iterator cend() const noexcept { return this->end(); }
        [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return std::make_reverse_iterator(this->begin()); }
        [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return std::make_reverse_iterator(this->end()); }
        [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return this->rbegin(); }
        [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return this->rend(); }

        /* element access */
        [[nodiscard]] constexpr const mapped_type& at(const key_type& x) const
        {
            const auto it = std::ranges::lower_bound(data_, x, compare_, &element_type::first);
            if (it == data_.end() or compare_(it->first, x) or compare_(x, it->first))
                throw std::out_of_range("static_map::at");
            return it->second;
        }

        // TODO: replace with std::optional<T&> accessor?
        [[nodiscard]] constexpr const mapped_type* at_if(const key_type& x) const &
        {
            const auto it = std::ranges::lower_bound(data_, x, compare_, &element_type::first);
            if (it == data_.end() or compare_(it->first, x) or compare_(x, it->first))
                return nullptr;
            return &(it->second);
        }

        constexpr const mapped_type* at_if(const key_type& x) && = delete;

        /* map operations */
        [[nodiscard]] constexpr const_iterator find(const key_type& x) const
        {
            const auto it = std::ranges::lower_bound(data_, x, compare_, &element_type::first);
            if (it == data_.end() or compare_(it->first, x) or compare_(x, it->first))
                return data_.end();
            return it;
        }

        [[nodiscard]] constexpr size_type count(const key_type& x) const
        {
            return this->contains(x);
        }

        [[nodiscard]] constexpr bool contains(const key_type& x) const
        {
            return std::ranges::binary_search(data_, x, compare_, &element_type::first);
        }

        [[nodiscard]] constexpr const_iterator lower_bound(const key_type& x) const
        {
            return std::ranges::lower_bound(data_, x, compare_, &element_type::first);
        }

        [[nodiscard]] constexpr const_iterator upper_bound(const key_type& x) const
        {
            return std::ranges::upper_bound(data_, x, compare_, &element_type::first);
        }

        [[nodiscard]] constexpr std::pair<const_iterator, const_iterator> equal_range(const key_type& x) const
        {
            return std::ranges::equal_range(data_, x, compare_, &element_type::first);
        }

        static_span<element_type> data_;
        [[no_unique_address]] key_compare compare_;
    };

    template<typename K, typename V, typename C, typename KeyCont, typename MappedCont>
    static_map(const std::flat_map<const K, V, C, KeyCont, MappedCont>& mapped_cont) -> static_map<const K, V, C>;
}
