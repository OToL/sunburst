#pragma once

#include <sb_core/memory/memory.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/core.h>

namespace sb {

class IAllocator
{
public:
    virtual ~IAllocator() = default;

    virtual MemoryArena allocate(usize const size) = 0;

    virtual MemoryArena allocate(usize const size, Alignment const alignment) = 0;

    virtual void deallocate(void * ptr) = 0;

    virtual void deallocate(MemoryArena arena) = 0;

    virtual b8 owns(void const * ptr) const = 0;
};

} // namespace sb
