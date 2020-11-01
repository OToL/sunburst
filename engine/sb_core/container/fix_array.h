#pragma once

#include <sb_core/core.h>
#include <sb_core/error.h>

#include <sb_std/type_traits>
#include <sb_std/utility>

namespace sb {

// @todo: do no call dtor for pod
// @todo: uninitialized push maybe just for pod
// @todo: try_emplace
// @todo: try_pushback
template <typename TType, usize CAPACITY>
class FixArray
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

    FixArray()
        : _size(0)
    {
    }

    FixArray & operator=(FixArray const &) = delete;
    FixArray(FixArray const &) = delete;

    ~FixArray()
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
        pointer const iter = reinterpret_cast<pointer>(&_data[0]);

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

    void push_back(const_reference value)
    {
        sbAssert(CAPACITY != _size, "Static vector capacity exceeded");

        new (&_data[_size]) value_type{value};
        ++_size;
    }

    void push_back(value_type && value)
    {
        sbAssert(CAPACITY != _size, "Static vector capacity exceeded");

        new (&_data[_size]) value_type{sbstd::move(value)};
        ++_size;
    }

    void pop_back()
    {
        sbAssert(0 != _size, "You cannot pop item from empty static vector");

        ((reference)_data[_size]).~value_type();
        --_size;
    }

    template <class... TArgs>
    reference emplace_back(TArgs &&... args)
    {
        sbAssert(CAPACITY != _size, "Static vector capacity exceeded");

        new (&_data[_size]) value_type{sbstd::forward<TArgs>(args)...};
        ++_size;

        return back();
    }

    reference back()
    {
        sbAssert(0 != _size);

        return ((reference)_data[_size - 1]);
    }

    const_reference back() const
    {
        sbAssert(0 != _size);

        return ((const_reference)&_data[_size - 1]);
    }

    iterator erase(iterator pos)
    {
        sbAssert((begin() <= pos) && (pos < end()));

        pointer item2Rem = pos;
        const_pointer const endItem = end();

        if ((item2Rem + 1) != endItem)
        {
            while ((item2Rem + 1) != endItem)
            {
                *item2Rem = sbstd::move(*(item2Rem + 1));
                ++item2Rem;
            }
        }

        item2Rem->~value_type();
        --_size;

        return pos;
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
        sbAssert(idx < _size);

        return reinterpret_cast<pointer>(&_data[0])[idx];
    }

    const_reference operator[](usize idx) const
    {
        sbAssert(idx < _size);

        return reinterpret_cast<const_pointer>(&_data[0])[idx];
    }

private:
    using StorageType = typename sbstd::aligned_storage<sizeof(value_type), alignof(value_type)>::type;

    StorageType _data[CAPACITY];
    size_type _size;
};

template <typename TType, usize CAPACITY>
typename FixArray<TType, CAPACITY>::iterator begin(FixArray<TType, CAPACITY> & vect)
{
    return vect.begin();
}

template <typename TType, usize CAPACITY>
typename FixArray<TType, CAPACITY>::iterator end(FixArray<TType, CAPACITY> & vect)
{
    return vect.end();
}

template <typename TType, usize CAPACITY>
typename FixArray<TType, CAPACITY>::const_iterator begin(FixArray<TType, CAPACITY> const & vect)
{
    return vect.begin();
}

template <typename TType, usize CAPACITY>
typename FixArray<TType, CAPACITY>::const_iterator end(FixArray<TType, CAPACITY> const & vect)
{
    return vect.end();
}

template <typename TType, usize CAPACITY>
using FArray = FixArray<TType, CAPACITY>;

} // namespace sb
