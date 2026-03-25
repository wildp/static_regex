# API reference

Header `<rx/regex.hpp>` synopsis:

```cpp
namespace rx {
  enum class mode {
    standard,
    fast,
    naive
  };

  template<typename CharT, std::size_t N>
  struct string_literal;

  template<string_literal Pattern, mode Mode>
  struct static_regex;

  template<std::bidirectional_iterator I, /* implementation details */>
    requires std::default_initializable<I> and std::copyable<I>
  class static_regex_match_result;

  template<std::bidirectional_iterator I>
  class submatch;

  template<std::ranges::bidirectional_range V, typename Regex>
    requires std::ranges::view<V>
  class regex_match_view;

  template<std::ranges::input_range V, int... Submatches>
    requires std::ranges::view<V>
  class submatches_view;

  template<std::ranges::input_range V, typename Fmt>
    requires std::ranges::view<V>
  class replace_view {};

  template<std::ranges::bidirectional_range V, typename Regex>
    requires std::ranges::view<V>
  class regex_split_view {}

  namespace views {
    inline constexpr /* range adaptor */ regex_match;
    inline constexpr /* range adaptor */ submatches;
    inline constexpr /* range-adaptor */ replace;
    inline constexpr /* range adaptor */ regex_split;

    template<int... Submatches>
      requires (sizeof...(Submatches) > 0)
    inline constexpr /* range-adaptor */ static_submatches;

    template<string_literal Fmt>
    inline constexpr /* range-adaptor */ static_replace;
  }

  namespace literals {
    template<string_literal Pattern>
    consteval static_regex<Pattern> operator ""_rx();

    template<string_literal Pattern>
    consteval static_regex<Pattern, mode::fast> operator ""_rxf();

    template<string_literal Pattern>
    consteval static_regex<Pattern, mode::fast> operator ""_rx_fast();

    template<string_literal Pattern>
    consteval static_regex<Pattern, mode::naive> operator ""_rxn();

    template<string_literal Pattern>
    consteval static_regex<Pattern, mode::naive> operator ""_rx_naive();
  } // namespace literals

  template<string_literal Fmt> struct fmt_t;
  template<string_literal Fmt> inline constexpr fmt_t<Fmt> fmt;

  template<class I, class O>
  using regex_replace_result = std::ranges::in_out_result<I, O>;

  /* see documentation for regex_replace template constraints */
  template<typename I, typename S, typename O, /* regex-like */ Regex, typename F, typename FmtS>
  constexpr auto regex_replace(I first, S last, O result, Regex pattern, F fmt, FmtS fmt_last)
    -> regex_replace_result<I, O>;

  template<typename R, typename O, /* regex-like */ Regex, typename FmtR>
  constexpr auto regex_replace(R&& r, O result, Regex pattern, FmtR&& fmt)
    -> regex_replace_result<std::ranges::borrowed_iterator_t<R>, O>;

  template<typename I, typename S, typename> O,/* regex-like */ Regex, typename CharT>
  constexpr auto regex_replace(I first, S last, O result, Regex pattern, std::basic_string_view<CharT> fmt)
    -> regex_replace_result<I, O>;

  template<typename R, typename O, /* regex-like */ Regex, typename CharT>
  constexpr auto regex_replace(R&& r, O result, Regex pattern, std::basic_string_view<CharT> fmt)
    -> regex_replace_result<std::ranges::borrowed_iterator_t<R>, O>;

  template<typename CharT, /* regex-like */ Regex>
  constexpr auto regex_replace(std::basic_string_view<CharT> sv, Regex pattern,
                               std::basic_string_view<std::type_identity_t<CharT>> fmt)
    -> std::basic_string<CharT>;

  template<typename CharT, /* regex-like */ Regex>
  constexpr auto regex_replace(const CharT* cstr, Regex pattern,
                               std::basic_string_view<std::type_identity_t<CharT>> fmt)
    -> std::basic_string<CharT>;

  template<typename I, typename S, typename O, /* static-regex-like */ Regex, string_literal Fmt>
  constexpr auto regex_replace(I first, S last, O result, Regex pattern, fmt_t<Fmt>)
    -> regex_replace_result<I, O>;

  template<typename R, typename O, /* static-regex-like */ Regex, string_literal Fmt>
  constexpr auto regex_replace(R&& r, O result, Regex pattern, fmt_t<Fmt>)
    -> regex_replace_result<std::ranges::borrowed_iterator_t<R>, O>;

  template<typename CharT, /* static-regex-like */ Regex, string_literal Fmt>
  constexpr auto regex_replace(std::basic_string_view<CharT> sv, Regex pattern, fmt_t<Fmt>)
    ->  std::basic_string<CharT>;

  template<typename CharT, /* static-regex-like */ Regex, string_literal Fmt>
  constexpr auto regex_replace(const CharT* cstr, Regex pattern, fmt_t<Fmt>)
    -> std::basic_string<CharT>;
} // namespace rx
```

