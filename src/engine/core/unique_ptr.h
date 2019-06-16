#pragma once

#include "memory/memory.h"

#include <libc++/memory>
#include <libc++/functional>
#include <libc++/utility>

namespace sb {

// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0316r0.html
template<class T, class TAllocator>
class AllocatorDelete
{
public:

    using AllocatorType = TAllocator;
    using ValueType = wstd::remove_all_extents_t<T>;
    using PointerType = ValueType *;

    template <class U>
    using RebindDelete = AllocatorDelete<U, TAllocator>;

    AllocatorDelete() = default;

    template <typename U>
    AllocatorDelete(AllocatorDelete<U, AllocatorType> const & src)
        : m_allocator(src.m_allocator)
    {
    }

    template<class OtherAllocator>
    AllocatorDelete(OtherAllocator&& other) :
        m_allocator(wstd::forward<OtherAllocator>(other))
    {
    }

    void operator()(PointerType p)
    {
        if constexpr(wstd::is_array_v<T>)
        {
            sbDeleteArray(p, m_allocator);
        }
        else
        {
            sbDelete(p, m_allocator);
        }
    }

private:

    AllocatorType m_allocator;
};

template<class T, class TAllocator>
class AllocatorDelete<T, TAllocator &>
{
public:

    using AllocatorType = TAllocator;
    using ValueType = wstd::remove_all_extents_t<T>;
    using PointerType = ValueType *;

    template <class U>
    using RebindDelete = AllocatorDelete<U, TAllocator &>;

    AllocatorDelete() = default;

    template <typename U>
    AllocatorDelete(AllocatorDelete<U, AllocatorType> const & src)
        : m_allocator(src.m_allocator)
    {
    }

    AllocatorDelete(wstd::reference_wrapper<AllocatorType> other) :
        m_allocator(other)
    {
    }

    void operator()(PointerType p)
    {
        if constexpr(wstd::is_array_v<T>)
        {
            sbDeleteArray(p, m_allocator.get());
        }
        else
        {
            sbDelete(p, m_allocator.get());
        }
    }

private:

    wstd::reference_wrapper<AllocatorType> m_allocator; 
};

template<class T>
class DefaultDelete
{
public:

    using PointerType = T *;

    template <class U>
    using RebindDelete = DefaultDelete<U>;

    DefaultDelete() = default;

    template <typename U>
    DefaultDelete(U const &)
    {
    }

    void operator()(PointerType p)
    {
        sbDelete(p);
    }
};

template<class T>
class DefaultDelete<T []>
{
public:

    using PointerType = T *;

    void operator()(PointerType p)
    {
        sbDeleteArray(p);
    }
};

template <typename T>
using AllocatorViewDelete = AllocatorDelete<T, AllocatorView>;

template <typename T, typename TDelete = DefaultDelete<T>>
using UniquePtr = wstd::detail::unique_ptr<T, TDelete>;

template <typename T>
struct is_reference_wrapper : wstd::false_type {};

template <typename T>
struct is_reference_wrapper<wstd::reference_wrapper<T>> : wstd::true_type {};

template<typename T, typename TAlloc, class... TArgs>
auto allocateUnique(TAlloc const & alloc, TArgs&&... args) -> wstd::enable_if_t<!wstd::is_array_v<T>, UniquePtr<T, AllocatorDelete<T, TAlloc>>>
{
    using DeleteType = AllocatorDelete<T, TAlloc>;
    
    return UniquePtr<T, DeleteType>{sbNew(T, alloc)(wstd::forward<TArgs>(args)...), DeleteType{alloc}};
}

template<class T, typename TAlloc, class... TArgs>
auto allocateUnique(wstd::reference_wrapper<TAlloc> alloc, TArgs&&... args) -> wstd::enable_if_t<!wstd::is_array_v<T>, UniquePtr<T, AllocatorDelete<T, TAlloc &>>>
{
    using DeleteType = AllocatorDelete<T, TAlloc &>;

    return UniquePtr<T, DeleteType>{sbNew(T, alloc.get())(wstd::forward<TArgs>(args)...), DeleteType{alloc}};
}

template<typename T, typename TAlloc>
auto allocateUnique(TAlloc const & alloc, usize cnt) -> wstd::enable_if_t<wstd::is_array_v<T>, UniquePtr<T, AllocatorDelete<T, TAlloc>>>
{
    using DeleteType = AllocatorDelete<T, TAlloc>;
    using ValueType = wstd::remove_all_extents_t<T>;

    return UniquePtr<T, DeleteType>{sbNewArray(ValueType, alloc)[cnt], DeleteType{alloc}};
}

template<class T, typename TAlloc, class... TArgs>
auto allocateUnique(wstd::reference_wrapper<TAlloc> alloc, usize cnt) -> wstd::enable_if_t<wstd::is_array_v<T>, UniquePtr<T, AllocatorDelete<T, TAlloc &>>>
{
    using DeleteType = AllocatorDelete<T, TAlloc &>;
    using ValueType = wstd::remove_all_extents_t<T>;

    return UniquePtr<T, DeleteType>{sbNewArray(ValueType, alloc.get())[cnt], DeleteType{alloc}};
}

template<class T>
auto makeUnique(usize cnt) -> wstd::enable_if_t<wstd::is_array_v<T>, UniquePtr<T>>
{
    return UniquePtr<T>{sbNewArray(wstd::remove_all_extents_t<T>)[cnt]};
}

template<class T, class... TArgs>
auto makeUnique(TArgs&&... args) -> wstd::enable_if_t<!wstd::is_array_v<T>, UniquePtr<T>>
{
    return UniquePtr<T>{sbNew(T)(wstd::forward<TArgs>(args)...)};
}

}

namespace SB_STD_NS {

    template< class U ,class T,  class A> 
    auto static_pointer_cast( std::unique_ptr<T, A> && r ) noexcept
    {
        return std::unique_ptr<U, typename A::template RebindDelete<U>>{static_cast<U *>(r.release()), r.get_deleter()};
    }

    template< class U , class T, class A> 
    auto static_pointer_cast( std::unique_ptr<T, A> &  r ) noexcept
    {
        return std::unique_ptr<U, typename A::template RebindDelete<U>>{static_cast<U *>(r.release()), r.get_deleter()};
    }

}
