module;

#include <cstddef>
#include <iterator>
#include <numeric>
#include <vector>

export module rx.ast:charclass;

// import std;
import rx.util;

export namespace rx::detail
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

    template<typename CharT>
    requires (not char_is_multibyte<CharT>)
    class char_class_impl
    {
    public:
        using char_type     = CharT;
        
        struct char_range
        {
            char_type lower;
            char_type upper;
        };

        constexpr explicit char_class_impl(named_character_class ncc, bool negate = false) :
            negated_{ negate }
        {
            insert(ncc);
        }

        constexpr explicit char_class_impl(bool negate = false) :
            negated_{ negate }
        {
        }

        constexpr void insert(char_type c);
        constexpr void insert(char_type first, char_type last);
        constexpr void insert(named_character_class ncc);
        constexpr void insert(const char_class_impl& cc);

        [[nodiscard]] constexpr bool is_negated() const noexcept { return negated_; }
        [[nodiscard]] constexpr const auto& get() const noexcept { return data_; }
        constexpr void make_negated(std::vector<char_range>& result) const;
    
        constexpr void make_caseless();

    private:
        constexpr std::size_t insert_impl(CharT c);

        std::vector<char_range> data_;
        bool                    negated_;
    };

    template<typename CharT>
    requires (not char_is_multibyte<CharT>)
    constexpr std::size_t char_class_impl<CharT>::insert_impl(const CharT c)
    {
        if (data_.empty())
        {
            data_.emplace_back(c, c);
            return 0;
        }

        std::size_t lb{ 0 };
        std::size_t ub{ data_.size() - 1 };

        while (true)
        {
            const std::size_t mid{ std::midpoint(lb, ub) };
            const auto [lower, upper]{ data_.at(mid) };

            if (lower != std::numeric_limits<char_type>::min() and c + 1 == lower)
            {
                /* extend range of pair by 1 */
                data_.at(mid).lower -= 1;

                /* attempt to merge with (mid - 1) */
                if (mid > 0 and (data_.at(mid).lower - data_.at(mid - 1).upper <= 1))
                {
                    data_.at(mid).lower = data_.at(mid - 1).lower;
                    data_.erase(std::next(data_.begin(), mid - 1));
                    return mid - 1;
                }
                else
                {
                    return mid;
                }
            }
            else if (upper != std::numeric_limits<char_type>::max() and c == upper + 1)
            {
                /* extend range of pair by 1 */
                data_.at(mid).upper += 1;

                /* attempt to merge with (mid + 1) */
                if (mid + 1 < data_.size() and (data_.at(mid + 1).lower - data_.at(mid).upper <= 1))
                {
                    data_.at(mid).upper = data_.at(mid + 1).upper;
                    data_.erase(std::next(data_.begin(), mid + 1));
                }
                
                return mid;
            }
            else if (c < lower)
            {
                if (mid == lb)
                {
                    /* insert pair before mid */
                    data_.emplace(std::next(data_.begin(), mid), c, c);
                    return mid;
                }
                else
                {
                    /* continue search */
                    ub = mid - 1;
                }

            }
            else if (c > upper)
            {
                if (mid == ub)
                {
                    /* insert pair after mid */
                    data_.emplace(std::next(data_.begin(), mid + 1), c, c);
                    return mid + 1;
                }
                else
                {
                    /* continue search */
                    lb = mid + 1;
                }
            }
            else /* (lower <= c and c <= upper) */
            {
                /* c is already in char class */
                return mid;
            }
        }
    }

    template<typename CharT>
    requires (not char_is_multibyte<CharT>)
    constexpr void char_class_impl<CharT>::insert(const char_type c)
    {
        insert_impl(c);
    }

    template<typename CharT>
    requires (not char_is_multibyte<CharT>)
    constexpr void char_class_impl<CharT>::insert(const char_type first, const char_type last)
    {
        const std::size_t target{ insert_impl(first) };

        auto& current{ data_.at(target) };

        if (current.upper > last)
            return; /* no need to merge */
        
        current.upper = last;
        
        /* attempt to merge */

        while (true)
        {
            if (target + 1 >= data_.size())
                break; /* target is at end of data_: can't merge */

            auto [lower, upper]{ data_.at(target + 1) };

            if (last >= upper)
            {
                /* range of (target) is a superset of (target + 1): erase (target + 1) */
                data_.erase(std::next(data_.begin(), target + 1));
            }
            else if (last + 1 >= lower)
            {
                /* we don't need to worry about signed integer overflow here, since if
                 * lower == numeric_limits<CharT>::max(), then upper must equal lower
                 * and so the previous branch (last >= upper) is taken instead          */

                /* ranges of (target) and (target + 1) partially overlap or are adjacent: merge */
                current.upper = upper;
                data_.erase(std::next(data_.begin(), target + 1));
                break;
            }
            else
            {
                /* ranges of (target) and (target + 1) do not overlap and are not adjacent: can't merge */
                break;
            }
        }
    }

    template<typename CharT>
    requires (not char_is_multibyte<CharT>)
    constexpr void char_class_impl<CharT>::insert(named_character_class ncc)
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
            insert('\x00', '\x7f');
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

    template<typename CharT>
    requires (not char_is_multibyte<CharT>)
    constexpr void char_class_impl<CharT>::insert(const char_class_impl<CharT>& cc)
    {
        std::vector<char_range> negated;

        if (cc.is_negated())
            make_negated(negated);

        const std::vector<char_range>& ref{ (cc.is_negated()) ? negated : cc.data_ };

        for (const auto& [lower, upper] : ref)
        {
            if (lower == upper)
                insert(lower);
            else
                insert(lower, upper);
        }
    }

    template<typename CharT>
    requires (not char_is_multibyte<CharT>)
    constexpr void char_class_impl<CharT>::make_negated(std::vector<char_range>& result) const
    {
        /* negate character ranges in cc */
        char_type tmp{ std::numeric_limits<CharT>::min() };
        bool final_insert{ true };

        for (const auto& [lower, upper] : data_)
        {
            if (tmp < lower)
                result.emplace_back(tmp, lower - 1);

            if (upper == std::numeric_limits<CharT>::max())
                final_insert = false;

            tmp = upper + 1;
        }

        if (final_insert)
            result.emplace_back(tmp, std::numeric_limits<CharT>::max());
    }


    template<typename CharT>
    requires (not char_is_multibyte<CharT>)
    constexpr void char_class_impl<CharT>::make_caseless()
    {
        static constexpr char_type offset{ 'a' - 'A' };

        std::vector<char_range> to_insert;

        for (const auto [lower, upper]: data_)
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