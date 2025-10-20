#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

#include <rx/api/submatch.hpp>
#include <rx/etc/string_literal.hpp>
#include <rx/gen/compile.hpp>
#include <rx/fsm/flags.hpp>


namespace rx::detail
{
    template<string_literal Pattern, fsm_flags Flags>
    struct p1306_matcher;


    template<std::bidirectional_iterator I>
    class compile_time_match_result_base
    {
    public:
        [[nodiscard]] static constexpr submatch<I> make_submatch(I first, I last)
        {
            return { first, last };
        }
    };
}

namespace rx
{
    template<std::bidirectional_iterator I, detail::final_capture_info C, detail::static_span<const detail::tdfa::reg_t> FinalRegisters, std::size_t RegCount>
    class compile_time_match_result : detail::compile_time_match_result_base<I>
    {
        using base = detail::compile_time_match_result_base<I>;

    public:
        using size_type         = std::size_t;
        using char_type         = std::remove_cv_t<std::iter_value_t<I>>;
        using submatch_type     = const submatch<I>;
        static constexpr size_type submatch_count{ C.capture_count() };

        /* iterator implementation */

        class proxy_iterator_impl
        {
            friend class compile_time_match_result;
            
            using it                = proxy_iterator_impl;
            using parent_type       = compile_time_match_result;

        public:
            using difference_type   = std::ptrdiff_t;
            using value_type        = submatch_type;

            constexpr proxy_iterator_impl() = default;
            
            constexpr value_type operator*() const { return (*ptr_)[pos_]; }
            constexpr value_type operator[](difference_type n) const { return (*ptr_)[pos_ + n]; }

            constexpr it& operator++() { ++pos_; return *this; }
            constexpr it operator++(int) { auto tmp{ *this }; ++(*this); return tmp; }
            constexpr it& operator--() { --pos_; return *this; }
            constexpr it operator--(int) { auto tmp{ *this }; --(*this); return tmp; }
            constexpr it& operator+=(difference_type n) { pos_ += n; return *this; }
            constexpr it& operator-=(difference_type n) { pos_ -= n; return *this; }

            constexpr friend bool operator==(const it&, const it&) = default;
            constexpr friend auto operator<=>(const it&, const it&) = default;
            constexpr friend it operator+(const it& lhs , difference_type rhs) { return {  lhs.ptr_, lhs.pos_ + rhs }; }
            constexpr friend it operator+(difference_type lhs , const it& rhs) { return {  rhs.ptr_, lhs + rhs.pos_ }; }
            constexpr friend it operator-(const it& lhs, difference_type rhs) { return {  lhs.ptr_, lhs.pos_ - rhs };  }
            constexpr friend difference_type operator-(const it& lhs, const it& rhs) { return rhs.pos_ - lhs.pos_; }

        private:
            constexpr proxy_iterator_impl(const parent_type* ptr, std::size_t pos) : ptr_{ ptr }, pos_{ pos } {}

            const parent_type* ptr_{ nullptr };
            std::size_t pos_{ 0 };
        };

        static_assert(std::random_access_iterator<proxy_iterator_impl>);

        using const_iterator            = proxy_iterator_impl;
        using const_reverse_iterator    = std::reverse_iterator<const_iterator>;
        using iterator                  = const_iterator;
        using reverse_iterator          = const_reverse_iterator;
        
        /* observers */

        [[nodiscard]] explicit(false) constexpr operator bool() const { return this->has_value(); }
        [[nodiscard]] constexpr size_type size() const { return (this->has_value()) ? submatch_count : 0; }

        /* array-like access */

        [[nodiscard]] constexpr submatch_type operator[](size_type n) const noexcept
        {
            template for (constexpr size_type N: std::define_static_array(detail::make_iota(submatch_count)))
            {
                if (n == N) return this->get<N>();
            }
            std::unreachable();
        }

        [[nodiscard]] constexpr submatch_type at(size_type i) const noexcept
        {
            this->range_check(i); return this->operator[](i);
        }

        /* iterator support */

        [[nodiscard]] constexpr const_iterator begin() const { return (this->has_value()) ? const_iterator{ this, 0 } : this->end(); }
        [[nodiscard]] constexpr const_iterator end() const { return { this, this->size() }; }
        [[nodiscard]] constexpr const_iterator cbegin() const { return this->begin(); }
        [[nodiscard]] constexpr const_iterator cend() const { return this->end(); } 
        [[nodiscard]] constexpr const_reverse_iterator rbegin() const { return std::make_reverse_iterator(this->end()); }
        [[nodiscard]] constexpr const_reverse_iterator rend() const { return std::make_reverse_iterator(this->begin()); }
        [[nodiscard]] constexpr const_reverse_iterator crbegin() const { return this->rbegin(); }
        [[nodiscard]] constexpr const_reverse_iterator crend() const { return this->rend(); } 