## Enum class `rx::mode`

`mode` enumerates the possible regular expression matching implementations that `static_regex` can use.

```cpp
namespace rx {
  enum class mode {
    standard,
    fast,
    naive
  };
}
```

`mode::standard` is the default implementation for `static_regex`.
It uses a deterministic finite automaton to match patterns in linear time.
When performing a search, it attempts to match the pattern beginning at each character in the input, leading to a quadratic worst case complexity.

`mode::fast` uses the same implementation as `mode::standard` but instead performs searches in linear time using a single deterministic finite automaton.
This mode is known to increase compile times (in some cases to an unacceptable duration), but depending on the pattern can result in performance improvements.
Patterns that feature repetition tend to see the greatest improvement, while simple patterns with character classes (including case-insensitive matching) may experience performance degredation.
When matching, `mode::fast` is identical to `mode::standard`.

`mode::naive` uses a naive backtracking-based implementation for matching.
The worst case complexity is exponential, in many cases trading runtime performance for faster compilation times.
Unlike the finite automaton implementations, backreferences, possessive matching, and branch-reset groups are additionally supported.
This mode should be avoided unless the additional features or improved compilation times are needed.


## Class template `rx::string_literal`

`string_literal` exists as a minimal placeholder for the proposed [`std::basic_fixed_string`](wg21.link/p3094), and allows for strings to be passed as template parameters.

``` cpp
namespace rx {
  template<typename CharT, std::size_t N>
  struct string_literal {
    using value_type = CharT;
    consteval string_literal(const value_type (&str)[N]);
    constexpr std::basic_string_view<value_type> view() const;
  };
} // namespace rx
```


## Class template `rx::static_regex`

`static_regex` represents a compile-time regular expression pattern.

```cpp
namespace rx {
  template<string_literal Pattern, mode Mode = mode::standard>
  struct static_regex {
    using char_type = decltype(Pattern)::value_type;

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

    /* range returning algorithms; see below  */
    template<typename I, typename S> static constexpr auto range(const I first, const S last);
    template<typename R>             static constexpr auto range(R&& r);
    template<typename CharT>         static constexpr auto range(const CharT* cstr);
  };
} // namespace rx
```

`static_regex` has no data members, and the code for matching `Pattern` is only generated when the relevant member function template is instantiated.
`Mode` dictates the implementation used by the matching functions *(see above section for more details)*.

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
Depending on the regex pattern, using these functions may allow for a smaller DFA to be generated.

`range` returns a `regex_match_view` over all substrings of the input matched by `Pattern`.
The template parameter requirements for `range` are the same as for `match`, `prefix_match`, and `search`.

Additionally, the following helpers are defined for convenience:

```cpp
namespace rx {
  namespace literals {
    template<string_literal Pattern>
    consteval static_regex<Pattern> operator ""_rx();

    template<string_literal Pattern>
    consteval static_regex<Pattern, mode::fast> operator ""_rxf();

    template<string_literal Pattern>
    consteval static_regex<Pattern, mode::fast> operator ""_rx_fast();

    template<string_literal Pattern>
    consteval static_regex<Pattern, mode::naive> operator ""_rxn();

    template<string_literal Pattern>
    consteval static_regex<Pattern, mode::naive> operator ""_rx_naive();
  } // namespace literals
} // namespace rx;
```


