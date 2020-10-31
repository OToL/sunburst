#include <sb_core/memory/allocator/global_heap_allocator.h>
#include <sb_core/bit.h>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

TEST_CASE("Global Heap Allocator allocate", "[global_heap_allocator]")
{
    GlobalHeapAllocator test_alloc;

    MemoryArena mem_arena = test_alloc.allocate(10U);
    REQUIRE(!mem_arena.isEmpty());
    REQUIRE(mem_arena.m_size >= 10U);

    test_alloc.deallocate(mem_arena.m_ptr);
}

TEST_CASE("Global Heap Allocator aligned allocate", "[global_heap_allocator]")
{
    GlobalHeapAllocator test_alloc;

    MemoryArena mem_arena = test_alloc.allocate(10U, ALIGNMENT_128B);
    REQUIRE(!mem_arena.isEmpty());
    REQUIRE(mem_arena.m_size >= 10U);
    REQUIRE(isAlignedTo(mem_arena.m_ptr, ALIGNMENT_128B));

    test_alloc.deallocate(mem_arena.m_ptr);
}

TEST_CASE("Global Heap Allocator default alignment", "[global_heap_allocator]")
{
    REQUIRE(GlobalHeapAllocator{}.getAlignment() == ALIGNMENT_DEFAULT);
}

#include <catch2/test_epilog.h>
