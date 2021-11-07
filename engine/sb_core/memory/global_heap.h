#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/allocator/allocator.h>

namespace sb {

class GlobalHeap final : public IAllocator
{
    GlobalHeap() = default;
    ~GlobalHeap() = default;

    friend GlobalHeap & getGlobalHeap();

public:
    GlobalHeap(GlobalHeap const &) = delete;
    GlobalHeap(GlobalHeap &&) = delete;
    GlobalHeap & operator=(GlobalHeap const &) = delete;
    GlobalHeap & operator=(GlobalHeap &&) = delete;

    MemoryArena allocate(usize const size) override;
    MemoryArena allocate(Alignment const alignment, usize const size) override;

    void deallocate(void * ptr) override;
    void deallocate(MemoryArena arena) override;

    b8 owns(void const * ptr) const;

    constexpr usize getAlignment() const
    {
        return DEFAULT_MEMORY_ALIGNMENT;
    }

    usize getBlockSize(void * ptr) const;
};

GlobalHeap & getGlobalHeap();

#define GHEAP getGlobalHeap()

} // namespace sb
