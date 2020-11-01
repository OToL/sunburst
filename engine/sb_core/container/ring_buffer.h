#pragma once

#include <sb_core/core.h>
#include <sb_core/error.h>
#include <sb_core/memory/allocator/container_allocator.h>

#include <sb_std/type_traits>

namespace sb {

template <typename TType, typename TAllocator = ContainerAllocator>
class RingBuffer
{
public:
    using ValueType = TType;
    static constexpr bool IS_TRIVIAL_VALUE = sbstd::is_trivial_v<ValueType>;

    RingBuffer(usize capacity, TAllocator alloc)
        : _alloc(alloc)
        , _tail(0)
        , _head(0)
        , _capacity(capacity)
        , _full(0)
    {
        sbAssert(0 != _capacity);

        _data = (ValueType *)_alloc.allocate(capacity * sizeof(ValueType), alignOf<ValueType>()).data;
        sbAssert(nullptr != _data);
    }

    RingBuffer(usize capacity)
        : _alloc()
        , _tail(0)
        , _head(0)
        , _capacity(capacity)
        , _full(0)
    {
        sbAssert(0 != _capacity);

        _data = (ValueType *)_alloc.allocate(capacity * sizeof(ValueType), alignOf<ValueType>()).data;
        sbAssert(nullptr != _data);
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
            // @todo: can be simplified with a for loop instead of using pop
            while (!empty())
            {
                pop();
            }
        }

        _alloc.deallocate(_data);
    }

    void put_overflow(TType const & val)
    {
        TType * curr_head_ptr = _data + _head;

        if (full())
        {
            if constexpr (IS_TRIVIAL_VALUE)
            {
                *curr_head_ptr = val;
            }
            else
            {
                curr_head_ptr->~TType();
                new (curr_head_ptr) ValueType(val);
            }

            _head = (_head + 1) % _capacity;
            _tail = _head;
        }
        else
        {
            if constexpr (IS_TRIVIAL_VALUE)
            {
                *curr_head_ptr = val;
            }
            else
            {
                new (curr_head_ptr) ValueType(val);
            }

            _head = (_head + 1) % _capacity;
            _full = (_head == _tail);
        }
    }

    template <class... TArgs>
    void emplace_put_overflow(TArgs &&... args)
    {
        TType * curr_head_ptr = _data + _head;

        if (full())
        {
            if constexpr (!IS_TRIVIAL_VALUE)
            {
                curr_head_ptr->~TType();
            }

            new (curr_head_ptr) ValueType(sbstd::forward<TArgs>(args)...);

            _head = (_head + 1) % _capacity;
            _tail = _head;
        }
        else
        {
            new (curr_head_ptr) ValueType(sbstd::forward<TArgs>(args)...);

            _head = (_head + 1) % _capacity;
            _full = (_head == _tail);
        }
    }

    bool put(TType const & val)
    {
        if (!full())
        {
            TType * curr_head_ptr = _data + _head;

            if constexpr (IS_TRIVIAL_VALUE)
            {
                *curr_head_ptr = val;
            }
            else
            {
                new (curr_head_ptr) ValueType(val);
            }

            _head = (_head + 1) % _capacity;
            _full = (_head == _tail);

            return true;
        }

        return false;
    }

    template <class... TArgs>
    bool emplace_put(TArgs &&... args)
    {
        if (!full())
        {
            TType * curr_head_ptr = _data + _head;

            new (curr_head_ptr) ValueType(sbstd::forward<TArgs>(args)...);

            _head = (_head + 1) % _capacity;
            _full = (_head == _tail);

            return true;
        }

        return false;
    }

    TType pop()
    {
        if (!empty())
        {
            TType * curr_tail_ptr = _data + _tail;

            TType const val = *curr_tail_ptr;

            if constexpr (!IS_TRIVIAL_VALUE)
            {
                curr_tail_ptr->~TType();
            }

            _tail = (_tail + 1) % _capacity;
            _full = 0;

            return val;
        }

        return TType{};
    }

    bool empty() const
    {
        return (_head == _tail) && (_full == 0);
    };

    bool full() const
    {
        return (_full != 0);
    };

    usize capacity() const
    {
        return _capacity;
    }

    usize size() const
    {
        if (!_full)
        {
            if (_head >= _tail)
            {
                return _head - _tail;
            }
            else
            {
                return _capacity + (_head - _tail);
            }
        }

        return _capacity;
    }

private:
    TAllocator _alloc;
    ValueType * _data;
    usize _tail;
    usize _head;
    usize _capacity;
    usize _full;
};

} // namespace sb
