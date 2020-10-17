#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/allocator/allocator_view.h>
#include <sb_core/unique_ptr.h>
#include <sb_core/error.h>

#include <sb_std/type_traits>

namespace sb {

template <typename TType>
class RingBuffer
{
public:
    using ValueType = TType;
    static constexpr bool IS_TRIVIAL_VALUE = sbstd::is_trivial_v<ValueType>;

    RingBuffer(usize capacity, AllocatorView const & alloc)
        : _begin(0)
        , _capacity(capacity)
        , _size(0)
    {
        sbAssert((0 != capacity) && alloc.isValid());

        _data = allocateUnique<u8[], AllocatorView>(alloc, capacity * sizeof(TType));
    }

    RingBuffer(usize capacity)
        : _begin(0)
        , _capacity(capacity)
        , _size(0)
    {
        sbAssert(0 != capacity);
        _data = allocateUnique<u8[], AllocatorView>(getGlobalHeapView(), capacity * sizeof(TType));
    }

    RingBuffer & operator=(RingBuffer const &) = delete;
    RingBuffer(RingBuffer const &) = delete;

    // @todo: to implement
    RingBuffer & operator=(RingBuffer &&) = delete;
    RingBuffer(RingBuffer &&) = delete;

    ~RingBuffer()
    {
        if constexpr (!IS_TRIVIAL_VALUE)
        {
            // @todo: just a for loop
            while (!empty())
            {
                pop();
            }
        }
    }

    void put_overflow(TType const & val)
    {
        if (full())
        {
            if constexpr (IS_TRIVIAL_VALUE)
            {
                *(reinterpret_cast<TType *>(_data.get()) + ((_begin + _size) % _capacity)) = val;
            }
            else
            {
                reinterpret_cast<TType *>(_data.get() + ((_begin + _size) % _capacity))->~TType();
                new (reinterpret_cast<TType *>(_data.get()) + ((_begin + _size) % _capacity)) ValueType(val);
            }

            _begin = (_begin + 1) % _capacity;
        }
        else
        {
            if constexpr (IS_TRIVIAL_VALUE)
            {
                *(reinterpret_cast<TType *>(_data.get()) + ((_begin + _size) % _capacity)) = val;
            }
            else
            {
                new (reinterpret_cast<TType *>(_data.get()) + ((_begin + _size) % _capacity)) ValueType(val);
            }

            ++_size;
        }
    }

    template <class... TArgs>
    void emplace_put_overflow(TArgs &&... args)
    {
        if (full())
        {
            if constexpr (!IS_TRIVIAL_VALUE)
            {
                reinterpret_cast<TType *>(_data.get() + ((_begin + _size) % _capacity))->~TType();
            }

            new (reinterpret_cast<TType *>(_data.get()) + ((_begin + _size) % _capacity))
                ValueType(sbstd::forward<TArgs>(args)...);
            _begin = (_begin + 1) % _capacity;
        }
        else
        {
            new (reinterpret_cast<TType *>(_data.get()) + ((_begin + _size) % _capacity))
                ValueType(sbstd::forward<TArgs>(args)...);
            ++_size;
        }
    }

    bool put(TType const & val)
    {
        if (!full())
        {
            if constexpr (IS_TRIVIAL_VALUE)
            {
                *(reinterpret_cast<TType *>(_data.get()) + ((_begin + _size) % _capacity)) = val;
            }
            else
            {
                new (reinterpret_cast<TType *>(_data.get()) + ((_begin + _size) % _capacity)) ValueType(val);
            }

            ++_size;

            return true;
        }

        return false;
    }

    template <class... TArgs>
    bool emplace_put(TArgs &&... args)
    {
        if (!full())
        {
            new (reinterpret_cast<TType *>(_data.get()) + ((_begin + _size) % _capacity))
                ValueType(sbstd::forward<TArgs>(args)...);
            ++_size;

            return true;
        }

        return false;
    }

    TType pop()
    {
        if (!empty())
        {
            TType & currItem = reinterpret_cast<TType *>(_data.get())[_begin];
            TType const val = currItem;

            if constexpr (!IS_TRIVIAL_VALUE)
            {
                currItem.~TType();
            }

            _begin = (_begin + 1) % _capacity;
            --_size;

            return val;
        }

        return TType{};
    }

    bool empty() const
    {
        return (_size == 0);
    };

    bool full() const
    {
        return (_size == _capacity);
    };

    usize capacity() const
    {
        return _capacity;
    }

    usize size() const
    {
        return _size;
    }

private:
    UniquePtr<u8[], AllocatorViewDelete<u8[]>> _data;
    usize _begin;
    usize _capacity;
    usize _size;

    // head
    // tail
};

} // namespace sb
