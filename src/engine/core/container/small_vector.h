#pragma once

#include <core/platform.h>
#include <core/error.h>
#include <core/conversion.h>

#include <libc++/utility>
#include <libc++/memory>

namespace sb {

template <typename TType>
class SmallVectorBase
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

    SmallVectorBase() = default;

    b8 empty() const
    {
        return (m_begin == m_end);
    }

    size_type size() const
    {
        return (size_type)(m_end - m_begin);
    }

    const_iterator cbegin() const
    {
        return m_begin;
    }

    const_iterator begin() const
    {
        return m_begin;
    }

    iterator begin()
    {
        return m_begin;
    }

    const_iterator cend() const
    {
        return m_end;
    }

    const_iterator end() const
    {
        return m_end;
    }

    iterator end()
    {
        return m_end;
    }

    const_pointer data() const
    {
        return m_begin;
    }

    pointer data()
    {
        return m_begin;
    }

    value_type & back()
    {
        sbAssert(!empty());

        return *(m_end - 1);
    }

    value_type const & back() const
    {
        sbAssert(!empty());

        return *(m_end - 1);
    }

    reference operator[](size_type idx)
    {
        sbAssert(idx < size());

        return *(m_begin + idx);
    }

    const_reference operator[](size_type idx) const
    {
        sbAssert(idx < size());

        return *(m_begin + idx);
    }

protected:
    SmallVectorBase(pointer begin)
        : m_begin(begin)
        , m_end(begin)
    {
    }

    pointer m_begin = nullptr;
    pointer m_end = nullptr;
};

// TODO: POD optimization i.e. no call to destructor
// TODO: misc operations
// TODO: reconsider base class
// TODO: consider using a pointer to end of storage instead of usize for capacity
template <typename TType, usize CAPACITY, typename TAllocator = wstd::allocator<TType>>
class SmallVector : public SmallVectorBase<TType>, protected TAllocator
{
    using BaseVector = SmallVectorBase<TType>;
    using BaseAllocator = TAllocator;

    sbCopyProtect(SmallVector);

    template <typename TType, usize CAPACITY, typename TAllocator = wstd::allocator<TType>>
    friend class SmallVector;

public:
    using typename BaseVector::const_iterator;
    using typename BaseVector::const_pointer;
    using typename BaseVector::const_reference;
    using typename BaseVector::iterator;
    using typename BaseVector::pointer;
    using typename BaseVector::reference;
    using typename BaseVector::size_type;
    using typename BaseVector::value_type;
    using allocator_type = TAllocator;

    SmallVector()
        : SmallVector(allocator_type{})
    {
    }

    explicit SmallVector(allocator_type const & alloc)
        : BaseVector(reinterpret_cast<pointer>(&m_buffer[0]))
        , BaseAllocator(alloc)
        , m_capacity(CAPACITY)
    {
    }

    SmallVector(size_type count, value_type const & value, allocator_type const & alloc = allocator_type())
        : BaseVector(reinterpret_cast<pointer>(&m_buffer[0]))
        , BaseAllocator(alloc)
        , m_capacity(CAPACITY)
    {
        buyMany(count);

        wstd::uninitialized_fill(m_begin, m_end, value);
    }

    SmallVector(size_type count, allocator_type const & alloc = allocator_type())
        : BaseVector(reinterpret_cast<pointer>(&m_buffer[0]))
        , BaseAllocator(alloc)
        , m_capacity(CAPACITY)
    {
        buyMany(count);

        wstd::uninitialized_default_construct(m_begin, m_end);
    }

    template< class TIterator >
    SmallVector(TIterator first, TIterator last, allocator_type const & alloc = allocator_type() )
        : BaseVector(reinterpret_cast<pointer>(&m_buffer[0]))
        , BaseAllocator(alloc)
        , m_capacity(CAPACITY)
    {
        buyMany(numericCast<size_type>(wstd::distance(first, last)));

        wstd::uninitialized_copy(first, last, m_begin);
    }

