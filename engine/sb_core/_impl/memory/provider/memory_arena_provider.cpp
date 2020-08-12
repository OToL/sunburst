#include <sb_core/memory/provider/memory_arena_provider.h>
#include <sb_core/bit.h>
#include <sb_core/error.h>

namespace sb {

MemoryArenaProvider::MemoryArenaProvider(MemoryArena const & arena)
    : m_arena(arena)
{
}

MemoryArena MemoryArenaProvider::allocate(usize const size)
{
    if (size <= m_arena.m_size)
    {
        MemoryArena arena = m_arena;
        m_arena = {};

        return arena;
    }

    return {};
}

MemoryArena MemoryArenaProvider::allocate(usize const size, Alignment const alignment)
{
    sbAssert(isAlignedTo((uptr)m_arena.m_ptr, alignment));

    return allocate(size);
}

void MemoryArenaProvider::deallocate(MemoryArena const & arena)
{
    if (sbExpect(m_arena.isEmpty()))
    {
        m_arena = arena;
    }
}

} // namespace sb
