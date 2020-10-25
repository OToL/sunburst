#pragma once

#include <sb_core/memory/memory.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/allocator/allocator.h>
#include <sb_core/core.h>

namespace sb {

class MemoryArenaAllocator final : public IAllocator
{
public:
    MemoryArenaAllocator() = default;

    MemoryArenaAllocator(MemoryArena arena);

    template <typename T, usize COUNT>
    MemoryArenaAllocator(T (&data)[COUNT])
        : _arena({data, sizeof(T) * COUNT})
    {
    }

    MemoryArena allocate(usize const size) override;

    MemoryArena allocate(usize const size, Alignment const alignment) override;

    void deallocate(void * ptr) override;

    b8 owns(void const * ptr) const override;

    void deallocateAll();

    MemoryArena getArena() const;

private:
    MemoryArena _arena = {};
    b8 _empty = false;
};

} // namespace sb
