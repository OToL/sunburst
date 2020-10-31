#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/memory_arena.h>

#include <sb_std/utility>
#include <sb_std/type_traits>

namespace sb::internal {

void * malloc(usize size);

void * malloc(usize size, Alignment alignment);

usize mallocUsableSize(void * memPtr);

void free(void * memPtr);

template <typename TType, typename THeap, typename... TArgs>
TType * newImpl(THeap & heap, TArgs &&... args)
{
    MemoryArena arena = heap.allocate(sizeof(TType), alignOf<TType>());

    if (!arena.isEmpty())
    {
        return ::new (arena.m_ptr) TType(sbstd::forward<TArgs &&>(args)...);
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
