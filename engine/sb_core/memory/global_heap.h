#pragma once

#include <sb_core/memory/allocator/allocator_view.h>
#include <sb_core/memory/allocator/global_heap_allocator.h>
#include <sb_core/memory/memory.h>

namespace sb {

class GlobalHeap
{
    GlobalHeap() = default;
    ~GlobalHeap() = default;

    friend GlobalHeap & getGlobalHeap();

public:
    GlobalHeap(GlobalHeap const &) = delete;
    GlobalHeap(GlobalHeap &&) = delete;
    GlobalHeap & operator=(GlobalHeap const &) = delete;
    GlobalHeap & operator=(GlobalHeap &&) = delete;

    MemoryArena allocate(usize const size);

    MemoryArena allocate(usize const size, Alignment const alignment);

    void deallocate(void * ptr);

    b8 owns(void const * ptr) const;

    constexpr usize getAlignment() const
    {
        return ALIGNMENT_DEFAULT;
    }

    usize getBlockSize(void * ptr) const;

private:
    GlobalHeapAllocator m_parent;
};

AllocatorView const & getGlobalHeapView();

GlobalHeap & getGlobalHeap();

#define GHEAP getGlobalHeap()

} // namespace sb
