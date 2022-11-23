#pragma once

#include <sb_core/memory/memory.h>
#include <sb_core/memory/allocator.h>
#include <sb_core/core.h>

namespace sb {

class GlobalHeapAllocator final : public IAllocator
{
public:
    GlobalHeapAllocator() = default;

    MemoryArena allocate(usize const size) override;

    MemoryArena allocate(Alignment const alignment, usize const size) override;

    void deallocate(void * ptr) override;

    void deallocate(MemoryArena arena) override
    {
        deallocate(arena.data);
    }

    b8 owns(void const * ptr) const override;

    usize getAlignment() const
    {
        return DEFAULT_MEMORY_ALIGNMENT;
    }

    usize getBlockSize(void * ptr) const;
};

} // namespace sb
