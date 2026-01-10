#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <ranges>

#include <rx/etc/string_literal.hpp>
#include <rx/fsm/flags.hpp>
#include <rx/gen/p1306.hpp>
#include <rx/gen/result.hpp>


namespace rx
{
    template<string_literal Pattern>
    requires std::same_as<char, typename decltype(Pattern)::char_type> /* temporary: remove later */
    struct static_regex
    {
        using char_type = decltype(Pattern)::char_type;

        template<std::bidirectional_iterator I>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] constexpr auto match(const I first, const I last) const
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::full_match> m{};
            return m(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] constexpr auto match(R&& r) const
        {
            return match(std::ranges::cbegin(r), std::ranges::cend(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] constexpr auto match(const CharT* cstr) const
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::full_match> m{};
            return m(cstr);
        }

        template<std::bidirectional_iterator I>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] constexpr auto starts_with(const I first, const I last) const
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::partial_match> m{};
            return m(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] constexpr auto starts_with(R&& r) const
        {
            return starts_with(std::ranges::cbegin(r), std::ranges::cend(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] constexpr auto starts_with(const CharT* cstr) const
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::partial_match> m{};
            return m(cstr);
        }

        template<std::bidirectional_iterator I>
        requires std::same_as<std::iter_value_t<I>, char_type>
        [[nodiscard]] constexpr auto search(const I first, const I last) const
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::search_single> m{};
            return m(first, last);
        }

        template<std::ranges::bidirectional_range R>
        requires std::same_as<std::ranges::range_value_t<R>, char_type>
        [[nodiscard]] constexpr auto search(R&& r) const
        {
            return search(std::ranges::cbegin(r), std::ranges::cend(r));
        }

