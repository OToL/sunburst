#include <core/memory/allocator/arena_mem_provider.h>
#include <core/bitwise.h>
#include <core/error.h>

namespace sb {

    ArenaMemProvider::ArenaMemProvider(MemoryArena const & arena)
        : m_arena(arena)
    {
    }

    MemoryArena ArenaMemProvider::allocate(usize const size)
    {
        if (size <= m_arena.m_size)
        {
            MemoryArena arena = m_arena;
            m_arena = {};

            return arena;
        }

        return {};
    }
    
    MemoryArena ArenaMemProvider::allocate(usize const size, Alignment const alignment)
    {
        sbAssert(isAlignedTo((uiptr)m_arena.m_ptr, alignment));

        return allocate(size);
    }

    void ArenaMemProvider::deallocate(MemoryArena const & arena)
    {
        if (sbExpectTrue(m_arena.isEmpty()))
        {
            m_arena = arena;
        }
    }

}
