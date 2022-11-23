#include <sb_core/memory/allocators/global_heap_allocator.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/memory/memory.h>

sb::MemoryArena sb::GlobalHeapAllocator::allocate(usize const size)
{
    return GHEAP.allocate(size);
}

sb::MemoryArena sb::GlobalHeapAllocator::allocate(Alignment const alignment, usize const size)
{
    return GHEAP.allocate(alignment, size);
}

void sb::GlobalHeapAllocator::deallocate(void * ptr)
{
    GHEAP.deallocate(ptr);
}

sb::usize sb::GlobalHeapAllocator::getBlockSize(void * ptr) const
{
    return GHEAP.getBlockSize(ptr);
}

sb::b8 sb::GlobalHeapAllocator::owns(void const * ptr) const
{
    return GHEAP.owns(ptr);
}
