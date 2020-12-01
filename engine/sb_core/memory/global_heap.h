#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory_arena.h>
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

    MemoryArena allocate(Alignment const alignment, usize const size);

    void deallocate(void * ptr);

    b8 owns(void const * ptr) const;

    constexpr usize getAlignment() const
    {
        return ALIGNMENT_DEFAULT;
    }

    usize getBlockSize(void * ptr) const;

private:
};

GlobalHeap & getGlobalHeap();

#define GHEAP getGlobalHeap()

} // namespace sb
