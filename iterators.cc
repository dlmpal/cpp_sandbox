// Iterators are a generalization of pointers, which enables
// C++ programs to handle all containers in uniform manner.
// Iterator semantics are similar to pointer semantics.
// There exist 6 categories of iterators, which
// can be ordered as follows, based on operation
// and storage requirements
// 1) InputIterator (read-only) and OutputIterator (write-only)
// 2) ForwardIterator (both read and write)
// 3) BiDirectionalIterator
// 4) RandomAccessIterator
// 5) ContiguousIterator
// Both InputIterator and OutputIterator can scan the container only forwards
// and only once. ForwardIterator can scan the container forward multiple times,
// while having both read and write priviledges. A BiDirectionalOperator has the
// same properties as the ForwardIterator, while also being able to scan the container
// backwards. A RandomAccessIterator expands upon the BiDirectionalIterator, having the
// ability to scan the container non-sequentially (jumping-around). Finally, ContiguousIterator
// has the added property that logically adjacent elements of the container are also adjacent
// in memory.
// The hierarchical nature of the iterator categories means that, e.g., a BiDirectionalIterator
// is also a ForwardIterator, which in-turn is both a InputIterator and OutputIterator.

#include <algorithm>
#include <numeric>
#include <ranges>
#include <format>
#include <iostream>

#define USE_CUSTOM_ITER 1
#define USE_SPACESHIP 1

// The DynamicArray class is wrapper around
// a heap-allocated array. It is, in essence,
// similar to std::vector, with reduced functionality.
// It will be a used as an example container, for which a
// custom iterator must be created.
template <typename T>
class DynamicArray
{
public:
    DynamicArray(std::size_t size)
        : size_(size),
          values_(new T[size]{})
    {
    }

    DynamicArray(std::size_t size, const T &value)
        : DynamicArray(size)
    {
        std::fill(values_, values_ + size_, value);
    }

    DynamicArray(std::initializer_list<T> l)
        : DynamicArray(l.size())
    {
        std::copy(l.begin(), l.end(), values_);
    }

    ~DynamicArray()
    {
        if (values_ != nullptr)
        {
            delete[] values_;
        }
    }

    DynamicArray(const DynamicArray &other)
        : DynamicArray(other.size_)
    {
        std::copy(other.values_, other.values_ + other.size_, values_);
    }

    DynamicArray(DynamicArray &&other)
        : DynamicArray(other.size_)
    {
        std::copy(other.values_, other.values_ + other.size_, values_);
        other.size_ = 0;
        delete[] other.values_;
        other.values_ = nullptr;
    }

    DynamicArray &operator=(const DynamicArray &other)
    {
        size_ = other.size_;
        delete[] values_;
        values_ = new T[size_];
        std::copy(other.values_, other.values_ + other.size_, values_);
        return *this;
    }

    DynamicArray &operator=(DynamicArray &&other)
    {
        size_ = other.size_;
        delete values_;
        values_ = new T[size_];
        std::copy(other.values_, other.values_ + other.size_, values_);
        other.size_ = 0;
        delete other.values_;
        return *this;
    }

    T &operator[](int idx)
    {
        if (idx >= size_)
        {
            throw std::range_error(std::format("Invalid index {} for DynamicArray of size {}\n", idx, size_));
        }
        return values_[idx];
    }

#if (USE_CUSTOM_ITER == 1)
    // Custom iterator for DynamicArray
    class iterator // : public std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, T *, T &>
    {
    public:
        // An iterator should specify these 5 properties:
        // 1) Category
        // 2) Difference type
        // 3) Value type
        // 4) Pointer type
        // 5) Reference type
        // Tags are used when interacting with STL functions (such as those in <algorithm>),
        // and help select the most appropriate implementation of such functions.
        // Instead of defining them, the custom iterator class can inherit from std::iterator,
        // but this practice is deprecated since C++17
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = value_type *;
        using reference = value_type &;

        // Iterators should be constructible, copy-constructible, copy-assignable,
        // swappable and destructuble.
        // By only defining this constructor, the compiler will generate the rest
        iterator(pointer ptr)
            : ptr_(ptr)
        {
        }

        // Default-construction is a requirement for ForwardIterator
        iterator() = default;

        // Dereference operator
        // Returns reference to the object the iterator points to
        // Required by: OutputIterator
        reference operator*()
        {
            return *ptr_;
        }

        // Const dereference operator
        // Returns const reference to the object the iterator points to
        // Required by: InputIterator
        const reference operator*() const
        {
            return *ptr_;
        }

        // Subscript operator
        // Returns a reference to the
        // Required by: RandomAccessIterator
        reference operator[](const difference_type &diff) const
        {
            return ptr_[diff];
        }

        // Pre-increment operator: ++iterator
        // Increment and return
        // Required by: InputIterator, OutputIterator
        iterator &operator++()
        {
            ptr_++;
            return *this;
        }

        // Post-increment operator: iterator++
        // Return, then increment
        // Required by: InputIterator, OutputIterator
        iterator operator++(int)
        {
            iterator tmp(*this);
            ++*this; // Pre-increment
            return tmp;
        }

        // Pre-decrement operator: --iterator
        // Decrement and return
        // Required by: BiDirectionalIterator
        iterator &operator--()
        {
            ptr_--;
            return *this;
        }

        // Post-decrement operator: iterator--
        // Return, then decrement
        // Required by: BiDirectionalIterator
        iterator operator--(int)
        {
            iterator tmp(*this);
            --*this; // Pre-decrement
            return tmp;
        }

        // it += diff
        // Required by: RandomAccessIterator
        iterator &operator+=(difference_type diff)
        {
            ptr_ += diff;
            return *this;
        }

