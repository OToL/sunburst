#include <core/memory/provider/global_heap_provider.h>
#include <core/memory/alloc.h>

namespace sb {

GlobalHeapProvider::GlobalHeapProvider() { }

MemoryArena GlobalHeapProvider::allocate(usize const size)
{
    return {sbMalloc(size), size};
}

MemoryArena GlobalHeapProvider::allocate(usize const size, Alignment const alignment)
{
    return {sbMalloc(size, alignment), size};
}

void GlobalHeapProvider::deallocate(MemoryArena const & arena)
{
    sbFree(arena.m_ptr);
}

} // namespace sb
