#pragma once

#include <core/platform.h>
#include <core/error.h>
#include <core/memory/memory_arena.h>
#include <core/conversion.h>
#include <core/bitwise.h>

namespace sb {
template <usize BLOCK_SIZE, usize BLOCK_ALIGNMENT>
class PoolAllocator
{
    sbCopyProtect(PoolAllocator);

    static constexpr usize ACTUAL_BLOCK_SIZE = alignUp(BLOCK_SIZE, BLOCK_ALIGNMENT);
    using NodeIdx = si32;
    static constexpr NodeIdx INVALID_NODE = -1;

    struct alignas(BLOCK_ALIGNMENT) Node
    {
        union
        {
            NodeIdx m_next;
            ui8 m_padding[ACTUAL_BLOCK_SIZE];
        };
    };

    static_assert(ACTUAL_BLOCK_SIZE >= sizeof(NodeIdx));
    static_assert(isPowerOf2(BLOCK_ALIGNMENT));

    void initFreeList()
    {
        si32 const block_cnt = numericCast<NodeIdx>(m_arena.m_size / ACTUAL_BLOCK_SIZE);
        sbAssert(0 != block_cnt);

        Node * node_iter = static_cast<Node *>(m_arena.m_ptr);

        for (NodeIdx idx = 0; (block_cnt - 1) != idx; ++idx)
        {
            node_iter->m_next = idx + 1;
            ++node_iter;
        }

        node_iter->m_next = INVALID_NODE;
        m_free_list = 0;
    }

public:
    struct InitParams
    {
        MemoryArena m_arena;
    };

    static constexpr usize ALIGNMENT = BLOCK_ALIGNMENT;

    PoolAllocator()
        : m_arena()
        , m_free_list(INVALID_NODE)
    {
    }

    PoolAllocator(InitParams const & init)
        : m_arena(init.m_arena)
        , m_free_list(0)
    {
        sbAssert(!m_arena.isEmpty());
        sbAssert(isAlignedTo(m_arena.m_ptr, ALIGNMENT));

        initFreeList();
    }

    constexpr usize getAlignment() const
    {
        return ALIGNMENT;
    }

    void * allocate(usize const size)
    {
        void * block_ptr = nullptr;

        if ((INVALID_NODE != m_free_list) && sbExpectTrue(size <= BLOCK_SIZE))
        {
            Node * const free_node = static_cast<Node *>(m_arena.m_ptr) + m_free_list;
            block_ptr = free_node;

            m_free_list = free_node->m_next;
        }

        return block_ptr;
    }

    void * allocate(usize const size, [[maybe_unused]] usize const alignment)
    {
        sbWarn(alignment == ALIGNMENT);

        return allocate(size);
    }

    void * allocate()
    {
        return allocate(ACTUAL_BLOCK_SIZE);
    }

    void deallocate(void * ptr)
    {
        sbAssert(0U == (static_cast<usize>(static_cast<ui8 *>(ptr) - static_cast<ui8 *>(m_arena.m_ptr)) % ACTUAL_BLOCK_SIZE));

        if (sbExpectTrue(m_arena.isInRange(ptr)))
        {
            Node * const dealloc_node = static_cast<Node *>(ptr);
            NodeIdx const dealloc_node_idx = numericCast<NodeIdx>(dealloc_node - static_cast<Node *>(m_arena.m_ptr));

            if (INVALID_NODE == m_free_list)
            {
                dealloc_node->m_next = INVALID_NODE;
            }
            else
            {
                dealloc_node->m_next = m_free_list;
            }

            m_free_list = dealloc_node_idx;
        }
    }

    void deallocateAll()
    {
        initFreeList();
    }

    b8 owns(void const * ptr)
    {
        return m_arena.isInRange(ptr);
    }

    MemoryArena getArena() const
    {
        return m_arena;
    }

private:
    MemoryArena m_arena;
    NodeIdx m_free_list;
};

template <typename TObject>
using ObjectPoolAllocator = PoolAllocator<sizeof(TObject), alignof(TObject)>;

} // namespace sb
