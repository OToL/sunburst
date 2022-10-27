#pragma once

#include <sb_core/core.h>
#include <sb_core/bit_operations.h>
#include <sb_core/cast.h>
#include <sb_core/error.h>
#include <sb_core/_impl/memory/allocator/pool_allocator_base.h>

namespace sb {

template <typename TMemProvider>
class PoolAllocator final : public PoolAllocatorBase
{
    sb_base(PoolAllocatorBase);

public:
    PoolAllocator() = default;

    PoolAllocator(usize block_size, usize block_count, Alignment default_align = DEFAULT_MEMORY_ALIGNMENT)
        : _mem_provider()
    {
        auto actual_block_size = alignUp(block_size, default_align);
        sb_assert(sizeof(NodeIdx) <= actual_block_size);

        auto mem_arena = _mem_provider.allocate(default_align, block_count * actual_block_size);
        sb_assert(!memory_arena::isEmpty(mem_arena));

        Base::init(mem_arena, actual_block_size, default_align);
    }

    PoolAllocator(TMemProvider const & mem_provider, usize block_size, usize block_count,
                  Alignment default_align = DEFAULT_MEMORY_ALIGNMENT)
        : _mem_provider(mem_provider)
    {
        auto actual_block_size = alignUp(block_size, default_align);
        sb_assert(sizeof(NodeIdx) <= actual_block_size);

        auto mem_arena = _mem_provider.allocate(default_align, block_count * actual_block_size);
        sb_assert(!memory_arena::isEmpty(mem_arena));

        Base::init(mem_arena, actual_block_size, default_align);
    }

    PoolAllocator & operator=(PoolAllocator const &) = delete;
    PoolAllocator & operator=(PoolAllocator &&) = delete;
    PoolAllocator(PoolAllocator const &) = delete;
    PoolAllocator(PoolAllocator &&) = delete;

    ~PoolAllocator() override
    {
        MemoryArena mem_arena = Base::getArena();
        if (!memory_arena::isEmpty(mem_arena))
        {
            _mem_provider.deallocate(mem_arena.data);
        }
    }

    using Base::allocate;
    using Base::deallocate;
    using Base::owns;

    using Base::deallocateAll;
    using Base::getAlignment;
    using Base::getArena;

private:
    TMemProvider _mem_provider;
};

} // namespace sb
