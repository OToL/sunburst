#pragma once

#include <sb_core/os.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/utility.h>
#include <sb_core/memory/allocator/allocator.h>

#include <sb_slw/type_traits>
#include <sb_slw/limits>
#include <sb_slw/utility>

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
    using difference_type = iptrdiff;
    using propagate_on_container_move_assignment = slw::true_type;
    using is_always_equal = slw::true_type;
    using allocator_type = IAllocator *;
    using allocator_type_reference = IAllocator &;

    template <class U>
    struct rebind
    {
        using other = STLAllocatorWrapper<U>;
    };

    constexpr STLAllocatorWrapper() = default;

    constexpr STLAllocatorWrapper(STLAllocatorWrapper const & src)
        : _alloc(src._alloc)
    {
    }

    constexpr STLAllocatorWrapper(STLAllocatorWrapper && src)
        : _alloc(src._alloc)
    {
        src._alloc = nullptr;
    }

    constexpr STLAllocatorWrapper(allocator_type src_alloc)
        : _alloc(src_alloc)
    {
    }

    constexpr STLAllocatorWrapper(allocator_type_reference src_alloc)
        : _alloc(&src_alloc)
    {
    }

    template <typename U>
    constexpr STLAllocatorWrapper(STLAllocatorWrapper<U> const & src)
        : _alloc(src.get())
    {
    }

    ~STLAllocatorWrapper() = default;

    STLAllocatorWrapper & operator=(STLAllocatorWrapper const & src)
    {
        if (this != src)
        {
            _alloc = src._alloc;
        }

        return *this;
    }

    STLAllocatorWrapper & operator=(STLAllocatorWrapper && src)
    {
        if (this != src)
        {
            _alloc = src._alloc;
            src._alloc = nullptr;
        }

        return *this;
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
        if (nullptr != _alloc)
            return allocate(nb).data;

        return nullptr;
    }

    [[nodiscard]] pointer allocate(size_type nb)
    {
        if (nullptr != _alloc)
            return (pointer)_alloc->allocate(alignof(T), nb * sizeof(T)).data;

        return nullptr;
    }

    void deallocate(pointer data, size_type /*nb*/)
    {
        _alloc->deallocate((void *)data);
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

    allocator_type get() const
    {
        return _alloc;
    }

private:
    allocator_type _alloc = nullptr;
};

} // namespace sb
