#include <sb_core/memory/global_heap.h>
#include <sb_core/error.h>
#include "memory_system.h"

sb::MemoryArena sb::GlobalHeap::allocate(usize const size)
{
    return memory_system::malloc(size);
}

sb::MemoryArena sb::GlobalHeap::allocate(Alignment const alignment, usize const size)
{
    return memory_system::malloc(alignment, size);
}

void sb::GlobalHeap::deallocate(void * ptr)
{
    sb::memory_system::free(ptr);
}

void sb::GlobalHeap::deallocate(MemoryArena arena)
{
    sb::memory_system::free(arena.data);
}

sb::b8 sb::GlobalHeap::owns(void const *) const
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    sb_not_implemented(__FUNCTION__);

    return false;
}

sb::usize sb::GlobalHeap::getBlockSize(void * ptr) const
{
    return memory_system::getBlockSize(ptr);
}

sb::GlobalHeap & sb::getGlobalHeap()
{
    static GlobalHeap _global_heap;

    return _global_heap;
}
