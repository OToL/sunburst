#pragma once

#include <sb_core/memory/memory.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/allocator/allocator.h>

namespace sb {

class IncrementalAllocatorBase : public IAllocator
{
    sbBaseClass(IAllocator);

protected:
    IncrementalAllocatorBase();
    ~IncrementalAllocatorBase() override = default;

    void init(MemoryArena arena, Alignment default_align);

public:
    IncrementalAllocatorBase(IncrementalAllocatorBase const &) = delete;
    IncrementalAllocatorBase(IncrementalAllocatorBase &&) = delete;

    IncrementalAllocatorBase & operator=(IncrementalAllocatorBase const &) = delete;
    IncrementalAllocatorBase & operator=(IncrementalAllocatorBase &&) = delete;

    MemoryArena allocate(usize const size) override;

    MemoryArena allocate(Alignment const alignment, usize const size) override;

    void deallocate(void * ptr) override;

    void deallocate(MemoryArena arena) override
    {
        deallocate(arena.data);
    }

    b8 owns(void const * ptr) const override;

    void deallocateAll();

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
    u8 * _top;
    Alignment _default_alignment;
};

} // namespace sb
