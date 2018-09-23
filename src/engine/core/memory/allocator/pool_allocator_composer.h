#pragma once

#include "pool_allocator.h"
#include "memory_arena_composer.h"
#include "global_heap_allocator.h"

namespace sb {

template <usize BLOCK_SIZE, usize BLOCK_ALIGNMENT, typename TProvider = GlobalHeapAllocator>
class PoolAllocatorComposer : public MemoryArenaComposer<TProvider, PoolAllocator<BLOCK_SIZE, BLOCK_ALIGNMENT>>
{
    using BaseClass = MemoryArenaComposer<TProvider, PoolAllocator<BLOCK_SIZE, BLOCK_ALIGNMENT>>;

public:
    PoolAllocatorComposer(usize block_count, typename TProvider::InitParams provider_init = {})
        : BaseClass(typename BaseClass::InitParams{block_count * BLOCK_SIZE, provider_init, {}})
    {
    }
};

template <typename TObject, typename TProvider = GlobalHeapAllocator>
using ObjectPoolAllocatorComposer = PoolAllocatorComposer<sizeof(TObject), alignof(TObject), TProvider>;

} // namespace sb
