#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/memory.h>
#include <sb_core/conversion.h>
#include <sb_core/error.h>
#include <sb_core/bit.h>
#include "allocator.h"

namespace sb {

template <usize BLOCK_SIZE, Alignment BLOCK_ALIGNMENT, typename TMemProvider>
class PoolAllocator final : public IAllocator
{
    sbBaseClass(IAllocator);

    static constexpr usize ACTUAL_BLOCK_SIZE = alignUp(BLOCK_SIZE, BLOCK_ALIGNMENT);
    using NodeIdx = s32;
    static constexpr NodeIdx INVALID_NODE = -1;

    struct alignas(BLOCK_ALIGNMENT) Node
    {
        union
        {
            NodeIdx m_next;
            u8 m_padding[ACTUAL_BLOCK_SIZE];
        };
    };

    static_assert(ACTUAL_BLOCK_SIZE >= sizeof(NodeIdx));
    static_assert(isPowerOf2(BLOCK_ALIGNMENT));

    void initFreeList()
    {
        s32 const block_cnt = numericConv<NodeIdx>(m_arena.m_size / ACTUAL_BLOCK_SIZE);
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
        usize m_block_count;
    };

    PoolAllocator()
        : m_mem_provider()
        , m_arena()
        , m_free_list(INVALID_NODE)
    {
    }

    PoolAllocator(InitParams const & init)
        : m_mem_provider()
        , m_arena(m_mem_provider.allocate(init.m_block_count * BLOCK_SIZE, BLOCK_ALIGNMENT))
        , m_free_list(0)
    {
        sbAssert(!m_arena.isEmpty());
        sbAssert(isAlignedTo(m_arena.m_ptr, BLOCK_ALIGNMENT));

        initFreeList();
    }

    PoolAllocator(InitParams const & init, TMemProvider const & mem_provider)
        : m_mem_provider(mem_provider)
        , m_arena(m_mem_provider.allocate(init.m_block_count * BLOCK_SIZE, BLOCK_ALIGNMENT))
        , m_free_list(0)
    {
        sbAssert(!m_arena.isEmpty());
        sbAssert(isAlignedTo(m_arena.m_ptr, BLOCK_ALIGNMENT));

        initFreeList();
    }

    PoolAllocator & operator=(PoolAllocator const &) = delete;
    PoolAllocator(PoolAllocator const &) = delete;

    ~PoolAllocator() override
    {
        m_mem_provider.deallocate(m_arena);
    }

    MemoryArena allocate(usize const size) override
    {
        void * block_ptr = nullptr;

        if ((INVALID_NODE != m_free_list) && sbExpect(size <= BLOCK_SIZE))
        {
            Node * const free_node = static_cast<Node *>(m_arena.m_ptr) + m_free_list;
            block_ptr = free_node;

            m_free_list = free_node->m_next;
        }

        return {block_ptr, size};
    }

    MemoryArena allocate(usize const size, [[maybe_unused]] Alignment const alignment) override
    {
        sbAssert(alignment <= BLOCK_ALIGNMENT);

        return allocate(size);
    }

    void deallocate(void * ptr) override
    {
        sbAssert(0U ==
                 (static_cast<usize>(static_cast<u8 *>(ptr) - static_cast<u8 *>(m_arena.m_ptr)) % ACTUAL_BLOCK_SIZE));

        if (sbExpect(m_arena.isInRange(ptr)))
        {
            Node * const dealloc_node = static_cast<Node *>(ptr);
            NodeIdx const dealloc_node_idx = numericConv<NodeIdx>(dealloc_node - static_cast<Node *>(m_arena.m_ptr));

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

    b8 owns(void const * ptr) const override
    {
        return m_arena.isInRange(ptr);
    }

    void * allocate()
    {
        return allocate(ACTUAL_BLOCK_SIZE).m_ptr;
    }

    void deallocateAll()
    {
        initFreeList();
    }

    constexpr Alignment getAlignment() const
    {
        return BLOCK_ALIGNMENT;
    }

    MemoryArena getArena() const
    {
        return m_arena;
    }

private:
    TMemProvider m_mem_provider;
    MemoryArena m_arena;
    NodeIdx m_free_list;
};

// TODO: ctor with number of objects instead of size
template <typename TObject, typename TMemoryProvider>
using ObjectPoolAllocator = PoolAllocator<sizeof(TObject), alignOf<TObject>(), TMemoryProvider>;

} // namespace sb
