#pragma once

#include <sb_core/memory/memory.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/allocator/allocator.h>

namespace sb {

class IncrementalAllocatorBase : public IAllocator
{
    sbBaseClass(IAllocator);

protected:
    void init(MemoryArena arena, Alignment default_align);

public:
    IncrementalAllocatorBase() = default;
    ~IncrementalAllocatorBase() = default;

    IncrementalAllocatorBase(IncrementalAllocatorBase const &) = delete;
    IncrementalAllocatorBase(IncrementalAllocatorBase &&) = delete;

    IncrementalAllocatorBase & operator=(IncrementalAllocatorBase const &) = delete;
    IncrementalAllocatorBase & operator=(IncrementalAllocatorBase &&) = delete;

    MemoryArena allocate(usize const size) override;

    MemoryArena allocate(usize const size, Alignment const alignment) override;

    void deallocate(void * ptr) override;

    b8 owns(void const * ptr) const override;

    void deallocateAll();

    Alignment getAlignment() const;

private:
    MemoryArena _arena;
    u8 * _top;
    Alignment _default_alignment;
};

} // namespace sb
