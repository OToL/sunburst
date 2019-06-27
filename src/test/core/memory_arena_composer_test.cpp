#include <core/memory/allocator/memory_arena_composer.h>
#include <core/memory/allocator/incremental_allocator.h>
#include <core/memory/allocator/global_heap_allocator.h>
#include <core/memory/memory.h>

#include <gtest/gtest_common.h>
#include <gtest/allocator_stats.h>

namespace sb::test {

class NullArenaAllocator
{
public:
    struct InitParams
    {
        MemoryArena m_arena;
    };

    static constexpr Alignment ALIGNMENT = ALIGN_DEFAULT;

    NullArenaAllocator(InitParams const & init)
        : m_arena(init.m_arena)
    {
    }

    constexpr usize getAlignment() const
    {
        return ALIGNMENT;
    }

    void * allocate(usize const)
    {
        return nullptr;
    }

    void * allocate(usize const, Alignment const)
    {
        return nullptr;
    }

    void deallocate(void *) {}

    void deallocateAll() {}

    b8 owns(void const *) const
    {
        return true;
    }

    MemoryArena getArena() const
    {
        return m_arena;
    }

private:
    MemoryArena m_arena;
};

using MemoryArenaComposerTest = MemoryArenaComposer<AllocatorView, NullArenaAllocator>;

TEST(MEMORY_ARENA_COMPOSER, RAII)
{
    gtestx::AllocatorStats tracker;

    {
        MemoryArenaComposerTest alloc{{64, AllocatorView::InitParams{tracker}, {{}}}};

        auto const & mem_provider_stats = tracker.getStats();
        EXPECT_EQ(mem_provider_stats.m_alloc_count, 1U);
        EXPECT_EQ(mem_provider_stats.m_allocated_byte, 64U);
    }

    auto const & mem_provider_stats = tracker.getStats();
    EXPECT_EQ(mem_provider_stats.m_alloc_count, 0U);
    EXPECT_EQ(mem_provider_stats.m_allocated_byte, 0U);
}

} // namespace sb::test
