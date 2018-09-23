#pragma once

#include <core/platform.h>
#include <core/memory/memory_arena.h>

namespace sb {

class IncrementalAllocator
{
    sbCopyProtect(IncrementalAllocator);

public:
    struct InitParams
    {
        MemoryArena m_arena;
    };

    static constexpr usize ALIGNMENT = GLOBAL_HEAP_MIN_ALIGNMENT;

    IncrementalAllocator() = default;

    explicit IncrementalAllocator(InitParams const & init);

    IncrementalAllocator(MemoryArena const & init);

    constexpr usize getAlignment() const
    {
        return ALIGNMENT;
    }

    void * allocate(usize const size);

    void * allocate(usize const size, usize const alignment);

    void deallocate(void * ptr);

    void deallocateAll();

    b8 owns(void const * ptr) const
    {
        return m_arena.isInRange(ptr);
    }

    MemoryArena getArena() const
    {
        return m_arena;
    }

private:
    MemoryArena m_arena;
    ui8 * m_top;
};

} // namespace sb