## Class template `rx::static_regex_match_result`

`static_regex_match_result` stores the results of a single compile-time regular expression match.

```cpp
namespace rx {
  template<std::bidirectional_iterator I, /* implementation details */>
    requires std::default_initializable<I> and std::copyable<I>
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
template<std::bidirectional_iterator I, /* ... */>
struct std::tuple_size<rx::static_regex_match_result<I, /* ... */>>;

template<std::size_t N, std::bidirectional_iterator I, /* ... */>
struct std::tuple_element<N, rx::static_regex_match_result<I, /* ... */>>;
```

The submatch with index `0` denotes the entire match, while any submatches after that correspond to the capturing groups in the matched regex pattern.
The public static data member `submatch_count` denotes the maximum number of submatches that an object contains.
It should be noted that `size()` returns `submatch_count` when `has_value()` is `true`, and `0` otherwise.

`static_regex_match_result` does not store individual submatches; instead submatches are re-constructed as needed.
Whenever an individual capturing group is not matched, a default-constructed submatch is returned.
As such, `iterator` satisfies `std::random_access_iterator` but due to being a proxy iterator only meets the requirements for *Cpp17InputIterator*.


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
    constexpr explicit(false) operator view_type() const
      requires std::contiguous_iterator<I>;;
    constexpr explicit(false) operator submatch<const_iterator>() const &
      requires (not std::same_as<const_iterator, iterator>);
    constexpr explicit(false) operator submatch<const_iterator>() &&
      requires (not std::same_as<const_iterator, iterator>);

    /* operators */
    friend constexpr bool operator==(const submatch& lhs, const submatch& rhs);
    friend constexpr bool operator==(const submatch& sub, const view_type view);
    friend constexpr auto operator<=>(const submatch& lhs, const submatch& rhs);
    friend constexpr auto operator<=>(const submatch& sub, const view_type view);

    template<typename CharT, typename Traits>
    friend auto operator<<(std::basic_ostream<CharT, Traits>& os, const submatch& sub)
      -> std::basic_ostream<CharT, Traits>;

    /* misc */
    void swap(submatch& other) noexcept(std::is_nothrow_swappable_v<I>);
  };
} // namespace rx

/* structured binding support for submatch */
template<std::bidirectional_iterator I>
struct std::tuple_size<rx::submatch<I>>;

template<std::size_t N, std::bidirectional_iterator I> requires (N < 2)
struct std::tuple_element<N, rx::submatch<I>>;

/* formatting support for submatch */
template<std::bidirectional_iterator I>
inline constexpr auto std::format_kind<rx::submatch<I>> = std::range_format::string;
```

A `submatch` can be seen as an iterator pair that denotes a matched subrange of the input, combined with an additional bool to track whether the object contains a match, which can be checked through `matched()` or `operator bool()`.
When a `submatch` is default-constructed, `matched() == false`.
It should be noted that `submatch` objects can be empty (i.e. `begin() == end()`) while still containing a match; `empty()` should be used to check for the presence of empty or non-match-containing objects.


## Class template `rx::regex_match_view`

`regex_match_view` provides a way to iterate over every regular expression match in an input string.

```cpp
namespace rx {
  template<std::ranges::bidirectional_range V, typename Regex>
    requires std::ranges::view<V>
  class regex_match_view {};

  /* rx::static_regex support */
  template<std::ranges::bidirectional_range V, string_literal Pattern, mode Mode>
    requires std::ranges::view<V>
  class regex_match_view<V, static_regex<Pattern, Mode>>
    : std::ranges::view_interface<regex_match_view<V, static_regex<Pattern, Mode>>> {
    struct iterator;

  public:
    regex_match_view() requires std::default_initializable<V> = default;

    constexpr explicit regex_match_view(V base, static_regex<Pattern, Mode> regex);

    constexpr V base() const& requires std::copy_constructible<V>;
    constexpr V base() &&;
    constexpr iterator begin();
    constexpr std::default_sentinel end();
  };