        template<typename CharT>
        requires std::same_as<CharT, char_type>
        [[nodiscard]] constexpr auto search(const CharT* cstr) const
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::search_single> m{};
            return m(cstr);
        }
    };


    // TODO: replace `decltype(std::ranges::cbegin(std::declval<R&>()))` with `std::ranges::const_iterator_t<T>`

    template<string_literal, typename T>
    class static_regex_iterator
    {
    public:
        constexpr static_regex_iterator() noexcept = default;
        friend constexpr bool operator==(static_regex_iterator, static_regex_iterator) noexcept = default;
    };

    using static_regex_iterator_sentinel = static_regex_iterator<"", void>;


    template<string_literal, typename, int...>
    class static_regex_token_iterator
    {
    public:
        constexpr static_regex_token_iterator() noexcept = default;
        friend constexpr bool operator==(static_regex_token_iterator, static_regex_token_iterator) noexcept = default;
    };

    using static_regex_token_iterator_sentinel = static_regex_token_iterator<"", void>;


    template<string_literal Pattern, std::bidirectional_iterator Iter>
    requires (std::same_as<std::iter_value_t<Iter>, typename static_regex<Pattern>::char_type>)
    class static_regex_iterator<Pattern, Iter>
    {
    public:
        using iterator_concept  = std::input_iterator_tag;
        using value_type        = static_regex_match_result<Iter, Pattern, detail::default_fsm_flags::search_all>;
        using difference_type   = std::ptrdiff_t;
        using pointer           = std::add_pointer_t<const value_type>;
        using reference         = std::add_lvalue_reference_t<const value_type>;
        using regex_type        = static_regex<Pattern>;

        constexpr static_regex_iterator() noexcept = default;

        constexpr static_regex_iterator(const Iter first, const Iter last, regex_type) :
            first_{ first }, last_{ last }
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::search_all> m{};
            result_ = m(first_, last_);
            first_ = result_.match_end();
        }

        template<std::ranges::bidirectional_range R>
        requires (std::same_as<std::ranges::range_value_t<R>, typename regex_type::char_type>
                  and std::same_as<decltype(std::ranges::cbegin(std::declval<R&>())), Iter>)
        constexpr static_regex_iterator(R&& r, regex_type) :
            static_regex_iterator(std::ranges::cbegin(r), std::ranges::cend(r), {}) {}

        constexpr static_regex_iterator& operator++()
        {
            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::search_all> m{};
            result_ = m(first_, last_, result_.continue_at());
            first_ = result_.match_end();
            return *this;
        }

        constexpr static_regex_iterator operator++(int)
        {
            const auto copy{ *this };
            this->operator++();
            return copy;
        }

        [[nodiscard]] constexpr reference operator*() const noexcept
        {
            return result_;
        }

        [[nodiscard]] constexpr pointer operator->() const noexcept
        {
            return &result_;
        }

        friend constexpr bool operator==(const static_regex_iterator& lhs, const static_regex_iterator& rhs) noexcept
        {
            return lhs.first_ == rhs.first_ and lhs.last_ == rhs.last_;
        }

        friend constexpr bool operator==(const static_regex_iterator& lhs, static_regex_iterator_sentinel) noexcept
        {
            return lhs.first_ == iter_type{};
        }

        template<string_literal, typename, int...>
        friend class static_regex_token_iterator;

    private:
        using iter_type = Iter;

        iter_type  first_;
        iter_type  last_;
        value_type result_;
    };


    template<string_literal Pattern, typename CharT>
    requires std::same_as<CharT, typename static_regex<Pattern>::char_type>
    class static_regex_iterator<Pattern, CharT>
    {
    public:
        using iterator_concept  = std::input_iterator_tag;
        using value_type        = static_regex_match_result<const CharT*, Pattern, detail::default_fsm_flags::search_all>;
        using difference_type   = std::ptrdiff_t;
        using pointer           = std::add_pointer_t<const value_type>;
        using reference         = std::add_lvalue_reference_t<const value_type>;
        using regex_type        = static_regex<Pattern>;

        constexpr static_regex_iterator() noexcept = default;

        constexpr static_regex_iterator(const CharT* str, regex_type) :
            ptr_{ str }
        {
            if (ptr_ == nullptr)
                return;

            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::search_all> m{};
            result_ = m(ptr_);
            ptr_ = result_.match_end();
        }

        constexpr static_regex_iterator& operator++()
        {
            if (ptr_ == nullptr)
                return *this;

            using namespace detail;
            p1306_matcher<Pattern, default_fsm_flags::search_all> m{};
            result_ = m(ptr_, result_.continue_at());
            ptr_ = result_.match_end();
            return *this;
        }

        constexpr static_regex_iterator operator++(int)
        {
            const auto copy{ *this };
            this->operator++();
            return copy;
        }

        [[nodiscard]] constexpr reference operator*() const noexcept
        {
            return result_;
        }

        [[nodiscard]] constexpr pointer operator->() const noexcept
        {
            return &result_;
        }

        friend constexpr bool operator==(const static_regex_iterator& lhs, const static_regex_iterator& rhs) noexcept
        {
            return lhs.ptr_ == rhs.ptr_;
        }

        friend constexpr bool operator==(const static_regex_iterator& lhs, static_regex_iterator_sentinel) noexcept
        {
            return lhs.ptr_ == nullptr;
        }

        template<string_literal, typename, int...>
        friend class static_regex_token_iterator;

    private:
        using cstr_type = const CharT*;

        cstr_type  ptr_{ nullptr };
        value_type result_{};
    };


    static_assert(std::input_iterator<static_regex_iterator<"", char>>);
    static_assert(std::input_iterator<static_regex_iterator<"", std::string::const_iterator>>);

    static_regex_iterator() -> static_regex_iterator<"", void>;

    template<std::bidirectional_iterator I, string_literal Pattern>
    static_regex_iterator(I, I, static_regex<Pattern>) -> static_regex_iterator<Pattern, I>;

    template<std::ranges::bidirectional_range R, string_literal Pattern>
    static_regex_iterator(R&&, static_regex<Pattern>) -> static_regex_iterator<Pattern, decltype(std::ranges::cbegin(std::declval<R&>()))>;

    template<typename CharT, string_literal Pattern>
    static_regex_iterator(const CharT*, static_regex<Pattern>) -> static_regex_iterator<Pattern, CharT>;


    template<string_literal Pattern, typename T, int... Submatches>
    requires (std::bidirectional_iterator<T> or std::same_as<T, typename static_regex<Pattern>::char_type>)
    class static_regex_token_iterator<Pattern, T, Submatches...>
    {
        using parent_type       = static_regex_iterator<Pattern, T>;

        template<int S>
        static constexpr bool submatch_is_valid{ ((-1 == S) or (S < parent_type::value_type::submatch_count)) };

        static_assert(sizeof...(Submatches) > 0);
        static_assert((submatch_is_valid<Submatches> and ...));

    public:
        using iterator_concept  = std::input_iterator_tag;
        using value_type        = typename parent_type::value_type::submatch_type;
        using difference_type   = std::ptrdiff_t;
        using pointer           = std::add_pointer<const value_type>;
        using reference         = std::add_lvalue_reference_t<const value_type>;
        using regex_type        = static_regex<Pattern>;

        constexpr static_regex_token_iterator() noexcept = default;
        
        constexpr static_regex_token_iterator(const T first, const T last, regex_type, std::integer_sequence<int, Submatches...>)
        requires (std::bidirectional_iterator<T>) :
            iterator_{ first, last, regex_type{} }
        {
            if constexpr (is_suffix_iterator)
                suffix_start_ = first;

            this->stash_result();
        }

        constexpr static_regex_token_iterator(const T first, const T last, regex_type, std::integral_constant<int, Submatches>...)
        requires (std::bidirectional_iterator<T> and sizeof...(Submatches) == 1) :
            static_regex_token_iterator(first, last, {}, std::integer_sequence<int, Submatches...>{}) {}

        template<std::ranges::bidirectional_range R>
        requires (std::same_as<std::ranges::range_value_t<R>, typename regex_type::char_type>
                  and std::same_as<decltype(std::ranges::cbegin(std::declval<R&>())), T>)
        constexpr static_regex_token_iterator(R&& r, regex_type, std::integer_sequence<int, Submatches...>) 
        requires (std::bidirectional_iterator<T>) :
            static_regex_token_iterator(std::ranges::cbegin(r), std::ranges::cend(r), {}, std::integer_sequence<int, Submatches...>{}) {}

        template<std::ranges::bidirectional_range R>
        requires (std::same_as<std::ranges::range_value_t<R>, typename regex_type::char_type>
                  and std::same_as<decltype(std::ranges::cbegin(std::declval<R&>())), T>)
        constexpr static_regex_token_iterator(R&& r, regex_type, std::integral_constant<int, Submatches>...)
        requires (std::bidirectional_iterator<T> and sizeof...(Submatches) == 1) :
            static_regex_token_iterator(std::ranges::cbegin(r), std::ranges::cend(r), {}, std::integer_sequence<int, Submatches...>{}) {}

         constexpr static_regex_token_iterator(const T* str, regex_type, std::integer_sequence<int, Submatches...>)
        requires (std::same_as<T, typename static_regex<Pattern>::char_type>) :
            iterator_{ str, regex_type{} }
        {
            if constexpr (is_suffix_iterator)
                suffix_start_ = str;

            this->stash_result();
        }

        constexpr static_regex_token_iterator(const T* str, regex_type, std::integral_constant<int, Submatches>...)
        requires (std::same_as<T, typename static_regex<Pattern>::char_type> and sizeof...(Submatches) == 1) :
            static_regex_token_iterator(str, {}, std::integer_sequence<int, Submatches...>{}) {}

        constexpr static_regex_token_iterator& operator++()
        {
            ++index_;

            if (index_ == submatches.size() and iterator_ != static_regex_iterator_sentinel{})
            {
                if constexpr (is_suffix_iterator)
                {
                    if constexpr (std::bidirectional_iterator<T>)
                        suffix_start_ = iterator_.first_;
                    else if constexpr (std::same_as<T, typename static_regex<Pattern>::char_type>)
                        suffix_start_ = iterator_.ptr_;
                }

                ++iterator_;
                index_ = 0;
            }

            if constexpr (is_suffix_iterator)
            {
                if (iterator_ == static_regex_iterator_sentinel{})
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

        constexpr static_regex_token_iterator operator++(int)
        {
            const auto copy{ *this };
            this->operator++();
            return copy;
        }

        [[nodiscard]] constexpr reference operator*() const noexcept
        {
            return result_;
        }

        [[nodiscard]] constexpr pointer operator->() const noexcept
        {
            return &result_;
        }

        friend constexpr bool operator==(const static_regex_token_iterator& lhs, const static_regex_token_iterator& rhs) noexcept
        {
            return lhs.iterator_ == rhs.iterator_ and lhs.index_ == rhs.index_;
        }

        friend constexpr bool operator==(const static_regex_token_iterator& lhs, static_regex_token_iterator_sentinel) noexcept
        {
            if constexpr (is_suffix_iterator)
            {
                return lhs.iterator_ == static_regex_iterator_sentinel{} and lhs.index_ == submatches.size();
            }
            else
            {
                return lhs.iterator_ == static_regex_iterator_sentinel{};
            }
        }

    private:
        static constexpr bool is_suffix_iterator{ ((Submatches == -1) or ...) };
        static constexpr std::array submatches{ Submatches... };

        using iter_type = typename value_type::const_iterator;
        using suffix_it = std::conditional_t<is_suffix_iterator, iter_type, std::monostate>;

        constexpr void stash_result() 
        {
            if constexpr (is_suffix_iterator)
            {
                if (submatches[index_] == -1)
                {
                    if (iterator_->has_value())
                    {
                        result_ = detail::submatch_factory<iter_type>::make_submatch(suffix_start_, iterator_->template get<0>().cbegin());
                    }
                    else
                    {
                        if constexpr (std::bidirectional_iterator<T>)
                            result_ = detail::submatch_factory<iter_type>::make_submatch(suffix_start_, iterator_.last_);
                        else if constexpr (std::same_as<T, typename static_regex<Pattern>::char_type>)
                            result_ = detail::submatch_factory<iter_type>::make_submatch(suffix_start_, std::basic_string_view{ suffix_start_ }.cend());        

                        if (result_.cbegin() == result_.cend())
                            index_ = submatches.size();
                    }

                    return;
                }
            }

            if (iterator_->has_value())
                result_ = iterator_->operator[](submatches[index_]);
        }


        parent_type iterator_;
        std::size_t index_{ 0 };
        value_type  result_;

        [[no_unique_address]] suffix_it suffix_start_;
    };

    static_regex_token_iterator() -> static_regex_token_iterator<"", void>;

    template<std::bidirectional_iterator Iter, string_literal Pattern, int I>
    static_regex_token_iterator(Iter, Iter, static_regex<Pattern>, std::integral_constant<int, I>) -> static_regex_token_iterator<Pattern, Iter, I>;

    template<std::bidirectional_iterator Iter, string_literal Pattern, int... Is>
    static_regex_token_iterator(Iter, Iter, static_regex<Pattern>, std::integer_sequence<int, Is...>) -> static_regex_token_iterator<Pattern, Iter, Is...>;

    template<std::ranges::bidirectional_range R, string_literal Pattern, int I>
    static_regex_token_iterator(R&&, static_regex<Pattern>, std::integral_constant<int, I>)-> static_regex_token_iterator<Pattern, decltype(std::ranges::cbegin(std::declval<R&>())), I>;

    template<std::ranges::bidirectional_range R, string_literal Pattern, int... Is>
    static_regex_token_iterator(R&&, static_regex<Pattern>, std::integer_sequence<int, Is...>)-> static_regex_token_iterator<Pattern, decltype(std::ranges::cbegin(std::declval<R&>())), Is...>;

    template<typename CharT, string_literal Pattern, int I>
    static_regex_token_iterator(const CharT*, static_regex<Pattern>, std::integral_constant<int, I>) -> static_regex_token_iterator<Pattern, CharT, I>;

    template<typename CharT, string_literal Pattern, int... Is>
    static_regex_token_iterator(const CharT*, static_regex<Pattern>, std::integer_sequence<int, Is...>) -> static_regex_token_iterator<Pattern, CharT, Is...>;


    namespace literals
    {
        template<string_literal P>
        consteval auto operator ""_rx()
        {
            return static_regex<P>();
        }
    }
}


