#pragma once

#include <sb_core/memory/memory_arena.h>
#include <sb_core/core.h>

namespace sb {

struct memory_system
{
    static void initialize();
    static void terminate();

    // Malloc interface
    static MemoryArena malloc(usize size);
    static MemoryArena malloc(Alignment alignment, usize size);
    static usize getBlockSize(void * memPtr);
    static void free(void * memPtr);
};

} // namespace sb