  template<typename R, string_literal Pattern, mode Mode>
  regex_match_view(R&&, static_regex<Pattern, Mode>)
    -> regex_match_view<std::views::all_t<R>, static_regex<Pattern, Mode>>;

  namespace views {
    inline constexpr /* range-adaptor */ regex_match;
  }
} // namespace rx
```

`regex_match_view` is always an input range.
Its range value is an instantiation of `static_regex_match_result`.


## Class template `rx::submatches_view`

`submatches_view` adapts a `regex_match_view` to flatten and iterate over the specified `submatch` objects from each regex match result object of the `regex_match_view`.

```cpp
namespace rx {
  template<std::ranges::input_range V, int... Submatches>
    requires std::ranges::view<V>
  class submatches_view {};

  template<std::ranges::input_range V, int... Submatches>
    requires std::ranges::view<V> and /* static-regex-match-view-like<V> */
  class submatches_view<V, Submatches...>
    : std::ranges::view_interface<submatches_view<V, Submatches...>> {
    struct iterator;

  public:
    submatches_view() requires std::default_initializable<V> = default;

    constexpr explicit submatches_view(V base, std::integer_sequence<int, Submatches...>)
      requires (sizeof...(Submatches) > 0);

    template<std::ranges::input_range R>
      requires std::same_as<std::ranges::range_value_t<R>, int>
    constexpr explicit submatches_view(V base, R&& submatches)
      requires (sizeof...(Submatches) == 0);

    constexpr V base() const& requires std::copy_constructible<V>;
    constexpr V base() &&;
    constexpr iterator begin();
    constexpr std::default_sentinel_t end();
  }

  template<typename R, int... Submatches>
  submatches_view(R&&, std::integer_sequence<int, Submatches...>)
    -> submatches_view<std::views::all_t<R>, Submatches...>;

  template<typename R, typename Submatches>
  submatches_view(R&&, Submatches&&)
    -> submatches_view<std::views::all_t<R>>;

  namespace views {
    inline constexpr /* range-adaptor */ submatches;

    template<int... Submatches>
      requires (sizeof...(Submatches) > 0)
    inline constexpr /* range-adaptor */ static_submatches;
  }
} // namespace rx
```

A list of submatch indicies can be supplied to `submatches_view` either at compile time or at run time.
In either case, the indicies must each be less than the submatch count in the regex match result object, and greater than or equal to `-1`.
The index of `-1` denotes the substring that is not matched by the regex pattern.

Where a `submatches_view` contains the submatch index of `-1`, the iterator becomes a suffix iterator just before comparing equal to the sentinel.
If dereferenced, the suffix iterator returns a submatch object for the substring between the end of the last match and the end of the input.

`submatches_view` is always an input range.
It should be noted that `submatches_view::iterator` is a proxy iterator.

`views::static_submatches<Ints...>` is equivalent to `views::submatches(std::integer_sequence<int, Ints...>)`.


## Class template `rx::regex_replace_view`

`regex_replace_view` adapts a `regex_match_view` to lazily produce a range where matched substrings are substituted for a replacement string.

```cpp
namespace rx {
  template<std::ranges::input_range V, typename Fmt>
    requires std::ranges::view<V>
  class replace_view {};

  template<std::ranges::input_range V, string_literal Fmt>
    requires std::ranges::view<V> and /* static-regex-match-view-like<V> */
  class replace_view<V, fmt_t<Fmt>> : std::ranges::view_interface<replace_view<V, fmt_t<Fmt>>> {
      struct iterator;

  public:
      replace_view() requires std::default_initializable<V> = default;

      constexpr explicit replace_view(V base, fmt_t<Fmt> fmt);

      constexpr V base() const& requires std::copy_constructible<V>;
      constexpr V base() &&;
      constexpr iterator begin();
      constexpr std::default_sentinel_t end();
  };

