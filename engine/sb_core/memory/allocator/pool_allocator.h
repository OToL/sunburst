#pragma once

#include <sb_core/core.h>
#include <sb_core/bit.h>
#include <sb_core/conversion.h>
#include <sb_core/error.h>
#include <sb_core/_impl/memory/allocator/pool_allocator_base.h>

namespace sb {

template <typename TMemProvider>
class PoolAllocator final : public PoolAllocatorBase
{
    sbBaseClass(PoolAllocatorBase);

public:
    PoolAllocator() = default;

    PoolAllocator(usize block_size, usize block_count, Alignment default_align = ALIGNMENT_DEFAULT)
        : _mem_provider()
    {
        auto actual_block_size = alignUp(block_size, default_align);
        sbAssert(sizeof(NodeIdx) <= actual_block_size);

        auto mem_arena = _mem_provider.allocate(block_count * actual_block_size, default_align);
        sbAssert(!mem_arena.isEmpty());

        BaseClass::init(mem_arena, actual_block_size, default_align);
    }

    PoolAllocator(TMemProvider const & mem_provider, usize block_size, usize block_count,
                  Alignment default_align = ALIGNMENT_DEFAULT)
        : _mem_provider(mem_provider)
    {
        auto actual_block_size = alignUp(block_size, default_align);
        sbAssert(sizeof(NodeIdx) <= actual_block_size);

        auto mem_arena = _mem_provider.allocate(block_count * actual_block_size, default_align);
        sbAssert(!mem_arena.isEmpty());

        BaseClass::init(mem_arena, actual_block_size, default_align);
    }

    PoolAllocator & operator=(PoolAllocator const &) = delete;
    PoolAllocator & operator=(PoolAllocator &&) = delete;
    PoolAllocator(PoolAllocator const &) = delete;
    PoolAllocator(PoolAllocator &&) = delete;

    ~PoolAllocator() override
    {
        MemoryArena mem_arena = BaseClass::getArena();
        if (!mem_arena.isEmpty())
        {
            _mem_provider.deallocate(mem_arena.data);
        }
    }

    using BaseClass::allocate;
    using BaseClass::deallocate;
    using BaseClass::owns;

    using BaseClass::deallocateAll;
    using BaseClass::getAlignment;
    using BaseClass::getArena;

private:
    TMemProvider _mem_provider;
};

} // namespace sb
