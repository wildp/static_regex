#pragma once

#include <algorithm>
#include <format>
#include <iterator>
#include <iosfwd>
#include <numeric>
#include <string>
#include <string_view>


namespace rx
{
    namespace detail
    {
        template<std::bidirectional_iterator Iter>
        class submatch_factory;
    }


    template<std::bidirectional_iterator Iter>
    class submatch
    {
    public:
        using value_type                = std::iter_value_t<Iter>;
#if __cpp_lib_ranges_as_const > 202311L
        using const_iterator            = std::const_iterator<Iter>;
#else
        using const_iterator            = Iter; /* semantically incorrect workaround */
#endif
        using const_reverse_iterator    = std::reverse_iterator<const_iterator>;
        using iterator                  = const_iterator;
        using reverse_iterator          = const_reverse_iterator;
        using size_type                 = std::size_t;
        using string_type               = std::basic_string<value_type>;
        using view_type                 = std::basic_string_view<value_type>;

        constexpr submatch() = default;

        /* observers */

        [[nodiscard]] constexpr bool matched() const noexcept { return first_ != const_iterator{}; }
        [[nodiscard]] constexpr explicit operator bool() const noexcept { return this->matched(); }
        [[nodiscard]] constexpr bool empty() const noexcept { return this->matched() ? first_ == last_ : true; };
        [[nodiscard]] constexpr size_type size() const { return this->matched() ? std::saturate_cast<size_type>(std::ranges::distance(first_, last_)) : 0; };
        [[nodiscard]] constexpr size_type length() const { return this->size(); };

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

        [[nodiscard]] constexpr string_type str() const { if (this->matched()) return { first_, last_ }; return {}; }
        [[nodiscard]] constexpr operator string_type() const { return this->str(); }
        [[nodiscard]] constexpr view_type view() const requires std::contiguous_iterator<const_iterator> { if (this->matched())  return { first_, last_ }; return {}; }
        [[nodiscard]] constexpr operator view_type() const requires std::contiguous_iterator<const_iterator> { return this->view(); }

        /* operators */

        [[nodiscard]] friend constexpr bool operator==(const submatch& lhs, const submatch& rhs)
        {
            return (lhs and rhs) ? std::ranges::equal(lhs, rhs) : lhs.matched() == rhs.matched();
        }

        [[nodiscard]] friend constexpr bool operator==(const submatch& sub, const view_type view)
        {
            return sub ? std::ranges::equal(sub, view) : false;
        }

        [[nodiscard]] friend constexpr auto operator<=>(const submatch& lhs, const submatch& rhs)
        {
            return (lhs and rhs) ? std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())
                                 : lhs.matched() <=> rhs.matched();
        }

        [[nodiscard]] friend constexpr auto operator<=>(const submatch& sub, const view_type view)
        {
            return sub ? std::lexicographical_compare_three_way(sub.begin(), sub.end(), view.begin(), view.end())
                       : std::strong_ordering::less;
        }

        template<typename CharT, typename Traits>
        [[nodiscard]] friend std::basic_ostream<CharT, Traits> operator<<(std::basic_ostream<CharT, Traits>& os, const submatch& sub)
        {
            if constexpr (std::contiguous_iterator<const_iterator>)
                return os << sub.view();
            else
                return os << sub.str();
        }

        /* misc */

        void swap(submatch& other) noexcept(std::is_nothrow_swappable_v<const_iterator>)
        {
            std::swap(first_, other.first_);
            std::swap(last_, other.last_);
        }

    private:
        constexpr submatch(iterator first, iterator last) :
            first_{ first }, last_{ last } {}

        friend class detail::submatch_factory<Iter>;

        const_iterator first_;
        const_iterator last_;
    };
}


/* structured binding support for submatch */

template<std::bidirectional_iterator Iter>
struct std::tuple_size<rx::submatch<Iter>> : integral_constant<std::size_t, 2> {};

template<std::size_t N, std::bidirectional_iterator Iter>
requires (N < 2)
struct std::tuple_element<N, rx::submatch<Iter>>
{
    using type = rx::submatch<Iter>::const_iterator;
};


/* formatting support for submatch */

template<std::bidirectional_iterator Iter>
constexpr auto std::format_kind<rx::submatch<Iter>> = std::range_format::string;

static_assert(std::formattable<rx::submatch<const char*>, char>);


/* submatch factory implementation */

namespace rx::detail
{
    template<std::bidirectional_iterator Iter>
    class submatch_factory
    {
    public:
        [[nodiscard]] static constexpr submatch<Iter> make_submatch(Iter first, Iter last)
        {
            return { first, last };
        }
    };
}