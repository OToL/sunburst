#include <sb_core/memory/allocators/null_allocator.h>

#include <extern_prolog.h>
#include <catch2/catch.hpp>
#include <extern_epilog.h>

using namespace sb;

TEST_CASE("Null Allocator allocate", "[null_allocator]")
{
    NullAllocator test_alloc;
    MemoryArena mem_arena = test_alloc.allocate(1);

    REQUIRE(mem_arena.isEmpty());
    REQUIRE(!test_alloc.owns(mem_arena.data));
}

TEST_CASE("Null Allocator aligned allocate", "[null_allocator]")
{
    NullAllocator test_alloc;
    MemoryArena mem_arena = test_alloc.allocate(16U, 1);

    REQUIRE(mem_arena.isEmpty());
    REQUIRE(!test_alloc.owns(mem_arena.data));
}

