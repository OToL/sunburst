#include <sb_core/memory/allocator/memory_arena_allocator.h>
#include <sb_core/bit_utility.h>
#include <sb_core/error/error.h>

sb::MemoryArenaAllocator::MemoryArenaAllocator(MemoryArena arena)
    : _arena(arena)
{
}

sb::MemoryArena sb::MemoryArenaAllocator::allocate(usize const size)
{
    if ((size != 0U) && !_empty && !sb::memory_arena::isEmpty(_arena) && (size <= _arena.size))
    {
        _empty = true;
        return _arena;
    }

    return {};
}

sb::MemoryArena sb::MemoryArenaAllocator::allocate(Alignment const alignment, usize const size)
{
    if ((size != 0U) && !_empty && !sb::memory_arena::isEmpty(_arena) && (size <= _arena.size))
    {
        u8 * base_mem_ptr = reinterpret_cast<u8 *>(alignUp(reinterpret_cast<uptr>(_arena.data), alignment));

        if (memory_arena::isInRange(_arena, base_mem_ptr, size))
        {
            _empty = true;
            return {base_mem_ptr, _arena.size - (base_mem_ptr - reinterpret_cast<u8 *>(_arena.data))};
        }
    }

    return {};
}

void sb::MemoryArenaAllocator::deallocate(void * ptr)
{
    sb_warning(memory_arena::isInRange(_arena, ptr));
    _empty = false;
}

sb::b8 sb::MemoryArenaAllocator::owns(void const * ptr) const
{
    return memory_arena::isInRange(_arena, ptr);
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
