#pragma once

#include <algorithm>
#include <format>
#include <iterator>
#include <iosfwd>
#include <string>
#include <string_view>


namespace rx
{
    namespace detail
    {
        template<std::bidirectional_iterator I>
        class compile_time_match_result_base;
    }


    template<std::bidirectional_iterator I>
    class submatch
    {
    public:
        using value_type                = std::iter_value_t<I>;
#if __cpp_lib_ranges_as_const > 202311L
        using const_iterator            = std::const_iterator<I>;
#else
        using const_iterator            = I; /* semantically incorrect workaround */
#endif
        using const_reverse_iterator    = std::reverse_iterator<const_iterator>;
        using iterator                  = const_iterator;
        using reverse_iterator          = const_reverse_iterator;

        constexpr submatch() = default;

        /* observers */

        [[nodiscard]] constexpr bool matched() const noexcept { return matched_; }
        [[nodiscard]] explicit constexpr operator bool() const noexcept { return this->matched(); }
        [[nodiscard]] constexpr bool empty() const noexcept { return this->matched() ? first_ == last_ : true; };
        [[nodiscard]] constexpr std::size_t size() const { return this->matched() ? std::ranges::distance(first_, last_) : 0; };
        [[nodiscard]] constexpr std::size_t length() const { return this->size(); };

        /* iterators */

        [[nodiscard]] constexpr const_iterator begin() const noexcept { return first_; }
        [[nodiscard]] constexpr const_iterator end() const noexcept { return last_; }
        [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return this->begin(); }
        [[nodiscard]] constexpr const_iterator cend() const noexcept { return this->end(); }
        [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return std::make_reverse_iterator(this->cend()); }
        [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return std::make_reverse_iterator(this->cbegin()); }
        [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return this->rend(); }
        [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return this->rbegin(); }

        /* structured binding support */

        template<std::size_t N, typename T> requires (N < 2)
        [[nodiscard]] constexpr auto&& get(this T&& self)
        {
            if constexpr (N == 0) return std::forward<T>(self).begin_;
            if constexpr (N == 1) return std::forward<T>(self).end_;
        } 

        /* conversion */

        [[nodiscard]] constexpr std::basic_string<value_type> str() const { if (this->matched()) return { first_, last_ }; return {}; }
        [[nodiscard]] constexpr operator std::basic_string<value_type>() const { return this->str(); }
        [[nodiscard]] constexpr std::basic_string_view<value_type> view() const { if (this->matched())  return { first_, last_ }; return {}; }
        [[nodiscard]] constexpr operator std::basic_string_view<value_type>() const { return this->view(); }

        /* operators */

        [[nodiscard]] friend constexpr bool operator==(const submatch& lhs, const submatch& rhs)
        {
            return (lhs.matched() and rhs.matched()) ? std::ranges::equal(lhs, rhs) : lhs.matched() == rhs.matched();
        }

        template<typename Traits, typename Alloc>
        [[nodiscard]] friend constexpr bool operator==(const submatch& lhs, const std::basic_string<value_type, Traits, Alloc>& rhs)
        {
            return lhs.matched() ? std::ranges::equal(lhs, rhs) : false;
        }

        template<typename Traits, typename Alloc>
        [[nodiscard]] friend constexpr bool operator==(const std::basic_string<value_type, Traits, Alloc>& lhs, const submatch& rhs)
        {
            return rhs.matched() ? std::ranges::equal(lhs, rhs) : false;
        }

        template<typename Traits>
        [[nodiscard]] friend constexpr bool operator==(const submatch& lhs, const std::basic_string_view<value_type, Traits>& rhs)
        {
            return lhs.matched() ? std::ranges::equal(lhs, rhs) : false;
        }

        template<typename Traits>
        [[nodiscard]] friend constexpr bool operator==(const std::basic_string_view<value_type, Traits>& lhs, const submatch& rhs)
        {
            return rhs.matched() ? std::ranges::equal(lhs, rhs) : false;
        }

        [[nodiscard]] friend constexpr auto operator<=>(const submatch& lhs, const submatch& rhs)
        {
            return (lhs.matched() and rhs.matched()) ? std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())
                                                     : lhs.matched() <=> rhs.matched();
        }

        template<typename Traits, typename Alloc>
        [[nodiscard]] friend constexpr auto operator<=>(const submatch& lhs, const std::basic_string<value_type, Traits, Alloc>& rhs)
        {
            return lhs.matched() ? std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) : std::strong_ordering::less;
        }

        template<typename Traits, typename Alloc>
        [[nodiscard]] friend constexpr auto operator<=>(const std::basic_string<value_type, Traits, Alloc>& lhs, const submatch& rhs)
        {
            return rhs.matched() ? std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) : std::strong_ordering::greater;
        }

        template<typename Traits>
        [[nodiscard]] friend constexpr auto operator<=>(const submatch& lhs, const std::basic_string_view<value_type, Traits>& rhs)
        {
            return lhs.matched() ? std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) : std::strong_ordering::less;
        }

        template<typename Traits>
        [[nodiscard]] friend constexpr auto operator<=>(const std::basic_string_view<value_type, Traits>& lhs, const submatch& rhs)
        {
            return rhs.matched() ? std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) : std::strong_ordering::greater;
        }

        template<typename CharT, typename Traits>
        [[nodiscard]] friend std::basic_ostream<CharT, Traits> operator<<(std::basic_ostream<CharT,Traits>& os, const submatch& m)
        {
            return os << m.view();
        }

        /* misc */

        void swap(submatch& other) noexcept(std::is_nothrow_swappable_v<iterator>)
        {
            std::swap(first_, other.first_);
            std::swap(last_, other.last_);
            std::swap(matched_, other.matched_);
        }

    private:
        constexpr submatch(iterator first, iterator last) :
            first_{ first }, last_{ last }, matched_{ true } {}

        friend class detail::compile_time_match_result_base<I>;

        const_iterator first_;
        const_iterator last_;
        bool matched_{ false };
    };
}


/* structured binding support for submatch */

template<std::bidirectional_iterator I>
struct std::tuple_size<rx::submatch<I>> : integral_constant<std::size_t, 2> {};

template<std::size_t N, std::bidirectional_iterator I>
requires (N < 2)
struct std::tuple_element<N, rx::submatch<I>>
{
    using type = rx::submatch<I>::iterator;
};


/* formatting support for submatch */

template<std::bidirectional_iterator I>
constexpr auto std::format_kind<rx::submatch<I>> = std::range_format::string;

static_assert(std::formattable<rx::submatch<const char*>, char>);