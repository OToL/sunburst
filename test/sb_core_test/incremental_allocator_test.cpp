#include <sb_core/memory/allocator/incremental_allocator.h>
#include <sb_core/memory/allocator/memory_arena_allocator.h>
#include <sb_core/memory/memory.h>

#include <catch2/test_prolog.h>
#include <catch2/xcatch.hpp>

using namespace sb;

constexpr usize TEST_BACKSTORE_CAPACITY = 10U;
using TestBackstore = u8[TEST_BACKSTORE_CAPACITY];

TEST_CASE("Incremetal Allocator null", "[incremental_allocator]")
{
    IncrementalAllocator<MemoryArenaAllocator> test_alloc;

    REQUIRE(isEmpty(test_alloc.allocate(1)));
}

TEST_CASE("Incremetal Allocator allocate", "[incremental_allocator]")
{
    SECTION("Packed alignment")
    {
        IncrementalAllocator<MemoryArenaAllocator> test_alloc(MemoryArena{nullptr, TEST_BACKSTORE_CAPACITY},
                                                              TEST_BACKSTORE_CAPACITY, ALIGNMENT_1B);

        for (usize idx = 0; idx != TEST_BACKSTORE_CAPACITY; ++idx)
        {
            auto const mem_arena = test_alloc.allocate(1);

            REQUIRE(!isEmpty(mem_arena));
            REQUIRE((uptr)mem_arena.data == idx);
            REQUIRE(test_alloc.owns(mem_arena.data));
        }

        REQUIRE(isEmpty(test_alloc.allocate(1)));
    }

    SECTION("Null alloc")
    {
        IncrementalAllocator<MemoryArenaAllocator> test_alloc(MemoryArena{nullptr, TEST_BACKSTORE_CAPACITY},
                                                              TEST_BACKSTORE_CAPACITY);

        REQUIRE(isEmpty(test_alloc.allocate(0)));
    }

    SECTION("Default aligmnent")
    {
        IncrementalAllocator<MemoryArenaAllocator> test_alloc(MemoryArena{nullptr, TEST_BACKSTORE_CAPACITY},
                                                              TEST_BACKSTORE_CAPACITY, ALIGNMENT_4B);

        for (usize idx = 0; idx != (TEST_BACKSTORE_CAPACITY / ALIGNMENT_4B + 1); ++idx)
        {
            auto mem_arena = test_alloc.allocate(1);
            REQUIRE(!isEmpty(mem_arena));
            REQUIRE((uptr)mem_arena.data == idx * ALIGNMENT_4B);
            REQUIRE(test_alloc.owns(mem_arena.data));
        }

        REQUIRE(isEmpty(test_alloc.allocate(1)));
    }
}

TEST_CASE("Incremetal Allocator aligned allocate", "[incremental_allocator]")
{
    SECTION("Alloc")
    {
        IncrementalAllocator<MemoryArenaAllocator> test_alloc(MemoryArena{nullptr, TEST_BACKSTORE_CAPACITY},
                                                              TEST_BACKSTORE_CAPACITY);

        for (usize idx = 0; idx != (TEST_BACKSTORE_CAPACITY / ALIGNMENT_4B + 1); ++idx)
        {
            auto mem_arena = test_alloc.allocate(1, ALIGNMENT_4B);
            REQUIRE(!isEmpty(mem_arena));
            REQUIRE((uptr)mem_arena.data == idx * ALIGNMENT_4B);
            REQUIRE(test_alloc.owns(mem_arena.data));
        }

        REQUIRE(isEmpty(test_alloc.allocate(1)));
    }

    SECTION("Null alloc")
    {
        IncrementalAllocator<MemoryArenaAllocator> test_alloc(MemoryArena{nullptr, TEST_BACKSTORE_CAPACITY},
                                                              TEST_BACKSTORE_CAPACITY);

        REQUIRE(isEmpty(test_alloc.allocate(0, ALIGNMENT_4B)));
    }
}

TEST_CASE("Incremetal Allocator deallocate all", "[incremental_allocator]")
{
    IncrementalAllocator<MemoryArenaAllocator> test_alloc(MemoryArena{nullptr, TEST_BACKSTORE_CAPACITY},
                                                          TEST_BACKSTORE_CAPACITY);
    MemoryArena mem_arena;
    usize alloc_count_1 = 0U;
    usize alloc_count_2 = 0U;

    do
    {
        mem_arena = test_alloc.allocate(1);
        if (!isEmpty(mem_arena))
        {
            ++alloc_count_1;
        }
    } while (!isEmpty(mem_arena));

    test_alloc.deallocateAll();

    do
    {
        mem_arena = test_alloc.allocate(1);
        if (!isEmpty(mem_arena))
        {
            ++alloc_count_2;
        }
    } while (!isEmpty(mem_arena));

    REQUIRE(alloc_count_1 != 0U);
    REQUIRE(alloc_count_1 == alloc_count_2);
}

#include <catch2/test_epilog.h>
