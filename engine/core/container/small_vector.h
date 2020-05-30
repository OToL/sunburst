#pragma once

#include <core/types.h>
#include <core/error.h>
#include <core/conversion.h>
#include <core/memory/allocator/stl_allocator.h>

#include <core/_impl/container/small_vector_base.h>

#include <libc++/utility>
#include <libc++/memory>
#include <libc++/algorithm>
#include <libc++/iterator>

namespace sb {

template <typename TType, usize BASE_CAPACITY, typename TAllocator = STLAllocator<TType>>
class SmallVector : public detail::SmallVectorBase<TType, TAllocator>
{
    sbBaseClass(detail::SmallVectorBase<TType, TAllocator>);

    template <typename, usize, typename>
    friend class SmallVector;

    using Impl = typename BaseClass::Impl;

    static constexpr usize BITS_PER_WORD = static_cast<usize>(sizeof(typename BaseClass::size_type) * 8);

public:
    using typename BaseClass::value_type;
    using typename BaseClass::reference;
    using typename BaseClass::const_reference;
    using typename BaseClass::pointer;
    using typename BaseClass::const_pointer;
    using typename BaseClass::iterator;
    using typename BaseClass::const_iterator;
    using typename BaseClass::size_type;
    using typename BaseClass::difference_type;
    using allocator_type = TAllocator;

    SmallVector()
        : SmallVector(reinterpret_cast<pointer>(&m_buffer[0]))
    {
    }

    explicit SmallVector(allocator_type const & alloc)
        : SmallVector(reinterpret_cast<pointer>(&m_buffer[0]), alloc)
    {
    }

    explicit SmallVector(size_type count, allocator_type const & alloc = allocator_type())
        : SmallVector(reinterpret_cast<pointer>(&m_buffer[0]), alloc)
    {
        buyMore(count);

        wstd::uninitialized_default_construct(m_impl.m_begin, m_impl.m_end);
    }

    SmallVector(size_type count, value_type const & value, allocator_type const & alloc = allocator_type())
        : SmallVector(reinterpret_cast<pointer>(&m_buffer[0]), alloc)
    {
        buyMore(count);

        wstd::uninitialized_fill(m_impl.m_begin, m_impl.m_end, value);
    }

    template< class TIterator >
    SmallVector(TIterator first, TIterator last, allocator_type const & alloc = allocator_type() )
        : SmallVector(reinterpret_cast<pointer>(&m_buffer[0]), alloc)
    {
        buyMore(numericCast<size_type>(last - first));

        wstd::uninitialized_copy(first, last, m_impl.m_begin);
    }

    template <usize BASE_CAPACITY_SRC, typename TSrcAllocator>
    SmallVector(SmallVector<TType, BASE_CAPACITY_SRC, TSrcAllocator> const & src)
        : SmallVector(reinterpret_cast<pointer>(&m_buffer[0]), src.m_impl.get_allocator())
    {
        buyMore(src.size());

        wstd::uninitialized_copy(wstd::begin(src), wstd::end(src), m_impl.m_begin);
    }

    template <usize BASE_CAPACITY_SRC, typename TSrcAllocator>
    SmallVector(SmallVector<TType, BASE_CAPACITY_SRC, TSrcAllocator> const & src, allocator_type const & alloc)
        : SmallVector(reinterpret_cast<pointer>(&m_buffer[0]), alloc)
    {
        buyMore(src.size());

        wstd::uninitialized_copy(wstd::begin(src), wstd::end(src), m_impl.m_begin);
    }

    template <usize BASE_CAPACITY_SRC>
    SmallVector(SmallVector<TType, BASE_CAPACITY_SRC, TAllocator> && src)
        : m_impl(src.m_impl.get_allocator())
    {
        if (!src.isSmallStorage() && (src.size() > BASE_CAPACITY))
        {
            wstd::swap(m_impl.m_begin, src.m_impl.m_begin);
            wstd::swap(m_impl.m_end, src.m_impl.m_end);
            wstd::swap(m_impl.m_storage_end, src.m_impl.m_storage_end);
        }
        else
        {
            m_impl.init(reinterpret_cast<pointer>(&m_buffer[0]), reinterpret_cast<pointer>(&m_buffer[0]) + BASE_CAPACITY);

            buyMore(src.size());
            wstd::uninitialized_move(wstd::begin(src), wstd::end(src), m_impl.m_begin);

            src.clear();
        }        
    }