        // it -= diff
        // Required by: RandomAccessIterator
        iterator &operator-=(difference_type diff)
        {
            ptr_ -= diff;
            return *this;
        }

        // Equality operator
        // Required by: ForwardIterator
        friend bool operator==(const iterator &lhs, const iterator &rhs)
        {
            return lhs.ptr_ == rhs.ptr_;
        }

        // Ineqquality operator
        // Required by: ForwardIterator
        // Note: Implicitly constructed from operator== since C++20
        friend bool operator!=(const iterator &lhs, const iterator &rhs)
        {
            return lhs.ptr_ != rhs.ptr_;
        }

// RandomAccessIterator requires that all comparison operators are defined
#if (USE_SPACESHIP == 1)
        // Spaceship operator
        // Required by: RandomAcessIterator
        friend bool operator<=>(const iterator &lhs, const iterator &rhs)
        {
            return lhs.ptr_ <=> rhs.ptr_;
        }
#else
        // Less-than operator
        // Required by: RandomAccessIterator
        friend bool operator<(const iterator &lhs, const iterator &rhs)
        {
            return lhs.ptr_ < rhs.ptr_;
        }

        // Less-than or equal to operator
        // Required by: RandomAccessIterator
        friend bool operator<=(const iterator &lhs, const iterator &rhs)
        {
            return lhs.ptr_ <= rhs.ptr_;
        }

        // Greater-than operator
        // Required by: RandomAccessIterator
        friend bool operator>(const iterator &lhs, const iterator &rhs)
        {
            return lhs.ptr_ > rhs.ptr_;
        }

        // Greater-than or equal to operator
        // Required by: RandomAccessIterator
        friend bool operator>=(const iterator &lhs, const iterator &rhs)
        {
            return lhs.ptr_ >= rhs.ptr_;
        }
#endif
        // result (diff) = lhs (it) - rhs (it)
        // Required by: RandomAccessIterator
        friend difference_type operator-(const iterator &lhs, const iterator &rhs)
        {
            return static_cast<difference_type>(lhs.ptr_ - rhs.ptr_);
        }

        // result (it) =  it - diff
        // Required by: RandomAccessIterator
        friend iterator operator-(const iterator &it, const difference_type &diff)
        {
            return iterator(it.ptr_ - diff);
        }

        // result (it) = diff - it
        // Required by: RandomAccessIterator
        friend iterator operator-(const difference_type &diff, const iterator &it)
        {
            return iterator(diff - it.ptr_);
        }

        // result (it) = it + diff
        // Required by: RandomAccessIterator
        friend iterator operator+(const iterator &it, const difference_type &diff)
        {
            return iterator(it.ptr_ + diff);
        }

        // result (it) = diff + it
        // Required by: RandomAccessIterator
        friend iterator operator+(const difference_type &diff, const iterator &it)
        {
            return iterator(diff + it.ptr_);
        }

    private:
        pointer ptr_{nullptr};
    };
    using const_iterator = const iterator;
#else
    // Alternative approach
    // If a class wraps an STL container (or C-style array),
    // it is not necessary to create a custom iterator from
    // scratch. It is adequate to provide the begin() and end()
    // functions.
    using iterator = T *;
    using const_iterator = T const *;
#endif

    iterator begin()
    {
        return iterator(values_);
    }

    iterator end()
    {
        return iterator(values_ + size_);
    }

    const_iterator cbegin() const
    {
        return const_iterator(values_);
    }

    const_iterator cend() const
    {
        return const_iterator(values_ + size_);
    }

private:
    std::size_t size_{0};
    T *values_{nullptr};
};

void check_iterator_type_traits()
{
    using T = int;

    static_assert(std::is_constructible_v<DynamicArray<T>::iterator>, "Not default-constructible");
    static_assert(std::is_default_constructible_v<DynamicArray<T>::iterator>, "Not default-constructible");
    static_assert(std::is_copy_constructible_v<DynamicArray<T>::iterator>, "Not copy-constructible");
    static_assert(std::is_move_constructible_v<DynamicArray<T>::iterator>, "Not copy-constructible");
    static_assert(std::is_swappable_v<DynamicArray<T>::iterator>, "Not copy-constructible");

    static_assert(std::input_iterator<DynamicArray<T>::iterator>, "Not InputIterator");
    static_assert(std::output_iterator<DynamicArray<T>::iterator, T>, "Not OutputIterator");
    static_assert(std::forward_iterator<DynamicArray<T>::iterator>, "Not ForwardIterator");
    static_assert(std::bidirectional_iterator<DynamicArray<T>::iterator>, "Not BiDirectionalIterator");
    static_assert(std::random_access_iterator<DynamicArray<T>::iterator>, "Not RandomAccessIterator");
}

int main()
{
    // Create a DynamicArray of size N
    std::size_t N = 10;
    DynamicArray<uint> dyn_arr_1(N);

    // Fill it with increasing numbers, starting from 1
    std::ranges::iota(dyn_arr_1, 1);

    // Create a second DynamicArray of size N
    // and fill it with all 1s
    DynamicArray<uint> dyn_arr_2(N);
    std::ranges::fill(dyn_arr_2, 1);

    // Compute partial-sum of dyn_arr_2
    std::inclusive_scan(dyn_arr_2.begin(), dyn_arr_2.end(), dyn_arr_2.begin());

    // Check whether the two arrays are equal elementwise
    if (std::ranges::equal(dyn_arr_1, dyn_arr_2))
    {
        std::cout << "The arrays are equal\n";
    }
    else
    {
        std::cout << "The arrays are not equal\n";
    }

    // Print the two arrays for comparison
    for (const auto &[e1, e2] : std::ranges::zip_view(dyn_arr_1, dyn_arr_2))
    {
        std::cout << e1 << " " << e2 << "\n";
    }

    return 0;
}