#pragma once

#include <sb_core/os.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/allocator/allocator.h>

#include <sb_std/type_traits>
#include <sb_std/limits>
#include <sb_std/utility>

namespace sb {

template <typename T>
class STLAllocatorWrapper
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
    using allocator_type = IAllocator *;
    using allocator_type_reference = IAllocator &;

    template <class U>
    struct rebind
    {
        typedef STLAllocatorWrapper<U> other;
    };

    constexpr STLAllocatorWrapper()
        : m_alloc(nullptr)
    {
    }

    constexpr STLAllocatorWrapper(STLAllocatorWrapper const & src)
        : m_alloc(src.m_alloc)
    {
    }

    constexpr STLAllocatorWrapper(allocator_type src_alloc)
        : m_alloc(src_alloc)
    {
    }

    constexpr STLAllocatorWrapper(allocator_type_reference src_alloc)
        : m_alloc(&src_alloc)
    {
    }

    template <typename U>
    constexpr STLAllocatorWrapper(STLAllocatorWrapper<U> const & src)
        : m_alloc(src.get())
    {
    }

    ~STLAllocatorWrapper() = default;

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
        if (nullptr != m_alloc)
            return allocate(nb).m_ptr;

        return nullptr;
    }

    [[nodiscard]] pointer allocate(size_type nb)
    {
        if (nullptr != m_alloc)
            return (pointer)m_alloc->allocate(nb * sizeof(T), alignOf<T>()).m_ptr;

        return nullptr;
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

} // namespace sb
