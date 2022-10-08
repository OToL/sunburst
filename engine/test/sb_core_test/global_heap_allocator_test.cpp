#include <sb_core/memory/allocator/global_heap_allocator.h>
#include <sb_core/bit.h>

#include <extern_prolog.h>
#include <catch2/catch.hpp>

using namespace sb;

TEST_CASE("Global Heap Allocator allocate", "[global_heap_allocator]")
{
    GlobalHeapAllocator test_alloc;

    MemoryArena mem_arena = test_alloc.allocate(10U);
    REQUIRE(!memory_arena::isEmpty(mem_arena));
    REQUIRE(mem_arena.size >= 10U);

    test_alloc.deallocate(mem_arena.data);
}

TEST_CASE("Global Heap Allocator aligned allocate", "[global_heap_allocator]")
{
    GlobalHeapAllocator test_alloc;

    MemoryArena mem_arena = test_alloc.allocate(128U, 10U);
    REQUIRE(!memory_arena::isEmpty(mem_arena));
    REQUIRE(mem_arena.size >= 10U);
    REQUIRE(isAlignedTo(mem_arena.data, 128U));

    test_alloc.deallocate(mem_arena.data);
}

TEST_CASE("Global Heap Allocator default alignment", "[global_heap_allocator]")
{
    REQUIRE(GlobalHeapAllocator{}.getAlignment() == DEFAULT_MEMORY_ALIGNMENT);
}

#include <extern_epilog.h>
