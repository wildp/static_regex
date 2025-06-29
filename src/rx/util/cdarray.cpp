module;

#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <type_traits>

export module rx.util:cdarray;

// import std;

export namespace rx::detail
{
    /* WARNING: cdarray is NOT thread-safe */
    template<typename T, typename Allocator = std::allocator<T>>
    class cdarray;

    template<typename T>
    class cdarray_iterator
    {
    public:
        using iterator_category     = std::contiguous_iterator_tag;
        using value_type            = T;
        using difference_type       = std::ptrdiff_t;
        using pointer               = std::add_pointer_t<T>;
        using reference             = std::add_lvalue_reference_t<T>;

        constexpr reference operator*() const { return *it_; }
        constexpr pointer operator->() const { return &operator*(); }
        constexpr cdarray_iterator& operator++() noexcept { ++it_; return *this; }
        constexpr cdarray_iterator operator++(int) noexcept { cdarray_iterator tmp{ *this }; ++(*this); return tmp; }
        constexpr cdarray_iterator& operator--() noexcept { --it_; return *this; }
        constexpr cdarray_iterator operator--(int) noexcept { cdarray_iterator tmp{ *this }; --(*this); return tmp; }
        constexpr friend bool operator==(const cdarray_iterator&, const cdarray_iterator&) noexcept = default;
        constexpr friend std::strong_ordering operator<=>(const cdarray_iterator&, const cdarray_iterator&) noexcept = default;
        constexpr cdarray_iterator& operator+=(difference_type d) noexcept { it_ += d; return *this; }
        constexpr friend cdarray_iterator operator+(const cdarray_iterator lhs, difference_type d) noexcept { return lhs.it_ + d; }
        constexpr friend cdarray_iterator operator+(difference_type d, const cdarray_iterator& rhs) noexcept { return d + rhs.it_; }
        constexpr cdarray_iterator& operator-=(difference_type d) noexcept { it_ -= d; return *this; }
        constexpr friend cdarray_iterator operator-(const cdarray_iterator lhs, difference_type d) noexcept { return lhs.it_ - d; }
        constexpr friend difference_type operator-(const cdarray_iterator lhs, const cdarray_iterator rhs) noexcept { return lhs.it_ - rhs.it_; }
        constexpr reference operator[](difference_type d) const { return it_[d]; }

        constexpr cdarray_iterator() noexcept = default;

    private:
        pointer it_{ nullptr };

        template<typename, typename>
        friend class cdarray;
        
        constexpr cdarray_iterator(const pointer& ptr) : it_{ ptr } {}
    };

    template<typename T, typename Allocator>
    class cdarray
    {
        static_assert(std::is_destructible_v<T>);
        static_assert(std::contiguous_iterator<cdarray_iterator<T>>);
        static_assert(std::contiguous_iterator<cdarray_iterator<const T>>);

    public:
        using value_type             = T;
        using allocator_type         = Allocator;
        using pointer                = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer          = typename std::allocator_traits<Allocator>::const_pointer;
        using reference              = value_type&;
        using const_reference        = const value_type&;
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;
        using iterator               = cdarray_iterator<T>;
        using const_iterator         = cdarray_iterator<const T>;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        /* construct/copy/destroy */

        constexpr cdarray() noexcept(noexcept(Allocator())) :
                cdarray(Allocator())
        {
        }

        constexpr explicit cdarray(const Allocator& alloc) noexcept :
                alloc_{ alloc }
        {
        }

        constexpr explicit cdarray(size_type count, const Allocator& alloc = Allocator()) :
                alloc_{ alloc }
        {
            data_ = std::allocator_traits<Allocator>::allocate(alloc_, count),
            end_ = data_ + count;

            for (pointer p{ data_ }; p != end_; ++p)
                std::allocator_traits<Allocator>::construct(alloc_, p);

            new_counter();
        }


        constexpr cdarray(size_type count, const T& value, const Allocator& alloc = Allocator()) :
                alloc_{ alloc }
        {
            data_ = std::allocator_traits<Allocator>::allocate(alloc_, count),
            end_ = data_ + count;

            for (pointer p{ data_ }; p != end_; ++p)
                std::allocator_traits<Allocator>::construct(alloc_, p, value);

            new_counter();
        }

