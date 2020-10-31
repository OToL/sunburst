#pragma once

#include <sb_core/os.h>
#include <sb_core/memory/allocator/allocator_view.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/memory/memory.h>

#include <sb_std/type_traits>
#include <sb_std/limits>
#include <sb_std/utility>

namespace sb {

template <typename T, typename TAllocator = void>
class STLAllocator
{
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = T const *;
    using reference = T &;
    using const_reference = T const &;
    using size_type = usize;
    using difference_type = sptrdiff;
    using propagate_on_container_move_assignment = sbstd::true_type;
    using is_always_equal = sbstd::true_type;
    using allocator_type = TAllocator;

    template <class U>
    struct rebind
    {
        typedef STLAllocator<U, TAllocator> other;
    };

    constexpr STLAllocator() = default;

    constexpr STLAllocator(STLAllocator const & src) = default;

    constexpr STLAllocator(TAllocator const & src_alloc)
        : m_alloc(src_alloc)
    {
    }

    template <typename U>
    constexpr STLAllocator(STLAllocator<U, TAllocator> const & src)
        : m_alloc(src.get())
    {
    }

    ~STLAllocator() = default;

    pointer address(reference x) const
    {
        return &x;
    }

    const_pointer address(const_reference x) const
    {
        return &x;
    }

    [[nodiscard]] pointer allocate(size_type nb, void const * /*hint*/)
    {
        return allocate(nb);
    }

    [[nodiscard]] pointer allocate(size_type nb)
    {
        return (pointer)m_alloc.allocate(nb * sizeof(T), alignOf<T>()).m_ptr;
    }

    void deallocate(pointer data, size_type /*nb*/)
    {
        m_alloc.deallocate((void *)data);
    }

    size_type max_size() const
    {
        return sbstd::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    template <class U, class... TArgs>
    void construct(U * p, TArgs &&... args)
    {
        new ((void *)p) U(sbstd::forward<TArgs>(args)...);
    }

    template <class U>
    void destroy(U * p)
    {
        p->~U();
    }

    allocator_type const & get() const
    {
        return m_alloc;
    }

private:
    allocator_type m_alloc;
};

template <typename T, typename TAllocator>
class STLAllocator<T, TAllocator *>
{
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = T const *;
    using reference = T &;
    using const_reference = T const &;
    using size_type = usize;
    using difference_type = sptrdiff;
    using propagate_on_container_move_assignment = sbstd::true_type;
    using is_always_equal = sbstd::true_type;
    using allocator_type = TAllocator *;

    template <class U>
    struct rebind
    {
        typedef STLAllocator<U, TAllocator *> other;
    };

    constexpr STLAllocator()
        : m_alloc(nullptr)
    {
    }

    constexpr STLAllocator(STLAllocator const & src)
        : m_alloc(src.m_alloc)
    {
    }

    constexpr STLAllocator(TAllocator * src_alloc)
        : m_alloc(src_alloc)
    {
    }

    template <typename U>
    constexpr STLAllocator(STLAllocator<U, TAllocator *> const & src)
        : m_alloc(src.get())
    {
    }

    ~STLAllocator() = default;

    pointer address(reference x) const
    {
        return &x;
    }

    const_pointer address(const_reference x) const
    {
        return &x;
    }

    [[nodiscard]] pointer allocate(size_type nb, void const * /*hint*/)
    {
        return allocate(nb);
    }

    [[nodiscard]] pointer allocate(size_type nb)
    {
        return (pointer)m_alloc->allocate(nb * sizeof(T), alignOf<T>());
    }

    void deallocate(pointer data, size_type /*nb*/)
    {
        m_alloc->deallocate((void *)data);
    }

    size_type max_size() const
    {
        return sbstd::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    template <class U, class... TArgs>
    void construct(U * p, TArgs &&... args)
    {
        new ((void *)p) U(sbstd::forward<TArgs>(args)...);
    }

    template <class U>
    void destroy(U * p)
    {
        p->~U();
    }

    allocator_type get() const
    {
        return m_alloc;
    }

private:
    allocator_type m_alloc;
};

template <typename T>
class STLAllocator<T, void>
{
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = T const *;
    using reference = T &;
    using const_reference = T const &;
    using size_type = usize;
    using difference_type = sptrdiff;
    using propagate_on_container_move_assignment = sbstd::true_type;
    using is_always_equal = sbstd::true_type;

    template <class U>
    struct rebind
    {
        typedef STLAllocator<U, void> other;
    };

    constexpr STLAllocator() = default;

    template <typename U>
    constexpr STLAllocator(STLAllocator<U> const &)
    {
    }

    pointer address(reference x) const
    {
        return &x;
    }

    const_pointer address(const_reference x) const
    {
        return &x;
    }

    [[nodiscard]] pointer allocate(size_type nb, void const * /*hint*/)
    {
        return allocate(nb);
    }

    [[nodiscard]] pointer allocate(size_type nb)
    {
        if constexpr (ALIGNMENT_DEFAULT <= alignof(T))
        {
            return (pointer)getGlobalHeap().allocate(nb * sizeof(T)).m_ptr;
        }
        else
        {
            return (pointer)getGlobalHeap().allocate(nb * sizeof(T), alignOf<T>()).m_ptr;
        }
    }

    void deallocate(pointer data, size_type /*nb*/)
    {
        getGlobalHeap().deallocate((void *)data);
    }

    size_type max_size() const
    {
        return sbstd::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    template <class U, class... TArgs>
    void construct(U * p, TArgs &&... args)
    {
        new ((void *)p) U(sbstd::forward<TArgs>(args)...);
    }

    template <class U>
    void destroy(U * p)
    {
        p->~U();
    }
};

template <typename T>
using STLAllocatorView = STLAllocator<T, AllocatorView>;

} // namespace sb
