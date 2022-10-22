#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/memory_arena.h>

#include <sb_slw/utility>
#include <sb_slw/type_traits>

namespace sb::internal {

template <typename TType, typename THeap, typename... TArgs>
TType * newImpl(THeap & heap, TArgs &&... args)
{
    MemoryArena arena = heap.allocate(alignof(TType), sizeof(TType));

    if (!memory_arena::isEmpty(arena))
    {
        return ::new (arena.data) TType(slw::forward<TArgs &&>(args)...);
    }

    return nullptr;
}

template <typename TType, typename THeap>
void deleteImpl(THeap & heap, TType * obj)
{
    if (obj != nullptr)
    {
        obj->~TType();
        heap.deallocate(obj);
    }
}

} // namespace sb::internal
