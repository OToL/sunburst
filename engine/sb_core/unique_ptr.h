#pragma once

#include <sb_core/memory/alloc.h>

#include <sb_std/memory>
#include <sb_std/functional>
#include <sb_std/utility>

namespace sb {

// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0316r0.html
template <class T, class TAllocator>
class AllocatorDelete
{
public:
    using AllocatorType = TAllocator;
    using ValueType = sbstd::remove_all_extents_t<T>;
    using PointerType = ValueType *;

    template <class U>
    using RebindDelete = AllocatorDelete<U, TAllocator>;

    AllocatorDelete() = default;

    template <typename U>
    AllocatorDelete(AllocatorDelete<U, AllocatorType> const & src)
        : m_allocator(src.m_allocator)
    {
    }

    template <class OtherAllocator>
    AllocatorDelete(OtherAllocator && other)
        : m_allocator(sbstd::forward<OtherAllocator>(other))
    {
    }

    void operator()(PointerType p)
    {
        if constexpr (sbstd::is_array_v<T>)
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

template <class T, class TAllocator>
class AllocatorDelete<T, TAllocator &>
{
public:
    using AllocatorType = TAllocator;
    using ValueType = sbstd::remove_all_extents_t<T>;
    using PointerType = ValueType *;

    template <class U>
    using RebindDelete = AllocatorDelete<U, TAllocator &>;

    AllocatorDelete() = default;

    template <typename U>
    AllocatorDelete(AllocatorDelete<U, AllocatorType> const & src)
        : m_allocator(src.m_allocator)
    {
    }

    AllocatorDelete(sbstd::reference_wrapper<AllocatorType> other)
        : m_allocator(other)
    {
    }

    void operator()(PointerType p)
    {
        if constexpr (sbstd::is_array_v<T>)
        {
            sbDeleteArray(p, m_allocator.get());
        }
        else
        {
            sbDelete(p, m_allocator.get());
        }
    }

private:
    sbstd::reference_wrapper<AllocatorType> m_allocator;
};

template <class T>
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

template <class T>
class DefaultDelete<T[]>
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
using UniquePtr = sbstd::internal::unique_ptr<T, TDelete>;

template <typename T>
struct is_reference_wrapper : sbstd::false_type
{
};

template <typename T>
struct is_reference_wrapper<sbstd::reference_wrapper<T>> : sbstd::true_type
{
};

template <typename T, typename TAlloc, class... TArgs>
auto allocateUnique(TAlloc const & alloc, TArgs &&... args)
    -> sbstd::enable_if_t<!sbstd::is_array_v<T>, UniquePtr<T, AllocatorDelete<T, TAlloc>>>
{
    using DeleteType = AllocatorDelete<T, TAlloc>;

    return UniquePtr<T, DeleteType>{sbNew(T, alloc)(sbstd::forward<TArgs>(args)...),
                                    DeleteType{alloc}};
}

template <class T, typename TAlloc, class... TArgs>
auto allocateUnique(sbstd::reference_wrapper<TAlloc> alloc, TArgs &&... args)
    -> sbstd::enable_if_t<!sbstd::is_array_v<T>, UniquePtr<T, AllocatorDelete<T, TAlloc &>>>
{
    using DeleteType = AllocatorDelete<T, TAlloc &>;

    return UniquePtr<T, DeleteType>{sbNew(T, alloc.get())(sbstd::forward<TArgs>(args)...),
                                    DeleteType{alloc}};
}

template <typename T, typename TAlloc>
auto allocateUnique(TAlloc const & alloc, usize cnt)
    -> sbstd::enable_if_t<sbstd::is_array_v<T>, UniquePtr<T, AllocatorDelete<T, TAlloc>>>
{
    using DeleteType = AllocatorDelete<T, TAlloc>;
    using ValueType = sbstd::remove_all_extents_t<T>;

    return UniquePtr<T, DeleteType>{sbNewArray(ValueType, alloc)[cnt], DeleteType{alloc}};
}

template <class T, typename TAlloc, class... TArgs>
auto allocateUnique(sbstd::reference_wrapper<TAlloc> alloc, usize cnt)
    -> sbstd::enable_if_t<sbstd::is_array_v<T>, UniquePtr<T, AllocatorDelete<T, TAlloc &>>>
{
    using DeleteType = AllocatorDelete<T, TAlloc &>;
    using ValueType = sbstd::remove_all_extents_t<T>;

    return UniquePtr<T, DeleteType>{sbNewArray(ValueType, alloc.get())[cnt], DeleteType{alloc}};
}

template <class T>
auto makeUnique(usize cnt) -> sbstd::enable_if_t<sbstd::is_array_v<T>, UniquePtr<T>>
{
    return UniquePtr<T>{sbNewArray(sbstd::remove_all_extents_t<T>)[cnt]};
}

template <class T, class... TArgs>
auto makeUnique(TArgs &&... args) -> sbstd::enable_if_t<!sbstd::is_array_v<T>, UniquePtr<T>>
{
    return UniquePtr<T>{sbNew(T)(sbstd::forward<TArgs>(args)...)};
}

} // namespace sb

namespace SB_STD_NS {

template <class U, class T, class A>
auto static_pointer_cast(std::unique_ptr<T, A> && r) noexcept
{
    return std::unique_ptr<U, typename A::template RebindDelete<U>>{static_cast<U *>(r.release()),
                                                                    r.get_deleter()};
}

template <class U, class T, class A>
auto static_pointer_cast(std::unique_ptr<T, A> & r) noexcept
{
    return std::unique_ptr<U, typename A::template RebindDelete<U>>{static_cast<U *>(r.release()),
                                                                    r.get_deleter()};
}

} // namespace SB_STD_NS
