#pragma once

#include <sb_core/memory/memory.h>
#include <sb_core/memory/allocator/allocator.h>
#include <sb_core/core.h>

namespace sb {

class GlobalHeapAllocator final : public IAllocator
{
public:
    GlobalHeapAllocator() = default;

    MemoryArena allocate(usize const size) override;

    MemoryArena allocate(usize const size, Alignment const alignment) override;

    void deallocate(void * ptr) override;

    void deallocate(MemoryArena arena) override
    {
        deallocate(arena.m_ptr);
    }

    b8 owns(void const * ptr) const override;

    usize getAlignment() const
    {
        return ALIGNMENT_DEFAULT;
    }

    usize getBlockSize(void * ptr) const;
};

} // namespace sb
