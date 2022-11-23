#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/allocator.h>

namespace sb {

class AllocatorWrapper final : public IAllocator
{
    sb_base(IAllocator);

public:
    AllocatorWrapper() = default;
    AllocatorWrapper(IAllocator * alloc);
    AllocatorWrapper(IAllocator & alloc);

    MemoryArena allocate(usize const size) override;

    MemoryArena allocate(Alignment const alignment, usize const size) override;

    void deallocate(void * ptr) override;

    void deallocate(MemoryArena arena) override;

    b8 owns(void const * ptr) const override;

private:
    IAllocator * _alloc = nullptr;
};

} // namespace sb
