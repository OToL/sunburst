#pragma once

#include <core/platform.h>
#include <core/error.h>

namespace sb {

class NullAllocator
{
public:
    struct InitParams
    {
    };

    static constexpr usize ALIGNMENT = GLOBAL_HEAP_MIN_ALIGNMENT;

    NullAllocator() {}

    NullAllocator(InitParams const &) {}

    constexpr usize getAlignment() const
    {
        return ALIGNMENT;
    }

    void * allocate(usize const)
    {
        return nullptr;
    }

    void * allocate(usize const, usize const)
    {
        return nullptr;
    }

    void deallocate([[maybe_unused]] void * ptr)
    {
        sbWarn(nullptr == ptr);
    }

    void deallocateAll() {}

    b8 owns(void * ptr) const
    {
        return (nullptr == ptr);
    }
};

} // namespace sb