    template <usize BASE_CAPACITY_SRC>
    SmallVector(SmallVector<TType, BASE_CAPACITY_SRC, TAllocator> && src, allocator_type const & alloc)
        : m_impl(alloc)
    {
        if (!src.isSmallStorage() && (src.size() > BASE_CAPACITY))
        {
            wstd::swap(m_impl.m_begin, src.m_impl.m_begin);
            wstd::swap(m_impl.m_end, src.m_impl.m_end);
            wstd::swap(m_impl.m_storage_end, src.m_impl.m_storage_end);
        }
        else
        {
            m_impl.init(reinterpret_cast<pointer>(&m_buffer[0]), reinterpret_cast<pointer>(&m_buffer[0]) + BASE_CAPACITY);

            buyMore(src.size());
            wstd::uninitialized_move(wstd::begin(src), wstd::end(src), m_impl.m_begin);

            src.clear();
        } 
    }

    ~SmallVector()
    {
        clear();

        if (!isSmallStorage())
        {
            m_impl.deallocate(m_impl.m_begin, (size_type)(m_impl.m_storage_end - m_impl.m_begin));
        }
    }

    template <usize BASE_CAPACITY_SRC>
    void swap(SmallVector<TType, BASE_CAPACITY_SRC, TAllocator> & src)
    {
        if (!isSmallStorage() && !src.isSmallStorage())
        {
            wstd::swap(m_impl.m_begin, src.m_impl.m_begin);
            wstd::swap(m_impl.m_end, src.m_impl.m_end);
            wstd::swap(m_impl.m_storage_end, src.m_impl.m_storage_end);
            wstd::swap(m_impl.get_allocator(), src.m_impl.get_allocator());
        }
        else
        {
            auto const src_size = src.size();
            auto const dst_size = size();
            auto const common_size = wstd::min(src_size, dst_size);

            if ((src_size <= capacity()) && (dst_size <= src.capacity()))
            {
                wstd::swap_ranges(m_impl.m_begin, m_impl.m_begin + common_size, src.m_impl.m_begin);

                if (src_size > dst_size)
                {
                    wstd::uninitialized_move(src.m_impl.m_begin + common_size, src.m_impl.m_end, m_impl.m_end);
                    wstd::destroy(src.m_impl.m_begin + common_size, src.m_impl.m_end);
                }
                else if (dst_size > src_size)
                {
                    wstd::uninitialized_move(m_impl.m_begin + common_size, m_impl.m_end, src.m_impl.m_end);
                    wstd::destroy(m_impl.m_begin + common_size, m_impl.m_end);
                }

                m_impl.m_end = m_impl.m_begin + src_size;
                src.m_impl.m_end = src.m_impl.m_begin + dst_size;

                wstd::swap(m_impl.get_allocator(), src.m_impl.get_allocator());   
            }
            else if (src_size > capacity())
            {
                size_type const new_dst_capacity = computeCapacity(src_size);
                TType * const new_dst_data = m_impl.allocate(new_dst_capacity);
                sbAssert(nullptr != new_dst_data);

                wstd::uninitialized_move(src.m_impl.m_begin, src.m_impl.m_end, new_dst_data);
                wstd::move(m_impl.m_begin, m_impl.m_end, src.m_impl.m_begin);
                wstd::destroy(m_impl.m_begin, m_impl.m_end);
                wstd::destroy(src.m_impl.m_begin + dst_size, src.m_impl.m_end);

                if (!isSmallStorage())
                {   
                    m_impl.deallocate(m_impl.m_begin, dst_size);
                }

                m_impl.m_begin = new_dst_data;  
                m_impl.m_end = m_impl.m_begin + src_size;
                m_impl.m_storage_end = m_impl.m_begin + new_dst_capacity;

                src.m_impl.m_end = src.m_impl.m_begin + dst_size;
            }
            else
            {
                size_type const new_src_capacity = computeCapacity(dst_size);
                TType * const new_src_data = src.m_impl.allocate(new_src_capacity);
                sbAssert(nullptr != new_src_data);

                wstd::uninitialized_move(m_impl.m_begin, m_impl.m_end, new_src_data);
                wstd::move(src.m_impl.m_begin, src.m_impl.m_end, m_impl.m_begin);
                wstd::destroy(src.m_impl.m_begin, src.m_impl.m_end);
                wstd::destroy(m_impl.m_begin + src_size, m_impl.m_end);

                if (!src.isSmallStorage())
                {   
                    src.m_impl.deallocate(src.m_impl.m_begin, src_size);
                }

                src.m_impl.m_begin = new_src_data;  
                src.m_impl.m_end = src.m_impl.m_begin + dst_size;
                src.m_impl.m_storage_end = src.m_impl.m_begin + new_src_capacity;

                m_impl.m_end = m_impl.m_begin + src_size;
            }
        }
    }

