#pragma once

#include <core/types.h>
#include "memory.h"

namespace sb {

class IAllocator
{
    // TODO: Make all function private and force using sbNew, sbMalloc, etc.
public:
    virtual ~IAllocator() = default;

    // TODO: return memory arena
    virtual void * allocate(usize const scize) = 0;

    // TODO: return memory arena
    virtual void * allocate(usize const size, Alignment const alignment) = 0;

    virtual void deallocate(void * ptr) = 0;

    virtual b8 owns(void const * ptr) const = 0;
};

} // namespace sb
