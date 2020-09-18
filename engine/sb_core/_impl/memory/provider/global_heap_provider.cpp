#include <sb_core/memory/provider/global_heap_provider.h>
#include <sb_core/memory/alloc.h>

sb::GlobalHeapProvider::GlobalHeapProvider() { }

sb::MemoryArena sb::GlobalHeapProvider::allocate(usize const size)
{
    return {sbMalloc(size), size};
}

sb::MemoryArena sb::GlobalHeapProvider::allocate(usize const size, Alignment const alignment)
{
    return {sbMalloc(size, alignment), size};
}

void sb::GlobalHeapProvider::deallocate(MemoryArena const & arena)
{
    sbFree(arena.m_ptr);
}
