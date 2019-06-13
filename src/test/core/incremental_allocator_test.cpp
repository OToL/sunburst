#include <core/memory/allocator/incremental_allocator.h>
#include <core/memory/global_heap.h>
#include <core/memory/new.h>

#include <gtest/gtest_common.h>

namespace sb::test {

TEST(INCREMENTAL_ALLOCATOR, Allocate)
{
    usize const alloc_count = 4;
    usize const arena_size = alloc_count * IncrementalAllocator::ALIGNMENT;

    void * arena_ptr = getGlobalHeap()->allocate(arena_size);
    EXPECT_NE(nullptr, arena_ptr);

    IncrementalAllocator alloc{{{arena_ptr, arena_size}}};

    for (usize idx = 0; idx != alloc_count; ++idx)
    {
        void * mem_ptr = alloc.allocate(IncrementalAllocator::ALIGNMENT);
        EXPECT_NE(nullptr, mem_ptr);
        EXPECT_TRUE(alloc.owns(mem_ptr));
    }

    EXPECT_EQ(nullptr, alloc.allocate(IncrementalAllocator::ALIGNMENT));

    getGlobalHeap()->deallocate(arena_ptr);
}

TEST(INCREMENTAL_ALLOCATOR, AlignedAllocate)
{
    usize const alloc_count = 4;
    usize const test_alignment = 16;
    usize const alloc_size = 8;
    usize const arena_size = alloc_count * test_alignment;

    void * arena_ptr = getGlobalHeap()->allocate(arena_size, test_alignment);
    EXPECT_NE(nullptr, arena_ptr);

    IncrementalAllocator alloc{{{arena_ptr, arena_size}}};

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
    usize const arena_size = alloc_count * IncrementalAllocator::ALIGNMENT;

    void * arena_ptr = getGlobalHeap()->allocate(arena_size);
    EXPECT_NE(nullptr, arena_ptr);

    IncrementalAllocator alloc{{{arena_ptr, arena_size}}};

    usize test_count = 2;

    while (test_count != 0)
    {
        for (usize idx = 0; idx != alloc_count; ++idx)
        {
            void * mem_ptr = alloc.allocate(IncrementalAllocator::ALIGNMENT);
            EXPECT_NE(nullptr, mem_ptr);
            EXPECT_TRUE(alloc.owns(mem_ptr));
        }

        EXPECT_EQ(nullptr, alloc.allocate(IncrementalAllocator::ALIGNMENT));

        alloc.deallocateAll();
        --test_count;
    }
}

} // namespace sb::test
