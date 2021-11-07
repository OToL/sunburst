#include <sb_core/memory/allocator/incremental_allocator.h>
#include <sb_core/memory/allocator/memory_arena_allocator.h>
#include <sb_core/memory/memory.h>

#include <extern_prolog.h>
#include <catch2/catch.hpp>

using namespace sb;

constexpr usize TEST_BACKSTORE_CAPACITY = 10U;
using TestBackstore = u8[TEST_BACKSTORE_CAPACITY];

TEST_CASE("Incremetal Allocator null", "[incremental_allocator]")
{
    IncrementalAllocator<MemoryArenaAllocator> test_alloc;

    REQUIRE(memory_arena::isEmpty(test_alloc.allocate(1)));
}

TEST_CASE("Incremetal Allocator allocate", "[incremental_allocator]")
{
    SECTION("Packed alignment")
    {
        IncrementalAllocator<MemoryArenaAllocator> test_alloc(MemoryArena{nullptr, TEST_BACKSTORE_CAPACITY},
                                                              TEST_BACKSTORE_CAPACITY, 1U);

        for (usize idx = 0; idx != TEST_BACKSTORE_CAPACITY; ++idx)
        {
            auto const mem_arena = test_alloc.allocate(1);

            REQUIRE(!memory_arena::isEmpty(mem_arena));
            REQUIRE(reinterpret_cast<uptr>(mem_arena.data) == idx);
            REQUIRE(test_alloc.owns(mem_arena.data));
        }

        REQUIRE(memory_arena::isEmpty(test_alloc.allocate(1)));
    }

    SECTION("Null alloc")
    {
        IncrementalAllocator<MemoryArenaAllocator> test_alloc(MemoryArena{nullptr, TEST_BACKSTORE_CAPACITY},
                                                              TEST_BACKSTORE_CAPACITY);

        REQUIRE(memory_arena::isEmpty(test_alloc.allocate(0)));
    }

    SECTION("Default aligmnent")
    {
        IncrementalAllocator<MemoryArenaAllocator> test_alloc(MemoryArena{nullptr, TEST_BACKSTORE_CAPACITY},
                                                              TEST_BACKSTORE_CAPACITY, 4U);

        for (usize idx = 0; idx != (TEST_BACKSTORE_CAPACITY / 4U + 1); ++idx)
        {
            auto mem_arena = test_alloc.allocate(1);
            REQUIRE(!memory_arena::isEmpty(mem_arena));
            REQUIRE(reinterpret_cast<uptr>(mem_arena.data) == idx * 4U);
            REQUIRE(test_alloc.owns(mem_arena.data));
        }

        REQUIRE(memory_arena::isEmpty(test_alloc.allocate(1)));
    }
}

TEST_CASE("Incremetal Allocator aligned allocate", "[incremental_allocator]")
{
    SECTION("Alloc")
    {
        IncrementalAllocator<MemoryArenaAllocator> test_alloc(MemoryArena{nullptr, TEST_BACKSTORE_CAPACITY},
                                                              TEST_BACKSTORE_CAPACITY);

        for (usize idx = 0; idx != (TEST_BACKSTORE_CAPACITY / 4U + 1); ++idx)
        {
            auto mem_arena = test_alloc.allocate(4U, 1);
            REQUIRE(!memory_arena::isEmpty(mem_arena));
            REQUIRE(reinterpret_cast<uptr>(mem_arena.data) == idx * 4U);
            REQUIRE(test_alloc.owns(mem_arena.data));
        }

        REQUIRE(memory_arena::isEmpty(test_alloc.allocate(1)));
    }

    SECTION("Null alloc")
    {
        IncrementalAllocator<MemoryArenaAllocator> test_alloc(MemoryArena{nullptr, TEST_BACKSTORE_CAPACITY},
                                                              TEST_BACKSTORE_CAPACITY);

        REQUIRE(memory_arena::isEmpty(test_alloc.allocate(4U, 0)));
    }
}

TEST_CASE("Incremetal Allocator deallocate all", "[incremental_allocator]")
{
    IncrementalAllocator<MemoryArenaAllocator> test_alloc(MemoryArena{nullptr, TEST_BACKSTORE_CAPACITY},
                                                          TEST_BACKSTORE_CAPACITY);
    MemoryArena mem_arena = {};
    usize alloc_count_1 = 0U;
    usize alloc_count_2 = 0U;

    do
    {
        mem_arena = test_alloc.allocate(1);
        if (!memory_arena::isEmpty(mem_arena))
        {
            ++alloc_count_1;
        }
    } while (!memory_arena::isEmpty(mem_arena));

    test_alloc.deallocateAll();

    do
    {
        mem_arena = test_alloc.allocate(1);
        if (!memory_arena::isEmpty(mem_arena))
        {
            ++alloc_count_2;
        }
    } while (!memory_arena::isEmpty(mem_arena));

    REQUIRE(alloc_count_1 != 0U);
    REQUIRE(alloc_count_1 == alloc_count_2);
}

#include <extern_epilog.h>
