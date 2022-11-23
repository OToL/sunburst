#include <sb_core/memory/allocators/container_allocator.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/memory/memory.h>

sb::MemoryArena sb::ContainerAllocator::allocate(usize const size)
{
    return GHEAP.allocate(size);
}

sb::MemoryArena sb::ContainerAllocator::allocate(Alignment const alignment, usize const size)
{
    return GHEAP.allocate(alignment, size);
}

void sb::ContainerAllocator::deallocate(void * ptr)
{
    GHEAP.deallocate(ptr);
}

sb::usize sb::ContainerAllocator::getBlockSize(void * ptr) const
{
    return GHEAP.getBlockSize(ptr);
}

sb::b8 sb::ContainerAllocator::owns(void const * ptr) const
{
    return GHEAP.owns(ptr);
}
