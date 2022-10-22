#pragma once

#include <sb_core/core.h>
#include <sb_core/cast.h>
#include <sb_core/error.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/utility.h>
#include <sb_core/memory/allocator/container_allocator.h>
#include <sb_core/_impl/container/dynamic_small_array_base.h>

#include <sb_slw/algorithm>
#include <sb_slw/iterator>
#include <sb_slw/memory>
#include <sb_slw/utility>

namespace sb {

template <typename TType, usize BASE_CAPACITY, typename TAllocator = ContainerAllocator>
class DynamicSmallArray : public internal::DynamicSmallArrayBase<TType, TAllocator>
{
    sb_base(internal::DynamicSmallArrayBase<TType, TAllocator>);

    template <typename, usize, typename>
    friend class DynamicSmallArray;

    using Impl = typename Base::Impl;

    static constexpr usize BITS_PER_WORD = static_cast<usize>(sizeof(typename Base::size_type) * 8);

public:
    using typename Base::const_iterator;
    using typename Base::const_pointer;
    using typename Base::const_reference;
    using typename Base::difference_type;
    using typename Base::iterator;
    using typename Base::pointer;
    using typename Base::reference;
    using typename Base::size_type;
    using typename Base::value_type;
    using allocator_type = TAllocator;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    DynamicSmallArray()
        : DynamicSmallArray(reinterpret_cast<pointer>(&_buffer[0]))
    {
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    explicit DynamicSmallArray(allocator_type const & alloc)
        : DynamicSmallArray(reinterpret_cast<pointer>(&_buffer[0]), alloc)
    {
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    explicit DynamicSmallArray(size_type count, allocator_type const & alloc = allocator_type())
        : DynamicSmallArray(reinterpret_cast<pointer>(&_buffer[0]), alloc)
    {
        buyMore(count);

        slw::uninitialized_default_construct(_impl._begin, _impl._end);
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    DynamicSmallArray(size_type count, value_type const & value, allocator_type const & alloc = allocator_type())
        : DynamicSmallArray(reinterpret_cast<pointer>(&_buffer[0]), alloc)
    {
        buyMore(count);

        slw::uninitialized_fill(_impl._begin, _impl._end, value);
    }

    template <class TIterator>
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    DynamicSmallArray(TIterator first, TIterator last, allocator_type const & alloc = allocator_type())
        : DynamicSmallArray(reinterpret_cast<pointer>(&_buffer[0]), alloc)
    {
        buyMore(integral_cast<size_type>(last - first));

        slw::uninitialized_copy(first, last, _impl._begin);
    }

    template <usize BASE_CAPACITY_SRC, typename TSrcAllocator>
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    DynamicSmallArray(DynamicSmallArray<TType, BASE_CAPACITY_SRC, TSrcAllocator> const & src)
        : DynamicSmallArray(reinterpret_cast<pointer>(&_buffer[0]), src._impl.get_allocator())
    {
        buyMore(src.size());

        slw::uninitialized_copy(slw::begin(src), slw::end(src), _impl._begin);
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    DynamicSmallArray(DynamicSmallArray const & src)
        : DynamicSmallArray(reinterpret_cast<pointer>(&_buffer[0]), src._impl.get_allocator())
    {
        buyMore(src.size());

        slw::uninitialized_copy(slw::begin(src), slw::end(src), _impl._begin);
    }

    template <usize BASE_CAPACITY_SRC, typename TSrcAllocator>
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    DynamicSmallArray(DynamicSmallArray<TType, BASE_CAPACITY_SRC, TSrcAllocator> const & src, allocator_type const & alloc)
        : DynamicSmallArray(reinterpret_cast<pointer>(&_buffer[0]), alloc)
    {
        buyMore(src.size());

        slw::uninitialized_copy(slw::begin(src), slw::end(src), _impl._begin);
    }

    template <usize BASE_CAPACITY_SRC>
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    DynamicSmallArray(DynamicSmallArray<TType, BASE_CAPACITY_SRC, TAllocator> && src)
        : _impl(src._impl.get_allocator())
    {
        if (!src.isSmallStorage() && (src.size() > BASE_CAPACITY))
        {
            slw::swap(_impl._begin, src._impl._begin);
            slw::swap(_impl._end, src._impl._end);
            slw::swap(_impl._storage_end, src._impl._storage_end);
        }
        else
        {
            _impl.init(reinterpret_cast<pointer>(&_buffer[0]), reinterpret_cast<pointer>(&_buffer[0]) + BASE_CAPACITY);

            buyMore(src.size());
            slw::uninitialized_move(slw::begin(src), slw::end(src), _impl._begin);

            src.clear();
        }
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    DynamicSmallArray(DynamicSmallArray && src)
        : _impl(src._impl.get_allocator())
    {
        if (!src.isSmallStorage() && (src.size() > BASE_CAPACITY))
        {
            slw::swap(_impl._begin, src._impl._begin);
            slw::swap(_impl._end, src._impl._end);
            slw::swap(_impl._storage_end, src._impl._storage_end);
        }
        else
        {
            _impl.init(reinterpret_cast<pointer>(&_buffer[0]), reinterpret_cast<pointer>(&_buffer[0]) + BASE_CAPACITY);

            buyMore(src.size());
            slw::uninitialized_move(slw::begin(src), slw::end(src), _impl._begin);

            src.clear();
        }
    }

    template <usize BASE_CAPACITY_SRC>
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    DynamicSmallArray(DynamicSmallArray<TType, BASE_CAPACITY_SRC, TAllocator> && src, allocator_type const & alloc)
        : _impl(alloc)
    {
        if (!src.isSmallStorage() && (src.size() > BASE_CAPACITY))
        {
            slw::swap(_impl._begin, src._impl._begin);
            slw::swap(_impl._end, src._impl._end);
            slw::swap(_impl._storage_end, src._impl._storage_end);
        }
        else
        {
            _impl.init(reinterpret_cast<pointer>(&_buffer[0]), reinterpret_cast<pointer>(&_buffer[0]) + BASE_CAPACITY);

            buyMore(src.size());
            slw::uninitialized_move(slw::begin(src), slw::end(src), _impl._begin);

            src.clear();
        }
    }

    ~DynamicSmallArray()
    {
        clear();

        if (!isSmallStorage())
        {
            _impl.deallocate(MemoryArena{_impl._begin, (size_type)(_impl._storage_end - _impl._begin)});
        }
    }

    template <usize BASE_CAPACITY_SRC>
    void swap(DynamicSmallArray<TType, BASE_CAPACITY_SRC, TAllocator> & src)
    {
        if (!isSmallStorage() && !src.isSmallStorage())
        {
            slw::swap(_impl._begin, src._impl._begin);
            slw::swap(_impl._end, src._impl._end);
            slw::swap(_impl._storage_end, src._impl._storage_end);
            slw::swap(_impl.get_allocator(), src._impl.get_allocator());
        }
        else
        {
            auto const src_size = src.size();
            auto const dst_size = size();
            auto const common_size = slw::min(src_size, dst_size);

            if ((src_size <= capacity()) && (dst_size <= src.capacity()))
            {
                slw::swap_ranges(_impl._begin, _impl._begin + common_size, src._impl._begin);

                if (src_size > dst_size)
                {
                    slw::uninitialized_move(src._impl._begin + common_size, src._impl._end, _impl._end);
                    slw::destroy(src._impl._begin + common_size, src._impl._end);
                }
                else if (dst_size > src_size)
                {
                    slw::uninitialized_move(_impl._begin + common_size, _impl._end, src._impl._end);
                    slw::destroy(_impl._begin + common_size, _impl._end);
                }

                _impl._end = _impl._begin + src_size;
                src._impl._end = src._impl._begin + dst_size;

                slw::swap(_impl.get_allocator(), src._impl.get_allocator());
            }
            else if (src_size > capacity())
            {
                size_type const new_dst_capacity = computeCapacity(src_size);
                auto const new_dst_data =
                    (TType *)_impl.allocate(alignof(value_type), new_dst_capacity * sizeof(value_type)).data;
                sb_assert(nullptr != new_dst_data);

                slw::uninitialized_move(src._impl._begin, src._impl._end, new_dst_data);
                slw::move(_impl._begin, _impl._end, src._impl._begin);
                slw::destroy(_impl._begin, _impl._end);
                slw::destroy(src._impl._begin + dst_size, src._impl._end);

                if (!isSmallStorage())
                {
                    _impl.deallocate({_impl._begin, dst_size});
                }

                _impl._begin = new_dst_data;
                _impl._end = _impl._begin + src_size;
                _impl._storage_end = _impl._begin + new_dst_capacity;

                src._impl._end = src._impl._begin + dst_size;
            }
            else
            {
                size_type const new_src_capacity = computeCapacity(dst_size);
                auto const new_src_data =
                    (TType *)src._impl.allocate(alignof(value_type), new_src_capacity * sizeof(value_type)).data;
                sb_assert(nullptr != new_src_data);

                slw::uninitialized_move(_impl._begin, _impl._end, new_src_data);
                slw::move(src._impl._begin, src._impl._end, _impl._begin);
                slw::destroy(src._impl._begin, src._impl._end);
                slw::destroy(_impl._begin + src_size, _impl._end);

                if (!src.isSmallStorage())
                {
                    src._impl.deallocate({src._impl._begin, src_size});
                }

                src._impl._begin = new_src_data;
                src._impl._end = src._impl._begin + dst_size;
                src._impl._storage_end = src._impl._begin + new_src_capacity;

                _impl._end = _impl._begin + src_size;
            }
        }
    }

    b8 empty() const
    {
        return (_impl._begin == _impl._end);
    }

    size_type size() const
    {
        return (size_type)(_impl._end - _impl._begin);
    }

    const_iterator cbegin() const
    {
        return _impl._begin;
    }

    const_iterator begin() const
    {
        return _impl._begin;
    }

    iterator begin()
    {
        return _impl._begin;
    }

    const_iterator cend() const
    {
        return _impl._end;
    }

    const_iterator end() const
    {
        return _impl._end;
    }

    iterator end()
    {
        return _impl._end;
    }

    const_pointer data() const
    {
        return _impl._begin;
    }

    pointer data()
    {
        return _impl._begin;
    }

    reference front()
    {
        sb_assert(!empty());

        return *_impl._begin;
    }

    const_reference front() const
    {
        sb_assert(!empty());

        return *_impl._begin;
    }

    reference back()
    {
        sb_assert(!empty());

        return *(_impl._end - 1);
    }

    const_reference back() const
    {
        sb_assert(!empty());

        return *(_impl._end - 1);
    }

    size_type capacity() const
    {
        return (size_type)(_impl._storage_end - _impl._begin);
    }

    constexpr size_type max_size() const
    {
        return (size_type) ~((difference_type)0);
    }

    void reserve(size_type new_cap)
    {
        if (new_cap > capacity())
        {
            extendStorage(new_cap);
        }
    }

    void resize(size_type count)
    {
        auto const curr_size = size();

        if (curr_size > count)
        {
            slw::destroy(_impl._begin + count, _impl._end);
            _impl._end = _impl._begin + count;
        }
        else if (curr_size < count)
        {
            auto const size_diff = (size_type)(count - curr_size);

            buyMore(size_diff);
            std::uninitialized_default_construct(_impl._begin + curr_size, _impl._end);
        }
    }

    void resize(size_type count, const value_type & value)
    {
        auto const curr_size = size();

        if (curr_size > count)
        {
            slw::destroy(_impl._begin + count, _impl._end);
            _impl._end = _impl._begin + count;
        }
        else if (curr_size < count)
        {
            auto const size_diff = (size_type)(count - curr_size);

            buyMore(size_diff);
            std::uninitialized_fill(_impl._begin + curr_size, _impl._end, value);
        }
    }

    void push_back(const_reference value)
    {
        buyMore(1);

        new (_impl._end - 1) value_type(value);

        return;
    }

    void push_back(value_type && value)
    {
        buyMore(1);

        new (_impl._end - 1) value_type(slw::move(value));

        return;
    }

    template <class... TArgs>
    value_type & emplace_back(TArgs &&... args)
    {
        buyMore(1);

        new (_impl._end - 1) TType(slw::forward<TArgs>(args)...);

        return *_impl._begin;
    }

    void pop_back()
    {
        sb_assert(!empty());

        slw::destroy_at(_impl._end - 1);
        --_impl._end;
    }

    void clear()
    {
        slw::destroy(_impl._begin, _impl._end);

        _impl._end = _impl._begin;
    }

    allocator_type get_allocator() const
    {
        return (allocator_type const &)_impl;
    }

    reference at(size_type idx)
    {
        sb_assert(idx < size());

        return *(_impl._begin + idx);
    }

    const_reference at(size_type idx) const
    {
        sb_assert(idx < size());

        return *(_impl._begin + idx);
    }

    reference operator[](size_type idx)
    {
        sb_assert(idx < size());

        return *(_impl._begin + idx);
    }

    const_reference operator[](size_type idx) const
    {
        sb_assert(idx < size());

        return *(_impl._begin + idx);
    }

    // NOLINTNEXTLINE(bugprone-unhandled-self-assignment)
    DynamicSmallArray & operator=(DynamicSmallArray const & src)
    {
        if (this != &src)
        {
            assign(src.begin(), src.end());
        }

        return *this;
    }

    template <usize BASE_CAPACITY_SRC, typename TSrcAllocator>
    DynamicSmallArray & operator=(DynamicSmallArray<TType, BASE_CAPACITY_SRC, TSrcAllocator> const & src)
    {
        assign(src.begin(), src.end());

        return *this;
    }

    // NOLINTNEXTLINE(bugprone-unhandled-self-assignment)
    DynamicSmallArray & operator=(DynamicSmallArray && src)
    {
        if (this != &src)
        {
            clear();
            this->swap(src);
        }

        return *this;
    }

    template <usize BASE_CAPACITY_SRC, typename TSrcAllocator>
    DynamicSmallArray & operator=(DynamicSmallArray<TType, BASE_CAPACITY_SRC, TSrcAllocator> && src)
    {
        clear();
        this->swap(src);

        return *this;
    }

    void shrink_to_fit()
    {
        if (!isSmallStorage())
        {
            size_type const new_capacity = size();
            size_type const curr_capacity = capacity();

            if (new_capacity != curr_capacity)
            {
                if (new_capacity <= BASE_CAPACITY)
                {
                    slw::uninitialized_move(_impl._begin, _impl._end, reinterpret_cast<pointer>(&_buffer[0]));
                    slw::destroy(_impl._begin, _impl._end);
                    _impl.deallocate({_impl._begin, curr_capacity});

                    _impl._begin = reinterpret_cast<pointer>(&_buffer[0]);
                }
                else
                {
                    pointer const new_data =
                        (TType *)_impl.allocate(alignof(value_type), new_capacity * sizeof(value_type)).data;
                    sb_assert(nullptr != new_data);

                    slw::uninitialized_move(_impl._begin, _impl._end, new_data);
                    slw::destroy(_impl._begin, _impl._end);
                    _impl.deallocate({_impl._begin, curr_capacity});

                    _impl._begin = new_data;
                }

                _impl._end = _impl._begin + new_capacity;
                _impl._storage_end = _impl._end;
            }
        }
    }

    template <class InputIt>
    void assign(InputIt src_begin, InputIt src_end)
    {
        auto const curr_size = size();
        auto const src_size = (size_type)slw::distance(src_begin, src_end);

        if (src_size <= curr_size)
        {
            slw::copy(src_begin, src_end, _impl._begin);
            slw::destroy(_impl._begin + src_size, end());

            _impl._end = _impl._begin + src_size;
        }
        else if (src_size > capacity())
        {
            auto const new_capacity = computeCapacity(src_size);
            pointer const new_data =
                (TType *)_impl.allocate(alignof(value_type), new_capacity * sizeof(value_type)).data;
            sb_assert(nullptr != new_data);

            slw::uninitialized_copy(src_begin, src_end, new_data);
            slw::destroy(_impl._begin, _impl._end);

            if (!isSmallStorage())
            {
                _impl.deallocate({_impl._begin, curr_size});
            }

            _impl._begin = new_data;
            _impl._end = new_data + src_size;
            _impl._storage_end = new_data + new_capacity;
        }
        else
        {
            slw::copy(src_begin, src_begin + curr_size, _impl._begin);
            slw::uninitialized_copy(src_begin + curr_size, src_end, _impl._end);

            _impl._end = _impl._begin + src_size;
        }
    }

    void assign(size_type count, TType const & value)
    {
        auto const curr_size = size();

        if (count <= curr_size)
        {
            slw::fill(_impl._begin, _impl._begin + count, value);
            slw::destroy(_impl._begin + count, _impl._end);

            _impl._end = _impl._begin + count;
        }
        else if (count > capacity())
        {
            auto const new_capacity = computeCapacity(count);
            pointer const new_data =
                (TType *)_impl.allocate(alignof(value_type), new_capacity * sizeof(value_type)).data;
            sb_assert(nullptr != new_data);

            slw::uninitialized_fill(new_data, new_data + count, value);
            slw::destroy(_impl._begin, _impl._end);

            if (!isSmallStorage())
            {
                _impl.deallocate({_impl._begin, curr_size});
            }

            _impl._begin = new_data;
            _impl._end = new_data + count;
            _impl._storage_end = new_data + new_capacity;
        }
        else
        {
            slw::fill(_impl._begin, _impl._end, value);
            slw::uninitialized_fill(_impl._end, _impl._begin + count, value);

            _impl._end = _impl._begin + count;
        }
    }

    // non-standard extensions

    bool isSmallStorage() const
    {
        return (_impl._begin == reinterpret_cast<const_pointer>(&_buffer[0]));
    }

    allocator_type & get_allocator_ref()
    {
        return _impl.get_allocator();
    }

    allocator_type const & get_allocator_ref() const
    {
        return _impl.get_allocator();
    }

private:
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    DynamicSmallArray(pointer begin, allocator_type const & alloc)
        : _impl(begin, begin + BASE_CAPACITY, alloc)
    {
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    DynamicSmallArray(pointer begin)
        : _impl(begin, begin + BASE_CAPACITY)
    {
    }

    static constexpr size_type alignSize(size_type new_size)
    {
        return new_size + (BITS_PER_WORD - 1) & ~((size_type)BITS_PER_WORD - 1);
    }

    size_type computeCapacity(size_type new_size) const
    {
        size_type const ms = max_size();
        sb_assert(new_size <= ms);

        size_type const curr_cap = capacity();

        if (curr_cap >= ms / 2)
        {
            return ms;
        }

        return slw::max(2 * curr_cap, alignSize(new_size));
    }

    void extendStorage(size_type new_capacity)
    {
        size_type const curr_capacity = capacity();
        size_type const curr_size = size();

        pointer const new_data = (TType *)_impl.allocate(alignof(value_type), new_capacity * sizeof(value_type)).data;
        sb_assert(nullptr != new_data);

        slw::uninitialized_move(_impl._begin, _impl._end, new_data);
        slw::destroy(_impl._begin, _impl._end);

        if (!isSmallStorage())
        {
            _impl.deallocate({_impl._begin, curr_capacity});
        }

        _impl._end = new_data + curr_size;
        _impl._begin = new_data;
        _impl._storage_end = _impl._begin + new_capacity;
    }

    void buyMore(size_type cnt)
    {
        size_type const curr_size = size();
        size_type const curr_capacity = capacity();

        if ((curr_size + cnt) <= curr_capacity)
        {
            _impl._end += cnt;
        }
        else
        {
            extendStorage(computeCapacity(size() + cnt));
            _impl._end += cnt;
        }
    }

    static constexpr usize BASE_CAPACITY_BYTES = BASE_CAPACITY * sizeof(TType);

    Impl _impl;
    alignas(value_type) u8 _buffer[BASE_CAPACITY_BYTES];
};

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::DynamicSmallArray<TType, BASE_CAPACITY, TAllocator>::iterator
    begin(sb::DynamicSmallArray<TType, BASE_CAPACITY, TAllocator> & vect)
{
    return vect.begin();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::DynamicSmallArray<TType, BASE_CAPACITY, TAllocator>::iterator
    end(sb::DynamicSmallArray<TType, BASE_CAPACITY, TAllocator> & vect)
{
    return vect.end();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::DynamicSmallArray<TType, BASE_CAPACITY, TAllocator>::const_iterator
    begin(sb::DynamicSmallArray<TType, BASE_CAPACITY, TAllocator> const & vect)
{
    return vect.begin();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::DynamicSmallArray<TType, BASE_CAPACITY, TAllocator>::const_iterator
    end(sb::DynamicSmallArray<TType, BASE_CAPACITY, TAllocator> const & vect)
{
    return vect.end();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::DynamicSmallArray<TType, BASE_CAPACITY, TAllocator>::const_iterator
    cbegin(sb::DynamicSmallArray<TType, BASE_CAPACITY, TAllocator> const & vect)
{
    return vect.cbegin();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::DynamicSmallArray<TType, BASE_CAPACITY, TAllocator>::const_iterator
    cend(sb::DynamicSmallArray<TType, BASE_CAPACITY, TAllocator> const & vect)
{
    return vect.cend();
}

template <typename TType, sb::usize BASE_CAPACITY, sb::usize BASE_CAPACITY2, typename TAllocator>
void swap(sb::DynamicSmallArray<TType, BASE_CAPACITY, TAllocator> & vect1,
          typename sb::DynamicSmallArray<TType, BASE_CAPACITY2, TAllocator> & vect2)
{
    vect1.swap(vect2);
}

template <typename TType, usize BASE_CAPACITY, typename TAllocator = ContainerAllocator>
using DSArray = DynamicSmallArray<TType, BASE_CAPACITY, TAllocator>;

} // namespace sb