  template<std::ranges::input_range V, std::ranges::bidirectional_range Fmt>
    requires std::ranges::view<V> and detail::static_regex_match_view_like<V> and std::ranges::view<Fmt>
  class replace_view<V, Fmt>
    : std::ranges::view_interface<replace_view<V, Fmt>> {
    struct iterator;

  public:
      replace_view() requires std::default_initializable<V> and std::default_initializable<Fmt> = default;

      constexpr explicit replace_view(V base, Fmt fmt);

      constexpr V base() const& requires std::copy_constructible<V>;
      constexpr V base() &&;
      constexpr iterator begin();
      constexpr std::default_sentinel_t end();
  };

  template<typename R, string_literal Fmt>
  replace_view(R&&, fmt_t<Fmt>)
    -> replace_view<std::views::all_t<R>, fmt_t<Fmt>>;

  template<typename R, std::ranges::bidirectional_range Fmt>
  replace_view(R&&, Fmt&&)
    -> replace_view<std::views::all_t<R>, std::views::all_t<Fmt>>;

  namespace views {
    inline constexpr /* range-adaptor */ replace;

    template<string_literal Fmt>
    inline constexpr /* range-adaptor */ static_replace;
  }
} // namespace rx
```

`replace_view` is always an input range.
It produces a range over the original input to the adapted `regex_match_view` in which all matches are substituted with the contents of a replacement string.
This replacement string is supplied in the form of either a bidirectional range, a c-style string, or a compile-time replacement string.
*(See documentation for `rx::regex_replace` for more details)*.

`views::static_replace<Fmt>` is equivalent to `views::replace(fmt<Fmt>)`.

## Class template `rx::regex_split_view`

`regex_split_view` splits an input string using a regular expression.

```cpp
namespace rx {
  template<std::ranges::bidirectional_range V, typename Regex>
    requires std::ranges::view<V>
  class regex_split_view {};

  template<std::ranges::bidirectional_range V, string_literal Pattern, mode Mode>
  requires std::ranges::view<V>
  class regex_split_view<V, static_regex<Pattern, Mode>>
    : std::ranges::view_interface<regex_split_view<V, static_regex<Pattern, Mode>>> {
      class iterator;
      class sentinel;

  public:
      regex_split_view() requires std::default_initializable<V> = default;
      constexpr explicit regex_split_view(V base, static_regex<Pattern, Mode> regex);

      V base() const& requires std::copy_constructible<V>;
      V base() && requires std::copy_constructible<V>;

      constexpr iterator begin();
      constexpr sentinel end();
      constexpr iterator end() requires std::ranges::common_range<V>;
  };

  template<typename R, string_literal Pattern, mode Mode>
  regex_split_view(R&&, static_regex<Pattern, Mode>)
    -> regex_split_view<std::views::all_t<R>, static_regex<Pattern, Mode>>;

  namespace views {
    inline constexpr /* range-adaptor */ regex_split;
  }
} // namespace rx
```

`r | views::regex_split(pattern)` produces the same result as `r | views::regex_match(pattern) | views::static_submatches<-1>`.
However, `regex_split_view` is a forward range, while the alternative results in an input range.
Additionally, the range value of `regex_split_view` is a common `std::ranges::subrange` of iterators to `r`, whereas `views::submatches` produces `submatch` objects.

The iterator for `submatches_view` is a proxy iterator.


## Algorithm function object `rx::regex_replace`

`regex_replace` substitutes regular expression matches in a input for a replacement string.

```cpp
namespace rx {
  template<string_literal Fmt> struct fmt_t;
  template<string_literal Fmt> inline constexpr fmt_t<Fmt> fmt;

  template<class I, class O>
  using regex_replace_result = std::ranges::in_out_result<I, O>;

  /* see below for template constraints */
  template<typename I, typename S, typename O, /* regex-like */ Regex, typename F, typename FmtS>
  constexpr auto regex_replace(I first, S last, O result, Regex pattern, F fmt, FmtS fmt_last)
    -> regex_replace_result<I, O>;

  template<typename R, typename O, /* regex-like */ Regex, typename FmtR>
  constexpr auto regex_replace(R&& r, O result, Regex pattern, FmtR&& fmt)
    -> regex_replace_result<std::ranges::borrowed_iterator_t<R>, O>;

