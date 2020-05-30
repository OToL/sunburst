#pragma once

#include <core/types.h>
#include <core/memory/memory_arena.h>
#include <core/memory/memory.h>

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