    template <usize CAPACITY_SRC>
    SmallVector(SmallVector<TType, CAPACITY_SRC, TAllocator> const & src)
        : BaseVector(src)
        , BaseAllocator(src.get_allocator_ref())
        , m_capacity(CAPACITY)
    {
        buyMany(src.size());

        wstd::uninitialized_copy(wstd::begin(src), wstd::end(src), m_begin);
    }

    ~SmallVector()
    {
        clear();

        if (!isSmallStorage())
        {
            BaseAllocator::deallocate(m_begin, m_capacity);
        }
    }

    size_type capacity() const
    {
        return m_capacity;
    }

    void push_back(const_reference value)
    {
        buyOne();

        new(m_end - 1) value_type(value);

        return;
    }

    void push_back(value_type && value)
    {
        buyOne();

        new (m_end - 1) value_type(wstd::move(value));

        return;
    }

    template <class... TArgs>
    value_type & emplace_back(TArgs &&... args)
    {
        auto const curr_size = m_end - m_begin;
        if (curr_size < (sptrdiff)m_capacity)
        {
            return *(new (m_end++) TType(wstd::forward<TArgs>(args)...));
        }

        sbNotImplemented("todo");

        return *m_begin;
    }

    void clear()
    {
        wstd::destroy(m_begin, m_end);

        m_end = m_begin;
    }

    allocator_type get_allocator() const
    {
        return *(BaseAllocator const *)this;
    }

private:

    allocator_type & get_allocator_ref() 
    {
        return *(BaseAllocator *)this;
    }

    allocator_type const & get_allocator_ref() const 
    {
        return *(BaseAllocator *)this;
    }

    bool isSmallStorage() const
    {
        return (m_begin == reinterpret_cast<const_pointer>(&m_buffer[0]));
    }

    void buyMany(size_type cnt)
    {
        usize const curr_size = size();

        if ((curr_size + cnt) < m_capacity)
        {
            m_end += cnt;
        }
        else
        {
            size_type const new_size = size() + cnt;
            // TODO: investigate better capacity calculation policy
            size_type const new_capacity = new_size + (CAPACITY >> 1);

            pointer const new_data = BaseAllocator::allocate(new_capacity);
            wstd::uninitialized_move(m_begin, m_end, new_data);

            wstd::destroy(m_begin, m_end);

            if (!isSmallStorage())
            {
                BaseAllocator::deallocate(m_begin, m_capacity);
            }

            m_end = new_data + curr_size + cnt;
            m_begin = new_data;
            m_capacity = new_capacity;   
        }
    }

    void buyOne()
    {
        usize const curr_size = size();

        if (curr_size < m_capacity)
        {
            ++m_end;
        }
        else
        {
            // TODO: investigate better capacity calculation policy
            size_type const new_capacity = m_capacity + (CAPACITY >> 1);

            pointer const new_data = BaseAllocator::allocate(new_capacity);
            wstd::uninitialized_move(m_begin, m_end, new_data);

            wstd::destroy(m_begin, m_end);

            if (!isSmallStorage())
            {
                BaseAllocator::deallocate(m_begin, m_capacity);
            }

            m_end = new_data + curr_size + 1;
            m_begin = new_data;
            m_capacity = new_capacity;   
        }        
    }

    static constexpr usize CAPACITY_BYTES = CAPACITY * sizeof(TType);

    using BaseVector::m_begin;
    using BaseVector::m_end;

    size_type m_capacity;
    alignas(value_type) ui8 m_buffer[CAPACITY_BYTES];
};

template <typename TType, usize CAPACITY>
typename SmallVectorBase<TType>::iterator begin( SmallVectorBase<TType> & vect)
{
    return vect.begin();
}

template <typename TType, usize CAPACITY>
typename SmallVectorBase<TType>::iterator end( SmallVectorBase<TType> & vect)
{
    return vect.end();
}

template <typename TType, usize CAPACITY>
typename SmallVectorBase<TType>::const_iterator begin( SmallVectorBase<TType> const & vect)
{
    return vect.begin();
}

template <typename TType, usize CAPACITY>
typename SmallVectorBase<TType>::const_iterator end( SmallVectorBase<TType> const & vect)
{
    return vect.end();
}

} // namespace sb
