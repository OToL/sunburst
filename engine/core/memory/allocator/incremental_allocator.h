#pragma once

#include <core/base.h>
#include <core/memory/memory.h>
#include <core/memory/memory_arena.h>
#include <core/bit.h>
#include <core/error.h>
#include "allocator.h"

namespace sb {

template <typename TMemProvider>
class IncrementalAllocator final : public IAllocator
{
    sbBaseClass(IAllocator);

public:
    struct InitParams
    {
        usize m_size;
        Alignment m_alignment = ALIGN_DEFAULT;
    };

    IncrementalAllocator() = default;

    IncrementalAllocator(InitParams const & init, TMemProvider const & mem_provider)
        : m_mem_provider(mem_provider)
        , m_alignment(init.m_alignment)
        , m_arena(m_mem_provider.allocate(init.m_size))
        , m_top(reinterpret_cast<ui8 *>(m_arena.m_ptr))
    {
    }

    IncrementalAllocator(InitParams const & init)
        : m_mem_provider()
        , m_alignment(init.m_alignment)
        , m_arena(m_mem_provider.allocate(init.m_size))
        , m_top(reinterpret_cast<ui8 *>(m_arena.m_ptr))
    {
    }

    IncrementalAllocator & operator=(IncrementalAllocator const &) = delete;
    IncrementalAllocator(IncrementalAllocator const &) = delete;

    ~IncrementalAllocator() override
    {
        m_mem_provider.deallocate(m_arena);
    }

    void * allocate(usize const size) override
    {
        void * mem_ptr = nullptr;

        if (nullptr != m_top)
        {
            ui8 * new_top =
                reinterpret_cast<ui8 *>(alignUp(reinterpret_cast<uiptr>(m_top), m_alignment));

            if (m_arena.isInRange(new_top, size))
            {
                mem_ptr = new_top;
                m_top = new_top + size;
            }
        }

        return mem_ptr;
    }

    void * allocate(usize const size, Alignment const alignment) override
    {
        void * mem_ptr = nullptr;

        if (nullptr != m_top)
        {
            ui8 * new_top =
                reinterpret_cast<ui8 *>(alignUp(reinterpret_cast<uiptr>(m_top), alignment));

            if (m_arena.isInRange(new_top, size))
            {
                mem_ptr = new_top;
                m_top = new_top + size;
            }
        }

        return mem_ptr;
    }

    void deallocate(void * ptr) override
    {
        sbWarn(m_arena.isInRange(ptr));
    }

    void deallocateAll()
    {
        m_top = static_cast<ui8 *>(m_arena.m_ptr);
    }

    b8 owns(void const * ptr) const override
    {
        return m_arena.isInRange(ptr);
    }

    TMemProvider const & getProvider() const
    {
        return m_mem_provider;
    }

    Alignment getAlignment() const
    {
        return m_alignment;
    }

private:
    TMemProvider m_mem_provider;
    Alignment m_alignment;
    MemoryArena m_arena;
    ui8 * m_top;
};

} // namespace sb
