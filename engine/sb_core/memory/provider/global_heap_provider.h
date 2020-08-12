#pragma once

#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/memory.h>

namespace sb {

struct GlobalHeapProvider
{
    GlobalHeapProvider();

    MemoryArena allocate(usize const size);

    MemoryArena allocate(usize const size, Alignment const alignment);

    void deallocate(MemoryArena const & arena);
};

} // namespace sb
