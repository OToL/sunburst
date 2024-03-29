#pragma once

#include <sb_core/system.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/memory/memory.h>

#include <sb_slw/type_traits>
#include <sb_slw/limits>
#include <sb_slw/utility>

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
    using difference_type = iptrdiff;
    using propagate_on_container_move_assignment = slw::true_type;
    using is_always_equal = slw::true_type;

    template <class U>
    struct rebind
    {
        using other = STLAllocator<U>;
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
        if constexpr (DEFAULT_MEMORY_ALIGNMENT <= alignof(T))
        {
            return (pointer)getGlobalHeap().allocate(nb * sizeof(T)).data;
        }
        else
        {
            return (pointer)getGlobalHeap().allocate(alignof(T), nb * sizeof(T)).data;
        }
    }

    void deallocate(pointer data, size_type /*nb*/)
    {
        getGlobalHeap().deallocate((void *)data);
    }

    size_type max_size() const
    {
        return slw::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    template <class U, class... TArgs>
    void construct(U * p, TArgs &&... args)
    {
        new ((void *)p) U(slw::forward<TArgs>(args)...);
    }

    template <class U>
    void destroy(U * p)
    {
        p->~U();
    }
};

} // namespace sb
