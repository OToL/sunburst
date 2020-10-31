#pragma once

#include <sb_core/core.h>
#include "allocator.h"

namespace sb {

class AllocatorWrapper final : public IAllocator
{
    sbBaseClass(IAllocator);

public:
    AllocatorWrapper();
    AllocatorWrapper(IAllocator * alloc);
    AllocatorWrapper(IAllocator & alloc);

    ~AllocatorWrapper() override = default;

    MemoryArena allocate(usize const size) override;

    MemoryArena allocate(usize const size, Alignment const alignment) override;

    void deallocate(void * ptr) override;

    void deallocate(MemoryArena arena) override;

    b8 owns(void const * ptr) const override;

private:
    IAllocator * _alloc;
};

} // namespace sb
