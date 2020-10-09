#include <sb_core/memory/allocator/pool_allocator.h>
#include <sb_core/memory/provider/memory_arena_provider.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/container/static_vector.h>

#include <gtest/gtest_common.h>
#include <gtest/allocator_stats.h>

#include <sb_std/algorithm>

namespace sb;

struct TestPoolObj
{
    u32 m_data;
};

constexpr usize OBJECT_COUNT = 10;
constexpr usize POOL_SIZE = OBJECT_COUNT * sizeof(TestPoolObj);

using PoolTestAllocator = PoolAllocator<sizeof(TestPoolObj), alignOf<TestPoolObj>(), MemoryArenaProvider>;

TEST(POOL_ALLOCATOR, AllocateAll)
{
    void * const pool_base = getGlobalHeap()->allocate(POOL_SIZE, alignOf<TestPoolObj>());
    MemoryArena const pool_arena = {pool_base, POOL_SIZE};

    {
        PoolTestAllocator alloc({OBJECT_COUNT}, {pool_arena});

        usize alloc_obj_cnt = 0;
        void * obj_ptr = alloc.allocate(sizeof(TestPoolObj));

        while (nullptr != obj_ptr)
        {
            ++alloc_obj_cnt;
            obj_ptr = alloc.allocate(sizeof(TestPoolObj));
            EXPECT_TRUE((alloc_obj_cnt == OBJECT_COUNT) || ((nullptr != obj_ptr) && alloc.owns(obj_ptr)));
        }

        EXPECT_EQ(alloc_obj_cnt, OBJECT_COUNT);
    }

    getGlobalHeap()->deallocate(pool_base);
}

TEST(POOL_ALLOCATOR, AlignAllocateAll)
{
    void * const pool_base = getGlobalHeap()->allocate(POOL_SIZE, alignOf<TestPoolObj>());
    MemoryArena const pool_arena = {pool_base, POOL_SIZE};

    {
        PoolTestAllocator alloc({OBJECT_COUNT}, {pool_arena});

        usize alloc_obj_cnt = 0;
        void * obj_ptr = alloc.allocate(sizeof(TestPoolObj));

        while (nullptr != obj_ptr)
        {
            ++alloc_obj_cnt;
            obj_ptr = alloc.allocate(sizeof(TestPoolObj), alignOf<TestPoolObj>());
            EXPECT_TRUE((alloc_obj_cnt == OBJECT_COUNT) || ((nullptr != obj_ptr) && alloc.owns(obj_ptr)));
        }

        EXPECT_EQ(alloc_obj_cnt, OBJECT_COUNT);
    }

    getGlobalHeap()->deallocate(pool_base);
}

TEST(POOL_ALLOCATOR, DeallocateAll)
{
    void * const pool_base = getGlobalHeap()->allocate(POOL_SIZE, alignOf<TestPoolObj>());
    MemoryArena const pool_arena = {pool_base, POOL_SIZE};

    usize test_cnt = 1;

    PoolTestAllocator alloc({OBJECT_COUNT}, {pool_arena});

    while (OBJECT_COUNT > test_cnt)
    {
        usize alloc_obj_cnt = 0;
        void * obj_ptr = alloc.allocate(sizeof(TestPoolObj));

        while (alloc_obj_cnt != test_cnt)
        {
            ++alloc_obj_cnt;
            obj_ptr = alloc.allocate(sizeof(TestPoolObj));
            EXPECT_TRUE((alloc_obj_cnt == OBJECT_COUNT) || ((nullptr != obj_ptr) && alloc.owns(obj_ptr)));
        }

        alloc.deallocateAll();

        ++test_cnt;
    }

    getGlobalHeap()->deallocate(pool_base);
}

TEST(POOL_ALLOCATOR, AllocateDeallocate)
{
    void * const pool_base = getGlobalHeap()->allocate(POOL_SIZE, alignOf<TestPoolObj>());
    MemoryArena const pool_arena = {pool_base, POOL_SIZE};
    PoolTestAllocator alloc({OBJECT_COUNT}, {pool_arena});

    struct TestOp
    {
        usize m_alloc_cnt;
        usize m_dealloc_cnt;
    };

    TestOp ops[] = {{5, 3}, {6, 1}, {2, 5}, {3, 4}};
    StaticVector<void *, OBJECT_COUNT> alloc_objs;

    for (auto const & op : ops)
    {
        usize alloc_cnt = op.m_alloc_cnt;

        while (alloc_cnt != 0)
        {
            void * const obj_ptr = alloc.allocate(sizeof(TestPoolObj));

            EXPECT_NE(nullptr, obj_ptr);
            EXPECT_TRUE(alloc.owns(obj_ptr));
            EXPECT_EQ(end(alloc_objs), sbstd::find(begin(alloc_objs), end(alloc_objs), obj_ptr));

            alloc_objs.push_back(obj_ptr);

            --alloc_cnt;
        }

        usize dealloc_cnt = op.m_dealloc_cnt;
        while (dealloc_cnt != 0)
        {
            auto obj_ptr = alloc_objs.back();

            auto obj_iter = sbstd::find(begin(alloc_objs), end(alloc_objs), obj_ptr);
            EXPECT_NE(end(alloc_objs), obj_ptr);
            alloc_objs.erase(obj_iter);

            alloc.deallocate(obj_ptr);

            --dealloc_cnt;
        }
    }

    getGlobalHeap()->deallocate(pool_base);
}
