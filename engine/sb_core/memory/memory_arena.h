#pragma once

#include <sb_core/core.h>

namespace sb {

struct MemoryArena
{
    void * data;
    usize size;
};

constexpr inline b8 isInRange(MemoryArena arena, void const * ptr, size_t data_size)
{
    return ((arena.data <= ptr) &&
            ((static_cast<u8 const *>(arena.data) + arena.size) >= (static_cast<u8 const *>(ptr) + data_size)));
}

constexpr inline b8 isInRange(MemoryArena arena, void const * ptr)
{
    return ((arena.data <= ptr) && ((static_cast<u8 const *>(arena.data) + arena.size) > ptr));
}

constexpr inline b8 isEmpty(MemoryArena arena)
{
    return (0U == arena.size);
}

} // namespace sb