  template<typename I, typename S, typename> O, /* regex-like */ Regex, typename CharT>
  constexpr auto regex_replace(I first, S last, O result, Regex pattern, std::basic_string_view<CharT> fmt)
    -> regex_replace_result<I, O>;

  template<typename R, typename O, /* regex-like */ Regex, typename CharT>
  constexpr auto regex_replace(R&& r, O result, Regex pattern, std::basic_string_view<CharT> fmt)
    -> regex_replace_result<std::ranges::borrowed_iterator_t<R>, O>;

  template<typename CharT, /* regex-like */ Regex>
  constexpr auto regex_replace(std::basic_string_view<CharT> sv, Regex pattern,
                               std::basic_string_view<std::type_identity_t<CharT>> fmt)
    -> std::basic_string<CharT>;

  template<typename CharT, /* regex-like */ Regex>
  constexpr auto regex_replace(const CharT* cstr, Regex pattern,
                               std::basic_string_view<std::type_identity_t<CharT>> fmt)
    -> std::basic_string<CharT>;


  template<typename I, typename S, typename O, /* static-regex-like */ Regex, string_literal Fmt>
  constexpr auto regex_replace(I first, S last, O result, Regex pattern, fmt_t<Fmt>)
    -> regex_replace_result<I, O>;

  template<typename R, typename O, /* static-regex-like */ Regex, string_literal Fmt>
  constexpr auto regex_replace(R&& r, O result, Regex pattern, fmt_t<Fmt>)
    -> regex_replace_result<std::ranges::borrowed_iterator_t<R>, O>;

  template<typename CharT, /* static-regex-like */ Regex, string_literal Fmt>
  constexpr auto regex_replace(std::basic_string_view<CharT> sv, Regex pattern, fmt_t<Fmt>)
    ->  std::basic_string<CharT>;

  template<typename CharT, /* static-regex-like */ Regex, string_literal Fmt>
  constexpr auto regex_replace(const CharT* cstr, Regex pattern, fmt_t<Fmt>)
    -> std::basic_string<CharT>;
} // namespace rx
```

`regex_replace` is an algorithm function object.
Invoking it copies the input range to either `output` or a `std::basic_string`, substituting all substrings matched by `pattern` with the replacement string `fmt`.

In the replacement string `fmt`, all occurences of `$N` (where `N` is a non-negative integer) are replaced with the corresponding submatch.
If `N` is greater than the number of submatches captured by `pattern`, an error is thrown.
If `N` refers to a capturing group which was not matched, `$N` is replaced with an empty string.

The template contraints for `regex_replace` are as follows:
+ All overloads:
  - `std::same_as<typename Regex::char_type, CharT>` holds.
  -  *(see note below)*
+ Iterator overloads (1, 3, 7):
  - `I` satisfies `std::bidirectional_iterator`.
  - `S` satisfies `std::sentinel_for<I>`.
  - `std::same_as<std::iter_value_t<I>, CharT>` holds.
+ Range overloads (2, 4, 8):
  - `R` satisfies `std::ranges::bidirectional_range`.
  - `std::same_as<std::ranges::range_value_t<R>, CharT>` holds.
+ Replacement string iterator overload (1):
  - `F` satisfies `std::bidirectional_iterator`.
  - `FmtS` satisfies `std::sentinel_for<F>`.
+ Replacement string range overload (2):
  - `FmtR` satisfies `std::ranges::bidirectional_range`.
  - `std::same_as<std::ranges::range_value_t<R>, CharT>` holds.
  - `std::convertible_to<FmtR, std::basic_string_view<CharT>>` **does not** hold.
+ Compile-time replacement string overloads (7, 8, 9, 10):
  - `std::same_as<typename decltype(Fmt)::value_type, CharT>` holds.

Note that for overloads (1, 2, 7, 8), there is an additional defaulted template parameter `typename CharT = Regex::char_type` which is not listed above.

Overloads 7-10 correspond to overloads 3-6, but instead take a compile-time replacement string, supplied as `fmt<"replacement-string">`.
For these, the existence of referenced capturing groups is checked at compile time, and if necessary a compile-time error is produced.