        template<class InputIter>
        constexpr cdarray(InputIter first, InputIter last, const Allocator& alloc = Allocator()) :
                alloc_{ alloc }
        {
            const std::size_t count{ std::distance(first, last) };
            data_ = std::allocator_traits<Allocator>::allocate(alloc_, count);
            end_ = data_ + count;

            for (; first != last; ++first)
                std::allocator_traits<Allocator>::construct(alloc_, end_, *first);

            new_counter();
        }

        template<typename R>
        requires (std::ranges::input_range<R> and std::convertible_to<std::ranges::range_reference_t<R>, T>)
        constexpr cdarray(std::from_range_t, R&& rg, const Allocator& alloc = Allocator()) :
                alloc_{ alloc }
        {
            const size_type count{ std::ranges::size(rg) };
            data_ = std::allocator_traits<Allocator>::allocate(alloc, count);
            end_ = data_ + count;

            for (const auto& item : rg)
                std::allocator_traits<Allocator>::construct(alloc_, end_, item);

            new_counter();
        }

        constexpr cdarray(const cdarray& other) noexcept :
                data_{ other.data_ },
                end_{ other.end_ },
                use_count_ptr_{ other.use_count_ptr_ },
                alloc_{ other.get_allocator() }
        {
            *use_count_ptr_ += 1;
        }

        constexpr cdarray(cdarray&& other) noexcept :
                data_{ std::exchange(other.data_, nullptr) },
                end_{ std::exchange(other.end_, nullptr) },
                use_count_ptr_{ std::exchange(other.use_count_ptr_, nullptr) },
                alloc_{ other.get_allocator() }
        {
        }
    
        constexpr cdarray(const cdarray& other, const std::type_identity_t<Allocator>& alloc) :
                data_{ other.data_ },
                end_{ other.end_ },
                use_count_ptr_{ other.use_count_ptr_ },
                alloc_{ alloc }
        {
            *use_count_ptr_ += 1;
        }

        constexpr cdarray(cdarray&& other, const std::type_identity_t<Allocator>& alloc) noexcept :
                data_{ std::exchange(other.data_, nullptr) },
                end_{ std::exchange(other.end_, nullptr) }, 
                use_count_ptr_{ std::exchange(other.use_count_ptr_, nullptr) },
                alloc_{ alloc }
        {
        }
        
        constexpr cdarray(std::initializer_list<T> init, const Allocator& alloc = Allocator()) :
                cdarray(init.begin(), init.end(), alloc)
        {
        }

        constexpr ~cdarray() { this->delete_if_needed(); }

        constexpr cdarray& operator=(const cdarray& other)
        {
            if (this == &other)
                return *this;

            data_ = other.data_;
            end_ = other.end_;
            use_count_ptr_ = other.use_count_ptr_;
            alloc_ = other.alloc_;

            *use_count_ptr_ += 1;

            return *this;
        }

        constexpr cdarray& operator=(cdarray&& other) noexcept(
            std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value or
            std::allocator_traits<Allocator>::is_always_equal::value)
        {
            this->delete_if_needed();
            data_ = std::exchange(other.data_, nullptr);
            end_ = std::exchange(other.end_, nullptr);
            use_count_ptr_ = std::exchange(other.use_count_ptr_, nullptr);
            alloc_ = other.alloc_;
            return *this;
        }

        constexpr allocator_type get_allocator() const noexcept { return alloc_; }

        /* misc operations */

        constexpr void fill(const T& value)
        {
            for (pointer p{ data_ }; p != end_; ++p)
                *p = value;
        }

        constexpr void swap(cdarray& other) noexcept(std::is_nothrow_swappable_v<T>)
        {
            std::swap(data_, other.data_);
            std::swap(end_, other.end_);
            std::swap(use_count_ptr_, other.use_count_ptr_);
            std::swap(alloc_, other.alloc_);
        }

        /* iterators */

