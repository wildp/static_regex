# API reference

Header `<rx/regex.hpp>` synopsis:

```cpp
namespace rx {
  template<typename CharT, std::size_t N>
  struct string_literal;

  template<string_literal Pattern>
  struct static_regex;

  template<std::bidirectional_iterator I, /* implementation details */>
  class static_regex_match_result;

  template<std::bidirectional_iterator I>
  class submatch;

  namespace literals {
    template<string_literal Pattern>
    consteval static_regex<Pattern> operator ""_rx();
  }
} // namespace rx
```


## Class template `rx::static_regex`

`static_regex` represents a compile-time regular expression pattern.

```cpp
namespace rx {
  template<string_literal Pattern>
  struct static_regex {
    using char_type = decltype(Pattern)::char_type;

    /* see below for template constraints and return type*/
    template<typename I, typename S> static constexpr auto match(const I first, const S last);
    template<typename R>             static constexpr auto match(R&& r);
    template<typename CharT>         static constexpr auto match(const CharT* cstr);
    template<typename I, typename S> static constexpr auto prefix_match(const I first, const S last);
    template<typename R>             static constexpr auto prefix_match(R&& r);
    template<typename CharT>         static constexpr auto prefix_match(const CharT* cstr);
    template<typename I, typename S> static constexpr auto search(const I first, const S last);
    template<typename R>             static constexpr auto search(R&& r);
    template<typename CharT>         static constexpr auto search(const CharT* cstr);

    /* bool-returning versions of the above */
    template<typename I, typename S> static constexpr bool is_match(const I first, const S last);
    template<typename R>             static constexpr bool is_match(R&& r);
    template<typename CharT>         static constexpr bool is_match(const CharT* cstr);
    template<typename I, typename S> static constexpr bool starts_with_match(const I first, const S last);
    template<typename R>             static constexpr bool starts_with_match(R&& r);
    template<typename CharT>         static constexpr bool starts_with_match(const CharT* cstr);
    template<typename I, typename S> static constexpr bool contains_match(const I first, const S last);
    template<typename R>             static constexpr bool contains_match(R&& r);
    template<typename CharT>         static constexpr bool contains_match(const CharT* cstr);
  };

  namespace literals {
    template<string_literal Pattern>
    consteval static_regex<Pattern> operator ""_rx();
  }
} // namespace rx
```

`static_regex` has no data members, and the code for matching `Pattern` is only generated when the relevant member function template is instantiated.

Instantiations of the member function templates `match`, `prefix_match`, and `search` return a `static_regex_match_result` corresponding to the substring (and capturing groups, if any) matched by `Pattern`.
`match` attempts to match the entire input, while `search` finds the first substring which matches `Pattern`.
`prefix_match` attempts to match a substring that begins at the start of the input.

Their template requirements are as follows:
1) Iterator overload:
  - `I` satisfies `std::bidirectional_iterator`.
  - `S` satisfies `std::sentinel_for<I>`.
  - `std::same_as<std::iter_value_t<I>, char_type>` holds.
2) Range overload:
  - `R` satisfies `std::ranges::bidirectional_range`.
  - `R` satisfies `std::ranges::borrowed_range`.
  - `std::same_as<std::ranges::range_value_t<R>, char_type>` holds.
2) C-style string overload:
  - `std::same_as<CharT, char_type>` holds.

`is_match`, `starts_with_match`, and `contains_match` correspond to `match`, `prefix_match`, and `contains_match` respectively, but return a bool instead.
With these, the requirement for `R` to satisfy `std::ranges::borrowed_range` is removed.
Depending on the regular expression involved, using these functions may allow for a smaller DFA to be generated.


## Class template `rx::static_regex_match_result`

`static_regex_match_result` stores the results of a single compile-time regular expression match.

