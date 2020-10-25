#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/_impl/memory/allocator/allocator_view.h>

#include <sb_std/type_traits>

namespace sb {

class AllocatorView
{
public:
    struct InitParams
    {
        InitParams() = default;

        template <typename T>
        InitParams(T & alloc)
            : m_vtable(&internal::getAllocatorVTable<T>())
            , m_alloc(&alloc)
        {
        }

        InitParams(AllocatorView & alloc)
            : m_vtable(alloc.m_vtable)
            , m_alloc(alloc.m_alloc)
        {
        }

        internal::AllocatorVTable const * m_vtable = nullptr;
        void * m_alloc = nullptr;
    };

    AllocatorView()
        : m_vtable(nullptr)
        , m_alloc(nullptr)
    {
    }

    AllocatorView(InitParams const & init)
        : m_vtable(init.m_vtable)
        , m_alloc(init.m_alloc)
    {
    }

    template <typename T>
    AllocatorView(T & alloc)
        : m_vtable(&internal::getAllocatorVTable<T>())
        , m_alloc(&alloc)
    {
    }

    AllocatorView(AllocatorView & src)
        : m_vtable(src.m_vtable)
        , m_alloc(src.m_alloc)
    {
    }

    AllocatorView(AllocatorView const & src)
        : m_vtable(src.m_vtable)
        , m_alloc(src.m_alloc)
    {
    }

    AllocatorView(AllocatorView &&) = default;

    AllocatorView & operator=(AllocatorView const &) = default;
    AllocatorView & operator=(AllocatorView &&) = default;

    MemoryArena allocate(usize const size)
    {
        return m_vtable->m_allocate(m_alloc, size);
    }

    MemoryArena allocate(usize const size, Alignment const alignment)
    {
        return m_vtable->m_aligned_allocate(m_alloc, size, alignment);
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