    b8 empty() const
    {
        return (m_impl.m_begin == m_impl.m_end);
    }

    size_type size() const
    {
        return (size_type)(m_impl.m_end - m_impl.m_begin);
    }

    const_iterator cbegin() const
    {
        return m_impl.m_begin;
    }

    const_iterator begin() const
    {
        return m_impl.m_begin;
    }

    iterator begin()
    {
        return m_impl.m_begin;
    }

    const_iterator cend() const
    {
        return m_impl.m_end;
    }

    const_iterator end() const
    {
        return m_impl.m_end;
    }

    iterator end()
    {
        return m_impl.m_end;
    }

    const_pointer data() const
    {
        return m_impl.m_begin;
    }

    pointer data()
    {
        return m_impl.m_begin;
    }

    reference front()
    {
        sbAssert(!empty());

        return *m_impl.m_begin;
    }

    const_reference front() const
    {
        sbAssert(!empty());

        return *m_impl.m_begin;
    }

    reference back()
    {
        sbAssert(!empty());

        return *(m_impl.m_end - 1);
    }

    const_reference back() const
    {
        sbAssert(!empty());

        return *(m_impl.m_end - 1);
    }

    size_type capacity() const
    {
        return (size_type)(m_impl.m_storage_end - m_impl.m_begin);
    }

    constexpr size_type max_size() const
    {
        return (size_type)~((difference_type)0);
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
            wstd::destroy(m_impl.m_begin + count, m_impl.m_end);
            m_impl.m_end = m_impl.m_begin + count;
            
        }
        else if (curr_size < count)
        {
            size_type const size_diff = (size_type)(count - curr_size);

            buyMore(size_diff);
            std::uninitialized_default_construct(m_impl.m_begin + curr_size, m_impl.m_end);
        }
    }

    void resize( size_type count, const value_type& value )
    {
        auto const curr_size = size();

        if (curr_size > count)
        {
            wstd::destroy(m_impl.m_begin + count, m_impl.m_end);
            m_impl.m_end = m_impl.m_begin + count;
            
        }
        else if (curr_size < count)
        {
            size_type const size_diff = (size_type)(count - curr_size);

            buyMore(size_diff);
            std::uninitialized_fill(m_impl.m_begin + curr_size, m_impl.m_end, value);
        }
    }

    void push_back(const_reference value)
    {
        buyMore(1);

        new(m_impl.m_end - 1) value_type(value);

        return;
    }

    void push_back(value_type && value)
    {
        buyMore(1);

        new (m_impl.m_end - 1) value_type(wstd::move(value));

        return;
    }

    template <class... TArgs>
    value_type & emplace_back(TArgs &&... args)
    {
        buyMore(1);

        new (m_impl.m_end - 1) TType(wstd::forward<TArgs>(args)...);

        return *m_impl.m_begin;
    }

    void pop_back()
    {
        sbAssert(!empty());

        wstd::destroy_at(m_impl.m_end -1);
        --m_impl.m_end;
    }

    void clear()
    {
        wstd::destroy(m_impl.m_begin, m_impl.m_end);

        m_impl.m_end = m_impl.m_begin;
    }

    allocator_type get_allocator() const
    {
        return (allocator_type const &)m_impl;
    }

    reference at(size_type idx)
    {
        sbAssert(idx < size());

        return *(m_impl.m_begin + idx);
    }

    const_reference at(size_type idx) const
    {
        sbAssert(idx < size());

        return *(m_impl.m_begin + idx);
    }

