#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/allocator/allocator.h>
#include <sb_core/_impl/memory/allocator/allocator_view.h>

#include <sb_std/type_traits>

namespace sb {

class AllocatorView : public IAllocator
{
public:
    AllocatorView()
        : m_vtable(nullptr)
        , m_alloc(nullptr)
    {
    }

    ~AllocatorView() override { }

    template <typename T>
    AllocatorView(T & alloc) requires !sbstd::is_same_v<T, AllocatorView>
        : m_vtable(&internal::getAllocatorVTable<T>()), m_alloc(&alloc)
    {
    }

    AllocatorView(AllocatorView const & src)
        : m_vtable(src.m_vtable)
        , m_alloc(src.m_alloc)
    {
    }

    AllocatorView(AllocatorView && src)
        : m_vtable(src.m_vtable)
        , m_alloc(src.m_alloc)
    {
        src.m_vtable = nullptr;
        src.m_alloc = nullptr;
    }

    AllocatorView & operator=(AllocatorView const & src)
    {
        m_vtable = src.m_vtable;
        m_alloc = src.m_alloc;

        return *this;
    }

    AllocatorView & operator=(AllocatorView && src)
    {
        m_vtable = src.m_vtable;
        m_alloc = src.m_alloc;

        src.m_vtable = nullptr;
        src.m_alloc = nullptr;

        return *this;
    }

    MemoryArena allocate(usize const size)
    {
        return m_vtable->m_allocate(m_alloc, size);
    }

    MemoryArena allocate(usize const size, Alignment const alignment)
    {
        return m_vtable->m_aligned_allocate(m_alloc, size, alignment);
    }

    void deallocate(MemoryArena arena) override
    {
        deallocate(arena.m_ptr);
    }

    void deallocate(void * ptr)
    {
        m_vtable->m_deallocate(m_alloc, ptr);
    }

    usize getAlignment() const
    {
        return m_vtable->m_alignment(m_alloc);
    }

    b8 owns(void const * ptr) const
    {
        return m_vtable->m_owns(m_alloc, ptr);
    }

    b8 isValid() const
    {
        return (nullptr != m_alloc);
    }

private:
    internal::AllocatorVTable const * m_vtable;
    void * m_alloc;
};

} // namespace sb
