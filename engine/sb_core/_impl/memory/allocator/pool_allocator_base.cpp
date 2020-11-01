#include "pool_allocator_base.h"
#include <sb_core/conversion.h>
#include <sb_core/error.h>

void sb::PoolAllocatorBase::initFreeList()
{
    s32 const block_cnt = numericConv<NodeIdx>(_arena.size / _actual_block_size);

    u8 * node_iter = static_cast<u8 *>(_arena.data);

    for (NodeIdx idx = 0; (block_cnt - 1) != idx; ++idx)
    {
        *reinterpret_cast<NodeIdx *>(node_iter) = idx + 1;
        node_iter += _actual_block_size;
    }

    *reinterpret_cast<NodeIdx *>(node_iter) = INVALID_NODE_IDX;
    _free_list_head = 0;
}

sb::PoolAllocatorBase::PoolAllocatorBase()
    : _arena()
    , _default_alignment(ALIGNMENT_DEFAULT)
    , _actual_block_size(0U)
    , _free_list_head(INVALID_NODE_IDX)
{
}

void sb::PoolAllocatorBase::init(MemoryArena arena, usize block_size, [[maybe_unused]] Alignment default_align)
{
    _default_alignment = ALIGNMENT_DEFAULT;
    _actual_block_size = block_size;
    _arena = arena;

    initFreeList();
}

sb::MemoryArena sb::PoolAllocatorBase::allocate(usize const size)
{
    if ((INVALID_NODE_IDX != _free_list_head) && (size <= _actual_block_size))
    {
        u8 * const free_node = static_cast<u8 *>(_arena.data) + (_free_list_head * _actual_block_size);
        void * block_ptr = free_node;

        _free_list_head = *reinterpret_cast<NodeIdx *>(free_node);

        return {block_ptr, _actual_block_size};
    }

    return {};
}

sb::MemoryArena sb::PoolAllocatorBase::allocate(usize const size, [[maybe_unused]] Alignment const alignment)
{
    if (alignment <= _default_alignment)
    {
        return allocate(size);
    }

    return {};
}

void sb::PoolAllocatorBase::deallocate(void * ptr)
{
    if (sbExpect(_arena.isInRange(ptr)))
    {
        auto const ptr_offset = static_cast<u8 *>(ptr) - static_cast<u8 *>(_arena.data);
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

sb::b8 sb::PoolAllocatorBase::owns(void const * ptr) const
{
    return _arena.isInRange(ptr);
}

void sb::PoolAllocatorBase::deallocateAll()
{
    initFreeList();
}

sb::MemoryArena sb::PoolAllocatorBase::allocate()
{
    return allocate(_actual_block_size);
}
