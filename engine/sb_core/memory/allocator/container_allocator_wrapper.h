#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/memory_arena.h>

namespace sb {

class IAllocator;

class ContainerAllocatorWrapper
{
public:
    ContainerAllocatorWrapper();
    ContainerAllocatorWrapper(IAllocator * alloc);
    ContainerAllocatorWrapper(IAllocator & alloc);

    ~ContainerAllocatorWrapper() = default;

    MemoryArena allocate(usize const size);

    MemoryArena allocate(usize const size, Alignment const alignment);

    void deallocate(void * ptr);

    void deallocate(MemoryArena arena);

    b8 owns(void const * ptr) const;

private:
    IAllocator * _alloc;
};

} // namespace sb
