#include <sb_core/memory/allocator/allocator_wrapper.h>

sb::AllocatorWrapper::AllocatorWrapper(IAllocator * alloc)
    : _alloc(alloc)
{
}

sb::AllocatorWrapper::AllocatorWrapper(IAllocator & alloc)
    : _alloc(&alloc)
{
}

sb::MemoryArena sb::AllocatorWrapper::allocate(usize const size)
{
    if (nullptr != _alloc)
    {
        return _alloc->allocate(size);
    }

    return {};
}

sb::MemoryArena sb::AllocatorWrapper::allocate(Alignment const alignment, usize const size)
{
    if (nullptr != _alloc)
    {
        return _alloc->allocate(alignment, size);
    }

    return {};
}

void sb::AllocatorWrapper::deallocate(void * ptr)
{
    if (nullptr != _alloc)
    {
        _alloc->deallocate(ptr);
    }
}

void sb::AllocatorWrapper::deallocate(MemoryArena arena)
{
    if (nullptr != _alloc)
    {
        _alloc->deallocate(arena);
    }
}

sb::b8 sb::AllocatorWrapper::owns(void const * ptr) const
{
    if (nullptr != _alloc)
    {
        return _alloc->owns(ptr);
    }

    return false;
}
