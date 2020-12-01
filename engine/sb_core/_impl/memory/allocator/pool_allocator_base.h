#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/allocator/allocator.h>

namespace sb {

class PoolAllocatorBase : public IAllocator
{
    sbBaseClass(IAllocator);

protected:
    using NodeIdx = s32;
    static constexpr NodeIdx INVALID_NODE_IDX = -1;

    void initFreeList();

    PoolAllocatorBase();

    ~PoolAllocatorBase() override = default;

    void init(MemoryArena arena, usize block_size, Alignment default_align);

public:
    PoolAllocatorBase & operator=(PoolAllocatorBase const &) = delete;
    PoolAllocatorBase & operator=(PoolAllocatorBase &&) = delete;
    PoolAllocatorBase(PoolAllocatorBase const &) = delete;
    PoolAllocatorBase(PoolAllocatorBase &&) = delete;

    MemoryArena allocate(usize const size) override;

    MemoryArena allocate(Alignment const alignment, usize const size) override;

    void deallocate(void * ptr) override;

    void deallocate(MemoryArena arena) override
    {
        deallocate(arena.data);
    }

    b8 owns(void const * ptr) const override;

    void deallocateAll();

    MemoryArena allocate();

    Alignment getAlignment() const
    {
        return _default_alignment;
    }

    MemoryArena getArena() const
    {
        return _arena;
    }

private:
    MemoryArena _arena;
    Alignment _default_alignment;
    usize _actual_block_size;
    NodeIdx _free_list_head;
};

} // namespace sb
