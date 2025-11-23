#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <rx/etc/bitcharset.hpp>
#include <rx/etc/charset.hpp>


namespace rx::detail
{
    enum class named_character_class : std::uint8_t
    {
        alphanumeric,
        alphabetic,
        ascii,
        blank,
        control,
        digits,
        graphical,
        lowercase,
        printable,
        punctuation,
        posix_whitespace,
        perl_whitespace,
        uppercase,
        word,
        hexdigits,
    };


    /* class definitions */

    class wide_char_class_impl
    {
    public:
        using char_type = char32_t;
        using underlying_type = charset<char_type>;

        constexpr explicit wide_char_class_impl(named_character_class ncc, bool negate = false) :
            negated_{ negate }, orig_negated_{ negate } { insert(ncc); }

        constexpr explicit wide_char_class_impl(bool negate = false) noexcept :
            negated_{ negate }, orig_negated_{ negate } {}

        constexpr void insert(char_type c) { data_.insert(c); }
        constexpr void insert(char_type first, char_type last) { data_.insert(first, last); };
        constexpr void insert(named_character_class ncc);
        constexpr void insert(const wide_char_class_impl& cc) { data_ |= cc.data_; };

        [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }
        [[nodiscard]] constexpr std::size_t count() const noexcept { return data_.count(); }
        [[nodiscard]] constexpr bool is_negated() const noexcept { return negated_; }
        [[nodiscard]] constexpr const auto& intervals() const noexcept { return data_.get_intervals(); }
        [[nodiscard]] constexpr const auto& get() const { return data_; }

        constexpr void normalise() noexcept { if (negated_) { data_.negate(); negated_ = false; } }
        constexpr void make_caseless();

        template<typename T>
        friend constexpr T make_denormalised(const T& original) noexcept;

    private:
        underlying_type data_;
        bool            negated_;
        bool            orig_negated_;
    };

    class narrow_char_class_impl
    {
    public:
        using char_type = char;
        using underlying_type = bitcharset<char>;

        constexpr explicit narrow_char_class_impl(named_character_class ncc, bool negate = false) noexcept :
            negated_{ negate }, orig_negated_{ negate } { insert(ncc); }

        constexpr explicit narrow_char_class_impl(bool negate = false) noexcept :
            negated_{ negate }, orig_negated_{ negate } {}

        constexpr void insert(char_type c) noexcept { data_.insert(c); }
        constexpr void insert(char_type first, char_type last) noexcept { data_.insert(first, last); }
        constexpr void insert(named_character_class ncc) noexcept;
        constexpr void insert(const narrow_char_class_impl& cc) noexcept { data_ |= cc.data_; };

        [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }
        [[nodiscard]] constexpr std::size_t count() const noexcept { return data_.count(); }
        [[nodiscard]] constexpr bool is_negated() const noexcept { return negated_; }
        [[nodiscard]] constexpr auto intervals() const { return data_.get_intervals(); }
        [[nodiscard]] constexpr const auto& get() const { return data_; }

        constexpr void normalise() noexcept { if (negated_) { data_.negate(); negated_ = false; } }
        constexpr void make_caseless() noexcept { data_.make_ascii_case_insensitive(); }

        template<typename T>
        friend constexpr T make_denormalised(const T& original) noexcept;

    private:
        underlying_type data_;
        bool            negated_;
        bool            orig_negated_;
    };


    /* member function definitions */

    constexpr void wide_char_class_impl::insert(named_character_class ncc)
    {
        switch (ncc)
        {
        case named_character_class::alphanumeric:
            insert('0', '9');
            [[fallthrough]];
        case named_character_class::alphabetic:
            insert('A', 'Z');
            insert('a', 'z');
            break;
        case named_character_class::ascii:
            insert('\x00', '\x7F');
            break;
        case named_character_class::blank:
            insert('\t');
            insert(' ');
            break;
        case named_character_class::control:
            insert('\x00', '\x1F');
            insert('\x7F');
            break;
        case named_character_class::digits:
            insert('0', '9');
            break;
        case named_character_class::graphical:
            insert('!', '~');
            break;
        case named_character_class::lowercase:
            insert('a', 'z');
            break;
        case named_character_class::printable:
            insert(' ', '~');
            break;
        case named_character_class::punctuation:
            insert('!', '/');
            insert(':', '@');
            insert('[', '`');
            insert('{', '~');
            break;
        case named_character_class::posix_whitespace:
            insert('\v');
            [[fallthrough]];
        case named_character_class::perl_whitespace:
            insert('\t');
            insert('\n');
            insert('\f');
            insert('\r');
            insert(' ');
            break;
        case named_character_class::uppercase:
            insert('A', 'Z');
            break;
        case named_character_class::word:
            insert('0', '9');
            insert('A', 'Z');
            insert('a', 'z');
            insert('_');
            break;
        case named_character_class::hexdigits:
            insert('0', '9');
            insert('A', 'F');
            insert('a', 'f');
            break;
        }
    }

