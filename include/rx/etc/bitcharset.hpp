#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <utility>
#include <vector>


namespace rx::detail
{
    template<typename CharT>
    class bitcharset
    {
        using integer_t = std::uint64_t;
        static constexpr std::size_t byte_bits{ std::numeric_limits<unsigned char>::digits }; 
        static constexpr std::size_t integer_bits{ std::numeric_limits<integer_t>::digits };
        static constexpr std::size_t array_size{ (0b1uz << (sizeof(CharT) * byte_bits)) / integer_bits };

        static constexpr std::size_t acceptable_numbers_of_bits_in_a_byte{ 8 };
        static_assert(byte_bits == acceptable_numbers_of_bits_in_a_byte);
        static_assert(sizeof(CharT) < sizeof(int));

    public:
        using char_type = CharT;
        using char_interval = std::pair<char_type, char_type>;

        
        constexpr bitcharset() noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] = 0;
        };


        /* observers */

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            integer_t result{ 0uz };
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result |= data_[i];
            return (result == 0); 
        }

        [[nodiscard]] constexpr std::size_t count() const noexcept
        {
            std::size_t result{ 0 };
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result += std::popcount(data_[i]);
            return result;
        }

        [[nodiscard]] constexpr std::vector<char_interval> get_intervals() const
        {
            constexpr auto offset_max{ static_cast<int>(integer_bits) };

            std::vector<char_interval> result;

            /* widen to allow position to equal std::numeric_limits<char_type>::max() + 1 */
            int position{ std::numeric_limits<char_type>::min() };

            for (std::size_t i{ 0 }; i < array_size; ++i)
            {
                integer_t tmp{ data_[i] };
                int offset{ 0 };

                while (true)
                {
                    const int zeros{ std::countr_zero(tmp) };

                    position += std::min(zeros, offset_max - offset);
                    offset += zeros;

                    if (offset >= offset_max)
                        break;

                    tmp >>= zeros;

                    const int ones{ std::countr_one(tmp) }; /* note: ones >= 1 is always true */
                    const auto prev_pos{ position }; 
                    
                    position += ones;
                    offset += ones;
                    
                    if (not result.empty() and result.back().second == prev_pos - 1)
                        result.back().second = position - 1;
                    else
                        result.emplace_back(prev_pos, position - 1);

                    if (offset >= offset_max)
                        break;

                    tmp >>= ones;
                }
            }

            return result;
        }


        /* modifiers */

        constexpr void clear() noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] = 0;
        }

        constexpr void negate() noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] = ~data_[i];
        }

        constexpr void insert(char_type c) noexcept
        {
            /* widen to accommodate signed chars */
            const auto input{ static_cast<int>(c) - std::numeric_limits<char_type>::min() };

            data_[input / integer_bits] |= 0b1uz << (input % integer_bits);
        }

        constexpr void insert(char_type first, char_type last) noexcept /* preconditions: first <= last */
        {
            /* widen to accommodate signed chars and to ensure last + 1 > last */
            const auto beg{ static_cast<int>(first) - std::numeric_limits<char_type>::min() };
            const auto end{ static_cast<int>(last) - std::numeric_limits<char_type>::min() + 1 }; 

            const integer_t select1{ (beg / integer_bits) };
            const integer_t select2{ (end / integer_bits) };
            const integer_t mask1{ (0b1uz << (beg % integer_bits)) - 1 };
            const integer_t mask2{ (0b1uz << (end % integer_bits)) - 1 };

            for (std::size_t i{ 0 }; i < array_size; ++i)
            {
                data_[i] |= (((i == select1) * mask1) | ((i < select1) * ~0uz))
                            ^ (((i == select2) * mask2) | ((i < select2) * ~0uz));
            }
        }


        /* operators */

        constexpr bitcharset& operator&=(const bitcharset& other) noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] &= other.data_[i];
            return *this;
        }

        constexpr bitcharset& operator|=(const bitcharset& other) noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] |= other.data_[i];
            return *this;
        }

        constexpr bitcharset& operator^=(const bitcharset& other) noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] ^= other.data_[i];
            return *this;
        }

        constexpr bitcharset& operator-=(const bitcharset& other) noexcept
        {
            for (std::size_t i{ 0 }; i < array_size; ++i)
                data_[i] &= ~other.data_[i];
            return *this;
        }

        friend constexpr bitcharset operator&(const bitcharset& lhs, const bitcharset& rhs) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = lhs.data_[i] & rhs.data_[i];
            return result;
        }

        friend constexpr bitcharset operator|(const bitcharset& lhs, const bitcharset& rhs) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = lhs.data_[i] | rhs.data_[i];
            return result;
        }

        friend constexpr bitcharset operator^(const bitcharset& lhs, const bitcharset& rhs) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = lhs.data_[i] ^ rhs.data_[i];
            return result;
        }

        friend constexpr bitcharset operator-(const bitcharset& lhs, const bitcharset& rhs) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = lhs.data_[i] & ~rhs.data_[i];
            return result;
        }

        friend constexpr bitcharset operator~(const bitcharset& arg) noexcept
        {
            bitcharset result;
            for (std::size_t i{ 0 }; i < array_size; ++i)
                result.data_[i] = ~arg.data_[i];
            return result;
        }

        friend constexpr bool operator==(const bitcharset&, const bitcharset&) noexcept = default;
        friend constexpr auto operator<=>(const bitcharset&, const bitcharset&) noexcept = default;


        /* additional member functions */

        constexpr void make_ascii_case_insensitive() noexcept requires std::same_as<char_type, char>
        {
            using integer_t = std::uint64_t;
            constexpr auto bits{ 64 };

            constexpr auto uppercase_beg{ static_cast<int>('a') - std::numeric_limits<char>::min() };
            constexpr auto uppercase_end{ static_cast<int>('z') - std::numeric_limits<char>::min() + 1 }; 
            constexpr auto lowercase_beg{ static_cast<int>('A') - std::numeric_limits<char>::min() };
            constexpr auto lowercase_end{ static_cast<int>('Z') - std::numeric_limits<char>::min() + 1 }; 

            /* ensure that A-Za-z lies in the same integer_t in bitcharset<char> */
            constexpr auto index{ uppercase_beg / bits };
            static_assert(uppercase_beg / bits == index);
            static_assert(uppercase_end / bits == index);
            static_assert(lowercase_beg / bits == index);
            static_assert(lowercase_end / bits == index);

            constexpr integer_t uppercase_mask{ ((0b1uz << (uppercase_beg % bits)) - 1) ^ ((0b1uz << (uppercase_end % bits)) - 1) };
            constexpr integer_t lowercase_mask{ ((0b1uz << (lowercase_beg % bits)) - 1) ^ ((0b1uz << (lowercase_end % bits)) - 1) };
            constexpr int uppercase_offset{ std::countr_zero(uppercase_mask) };
            constexpr int lowercase_offset{ std::countr_zero(lowercase_mask) };

            integer_t mask1{ (data_[index] & lowercase_mask) >> lowercase_offset };
            integer_t mask2{ (data_[index] & uppercase_mask) >> uppercase_offset };

            data_[index] |= (mask1 << uppercase_offset);
            data_[index] |= (mask2 << lowercase_offset);
        }

    private:
        std::array<integer_t, array_size> data_;
    };
}