    reference operator [] (size_type idx)
    {
        sbAssert(idx < size());

        return *(m_impl.m_begin + idx);
    }

    const_reference operator [] (size_type idx) const
    {
        sbAssert(idx < size());

        return *(m_impl.m_begin + idx);
    }

    SmallVector & operator = (SmallVector const & src)
    {
        if (this != &src)
        {
            assign(src.begin(), src.end());
        }

        return *this;
    }    

    template <usize BASE_CAPACITY_SRC, typename TSrcAllocator>
    SmallVector & operator = (SmallVector<TType, BASE_CAPACITY_SRC,  TSrcAllocator> const & src)
    {
        assign(src.begin(), src.end());
        
        return *this;
    }

    SmallVector & operator = (SmallVector && src)
    {
        if (this != &src)
        {
            clear();
            this->swap(src);
        }

        return *this;
    }

    template <usize BASE_CAPACITY_SRC, typename TSrcAllocator>
    SmallVector & operator = (SmallVector<TType, BASE_CAPACITY_SRC,  TSrcAllocator> && src)
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
                    wstd::uninitialized_move(m_impl.m_begin, m_impl.m_end, reinterpret_cast<pointer>(&m_buffer[0]));
                    wstd::destroy(m_impl.m_begin, m_impl.m_end);
                    m_impl.deallocate(m_impl.m_begin, curr_capacity);

                    m_impl.m_begin = reinterpret_cast<pointer>(&m_buffer[0]);
                    
                }
                else
                {
                    pointer const new_data = m_impl.allocate(new_capacity);
                    sbAssert(nullptr != new_data);

                    wstd::uninitialized_move(m_impl.m_begin, m_impl.m_end, new_data);
                    wstd::destroy(m_impl.m_begin, m_impl.m_end);
                    m_impl.deallocate(m_impl.m_begin, curr_capacity);

                    m_impl.m_begin = new_data;
                }

                m_impl.m_end = m_impl.m_begin + new_capacity;
                m_impl.m_storage_end = m_impl.m_end;
            }
        }
    }

    template<class InputIt>
    void assign(InputIt src_begin, InputIt src_end)
    {
        auto const curr_size = size();
        auto const src_size = (size_type)wstd::distance(src_begin, src_end);

        if (src_size <= curr_size)
        {
            wstd::copy(src_begin, src_end, m_impl.m_begin);
            wstd::destroy(m_impl.m_begin + src_size, end());

            m_impl.m_end = m_impl.m_begin + src_size; 
        }
        else if (src_size > capacity())
        {
            auto const new_capacity = computeCapacity(src_size);
            pointer const new_data = m_impl.allocate(new_capacity);
            sbAssert(nullptr != new_data);

            wstd::uninitialized_copy(src_begin, src_end, new_data);
            wstd::destroy(m_impl.m_begin, m_impl.m_end);

            if (!isSmallStorage())
            {
                m_impl.deallocate(m_impl.m_begin, curr_size);
            }

            m_impl.m_begin = new_data;
            m_impl.m_end = new_data + src_size;
            m_impl.m_storage_end = new_data + new_capacity;
        }
        else
        {
            wstd::copy(src_begin, src_begin + curr_size, m_impl.m_begin);
            wstd::uninitialized_copy(src_begin + curr_size, src_end, m_impl.m_end);

            m_impl.m_end = m_impl.m_begin + src_size;
        }
    }

    void assign(size_type count, TType const & value)
    {
        auto const curr_size = size();

        if (count <= curr_size)
        {
            wstd::fill(m_impl.m_begin, m_impl.m_begin + count, value);
            wstd::destroy(m_impl.m_begin + count, m_impl.m_end);

            m_impl.m_end = m_impl.m_begin + count; 
        }
        else if (count > capacity())
        {
            auto const new_capacity = computeCapacity(count);
            pointer const new_data = m_impl.allocate(new_capacity);
            sbAssert(nullptr != new_data);

            wstd::uninitialized_fill(new_data, new_data + count, value);
            wstd::destroy(m_impl.m_begin, m_impl.m_end);

            if (!isSmallStorage())
            {
                m_impl.deallocate(m_impl.m_begin, curr_size);
            }

            m_impl.m_begin = new_data;
            m_impl.m_end = new_data + count;
            m_impl.m_storage_end = new_data + new_capacity;
        }
        else
        {
            wstd::fill(m_impl.m_begin, m_impl.m_end, value);
            wstd::uninitialized_fill(m_impl.m_end, m_impl.m_begin + count, value);

            m_impl.m_end = m_impl.m_begin + count;
        }
    }

    // non-standard extensions

    bool isSmallStorage() const
    {
        return (m_impl.m_begin == reinterpret_cast<const_pointer>(&m_buffer[0]));
    }

    allocator_type & get_allocator_ref()
    {
        return m_impl.get_allocator();
    }

    allocator_type const & get_allocator_ref() const
    {
        return m_impl.get_allocator();
    }

