#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include "allocator.h"

namespace sb {

class NullAllocator final : public IAllocator
{
    sbBaseClass(IAllocator);

public:
    struct InitParams
    {
    };

    NullAllocator() = default;

    NullAllocator(InitParams const &);

    MemoryArena allocate(usize const) override;

    MemoryArena allocate(usize const, Alignment const) override;

    void deallocate(void * ptr) override;

    b8 owns(void const * ptr) const override;

    void deallocateAll();

    constexpr Alignment getAlignment()
    {
        return ALIGNMENT_DEFAULT;
    }
};

} // namespace sb
