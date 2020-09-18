#include <sb_core/memory/provider/memory_arena_provider.h>
#include <sb_core/bit.h>
#include <sb_core/error.h>

sb::MemoryArenaProvider::MemoryArenaProvider(MemoryArena const & arena)
    : m_arena(arena)
{
}

sb::MemoryArena sb::MemoryArenaProvider::allocate(usize const size)
{
    if (size <= m_arena.m_size)
    {
        MemoryArena arena = m_arena;
        m_arena = {};

        return arena;
    }

    return {};
}

sb::MemoryArena sb::MemoryArenaProvider::allocate(usize const size, Alignment const alignment)
{
    sbAssert(isAlignedTo((uptr)m_arena.m_ptr, alignment));

    return allocate(size);
}

void sb::MemoryArenaProvider::deallocate(MemoryArena const & arena)
{
    if (sbExpect(m_arena.isEmpty()))
    {
        m_arena = arena;
    }
}