private:

    SmallVector(pointer begin, allocator_type const & alloc)
        : m_impl(begin, begin + BASE_CAPACITY, alloc)
    {
    }

    SmallVector(pointer begin)
        :  m_impl(begin, begin + BASE_CAPACITY)
    {
    }

    static constexpr size_type alignSize(size_type new_size) 
    {
        return new_size + (BITS_PER_WORD-1) & ~((size_type)BITS_PER_WORD-1);
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

        return wstd::max(2*curr_cap, alignSize(new_size));
    }

    void extendStorage(size_type new_capacity)
    {
        size_type const curr_capacity = capacity(); 
        size_type const curr_size = size();

        pointer const new_data = m_impl.allocate(new_capacity);
        sbAssert(nullptr != new_data);
        
        wstd::uninitialized_move(m_impl.m_begin, m_impl.m_end, new_data);
        wstd::destroy(m_impl.m_begin, m_impl.m_end);

        if (!isSmallStorage())
        {
            m_impl.deallocate(m_impl.m_begin, curr_capacity);
        }

        m_impl.m_end = new_data + curr_size;
        m_impl.m_begin = new_data;
        m_impl.m_storage_end = m_impl.m_begin + new_capacity;
    }

    void buyMore(size_type cnt)
    {
        size_type const curr_size = size();
        size_type const curr_capacity = capacity(); 

        if ((curr_size + cnt) <= curr_capacity)
        {
            m_impl.m_end += cnt;
        }
        else
        {
            extendStorage(computeCapacity(size() + cnt));
            m_impl.m_end += cnt;
        }
    }

    static constexpr usize BASE_CAPACITY_BYTES = BASE_CAPACITY * sizeof(TType);

    Impl m_impl;
    alignas(value_type) ui8 m_buffer[BASE_CAPACITY_BYTES];
};

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::SmallVector<TType, BASE_CAPACITY, TAllocator>::iterator begin( sb::SmallVector<TType, BASE_CAPACITY, TAllocator> & vect)
{
    return vect.begin();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::SmallVector<TType, BASE_CAPACITY, TAllocator>::iterator end( sb::SmallVector<TType, BASE_CAPACITY, TAllocator> & vect)
{
    return vect.end();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::SmallVector<TType, BASE_CAPACITY, TAllocator>::const_iterator begin( sb::SmallVector<TType, BASE_CAPACITY, TAllocator> const & vect)
{
    return vect.begin();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::SmallVector<TType, BASE_CAPACITY, TAllocator>::const_iterator end( sb::SmallVector<TType, BASE_CAPACITY, TAllocator> const & vect)
{
    return vect.end();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::SmallVector<TType, BASE_CAPACITY, TAllocator>::const_iterator cbegin( sb::SmallVector<TType, BASE_CAPACITY, TAllocator> const & vect)
{
    return vect.cbegin();
}

template <typename TType, sb::usize BASE_CAPACITY, typename TAllocator>
typename sb::SmallVector<TType, BASE_CAPACITY, TAllocator>::const_iterator cend( sb::SmallVector<TType, BASE_CAPACITY, TAllocator> const & vect)
{
    return vect.cend();
}

template <typename TType, sb::usize BASE_CAPACITY, sb::usize BASE_CAPACITY2, typename TAllocator>
void swap( sb::SmallVector<TType, BASE_CAPACITY, TAllocator> & vect1, typename sb::SmallVector<TType, BASE_CAPACITY2, TAllocator> & vect2)
{
    vect1.swap(vect2);
}

}
