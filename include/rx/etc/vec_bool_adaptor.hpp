#pragma once

#include <ranges>
#include <vector>

#ifdef RX_USE_BOOST_DYNAMIC_BITSET
    #include <boost/dynamic_bitset.hpp>
#endif

namespace rx::detail
{
#ifndef RX_USE_BOOST_DYNAMIC_BITSET
    class vec_bool_adaptor
    {
    public:
        vec_bool_adaptor() = default;

        constexpr vec_bool_adaptor(std::size_t sz, bool x = false)
            : data_(sz, x) {}

        [[nodiscard]] constexpr auto size() const { return data_.size(); }
        [[nodiscard]] constexpr auto operator[](std::size_t n) { return data_[n]; }
        [[nodiscard]] constexpr auto operator[](std::size_t n) const { return data_[n]; }
        [[nodiscard]] constexpr auto at(std::size_t n) { return data_.at(n); }
        [[nodiscard]] constexpr auto at(std::size_t n) const { return data_.at(n); }
        [[nodiscard]] constexpr auto begin() { return data_.begin(); }
        [[nodiscard]] constexpr auto begin() const { return data_.begin(); }
        [[nodiscard]] constexpr auto end() { return data_.end(); }
        [[nodiscard]] constexpr auto end() const { return data_.end(); }
        [[nodiscard]] constexpr auto cbegin() const { return data_.cbegin(); }
        [[nodiscard]] constexpr auto cend() const { return data_.cend(); }
        [[nodiscard]] constexpr auto rbegin() { return data_.rbegin(); }
        [[nodiscard]] constexpr auto rbegin() const { return data_.rbegin(); }
        [[nodiscard]] constexpr auto rend() { return data_.rend(); }
        [[nodiscard]] constexpr auto rend() const { return data_.rend(); }
        [[nodiscard]] constexpr auto crbegin() const { return data_.crbegin(); }
        [[nodiscard]] constexpr auto crend() const { return data_.crend(); }

        [[nodiscard]] constexpr std::size_t count() const
        {
            std::size_t result{ 0 };
            for (auto a : data_)
                result += a;
            return result;
        }

        constexpr void resize(std::size_t sz, bool x = false)
        {
            data_.resize(sz, x);
        }

        constexpr void push_back(bool x)
        {
            data_.push_back(x);
        }

        constexpr void reset()
        {
            for (auto a : data_)
                a = false;
        }

        constexpr void flip()
        {
            for (auto a : data_)
                a.flip();
        }

        constexpr vec_bool_adaptor& operator&=(const vec_bool_adaptor& other)
        {
            auto zv = std::views::zip(data_, other.data_);
            for (auto [a, b] : zv)
                a = (a and b);
            return *this;
        }

        constexpr vec_bool_adaptor& operator|=(const vec_bool_adaptor& other)
        {
            auto zv = std::views::zip(data_, other.data_);
            for (auto [a, b] : zv)
                a = (a or b);
            return *this;
        }

        constexpr vec_bool_adaptor& operator^=(const vec_bool_adaptor& other)
        {
            auto zv = std::views::zip(data_, other.data_);
            for (auto [a, b] : zv)
                a = (a != b);
            return *this;
        }

        constexpr vec_bool_adaptor& operator-=(const vec_bool_adaptor& other)
        {
            auto zv = std::views::zip(data_, other.data_);
            for (auto [a, b] : zv)
                a = (a and not b);
            return *this;
        }

        [[nodiscard]] constexpr vec_bool_adaptor operator~() const
        {
            vec_bool_adaptor result;
            result.data_.reserve(data_.size());
            for (auto a : data_)
                result.data_.push_back(not a);
            return result;
        }

        [[nodiscard]] friend constexpr vec_bool_adaptor operator&(const vec_bool_adaptor& x, const vec_bool_adaptor& y)
        {
            auto zv = std::views::zip(x.data_, y.data_);
            vec_bool_adaptor result;
            result.data_.reserve(zv.size());
            for (auto [a, b] : zv)
                result.data_.push_back(a and b);
            return result;
        }

        [[nodiscard]] friend constexpr vec_bool_adaptor operator|(const vec_bool_adaptor& x, const vec_bool_adaptor& y)
        {
            auto zv = std::views::zip(x.data_, y.data_);
            vec_bool_adaptor result;
            result.data_.reserve(zv.size());
            for (auto [a, b] : zv)
                result.data_.push_back(a or b);
            return result;
        }

        [[nodiscard]] friend constexpr vec_bool_adaptor operator^(const vec_bool_adaptor& x, const vec_bool_adaptor& y)
        {
            auto zv = std::views::zip(x.data_, y.data_);
            vec_bool_adaptor result;
            result.data_.reserve(zv.size());
            for (auto [a, b] : zv)
                result.data_.push_back(a != b);
            return result;
        }

        [[nodiscard]] friend constexpr vec_bool_adaptor operator-(const vec_bool_adaptor& x, const vec_bool_adaptor& y)
        {
            auto zv = std::views::zip(x.data_, y.data_);
            vec_bool_adaptor result;
            result.data_.reserve(zv.size());
            for (auto [a, b] : zv)
                result.data_.push_back(a and not b);
            return result;
        }

        [[nodiscard]] friend constexpr bool operator==(const vec_bool_adaptor& x, const vec_bool_adaptor& y)
        {
            return x.data_ == y.data_;
        }

        [[nodiscard]] friend constexpr auto operator<=>(const vec_bool_adaptor& x, const vec_bool_adaptor& y)
        {
            return x.data_ <=> y.data_;
        }


    private:
        std::vector<bool> data_;
    };
#endif

#ifdef RX_USE_BOOST_DYNAMIC_BITSET
    using bitset_t = boost::dynamic_bitset<std::size_t>;
#else
    using bitset_t = vec_bool_adaptor;
#endif
}
