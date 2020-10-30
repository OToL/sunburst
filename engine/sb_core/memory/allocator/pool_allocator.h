#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/memory.h>
#include <sb_core/conversion.h>
#include <sb_core/error.h>
#include <sb_core/bit.h>
#include "allocator.h"

namespace sb {

// todo(ebeau): make it final again
template <typename TMemProvider>
class PoolAllocator : public IAllocator
{
    sbBaseClass(IAllocator);

    using NodeIdx = s32;
    static constexpr NodeIdx INVALID_NODE_IDX = -1;

    void initFreeList()
    {
        s32 const block_cnt = numericConv<NodeIdx>(_arena.m_size / _actual_block_size);

        u8 * node_iter = static_cast<u8 *>(_arena.m_ptr);

        for (NodeIdx idx = 0; (block_cnt - 1) != idx; ++idx)
        {
            *reinterpret_cast<NodeIdx *>(node_iter) = idx + 1;
            node_iter += _actual_block_size;
        }

        *reinterpret_cast<NodeIdx *>(node_iter) = INVALID_NODE_IDX;
        _free_list_head = 0;
    }

public:
    PoolAllocator()
        : _default_alignment(ALIGNMENT_DEFAULT)
        , _actual_block_size(0U)
        , _mem_provider()
        , _arena()
        , _free_list_head(INVALID_NODE_IDX)
    {
    }

    PoolAllocator(usize block_size, usize block_count, Alignment default_align = ALIGNMENT_DEFAULT)
        : _default_alignment(default_align)
        , _mem_provider()
        , _free_list_head(INVALID_NODE_IDX)
    {
        _actual_block_size = alignUp(block_size, default_align);
        sbAssert(sizeof(NodeIdx) <= _actual_block_size);

        _arena = _mem_provider.allocate(block_count * _actual_block_size, default_align);
        sbAssert(!_arena.isEmpty());

        initFreeList();
    }

    PoolAllocator(TMemProvider const & mem_provider, usize block_size, usize block_count,
                  Alignment default_align = ALIGNMENT_DEFAULT)
        : _default_alignment(default_align)
        , _mem_provider(mem_provider)
        , _free_list_head(INVALID_NODE_IDX)
    {
        _actual_block_size = alignUp(block_size, default_align);
        sbAssert(sizeof(NodeIdx) <= _actual_block_size);

        _arena = _mem_provider.allocate(block_count * _actual_block_size, default_align);
        sbAssert(!_arena.isEmpty());

        initFreeList();
    }

    PoolAllocator & operator=(PoolAllocator const &) = delete;
    PoolAllocator & operator=(PoolAllocator &&) = delete;
    PoolAllocator(PoolAllocator const &) = delete;
    PoolAllocator(PoolAllocator &&) = delete;

    ~PoolAllocator() override
    {
        if (!_arena.isEmpty())
        {
            _mem_provider.deallocate(_arena.m_ptr);
        }
    }

    MemoryArena allocate(usize const size) override
    {
        if ((INVALID_NODE_IDX != _free_list_head) && (size <= _actual_block_size))
        {
            u8 * const free_node = static_cast<u8 *>(_arena.m_ptr) + (_free_list_head * _actual_block_size);
            void * block_ptr = free_node;

            _free_list_head = *reinterpret_cast<NodeIdx *>(free_node);

            return {block_ptr, _actual_block_size};
        }

        return {};
    }

    MemoryArena allocate(usize const size, [[maybe_unused]] Alignment const alignment) override
    {
        if (alignment <= _default_alignment)
        {
            return allocate(size);
        }

        return {};
    }

    void deallocate(void * ptr) override
    {
        if (sbExpect(_arena.isInRange(ptr)))
        {
            auto const ptr_offset = static_cast<u8 *>(ptr) - static_cast<u8 *>(_arena.m_ptr);
            sbAssert(0U == (ptr_offset % _actual_block_size));

            NodeIdx * const dealloc_node = static_cast<NodeIdx *>(ptr);
            NodeIdx const dealloc_node_idx = numericConv<NodeIdx>(ptr_offset / _actual_block_size);

            if (INVALID_NODE_IDX == _free_list_head)
            {
                *dealloc_node = INVALID_NODE_IDX;
            }
            else
            {
                *dealloc_node = _free_list_head;
            }

            _free_list_head = dealloc_node_idx;
        }
    }

    b8 owns(void const * ptr) const override
    {
        return _arena.isInRange(ptr);
    }

    void deallocateAll()
    {
        initFreeList();
    }

    MemoryArena allocate()
    {
        return allocate(_actual_block_size);
    }

    Alignment getAlignment() const
    {
        return _default_alignment;
    }

    MemoryArena getArena() const
    {
        return _arena;
    }

private:
    Alignment _default_alignment;
    usize _actual_block_size;
    TMemProvider _mem_provider;
    MemoryArena _arena;
    NodeIdx _free_list_head;
};

} // namespace sb
