#include <sb_core/memory/allocators/container_allocator_wrapper.h>
#include <sb_core/memory/allocator.h>

sb::ContainerAllocatorWrapper::ContainerAllocatorWrapper(IAllocator * alloc)
    : _alloc(alloc)
{
}

sb::ContainerAllocatorWrapper::ContainerAllocatorWrapper(IAllocator & alloc)
    : _alloc(&alloc)
{
}

sb::MemoryArena sb::ContainerAllocatorWrapper::allocate(usize const size)
{
    if (nullptr != _alloc)
    {
        return _alloc->allocate(size);
    }

    return {};
}

sb::MemoryArena sb::ContainerAllocatorWrapper::allocate(Alignment const alignment, usize const size)
{
    if (nullptr != _alloc)
    {
        return _alloc->allocate(alignment, size);
    }

    return {};
}

void sb::ContainerAllocatorWrapper::deallocate(void * ptr)
{
    if (nullptr != _alloc)
    {
        _alloc->deallocate(ptr);
    }
}

void sb::ContainerAllocatorWrapper::deallocate(MemoryArena arena)
{
    if (nullptr != _alloc)
    {
        _alloc->deallocate(arena);
    }
}

sb::b8 sb::ContainerAllocatorWrapper::owns(void const * ptr) const
{
    if (nullptr != _alloc)
    {
        return _alloc->owns(ptr);
    }

    return false;
}