    constexpr void wide_char_class_impl::make_caseless()
    {
        static constexpr char_type offset{ 'a' - 'A' };

        std::vector<underlying_type::char_interval> to_insert;

        for (const auto [lower, upper] : data_.get_intervals())
        {
            if (upper >= 'a' and lower <= 'Z')
            {
                /* [lower, upper] only covers both uppercase and lowercase letters */
                to_insert.emplace_back(std::max<char_type>(lower, 'A') + offset, std::min<char_type>(upper, 'a'));
                to_insert.emplace_back(std::max<char_type>(lower, 'A'), std::min<char_type>(upper, 'a') - offset);
            }
            else if (upper >= 'A' and lower <= 'Z')
            {
                /* [lower, upper] only covers uppercase letters */
                to_insert.emplace_back(std::max<char_type>(lower, 'A') + offset, std::min<char_type>(upper, 'A') + offset);
            }
            else if (upper >= 'a' and lower <= 'z')
            {
                /* [lower, upper] only covers lowercase letters */
                to_insert.emplace_back(std::max<char_type>(lower, 'a') - offset, std::min<char_type>(upper, 'a') - offset);
            }
        }
        
        for (const auto [lower, upper] : to_insert)
            insert(lower, upper);
    }
        
    constexpr void narrow_char_class_impl::insert(named_character_class ncc) noexcept
    {
        using bcs = bitcharset<char>;
        using p = typename bcs::char_interval;

        switch (ncc)
        {
        case named_character_class::alphanumeric:
            data_ |= bcs{ p{ '0', '9' }, p{ 'A', 'Z' }, p{ 'a', 'z' } };
            break;
        case named_character_class::alphabetic:
            data_ |= bcs{ p{ 'A', 'Z' }, p{ 'a', 'z' } };
            break;
        case named_character_class::ascii:
            data_ |= bcs{ p{ 0x00, 0x7F } };
            break;
        case named_character_class::blank:
            data_ |= bcs{ '\t', ' ' };
            break;
        case named_character_class::control:
            data_ |= bcs{ p{ 0x00, 0x1F }, 0x7F };
            break;
        case named_character_class::digits:
            data_ |= bcs{ p{ '0', '9' } };
            break;
        case named_character_class::graphical:
            data_ |= bcs{ p{ '!', '~' } };
            break;
        case named_character_class::lowercase:
            data_ |= bcs{ p{ 'a', 'z' } };
            break;
        case named_character_class::printable:
            data_ |= bcs{ p{ ' ', '~' } };
            break;
        case named_character_class::punctuation:
            data_ |= bcs{ p{ '!', '/' }, p{ ':', '@' }, p{ '[', '`' }, p{ '{', '~' } };
            break;
        case named_character_class::posix_whitespace:
            data_ |= bcs{ '\v', '\t', '\n', '\f', '\r', ' ' };
            break;
        case named_character_class::perl_whitespace:
            data_ |= bcs{ '\t', '\n', '\f', '\r', ' '};
            break;
        case named_character_class::uppercase:
            data_ |= bcs{ p{ 'A', 'Z' } };
            break;
        case named_character_class::word:
            data_ |= bcs{ p{ '0', '9' }, p{ 'A', 'Z' }, p{ 'a', 'z' }, '_' };
            break;
        case named_character_class::hexdigits:
            data_ |= bcs{ p{ '0', '9' }, p{ 'A', 'F' }, p{ 'a', 'f' } };
            break;
        }
    }

    template<typename T>
    [[maybe_unused]] [[nodiscard]] constexpr T make_denormalised(const T& original) noexcept
    {
        static_assert(std::same_as<T, wide_char_class_impl> or std::same_as<T, narrow_char_class_impl>);

        T result{ original.orig_negated_ };
        if (original.negated_ == false and original.orig_negated_ == true)
            result.data_ = ~original.data_;
        else
            result.data_ = original.data_;
        return result;
    }
}