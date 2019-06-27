#pragma once

#include <core/memory/allocator/allocator_view.h>
#include <core/memory/memory.h>
#include <core/platform.h>

namespace sb {

class GlobalHeapAllocator
{
    sbCopyProtect(GlobalHeapAllocator);

public:
    struct InitParams
    {
    };

    static constexpr Alignment ALIGNMENT = ALIGN_DEFAULT;

    GlobalHeapAllocator() {}

    explicit GlobalHeapAllocator(InitParams const &) {}

    ~GlobalHeapAllocator() {}

    void * allocate(usize const size);

    void * allocate(usize const size, Alignment const alignment);

    void deallocate(void * ptr);

    void deallocateAll();

    b8 owns(void const * ptr) const;

    usize getAlignment() const
    {
        return ALIGNMENT;
    }

    usize getBlockSize(void * ptr) const;
};

} // namespace sb
