#pragma once

#include <sb_core/core.h>
#include <sb_core/conversion.h>
#include <sb_core/error/error.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/allocator/container_allocator.h>
#include <sb_core/_impl/container/small_array_base.h>

#include <sb_std/algorithm>
#include <sb_std/iterator>
#include <sb_std/memory>
#include <sb_std/utility>

namespace sb {

template <typename TType, usize BASE_CAPACITY, typename TAllocator = ContainerAllocator>
class SmallArray : public internal::SmallArrayBase<TType, TAllocator>
{
    sbBaseClass(internal::SmallArrayBase<TType, TAllocator>);

    template <typename, usize, typename>
    friend class SmallArray;

    using Impl = typename BaseClass::Impl;

    static constexpr usize BITS_PER_WORD = static_cast<usize>(sizeof(typename BaseClass::size_type) * 8);

public:
    using typename BaseClass::const_iterator;
    using typename BaseClass::const_pointer;
    using typename BaseClass::const_reference;
    using typename BaseClass::difference_type;
    using typename BaseClass::iterator;
    using typename BaseClass::pointer;
    using typename BaseClass::reference;
    using typename BaseClass::size_type;
    using typename BaseClass::value_type;
    using allocator_type = TAllocator;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    SmallArray()
        : SmallArray(reinterpret_cast<pointer>(&_buffer[0]))
    {
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    explicit SmallArray(allocator_type const & alloc)
        : SmallArray(reinterpret_cast<pointer>(&_buffer[0]), alloc)
    {
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    explicit SmallArray(size_type count, allocator_type const & alloc = allocator_type())
        : SmallArray(reinterpret_cast<pointer>(&_buffer[0]), alloc)
    {
        buyMore(count);

        sbstd::uninitialized_default_construct(_impl._begin, _impl._end);
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    SmallArray(size_type count, value_type const & value, allocator_type const & alloc = allocator_type())
        : SmallArray(reinterpret_cast<pointer>(&_buffer[0]), alloc)
    {
        buyMore(count);

        sbstd::uninitialized_fill(_impl._begin, _impl._end, value);
    }

    template <class TIterator>
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    SmallArray(TIterator first, TIterator last, allocator_type const & alloc = allocator_type())
        : SmallArray(reinterpret_cast<pointer>(&_buffer[0]), alloc)
    {
        buyMore(numericConv<size_type>(last - first));

        sbstd::uninitialized_copy(first, last, _impl._begin);
    }

    template <usize BASE_CAPACITY_SRC, typename TSrcAllocator>
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    SmallArray(SmallArray<TType, BASE_CAPACITY_SRC, TSrcAllocator> const & src)
        : SmallArray(reinterpret_cast<pointer>(&_buffer[0]), src._impl.get_allocator())
    {
        buyMore(src.size());

        sbstd::uninitialized_copy(sbstd::begin(src), sbstd::end(src), _impl._begin);
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    SmallArray(SmallArray const & src)
        : SmallArray(reinterpret_cast<pointer>(&_buffer[0]), src._impl.get_allocator())
    {
        buyMore(src.size());

        sbstd::uninitialized_copy(sbstd::begin(src), sbstd::end(src), _impl._begin);
    }

    template <usize BASE_CAPACITY_SRC, typename TSrcAllocator>
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    SmallArray(SmallArray<TType, BASE_CAPACITY_SRC, TSrcAllocator> const & src, allocator_type const & alloc)
        : SmallArray(reinterpret_cast<pointer>(&_buffer[0]), alloc)
    {
        buyMore(src.size());

        sbstd::uninitialized_copy(sbstd::begin(src), sbstd::end(src), _impl._begin);
    }

    template <usize BASE_CAPACITY_SRC>
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    SmallArray(SmallArray<TType, BASE_CAPACITY_SRC, TAllocator> && src)
        : _impl(src._impl.get_allocator())
    {
        if (!src.isSmallStorage() && (src.size() > BASE_CAPACITY))
        {
            sbstd::swap(_impl._begin, src._impl._begin);
            sbstd::swap(_impl._end, src._impl._end);
            sbstd::swap(_impl._storage_end, src._impl._storage_end);
        }
        else
        {
            _impl.init(reinterpret_cast<pointer>(&_buffer[0]), reinterpret_cast<pointer>(&_buffer[0]) + BASE_CAPACITY);

            buyMore(src.size());
            sbstd::uninitialized_move(sbstd::begin(src), sbstd::end(src), _impl._begin);

            src.clear();
        }
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    SmallArray(SmallArray && src)
        : _impl(src._impl.get_allocator())
    {
        if (!src.isSmallStorage() && (src.size() > BASE_CAPACITY))
        {
            sbstd::swap(_impl._begin, src._impl._begin);
            sbstd::swap(_impl._end, src._impl._end);
            sbstd::swap(_impl._storage_end, src._impl._storage_end);
        }
        else
        {
            _impl.init(reinterpret_cast<pointer>(&_buffer[0]), reinterpret_cast<pointer>(&_buffer[0]) + BASE_CAPACITY);

            buyMore(src.size());
            sbstd::uninitialized_move(sbstd::begin(src), sbstd::end(src), _impl._begin);

            src.clear();
        }
    }

    template <usize BASE_CAPACITY_SRC>
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    SmallArray(SmallArray<TType, BASE_CAPACITY_SRC, TAllocator> && src, allocator_type const & alloc)
        : _impl(alloc)
    {
        if (!src.isSmallStorage() && (src.size() > BASE_CAPACITY))
        {
            sbstd::swap(_impl._begin, src._impl._begin);
            sbstd::swap(_impl._end, src._impl._end);
            sbstd::swap(_impl._storage_end, src._impl._storage_end);
        }
        else
        {
            _impl.init(reinterpret_cast<pointer>(&_buffer[0]), reinterpret_cast<pointer>(&_buffer[0]) + BASE_CAPACITY);

            buyMore(src.size());
            sbstd::uninitialized_move(sbstd::begin(src), sbstd::end(src), _impl._begin);

            src.clear();
        }
    }

    ~SmallArray()
    {
        clear();

        if (!isSmallStorage())
        {
            _impl.deallocate(MemoryArena{_impl._begin, (size_type)(_impl._storage_end - _impl._begin)});
        }
    }

    template <usize BASE_CAPACITY_SRC>
    void swap(SmallArray<TType, BASE_CAPACITY_SRC, TAllocator> & src)
    {
        if (!isSmallStorage() && !src.isSmallStorage())
        {
            sbstd::swap(_impl._begin, src._impl._begin);
            sbstd::swap(_impl._end, src._impl._end);
            sbstd::swap(_impl._storage_end, src._impl._storage_end);
            sbstd::swap(_impl.get_allocator(), src._impl.get_allocator());
        }
        else
        {
            auto const src_size = src.size();
            auto const dst_size = size();
            auto const common_size = sbstd::min(src_size, dst_size);

            if ((src_size <= capacity()) && (dst_size <= src.capacity()))
            {
                sbstd::swap_ranges(_impl._begin, _impl._begin + common_size, src._impl._begin);

                if (src_size > dst_size)
                {
                    sbstd::uninitialized_move(src._impl._begin + common_size, src._impl._end, _impl._end);
                    sbstd::destroy(src._impl._begin + common_size, src._impl._end);
                }
                else if (dst_size > src_size)
                {
                    sbstd::uninitialized_move(_impl._begin + common_size, _impl._end, src._impl._end);
                    sbstd::destroy(_impl._begin + common_size, _impl._end);
                }

                _impl._end = _impl._begin + src_size;
                src._impl._end = src._impl._begin + dst_size;

                sbstd::swap(_impl.get_allocator(), src._impl.get_allocator());
            }
            else if (src_size > capacity())
            {
                size_type const new_dst_capacity = computeCapacity(src_size);
                auto const new_dst_data =
                    (TType *)_impl.allocate(alignOf<value_type>(), new_dst_capacity * sizeof(value_type)).data;
                sbAssert(nullptr != new_dst_data);

                sbstd::uninitialized_move(src._impl._begin, src._impl._end, new_dst_data);
                sbstd::move(_impl._begin, _impl._end, src._impl._begin);
                sbstd::destroy(_impl._begin, _impl._end);
                sbstd::destroy(src._impl._begin + dst_size, src._impl._end);

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
                    (TType *)src._impl.allocate(alignOf<value_type>(), new_src_capacity * sizeof(value_type)).data;
                sbAssert(nullptr != new_src_data);

                sbstd::uninitialized_move(_impl._begin, _impl._end, new_src_data);
                sbstd::move(src._impl._begin, src._impl._end, _impl._begin);
                sbstd::destroy(src._impl._begin, src._impl._end);
                sbstd::destroy(_impl._begin + src_size, _impl._end);

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
        sbAssert(!empty());

        return *_impl._begin;
    }

    const_reference front() const
    {
        sbAssert(!empty());

        return *_impl._begin;
    }

    reference back()
    {
        sbAssert(!empty());

        return *(_impl._end - 1);
    }

    const_reference back() const
    {
        sbAssert(!empty());

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
            sbstd::destroy(_impl._begin + count, _impl._end);
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
            sbstd::destroy(_impl._begin + count, _impl._end);
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

        new (_impl._end - 1) value_type(sbstd::move(value));

        return;
    }

    template <class... TArgs>
    value_type & emplace_back(TArgs &&... args)
    {
        buyMore(1);

        new (_impl._end - 1) TType(sbstd::forward<TArgs>(args)...);

        return *_impl._begin;
    }

    void pop_back()
    {
        sbAssert(!empty());

        sbstd::destroy_at(_impl._end - 1);
        --_impl._end;
    }

    void clear()
    {
        sbstd::destroy(_impl._begin, _impl._end);

        _impl._end = _impl._begin;
    }

    allocator_type get_allocator() const
    {
        return (allocator_type const &)_impl;
    }

    reference at(size_type idx)
    {
        sbAssert(idx < size());

        return *(_impl._begin + idx);
    }

    const_reference at(size_type idx) const
    {
        sbAssert(idx < size());

        return *(_impl._begin + idx);
    }

    reference operator[](size_type idx)
    {
        sbAssert(idx < size());

        return *(_impl._begin + idx);
    }

    const_reference operator[](size_type idx) const
    {
        sbAssert(idx < size());

        return *(_impl._begin + idx);
    }

    // NOLINTNEXTLINE(bugprone-unhandled-self-assignment)
    SmallArray & operator=(SmallArray const & src)
    {
        if (this != &src)
        {
            assign(src.begin(), src.end());
        }

        return *this;
    }

    template <usize BASE_CAPACITY_SRC, typename TSrcAllocator>
    SmallArray & operator=(SmallArray<TType, BASE_CAPACITY_SRC, TSrcAllocator> const & src)
    {
        assign(src.begin(), src.end());

        return *this;
    }

    // NOLINTNEXTLINE(bugprone-unhandled-self-assignment)
    SmallArray & operator=(SmallArray && src)
    {
        if (this != &src)
        {
            clear();
            this->swap(src);
        }

        return *this;
    }

    template <usize BASE_CAPACITY_SRC, typename TSrcAllocator>
    SmallArray & operator=(SmallArray<TType, BASE_CAPACITY_SRC, TSrcAllocator> && src)
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
                    sbstd::uninitialized_move(_impl._begin, _impl._end, reinterpret_cast<pointer>(&_buffer[0]));
                    sbstd::destroy(_impl._begin, _impl._end);
                    _impl.deallocate({_impl._begin, curr_capacity});

                    _impl._begin = reinterpret_cast<pointer>(&_buffer[0]);
                }
                else
                {
                    pointer const new_data =
                        (TType *)_impl.allocate(alignOf<value_type>(), new_capacity * sizeof(value_type)).data;
                    sbAssert(nullptr != new_data);

                    sbstd::uninitialized_move(_impl._begin, _impl._end, new_data);
                    sbstd::destroy(_impl._begin, _impl._end);
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
        auto const src_size = (size_type)sbstd::distance(src_begin, src_end);

        if (src_size <= curr_size)
        {
            sbstd::copy(src_begin, src_end, _impl._begin);
            sbstd::destroy(_impl._begin + src_size, end());

            _impl._end = _impl._begin + src_size;
        }
        else if (src_size > capacity())
        {
            auto const new_capacity = computeCapacity(src_size);
            pointer const new_data =
                (TType *)_impl.allocate(alignOf<value_type>(), new_capacity * sizeof(value_type)).data;
            sbAssert(nullptr != new_data);

            sbstd::uninitialized_copy(src_begin, src_end, new_data);
            sbstd::destroy(_impl._begin, _impl._end);

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
            sbstd::copy(src_begin, src_begin + curr_size, _impl._begin);
            sbstd::uninitialized_copy(src_begin + curr_size, src_end, _impl._end);

            _impl._end = _impl._begin + src_size;
        }
    }

    void assign(size_type count, TType const & value)
    {
        auto const curr_size = size();

        if (count <= curr_size)
        {
            sbstd::fill(_impl._begin, _impl._begin + count, value);
            sbstd::destroy(_impl._begin + count, _impl._end);

            _impl._end = _impl._begin + count;
        }
        else if (count > capacity())
        {
            auto const new_capacity = computeCapacity(count);
            pointer const new_data =
                (TType *)_impl.allocate(alignOf<value_type>(), new_capacity * sizeof(value_type)).data;
            sbAssert(nullptr != new_data);

            sbstd::uninitialized_fill(new_data, new_data + count, value);
            sbstd::destroy(_impl._begin, _impl._end);

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
            sbstd::fill(_impl._begin, _impl._end, value);
            sbstd::uninitialized_fill(_impl._end, _impl._begin + count, value);

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
    SmallArray(pointer begin, allocator_type const & alloc)
        : _impl(begin, begin + BASE_CAPACITY, alloc)
    {
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    SmallArray(pointer begin)
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
        sbAssert(new_size <= ms);

        size_type const curr_cap = capacity();

        if (curr_cap >= ms / 2)
        {
            return ms;
        }

        return sbstd::max(2 * curr_cap, alignSize(new_size));
    }

    void extendStorage(size_type new_capacity)
    {
        size_type const curr_capacity = capacity();
        size_type const curr_size = size();

        pointer const new_data = (TType *)_impl.allocate(alignOf<value_type>(), new_capacity * sizeof(value_type)).data;
        sbAssert(nullptr != new_data);

        sbstd::uninitialized_move(_impl._begin, _impl._end, new_data);
        sbstd::destroy(_impl._begin, _impl._end);

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
typename sb::SmallArray<TType, BASE_CAPACITY, TAllocator>::iterator
    begin(sb::SmallArray<TType, BASE_CAPACITY, TAllocator> & vect)
{
    return vect.begin();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::SmallArray<TType, BASE_CAPACITY, TAllocator>::iterator
    end(sb::SmallArray<TType, BASE_CAPACITY, TAllocator> & vect)
{
    return vect.end();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::SmallArray<TType, BASE_CAPACITY, TAllocator>::const_iterator
    begin(sb::SmallArray<TType, BASE_CAPACITY, TAllocator> const & vect)
{
    return vect.begin();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::SmallArray<TType, BASE_CAPACITY, TAllocator>::const_iterator
    end(sb::SmallArray<TType, BASE_CAPACITY, TAllocator> const & vect)
{
    return vect.end();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::SmallArray<TType, BASE_CAPACITY, TAllocator>::const_iterator
    cbegin(sb::SmallArray<TType, BASE_CAPACITY, TAllocator> const & vect)
{
    return vect.cbegin();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::SmallArray<TType, BASE_CAPACITY, TAllocator>::const_iterator
    cend(sb::SmallArray<TType, BASE_CAPACITY, TAllocator> const & vect)
{
    return vect.cend();
}

template <typename TType, sb::usize BASE_CAPACITY, sb::usize BASE_CAPACITY2, typename TAllocator>
void swap(sb::SmallArray<TType, BASE_CAPACITY, TAllocator> & vect1,
          typename sb::SmallArray<TType, BASE_CAPACITY2, TAllocator> & vect2)
{
    vect1.swap(vect2);
}

template <typename TType, usize BASE_CAPACITY, typename TAllocator = ContainerAllocator>
using SArray = SmallArray<TType, BASE_CAPACITY, TAllocator>;

} // namespace sb
