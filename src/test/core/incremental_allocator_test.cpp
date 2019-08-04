#include <core/memory/allocator/incremental_allocator.h>
#include <core/memory/allocator/memory_arena_provider.h>
#include <core/memory/global_heap.h>
#include <core/memory/memory.h>
#include <core/bitwise.h>

#include <gtest/gtest_common.h>

namespace sb::test {

using TestIncrementalAllocator = IncrementalAllocator<MemoryArenaProvider>;
constexpr Alignment TEST_ALLOC_DEFAULT_ALIGN = ALIGN_8B;

TEST(INCREMENTAL_ALLOCATOR, Allocate)
{
    usize const alloc_count = 4;
    usize const arena_size = alloc_count * TEST_ALLOC_DEFAULT_ALIGN;

    void * arena_ptr = getGlobalHeap()->allocate(arena_size);
    EXPECT_NE(nullptr, arena_ptr);

    TestIncrementalAllocator alloc{{arena_size, TEST_ALLOC_DEFAULT_ALIGN}, {{arena_ptr, arena_size}}};

    for (usize idx = 0; idx != alloc_count; ++idx)
    {
        void * mem_ptr = alloc.allocate(TEST_ALLOC_DEFAULT_ALIGN);
        EXPECT_NE(nullptr, mem_ptr);
        EXPECT_TRUE(alloc.owns(mem_ptr));
    }

    EXPECT_EQ(nullptr, alloc.allocate(TEST_ALLOC_DEFAULT_ALIGN));

    getGlobalHeap()->deallocate(arena_ptr);
}

TEST(INCREMENTAL_ALLOCATOR, AlignedAllocate)
{
    usize const alloc_count = 4;
    Alignment const test_alignment = ALIGN_16B;
    usize const alloc_size = 8;
    usize const arena_size = alloc_count * test_alignment;

    void * arena_ptr = getGlobalHeap()->allocate(arena_size, test_alignment);
    EXPECT_NE(nullptr, arena_ptr);

    TestIncrementalAllocator alloc{{arena_size, TEST_ALLOC_DEFAULT_ALIGN}, {{arena_ptr, arena_size}}};

    for (usize idx = 0; idx != alloc_count; ++idx)
    {
        void * mem_ptr = alloc.allocate(alloc_size, test_alignment);
        EXPECT_NE(nullptr, mem_ptr);
        EXPECT_TRUE(isAlignedTo(reinterpret_cast<uiptr>(mem_ptr), test_alignment));
        EXPECT_TRUE(alloc.owns(mem_ptr));
    }

    EXPECT_EQ(nullptr, alloc.allocate(8, test_alignment));
    EXPECT_NE(nullptr, alloc.allocate(8));
    EXPECT_EQ(nullptr, alloc.allocate(8));

    getGlobalHeap()->deallocate(arena_ptr);
}

TEST(INCREMENTAL_ALLOCATOR, DeallocateAll)
{
    usize const alloc_count = 4;
    usize const arena_size = alloc_count * TEST_ALLOC_DEFAULT_ALIGN;

    void * arena_ptr = getGlobalHeap()->allocate(arena_size);
    EXPECT_NE(nullptr, arena_ptr);

    TestIncrementalAllocator alloc{{arena_size, TEST_ALLOC_DEFAULT_ALIGN}, {{arena_ptr, arena_size}}};

    usize test_count = 2;

    while (test_count != 0)
    {
        for (usize idx = 0; idx != alloc_count; ++idx)
        {
            void * mem_ptr = alloc.allocate(TEST_ALLOC_DEFAULT_ALIGN);
            EXPECT_NE(nullptr, mem_ptr);
            EXPECT_TRUE(alloc.owns(mem_ptr));
        }

        EXPECT_EQ(nullptr, alloc.allocate(TEST_ALLOC_DEFAULT_ALIGN));

        alloc.deallocateAll();
        --test_count;
    }
}


} // namespace sb::test
