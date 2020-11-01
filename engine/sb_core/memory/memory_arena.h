#pragma once

#include <sb_core/core.h>

namespace sb {

// @todo: move member functions to global scope
struct MemoryArena
{
    // @todo: change to free function
    b8 isInRange(void const * ptr, size_t _size) const
    {
        return ((data <= ptr) && ((static_cast<u8 const *>(data) + _size) >= (static_cast<u8 const *>(ptr) + size)));
    }

    // @todo: change to free function
    b8 isInRange(void const * ptr) const
    {
        return ((data <= ptr) && ((static_cast<u8 const *>(data) + size) > ptr));
    }

    b8 isEmpty() const
    {
        return (0U == size);
    }

    void * data = nullptr;
    usize size = 0;
};

} // namespace sb
