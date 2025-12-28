#pragma once

#include <iterator>
#include <span>
#include <stdexcept>

namespace rx::detail
{
    template<class T>
    requires (std::is_const_v<T>)
    class static_span
    {
    public:
        /* types */
        using element_type              = T;
        using value_type                = std::remove_cv_t<element_type>;
        using size_type                 = std::size_t;
        using difference_type           = std::ptrdiff_t;
        using pointer                   = element_type*;
        using const_pointer             = element_type*;
        using reference                 = element_type&;
        using const_reference           = const element_type&;
        using const_iterator            = const_pointer;
        using iterator                  = const_iterator;
        using const_reverse_iterator    = std::reverse_iterator<iterator>;
        using reverse_iterator          = const_reverse_iterator;

        /* constructors and assignment */
        constexpr static_span() noexcept = default; 

        template<std::size_t N = std::dynamic_extent>
        consteval static_span(std::span<T, N> span) noexcept :
            data_{ span.data() }, size_{ span.size() }
        {
        }

        template<std::size_t N = std::dynamic_extent>
        consteval static_span& operator=(std::span<T, N> span) noexcept
        {
            data_ = span.data();
            size_ = span.size();
            return *this;
        }

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
        [[nodiscard]] constexpr const_iterator end() const noexcept { return data_ + this->size() ; }
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
        pointer data_{ nullptr };
        size_type size_{ 0 };
    };

    template<class R>
    static_span(R&&) -> static_span<std::add_const_t<std::remove_reference_t<std::ranges::range_reference_t<R>>>>;
}