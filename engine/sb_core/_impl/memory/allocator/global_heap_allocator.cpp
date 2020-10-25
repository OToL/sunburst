#include <sb_core/error.h>
#include <sb_core/memory/allocator/allocator_view.h>
#include <sb_core/memory/allocator/global_heap_allocator.h>
#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/alloc.h>

sb::MemoryArena sb::GlobalHeapAllocator::allocate(usize const size)
{
    return {sbMalloc(size), size};
}

sb::MemoryArena sb::GlobalHeapAllocator::allocate(usize const size, Alignment const alignment)
{
    return {sbMalloc(size, alignment), size};
}

void sb::GlobalHeapAllocator::deallocate(void * ptr)
{
    sbFree(ptr);
}

sb::usize sb::GlobalHeapAllocator::getBlockSize(void * ptr) const
{
    return sbMallocUsabeSize(ptr);
}

sb::b8 sb::GlobalHeapAllocator::owns(void const *) const
{
    sbNotImplemented(__FUNCTION__);

    return false;
}
