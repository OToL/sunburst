#include <sb_core/memory/global_heap.h>
#include <sb_core/_impl/memory/malloc.h>
#include <sb_core/error/error.h>

sb::MemoryArena sb::GlobalHeap::allocate(usize const size)
{
    return internal::malloc(size);
}

sb::MemoryArena sb::GlobalHeap::allocate(Alignment const alignment, usize const size)
{
    return internal::malloc(alignment, size);
}

void sb::GlobalHeap::deallocate(void * ptr)
{
    internal::free(ptr);
}

void sb::GlobalHeap::deallocate(MemoryArena arena)
{
    internal::free(arena.data);
}

sb::b8 sb::GlobalHeap::owns(void const *) const
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    sbNotImplemented(__FUNCTION__);

    return false;
}

sb::usize sb::GlobalHeap::getBlockSize(void * ptr) const
{
    return internal::mallocUsableSize(ptr);
}

sb::GlobalHeap & sb::getGlobalHeap()
{
    static GlobalHeap _global_heap;

    return _global_heap;
}
