#include <sb_core/memory/allocator/memory_arena_allocator.h>
#include <sb_core/bit.h>
#include <sb_core/error.h>

sb::MemoryArenaAllocator::MemoryArenaAllocator(MemoryArena arena)
    : _arena(arena)
{
}

sb::MemoryArena sb::MemoryArenaAllocator::allocate(usize const size)
{
    if ((size != 0U) && !_empty && !_arena.isEmpty() && (size <= _arena.m_size))
    {
        _empty = true;
        return _arena;
    }

    return {};
}

sb::MemoryArena sb::MemoryArenaAllocator::allocate(usize const size, Alignment const alignment)
{
    if ((size != 0U) && !_empty && !_arena.isEmpty() && (size <= _arena.m_size))
    {
        u8 * base_mem_ptr = reinterpret_cast<u8 *>(alignUp(reinterpret_cast<uptr>(_arena.m_ptr), alignment));

        if (_arena.isInRange(base_mem_ptr, size))
        {
            _empty = true;
            return {base_mem_ptr, _arena.m_size - (base_mem_ptr - reinterpret_cast<u8 *>(_arena.m_ptr))};
        }
    }

    return {};
}

void sb::MemoryArenaAllocator::deallocate(void * ptr)
{
    sbWarn(_arena.isInRange(ptr));
    _empty = false;
}

sb::b8 sb::MemoryArenaAllocator::owns(void const * ptr) const
{
    return _arena.isInRange(ptr);
}

void sb::MemoryArenaAllocator::deallocateAll()
{
    _empty = false;
}

sb::MemoryArena sb::MemoryArenaAllocator::getArena() const
{
    return _arena;
}

sb::b8 sb::MemoryArenaAllocator::isEmpty() const
{
    return _empty;
}