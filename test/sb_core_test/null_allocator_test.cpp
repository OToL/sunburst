#include <sb_core/memory/allocator/null_allocator.h>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

TEST_CASE("Null Allocator allocate", "[null_allocator]")
{
    NullAllocator test_alloc;
    MemoryArena mem_arena = test_alloc.allocate(1);

    REQUIRE(mem_arena.isEmpty());
    REQUIRE(!test_alloc.owns(mem_arena.m_ptr));
}

TEST_CASE("Null Allocator aligned allocate", "[null_allocator]")
{
    NullAllocator test_alloc;
    MemoryArena mem_arena = test_alloc.allocate(1, ALIGNMENT_16B);

    REQUIRE(mem_arena.isEmpty());
    REQUIRE(!test_alloc.owns(mem_arena.m_ptr));
}

#include <catch2/test_epilog.h>