        [[nodiscard]] constexpr iterator begin() { this->copy_if_needed(); return data_; }
        [[nodiscard]] constexpr const_iterator begin() const noexcept { return data_; }
        [[nodiscard]] constexpr iterator end() { this->copy_if_needed(); return end_; }
        [[nodiscard]] constexpr const_iterator end() const noexcept { return end_; }
        [[nodiscard]] constexpr reverse_iterator rbegin() { return std::make_reverse_iterator(this->end()); };
        [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return std::make_reverse_iterator(this->end()); };
        [[nodiscard]] constexpr reverse_iterator rend() { return std::make_reverse_iterator(this->begin()); };
        [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return std::make_reverse_iterator(this->begin()); }

        [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return this->begin(); }
        [[nodiscard]] constexpr const_iterator cend() const noexcept { return this->end(); }
        [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return this->rbegin(); }
        [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return this->rend(); }

        /* capacity */

        [[nodiscard]] constexpr bool empty() const noexcept { return (data_ == end_); }
        [[nodiscard]] constexpr size_type size() const noexcept { return std::distance(data_, end_); }
        [[nodiscard]] constexpr size_type max_size() const noexcept { return this->size(); };

        /* element access */

        [[nodiscard]] constexpr reference operator[](size_type n) { this->copy_if_needed(); return data_[n]; }
        [[nodiscard]] constexpr const_reference operator[](size_type n) const {return data_[n]; }
        [[nodiscard]] constexpr reference at(size_type n) { this->range_check(n); return this->operator[](n); }
        [[nodiscard]] constexpr const_reference at(size_type n) const { this->range_check(n); return this->operator[](n); }
        [[nodiscard]] constexpr const_reference cat(size_type n) const { return this->at(n); }
        [[nodiscard]] constexpr reference front() { this->copy_if_needed(); return *data_; }
        [[nodiscard]] constexpr const_reference front() const { return *data_; }
        [[nodiscard]] constexpr reference back() { this->copy_if_needed(); return *std::prev(end_); }
        [[nodiscard]] constexpr const_reference back() const { return *std::prev(end_); }

        /* data access */

        [[nodiscard]] constexpr T* data() noexcept { this->copy_if_needed(); return data_; }
        [[nodiscard]] constexpr const T* data() const noexcept { return data_; }

    private:
        constexpr void new_counter(size_type value = 1)
        {
            std::allocator<size_type> alloc;
            use_count_ptr_ = std::allocator_traits<decltype(alloc)>::allocate(alloc, 1);
            std::allocator_traits<decltype(alloc)>::construct(alloc, use_count_ptr_, value);
        }

        constexpr void delete_counter()
        {
            std::allocator<size_type> alloc;
            std::allocator_traits<decltype(alloc)>::deallocate(alloc, use_count_ptr_, 1);
        }

        constexpr void copy_if_needed()
        {
            if (use_count_ptr_ != nullptr and *use_count_ptr_ > 1)
            {
                size_type count{ this->size() };
                pointer first{ data_ };
                pointer last{ end_ };

                data_ = std::allocator_traits<Allocator>::allocate(alloc_, count);
                end_ = data_;

                while (first != last)
                {
                    std::allocator_traits<Allocator>::construct(alloc_, end_, *first);
                    ++first;
                    ++end_;
                }

                *use_count_ptr_ -= 1;
                new_counter(1);
            }
        }

        constexpr void delete_if_needed()
        {
            /* only call this from the destructor and move assignement operator */
            if (use_count_ptr_ != nullptr)
            {
                if (*use_count_ptr_ <= 1)
                {
                    *use_count_ptr_ = 0;
                    size_type count{ this->size() };

                    if constexpr (not std::is_trivially_destructible_v<T>)
                    {
                        for (pointer p{ data_ }; p != end_; ++p) 
                            std::allocator_traits<Allocator>::destroy(alloc_, p);
                    }

                    if (data_ != nullptr)
                        std::allocator_traits<Allocator>::deallocate(alloc_, data_, count);

                    delete_counter();
                }
                else
                {
                    *use_count_ptr_ -= 1;
                }
            }
        }

        constexpr void range_check(size_type n) const
        {
            if (n >= this->size())
                throw std::out_of_range("tag_vector::range_check: n >= this->size()");
        }

        pointer                     data_{ nullptr };
        pointer                     end_{ nullptr };
        size_type*                  use_count_ptr_{ nullptr };

        [[no_unique_address]]
        allocator_type              alloc_;
    };
}