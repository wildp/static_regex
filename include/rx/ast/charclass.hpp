// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <vector>

#include "../etc/bitcharset.hpp"
#include "../etc/charset.hpp"
#include "../etc/static_charset.hpp"


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

    template<bool IsNarrow>
    class char_class_impl
    {
    public:
        using char_type = std::conditional_t<IsNarrow, char, char32_t>;
        using underlying_type = std::conditional_t<IsNarrow, bitcharset<char_type>, charset<char_type>>;

        constexpr explicit char_class_impl(named_character_class ncc, bool negate = false) noexcept :
            negated_{ negate }, orig_negated_{ negate } { insert(ncc); }

        constexpr explicit char_class_impl(bool negate = false) noexcept :
            negated_{ negate }, orig_negated_{ negate } {}

        constexpr void insert(char_type c) noexcept(IsNarrow) { data_.insert(c); }
        constexpr void insert(char_type first, char_type last) noexcept(IsNarrow) { data_.insert(first, last); }
        constexpr void insert(named_character_class ncc) noexcept(IsNarrow);
        constexpr void insert(const char_class_impl& cc) noexcept(IsNarrow) { data_ |= cc.data_; };

        [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }
        [[nodiscard]] constexpr std::size_t count() const noexcept { return data_.count(); }
        [[nodiscard]] constexpr bool is_negated() const noexcept { return negated_; }
        [[nodiscard]] constexpr auto intervals() const noexcept(not IsNarrow) { return data_.get_intervals(); }
        [[nodiscard]] constexpr const auto& get() const noexcept { return data_; }

        constexpr void normalise() noexcept { if (negated_) { data_.negate(); negated_ = false; } }
        constexpr void make_caseless() noexcept(IsNarrow);

        [[nodiscard]] constexpr char_class_impl denormalise() const noexcept(IsNarrow);

    private:
        underlying_type data_;
        bool            negated_;
        bool            orig_negated_;
    };


    /* member function definitions */

    template<bool IsNarrow>
    constexpr void char_class_impl<IsNarrow>::make_caseless() noexcept(IsNarrow)
    {
        if constexpr (IsNarrow)
        {
            data_.make_ascii_case_insensitive();
        }
        else
        {
            static constexpr char_type offset{ 'a' - 'A' };
            std::vector<typename underlying_type::char_interval> to_insert;

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
    }

    template<bool IsNarrow>
    constexpr void char_class_impl<IsNarrow>::insert(named_character_class ncc) noexcept(IsNarrow)
    {
        using cs = nontransient_constexpr_version_of_t<underlying_type>;
        using p = cs::char_interval;

        static constexpr cs alnum{ p{ '0', '9' }, p{ 'A', 'Z' }, p{ 'a', 'z' } };
        static constexpr cs alpha{ p{ 'A', 'Z' }, p{ 'a', 'z' } };
        static constexpr cs ascii{ p{ 0x00, 0x7F } };
        static constexpr cs blank{ '\t', ' ' };
        static constexpr cs cntrl{ p{ 0x00, 0x1F }, 0x7F };
        static constexpr cs digit{ p{ '0', '9' } };
        static constexpr cs graph{ p{ '!', '~' } };
        static constexpr cs lower{ p{ 'a', 'z' } };
        static constexpr cs print{ p{ ' ', '~' } };
        static constexpr cs punct{ p{ '!', '/' }, p{ ':', '@' }, p{ '[', '`' }, p{ '{', '~' } };
        static constexpr cs space{ '\v', '\t', '\n', '\f', '\r', ' ' };
        static constexpr cs perls{ '\t', '\n', '\f', '\r', ' ' };
        static constexpr cs upper{ p{ 'A', 'Z' } };
        static constexpr cs word{ p{ '0', '9' }, p{ 'A', 'Z' }, p{ 'a', 'z' }, '_' };
        static constexpr cs xdigit{ p{ '0', '9' }, p{ 'A', 'F' }, p{ 'a', 'f' } };

        switch (ncc)
        {
        case named_character_class::alphanumeric:     data_ |= alnum;  break;
        case named_character_class::alphabetic:       data_ |= alpha;  break;
        case named_character_class::ascii:            data_ |= ascii;  break;
        case named_character_class::blank:            data_ |= blank;  break;
        case named_character_class::control:          data_ |= cntrl;  break;
        case named_character_class::digits:           data_ |= digit;  break;
        case named_character_class::graphical:        data_ |= graph;  break;
        case named_character_class::lowercase:        data_ |= lower;  break;
        case named_character_class::printable:        data_ |= print;  break;
        case named_character_class::punctuation:      data_ |= punct;  break;
        case named_character_class::posix_whitespace: data_ |= space;  break;
        case named_character_class::perl_whitespace:  data_ |= perls;  break;
        case named_character_class::uppercase:        data_ |= upper;  break;
        case named_character_class::word:             data_ |= word;   break;
        case named_character_class::hexdigits:        data_ |= xdigit; break;
        }
    }

    template<bool IsNarrow>
    constexpr auto char_class_impl<IsNarrow>::denormalise() const noexcept(IsNarrow) -> char_class_impl
    {
        char_class_impl result{ orig_negated_ };
        if (negated_ == false and orig_negated_ == true)
            result.data_ = ~data_;
        else
            result.data_ = data_;
        return result;
    }
}
