#pragma once

#include <sb_core/os.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/memory/memory.h>

#include <sb_std/type_traits>
#include <sb_std/limits>
#include <sb_std/utility>

namespace sb {

template <typename T>
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

    template <class U>
    struct rebind
    {
        typedef STLAllocator<U> other;
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
            return (pointer)getGlobalHeap().allocate(nb * sizeof(T)).data;
        }
        else
        {
            return (pointer)getGlobalHeap().allocate(nb * sizeof(T), alignOf<T>()).data;
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

} // namespace sb
