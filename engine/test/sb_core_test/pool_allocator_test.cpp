#include <sb_core/memory/allocators/pool_allocator.h>
#include <sb_core/memory/allocators/memory_arena_allocator.h>
#include <sb_core/containers/dynamic_fix_array.h>

#include <sb_slw/algorithm>

#include <extern_prolog.h>
#include <catch2/catch.hpp>
#include <extern_epilog.h>

using namespace sb;

constexpr usize TEST_OBJECT_COUNT = 10;
constexpr Alignment TEST_OBJECT_ALIGNMENT = DEFAULT_MEMORY_ALIGNMENT;

struct TestPoolObj
{
    alignas(TEST_OBJECT_ALIGNMENT) u64 m_data;
};
static_assert(sizeof(TestPoolObj) == TEST_OBJECT_ALIGNMENT);

using TestObjectArray = TestPoolObj[TEST_OBJECT_COUNT];
using TestPoolAllocator = PoolAllocator<MemoryArenaAllocator>;

TEST_CASE("Pool Allocator empty", "[pool_allocator]")
{
    SECTION("Allocate")
    {
        TestPoolAllocator test_alloc;
        REQUIRE(test_alloc.allocate().isEmpty());
        REQUIRE(test_alloc.allocate(sizeof(TestPoolObj)).isEmpty());
    }

    SECTION("Aligned allocate")
    {
        TestPoolAllocator test_alloc;
        REQUIRE(test_alloc.allocate(TEST_OBJECT_ALIGNMENT, sizeof(TestPoolObj)).isEmpty());
    }
}

TEST_CASE("Pool Allocator edge cases", "[pool_allocator]")
{
    SECTION("Allocate bigger object")
    {
        TestObjectArray testObjectArray;
        TestPoolAllocator test_alloc({testObjectArray}, sizeof(TestPoolObj), TEST_OBJECT_COUNT);

        REQUIRE(test_alloc.allocate(sizeof(TestPoolObj) + 1).isEmpty());
        REQUIRE(test_alloc.allocate(TEST_OBJECT_ALIGNMENT, sizeof(TestPoolObj) + 1).isEmpty());
    }

    SECTION("Allocate smaller object")
    {
        TestObjectArray testObjectArray;
        TestPoolAllocator test_alloc({testObjectArray}, sizeof(TestPoolObj), TEST_OBJECT_COUNT);

        REQUIRE(test_alloc.allocate(sizeof(TestPoolObj) / 2).size == sizeof(TestPoolObj));
    }

    SECTION("Allocate larger alignment")
    {
        TestObjectArray testObjectArray;
        TestPoolAllocator test_alloc({testObjectArray}, sizeof(TestPoolObj), TEST_OBJECT_COUNT);

        REQUIRE(16U > TEST_OBJECT_ALIGNMENT);
        REQUIRE(test_alloc.allocate(16U, sizeof(TestPoolObj)).isEmpty());
    }
}

TEST_CASE("Pool Allocator consume all", "[pool_allocator]")
{
    SECTION("Allocate all")
    {
        TestObjectArray testObjectArray;
        TestPoolAllocator test_alloc({testObjectArray}, sizeof(TestPoolObj), TEST_OBJECT_COUNT);

        usize alloc_obj_cnt = 0;
        MemoryArena mem_arena = {};

        do
        {
            mem_arena = test_alloc.allocate(sizeof(TestPoolObj));

            if (!mem_arena.isEmpty())
            {
                REQUIRE(mem_arena.size == sizeof(TestPoolObj));
                REQUIRE(test_alloc.owns(mem_arena.data));
                REQUIRE(mem_arena.data == &testObjectArray[alloc_obj_cnt]);
                ++alloc_obj_cnt;
            }
        } while (!mem_arena.isEmpty());

        REQUIRE(alloc_obj_cnt == TEST_OBJECT_COUNT);
    }

    SECTION("Aligned allocate all")
    {
        TestObjectArray testObjectArray;
        TestPoolAllocator test_alloc({testObjectArray}, sizeof(TestPoolObj), TEST_OBJECT_COUNT);

        usize alloc_obj_cnt = 0;
        MemoryArena mem_arena = {};

        do
        {
            mem_arena = test_alloc.allocate(TEST_OBJECT_ALIGNMENT, sizeof(TestPoolObj));

            if (!mem_arena.isEmpty())
            {
                REQUIRE(mem_arena.size == sizeof(TestPoolObj));
                REQUIRE(test_alloc.owns(mem_arena.data));
                REQUIRE(mem_arena.data == &testObjectArray[alloc_obj_cnt]);
                ++alloc_obj_cnt;
            }
        } while (!mem_arena.isEmpty());

        REQUIRE(alloc_obj_cnt == TEST_OBJECT_COUNT);
    }
}

TEST_CASE("Pool Allocator deallocate all", "[pool_allocator]")
{
    TestObjectArray testObjectArray;
    TestPoolAllocator test_alloc({testObjectArray}, sizeof(TestPoolObj), TEST_OBJECT_COUNT);

    usize test_iter_count = 2;

    while (0 != test_iter_count)
    {
        usize alloc_obj_cnt = 0;
        MemoryArena mem_arena = {};

        do
        {
            mem_arena = test_alloc.allocate(sizeof(TestPoolObj));

            if (!mem_arena.isEmpty())
            {
                REQUIRE(mem_arena.size == sizeof(TestPoolObj));
                REQUIRE(test_alloc.owns(mem_arena.data));
                REQUIRE(mem_arena.data == &testObjectArray[alloc_obj_cnt]);
                ++alloc_obj_cnt;
            }
        } while (!mem_arena.isEmpty());

        REQUIRE(alloc_obj_cnt == TEST_OBJECT_COUNT);
        test_alloc.deallocateAll();

        --test_iter_count;
    }
}

TEST_CASE("Pool Allocator deallocate", "[pool_allocator]")
{
    TestObjectArray testObjectArray;
    TestPoolAllocator test_alloc({testObjectArray}, sizeof(TestPoolObj), TEST_OBJECT_COUNT);

    struct TestOp
    {
        usize m_alloc_cnt;
        usize m_dealloc_cnt;
    };

    TestOp ops[] = {{5U, 3U}, {6U, 1U}, {2U, 5U}, {3U, 4U}};
    DFArray<void *, TEST_OBJECT_COUNT> alloc_objs;

    for (auto const & op : ops)
    {
        usize alloc_cnt = op.m_alloc_cnt;
        while (alloc_cnt != 0)
        {
            auto const mem_arena = test_alloc.allocate(sizeof(TestPoolObj));

            REQUIRE(!mem_arena.isEmpty());
            REQUIRE(test_alloc.owns(mem_arena.data));
            REQUIRE(end(alloc_objs) == slw::find(begin(alloc_objs), end(alloc_objs), mem_arena.data));

            alloc_objs.push_back(mem_arena.data);

            --alloc_cnt;
        }

        usize dealloc_cnt = op.m_dealloc_cnt;
        while (dealloc_cnt != 0)
        {
            auto obj_ptr = alloc_objs.back();

            auto obj_iter = slw::find(begin(alloc_objs), end(alloc_objs), obj_ptr);
            REQUIRE(end(alloc_objs) != obj_ptr);
            alloc_objs.erase(obj_iter);

            test_alloc.deallocate(obj_ptr);

            --dealloc_cnt;
        }
    }
}

