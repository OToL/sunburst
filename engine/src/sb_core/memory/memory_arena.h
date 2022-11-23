#pragma once

#include <sb_core/core.h>

namespace sb {

struct MemoryArena
{
    void * data = nullptr;
    usize size = 0ULL;

    constexpr inline b8 isInRange(void const * ptr, size_t data_size) const
    {
        return ((data <= ptr) && ((static_cast<u8 const *>(data) + size) >= (static_cast<u8 const *>(ptr) + data_size)));
    }

    constexpr inline b8 isInRange(void const * ptr) const
    {
        return ((data <= ptr) && ((static_cast<u8 const *>(data) + size) > ptr));
    }

    constexpr inline b8 isEmpty() const
    {
        return (0U == size);
    }

    constexpr inline b8 isValid() const
    {
        return (0U != size) && (nullptr != data);
    }
};

} // namespace sb
