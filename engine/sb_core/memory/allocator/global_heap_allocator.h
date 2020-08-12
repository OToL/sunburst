#pragma once

#include <sb_core/memory/memory.h>
#include <sb_core/memory/allocator/allocator.h>
#include <sb_base/base.h>

namespace sb {

class GlobalHeapAllocator final : public IAllocator
{
public:
    struct InitParams
    {
    };

    GlobalHeapAllocator() = default;

    explicit GlobalHeapAllocator(InitParams const &) { }

    void * allocate(usize const size) override;

    void * allocate(usize const size, Alignment const alignment) override;

    void deallocate(void * ptr) override;

    b8 owns(void const * ptr) const override;

    usize getAlignment() const
    {
        return ALIGN_DEFAULT;
    }

    usize getBlockSize(void * ptr) const;
};

} // namespace sb
