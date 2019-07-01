#pragma once

#include <core/platform.h>
#include <core/memory/memory_arena.h>
#include <core/memory/memory.h>

namespace sb {

struct ArenaMemProvider
{
    ArenaMemProvider(MemoryArena const & arena);

    MemoryArena allocate(usize const size);
    
    MemoryArena allocate(usize const size, Alignment const alignment);

    void deallocate(MemoryArena const &);

    MemoryArena m_arena;
};

}
