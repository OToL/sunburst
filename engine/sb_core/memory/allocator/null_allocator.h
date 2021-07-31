#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include "allocator.h"

namespace sb {

class NullAllocator final : public IAllocator
{
    sb_base(IAllocator);

public:
    struct InitParams
    {
    };

    NullAllocator() = default;

    NullAllocator(InitParams const &);

    MemoryArena allocate(usize const) override;

    MemoryArena allocate(Alignment const, usize const) override;

    void deallocate(void * ptr) override;

    void deallocate(MemoryArena arena) override;

    b8 owns(void const * ptr) const override;

    void deallocateAll();

    constexpr Alignment getAlignment()
    {
        return ALIGNMENT_DEFAULT;
    }
};

} // namespace sb
