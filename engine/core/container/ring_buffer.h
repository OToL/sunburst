#pragma once

#include <base/base.h>
#include <core/memory/allocator/allocator_view.h>
#include <core/unique_ptr.h>
#include <core/error.h>

namespace sb {

template <typename TType>
class RingBuffer
{
public:
    RingBuffer(usize capacity, AllocatorView const & alloc)
        : m_begin(0)
        , m_capacity(capacity)
        , m_size(0)
    {
        sbAssert((0 != capacity) && alloc.isValid());

        m_data = allocateUnique<ui8[], AllocatorView>(alloc, capacity * sizeof(TType));
    }

    RingBuffer(usize capacity)
        : m_begin(0)
        , m_capacity(capacity)
        , m_size(0)
    {
        sbAssert(0 != capacity);
        m_data =
            allocateUnique<ui8[], AllocatorView>(getGlobalHeapView(), capacity * sizeof(TType));
    }

    RingBuffer & operator=(RingBuffer const &) = delete;
    RingBuffer(RingBuffer const &) = delete;

    ~RingBuffer()
    {
        while (!empty())
        {
            pop();
        }
    }

    bool push(TType const & val)
    {
        if (!full())
        {
            reinterpret_cast<TType *>(m_data.get())[(m_begin + m_size) % m_capacity] = val;
            ++m_size;

            return true;
        }
        else
        {
            TType & prevItem =
                reinterpret_cast<TType *>(m_data.get())[(m_begin + m_size) % m_capacity];
            prevItem.~TType();
            prevItem = val;

            return false;
        }
    }

    TType pop()
    {
        if (!empty())
        {
            TType & currItem = reinterpret_cast<TType *>(m_data.get())[m_begin];
            TType const val = currItem;
            currItem.~TType();

            m_begin = (m_begin + 1) % m_capacity;
            --m_size;

            return val;
        }

        return TType{};
    }

    bool empty() const
    {
        return (m_size == 0);
    };

    bool full() const
    {
        return (m_size == m_capacity);
    };

    usize capacity() const
    {
        return m_capacity;
    }

    usize size() const
    {
        return m_size;
    }

private:
    UniquePtr<ui8[], AllocatorViewDelete<ui8[]>> m_data;
    usize m_begin;
    usize m_capacity;
    usize m_size;
};

} // namespace sb
