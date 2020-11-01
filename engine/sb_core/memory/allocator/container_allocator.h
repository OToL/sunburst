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

    MemoryArena allocate(usize const size, Alignment const alignment);

    void deallocate(void * ptr);

    void deallocate(MemoryArena arena)
    {
        deallocate(arena.m_ptr);
    }

    b8 owns(void const * ptr) const;

    usize getAlignment() const
    {
        return ALIGNMENT_DEFAULT;
    }

    usize getBlockSize(void * ptr) const;
};

} // namespace sb