        /* tuple support */

        template<size_type N>
        requires (N < submatch_count)
        [[nodiscard]] constexpr submatch_type get() const noexcept
        {
            using namespace detail;

            constexpr auto current{ C.captures[N] };

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
                        return base::make_submatch(std::next(this->get_tag<current.first.tag_number>(), current.first.offset),
                                                   std::next(this->get_tag<current.second.tag_number>(), current.second.offset));
                    }
                }
                else
                {
                    if (this->tag_enabled<current.first.tag_number>() and this->tag_enabled<current.second.tag_number>())
                    {
                        return base::make_submatch(std::next(this->get_tag<current.first.tag_number>(), current.first.offset),
                                                   std::next(this->get_tag<current.second.tag_number>(), current.second.offset));
                    }
                }
            }

            return {};
        }

        template<string_literal Pattern, detail::fsm_flags Flags> friend struct detail::p1306_matcher;
    
    private:
        using iterator_type = I;
        static constexpr bool iterator_is_pointer{ std::is_pointer_v<iterator_type> };
        static constexpr bool has_match_start{ C.has_match_start() };
        using registers_type = std::conditional_t<RegCount == 0, std::monostate, std::array<iterator_type, RegCount>>;
        using enabled_type = std::conditional_t<RegCount == 0 or iterator_is_pointer, std::monostate, std::array<bool, RegCount>>;
        using match_start_type = std::conditional_t<has_match_start, iterator_type, std::monostate>;
        using match_end_type = std::conditional_t<iterator_is_pointer, iterator_type, std::optional<iterator_type>>;

        explicit constexpr compile_time_match_result(iterator_type start)
        requires (iterator_is_pointer) : match_end_{ nullptr }
        {
            if constexpr (has_match_start) match_start_ = start;
            if constexpr (RegCount > 0) reg_.fill(nullptr);
        }

        explicit constexpr compile_time_match_result(iterator_type start)
        requires (not iterator_is_pointer) : match_end_{}
        {
            if constexpr (has_match_start) match_start_ = start;
            if constexpr (RegCount > 0) enabled_.fill(false);
        }

        template<detail::tag_number_t N>
        [[nodiscard]] constexpr bool tag_enabled() const
        {
            if constexpr (N == detail::start_of_input_tag or N == detail::end_of_input_tag)
                return true;
            else if constexpr (iterator_is_pointer)
                return reg_[FinalRegisters[N]] != nullptr;
            else
                return enabled_[FinalRegisters[N]];
        }

        template<detail::tag_number_t N>
        [[nodiscard]] constexpr iterator_type get_tag() const
        {
            if constexpr (N == detail::start_of_input_tag)
            {
                return match_start_;
            }
            else if constexpr (N == detail::end_of_input_tag)
            {
                if constexpr (iterator_is_pointer)
                    return match_end_;
                else
                    return *match_end_;
            }
            else
            {
                return reg_[FinalRegisters[N]];
            }
        }

        constexpr void range_check(size_type n) const
        {
            if (n >= this->size())
                throw std::out_of_range("compile_time_match_result::range_check: n >= this->size()");
        }

        [[nodiscard]] constexpr bool has_value() const
        {
            return static_cast<bool>(match_end_);
        }

        [[no_unique_address]] registers_type reg_;
        [[no_unique_address]] enabled_type enabled_;
        [[no_unique_address]] match_start_type match_start_;
        match_end_type match_end_;
    };
}


/* structured binding support for compile_time_match_result */

template<std::bidirectional_iterator I,
        rx::detail::final_capture_info C,
        rx::detail::static_span<const rx::detail::tdfa::reg_t> FinalRegisters,
        std::size_t RegCount>
struct std::tuple_size<rx::compile_time_match_result<I, C, FinalRegisters, RegCount>> :
    integral_constant<std::size_t, rx::compile_time_match_result<I, C, FinalRegisters, RegCount>::submatch_count> {};

template<std::size_t N,
        std::bidirectional_iterator I,
        rx::detail::final_capture_info C,
        rx::detail::static_span<const rx::detail::tdfa::reg_t> FinalRegisters,
        std::size_t RegCount>
requires (N < rx::compile_time_match_result<I, C, FinalRegisters, RegCount>::submatch_count)
struct std::tuple_element<N, rx::compile_time_match_result<I, C, FinalRegisters, RegCount>>
{
    using type = rx::compile_time_match_result<I, C, FinalRegisters, RegCount>::submatch_type;
};

