#pragma once

#include <sb_core/core.h>

#include <sb_core/error.h>

#include <sb_std/type_traits>
#include <sb_std/utility>

namespace sb {

// TODO: do no call dtor for pod
// TODO: uninitialized push maybe just for pod
// TODO: try_emplace
// TODO: try_pushback
template <typename TType, usize CAPACITY>
class StaticVector
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

    StaticVector()
        : m_size(0)
    {
    }

    StaticVector & operator=(StaticVector const &) = delete;
    StaticVector(StaticVector const &) = delete;

    ~StaticVector()
    {
        clear();
    }

    size_type size() const
    {
        return m_size;
    }

    size_type capacity() const
    {
        return CAPACITY;
    }

    void clear()
    {
        pointer const iter = reinterpret_cast<pointer>(&m_data[0]);

        for (usize idx = 0; idx != m_size; ++idx)
        {
            iter->~value_type();
        }

        m_size = 0;
    }

    bool empty() const
    {
        return (0 == m_size);
    }

    void push_back(const_reference value)
    {
        sbAssert(CAPACITY != m_size, "Static vector capacity exceeded");

        new (&m_data[m_size]) value_type{value};
        ++m_size;
    }

    void push_back(value_type && value)
    {
        sbAssert(CAPACITY != m_size, "Static vector capacity exceeded");

        new (&m_data[m_size]) value_type{sbstd::move(value)};
        ++m_size;
    }

    void pop_back()
    {
        sbAssert(0 != m_size, "You cannot pop item from empty static vector");

        ((reference)m_data[m_size]).~value_type();
        --m_size;
    }

    template <class... TArgs>
    reference emplace_back(TArgs &&... args)
    {
        sbAssert(CAPACITY != m_size, "Static vector capacity exceeded");

        new (&m_data[m_size]) value_type{sbstd::forward<TArgs>(args)...};
        ++m_size;

        return back();
    }

    reference back()
    {
        sbAssert(0 != m_size);

        return ((reference)m_data[m_size - 1]);
    }

    const_reference back() const
    {
        sbAssert(0 != m_size);

        return ((const_reference)&m_data[m_size - 1]);
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
        --m_size;

        return pos;
    }

    const_pointer data() const
    {
        return reinterpret_cast<const_pointer>(&m_data[0]);
    }

    pointer data()
    {
        return reinterpret_cast<pointer>(&m_data[0]);
    }

    const_iterator begin() const
    {
        return reinterpret_cast<const_pointer>(&m_data[0]);
    }

    iterator begin()
    {
        return reinterpret_cast<iterator>(&m_data[0]);
    }

    const_iterator end() const
    {
        return reinterpret_cast<const_iterator>(&m_data[0]) + m_size;
    }

    iterator end()
    {
        return reinterpret_cast<iterator>(&m_data[0]) + m_size;
    }

    reference operator[](usize idx)
    {
        sbAssert(idx < m_size);

        return reinterpret_cast<pointer>(&m_data[0])[idx];
    }

    const_reference operator[](usize idx) const
    {
        sbAssert(idx < m_size);

        return reinterpret_cast<const_pointer>(&m_data[0])[idx];
    }

private:
    using StorageType =
        typename sbstd::aligned_storage<sizeof(value_type), alignof(value_type)>::type;

    StorageType m_data[CAPACITY];
    size_type m_size;
};

template <typename TType, usize CAPACITY>
typename StaticVector<TType, CAPACITY>::iterator begin(StaticVector<TType, CAPACITY> & vect)
{
    return vect.begin();
}

template <typename TType, usize CAPACITY>
typename StaticVector<TType, CAPACITY>::iterator end(StaticVector<TType, CAPACITY> & vect)
{
    return vect.end();
}

template <typename TType, usize CAPACITY>
typename StaticVector<TType, CAPACITY>::const_iterator
    begin(StaticVector<TType, CAPACITY> const & vect)
{
    return vect.begin();
}

template <typename TType, usize CAPACITY>
typename StaticVector<TType, CAPACITY>::const_iterator
    end(StaticVector<TType, CAPACITY> const & vect)
{
    return vect.end();
}

} // namespace sb
