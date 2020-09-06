#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/memory.h>

namespace sb {

struct MemoryArenaProvider
{
    MemoryArenaProvider(MemoryArena const & arena);

    MemoryArena allocate(usize const size);

    MemoryArena allocate(usize const size, Alignment const alignment);

    void deallocate(MemoryArena const & arena);

    MemoryArena m_arena;
};

} // namespace sb