```cpp
namespace rx {
  template<std::bidirectional_iterator I, /* implementation details */>
  class static_regex_match_result {
  public:
    using size_type              = std::size_t;
    using char_type              = std::remove_cv_t<std::iter_value_t<I>>;
    using submatch_type          = submatch<I>;
    using iterator               = /* proxy iterator to submatch_type */;
    using reverse_iterator       = /* reverse proxy iterator to submatch_type */;
    using const_iterator         = /* proxy iterator to const submatch_type */;
    using const_reverse_iterator = /* reverse proxy iterator to const submatch_type */;
  
    static constexpr size_type submatch_count;
  
    constexpr static_regex_match_result() noexcept;
  
    /* observers */
    constexpr bool has_value() const;
    constexpr explicit(false) operator bool() const;
    constexpr size_type size() const;
  
    /* array-like access */
    constexpr submatch_type operator[](size_type n) const noexcept;
    constexpr submatch_type at(size_type i) const;
  
    /* iterator support */
    constexpr iterator begin() const;
    constexpr iterator end() const;
    constexpr reverse_iterator rbegin() const;
    constexpr reverse_iterator rend() const;
    constexpr const_iterator cbegin() const;
    constexpr const_iterator cend() const;
    constexpr const_reverse_iterator crbegin() const;
    constexpr const_reverse_iterator crend() const;
  
    /* tuple support */
    template<size_type N> requires (N < submatch_count) constexpr submatch_type get() const noexcept;
  };
} // namespace rx

/* structured binding support for static_regex_match_result */
template<std::bidirectional_iterator I, /* ... */> struct std::tuple_size<rx::static_regex_match_result<I, /* ... */>>;
template<std::size_t N, std::bidirectional_iterator I, /* ... */> struct std::tuple_element<N, rx::static_regex_match_result<I, /* ... */>>;
```

The submatch with index `0` denotes the entire match, while any submatches after that correspond to the capturing groups in the matched regex pattern. 
Whenever an individual capturing group is not matched, a default-constructed submatch is returned.

Note: `size()` returns `submatch_count` when `has_value()` is `true`, and `0` otherwise.


## Class template `rx::submatch`

`submatch` identifies a single regular expression submatch.

``` cpp
namespace rx {
  template<std::bidirectional_iterator I>
  class submatch {
  public:
    using iterator               = I;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_iterator         = std::const_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using value_type             = std::iter_value_t<iterator>;
    using size_type              = std::size_t;
    using string_type            = std::basic_string<value_type>;
    using view_type              = std::basic_string_view<value_type>;
  
    constexpr submatch() = default;
  
    /* observers */
    constexpr bool matched() const noexcept;
    constexpr explicit(false) operator bool() const noexcept;
    constexpr bool empty() const noexcept;
    constexpr size_type size() const;
    constexpr size_type length() const;
  
    /* iterators */
    constexpr iterator begin() const noexcept;
    constexpr iterator end() const noexcept;
    constexpr reverse_iterator rbegin() const noexcept;
    constexpr reverse_iterator rend() const noexcept;
    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;
      
    /* structured binding support */
    template<std::size_t N> requires (N < 2) constexpr const auto& get() const &;
    template<std::size_t N> requires (N < 2) constexpr auto&& get() &&;
  
    /* conversion */
    constexpr string_type str() const;
    constexpr view_type view() const requires std::contiguous_iterator<I>;
    constexpr explicit(false) operator string_type() const;
    constexpr explicit(false) operator view_type() const requires std::contiguous_iterator<I>;;
    constexpr explicit(false) operator submatch<const_iterator>() const & requires (not std::same_as<const_iterator, iterator>);
    constexpr explicit(false) operator submatch<const_iterator>() && requires (not std::same_as<const_iterator, iterator>);
  
    /* operators */
    friend constexpr bool operator==(const submatch& lhs, const submatch& rhs);
    friend constexpr bool operator==(const submatch& sub, const view_type view);
    friend constexpr auto operator<=>(const submatch& lhs, const submatch& rhs);
    friend constexpr auto operator<=>(const submatch& sub, const view_type view);
    template<typename CharT, typename Traits> 
    friend std::basic_ostream<CharT, Traits> operator<<(std::basic_ostream<CharT, Traits>& os, const submatch& sub);
  
    /* misc */
    void swap(submatch& other) noexcept(std::is_nothrow_swappable_v<I>);
  };
} // namespace rx

/* structured binding support for submatch */
template<std::bidirectional_iterator I> struct std::tuple_size<rx::submatch<I>>;
template<std::size_t N, std::bidirectional_iterator I> requires (N < 2)  struct std::tuple_element<N, rx::submatch<I>>;

/* formatting support for submatch */
template<std::bidirectional_iterator I> inline constexpr auto std::format_kind<rx::submatch<I>> = std::range_format::string;
```

When a `submatch` is default-constructed, `matched() == false`.

Note: `matched()` and `operator bool()` both return `true` when the `submatch` object does not contain a match and `false` otherwise,
while `empty()` returns `true` additionally when the `submatch` object has matched an empty string.


## Class template `rx::string_literal`

`string_literal` exists as a minimal placeholder for the proposed [`std::basic_fixed_string`](wg21.link/p3094), and allows for strings to be passed as template parameters.

``` cpp
namespace rx
{
  template<typename CharT, std::size_t N>
  struct string_literal {
    using char_type = CharT;
    consteval string_literal(const char_type (&str)[N]);
    constexpr std::basic_string_view<char_type> view() const;
  };
} // namespace rx
```