#include <sb_core/memory/allocator/global_heap_allocator.h>
#include <sb_core/bit.h>

#include <catch2/test_prolog.h>
#include <catch2/xcatch.hpp>

using namespace sb;

TEST_CASE("Global Heap Allocator allocate", "[global_heap_allocator]")
{
    GlobalHeapAllocator test_alloc;

    MemoryArena mem_arena = test_alloc.allocate(10U);
    REQUIRE(!memarena_isEmpty(mem_arena));
    REQUIRE(mem_arena.size >= 10U);

    test_alloc.deallocate(mem_arena.data);
}

TEST_CASE("Global Heap Allocator aligned allocate", "[global_heap_allocator]")
{
    GlobalHeapAllocator test_alloc;

    MemoryArena mem_arena = test_alloc.allocate(ALIGNMENT_128B, 10U);
    REQUIRE(!memarena_isEmpty(mem_arena));
    REQUIRE(mem_arena.size >= 10U);
    REQUIRE(isAlignedTo(mem_arena.data, ALIGNMENT_128B));

    test_alloc.deallocate(mem_arena.data);
}

TEST_CASE("Global Heap Allocator default alignment", "[global_heap_allocator]")
{
    REQUIRE(GlobalHeapAllocator{}.getAlignment() == ALIGNMENT_DEFAULT);
}

#include <catch2/test_epilog.h>
