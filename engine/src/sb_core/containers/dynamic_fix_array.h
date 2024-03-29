#pragma once

#include <sb_core/core.h>
#include <sb_core/error.h>

#include <sb_slw/type_traits>
#include <sb_slw/utility>
#include <sb_slw/algorithm>
#include <sb_slw/memory>

namespace sb {

// @todo: do no call dtor for pod
// @todo: uninitialized push maybe just for pod
// @todo: try_emplace
// @todo: try_pushback
template <typename TType, usize CAPACITY>
// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
// Dynamic Array with Fix size capacity
class DynamicFixArray
{
public:
    using value_type = TType;
    using reference = TType &;
    using const_reference = TType const &;
    using pointer = TType *;
    using const_pointer = TType const *;
    using iterator = TType *;
    using const_iterator = TType const *;
    using size_type = usize;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    DynamicFixArray() = default;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    DynamicFixArray(size_type count, value_type const & value)
    {
        sb_assert(count <= CAPACITY);

        slw::uninitialized_fill((pointer)_data, ((pointer)_data) + count, value);
        _size = count;
    }

    DynamicFixArray(DynamicFixArray const &) = delete;
    DynamicFixArray(DynamicFixArray &&) = delete;
    DynamicFixArray & operator=(DynamicFixArray const &) = delete;
    DynamicFixArray & operator=(DynamicFixArray &&) = delete;

    ~DynamicFixArray()
    {
        clear();
    }

    size_type size() const
    {
        return _size;
    }

    size_type capacity() const
    {
        return CAPACITY;
    }

    void clear()
    {
        auto const iter = reinterpret_cast<pointer>(&_data[0]);

        for (usize idx = 0; idx != _size; ++idx)
        {
            iter->~value_type();
        }

        _size = 0;
    }

    bool empty() const
    {
        return (0 == _size);
    }

    bool full() const
    {
        return (CAPACITY == _size);
    }

    iterator expand(size_type cnt)
    {
        if (sb_expect((_size + cnt) <= CAPACITY))
        {
            auto new_start_iter = end();

            slw::uninitialized_default_construct((pointer)_data + _size, ((pointer)_data) + _size + cnt);
            _size += cnt;

            return new_start_iter;
        }

        return end();
    }

    void push_back(const_reference value)
    {
        sb_assert(CAPACITY != _size, "Static vector capacity exceeded");

        new (end()) value_type{value};
        ++_size;
    }

    void push_back(value_type && value)
    {
        sb_assert(CAPACITY != _size, "Static vector capacity exceeded");

        new (end()) value_type{slw::move(value)};
        ++_size;
    }

    void pop_back()
    {
        sb_assert(0 != _size, "You cannot pop item from empty static vector");

        (reinterpret_cast<pointer>(&_data[0]) + _size - 1)->~value_type();
        --_size;
    }

    template <class... TArgs>
    reference emplace_back(TArgs &&... args)
    {
        sb_assert(CAPACITY != _size, "Static vector capacity exceeded");

        new (end()) value_type{slw::forward<TArgs>(args)...};
        ++_size;

        return back();
    }

    reference back()
    {
        sb_assert(0 != _size);

        return *(reinterpret_cast<pointer>(&_data[0]) + _size - 1);
    }

    const_reference back() const
    {
        sb_assert(0 != _size);

        return *(reinterpret_cast<const_pointer>(&_data[0]) + _size - 1);
    }

    iterator erase(const_iterator pos)
    {
        sb_assert((begin() <= pos) && (pos < end()));

        iterator mutable_pos = begin() + (pos - begin());

        slw::move(mutable_pos + 1, end(), mutable_pos);
        (end() - 1)->~value_type();
        --_size;

        return mutable_pos;
    }

    const_pointer data() const
    {
        return reinterpret_cast<const_pointer>(&_data[0]);
    }

    pointer data()
    {
        return reinterpret_cast<pointer>(&_data[0]);
    }

    const_iterator begin() const
    {
        return reinterpret_cast<const_pointer>(&_data[0]);
    }

    iterator begin()
    {
        return reinterpret_cast<iterator>(&_data[0]);
    }

    const_iterator end() const
    {
        return reinterpret_cast<const_iterator>(&_data[0]) + _size;
    }

    iterator end()
    {
        return reinterpret_cast<iterator>(&_data[0]) + _size;
    }

    reference operator[](usize idx)
    {
        sb_assert(idx < _size);

        return reinterpret_cast<pointer>(&_data[0])[idx];
    }

    const_reference operator[](usize idx) const
    {
        sb_assert(idx < _size);

        return reinterpret_cast<const_pointer>(&_data[0])[idx];
    }

private:
    static constexpr usize BASE_CAPACITY_BYTES = CAPACITY * sizeof(value_type);

    alignas(value_type) u8 _data[BASE_CAPACITY_BYTES];
    size_type _size = 0ULL;
};

template <typename TType, usize CAPACITY>
typename DynamicFixArray<TType, CAPACITY>::iterator begin(DynamicFixArray<TType, CAPACITY> & vect)
{
    return vect.begin();
}

template <typename TType, usize CAPACITY>
typename DynamicFixArray<TType, CAPACITY>::iterator end(DynamicFixArray<TType, CAPACITY> & vect)
{
    return vect.end();
}

template <typename TType, usize CAPACITY>
typename DynamicFixArray<TType, CAPACITY>::const_iterator begin(DynamicFixArray<TType, CAPACITY> const & vect)
{
    return vect.begin();
}

template <typename TType, usize CAPACITY>
typename DynamicFixArray<TType, CAPACITY>::const_iterator end(DynamicFixArray<TType, CAPACITY> const & vect)
{
    return vect.end();
}

template <typename TType, usize CAPACITY>
using DFArray = DynamicFixArray<TType, CAPACITY>;

} // namespace sb
