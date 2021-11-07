#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/memory_arena.h>

namespace sb {

class ContainerAllocator
{
public:
    ContainerAllocator() = default;

    MemoryArena allocate(usize const size);

    MemoryArena allocate(Alignment const alignment, usize const size);

    void deallocate(void * ptr);

    void deallocate(MemoryArena arena)
    {
        deallocate(arena.data);
    }

    b8 owns(void const * ptr) const;

    usize getAlignment() const
    {
        return DEFAULT_MEMORY_ALIGNMENT;
    }

    usize getBlockSize(void * ptr) const;
};

} // namespace sb
