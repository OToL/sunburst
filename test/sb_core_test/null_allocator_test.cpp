#include <sb_core/memory/allocator/null_allocator.h>

#include <extern_prolog.h>
#include <catch2/catch.hpp>

using namespace sb;

TEST_CASE("Null Allocator allocate", "[null_allocator]")
{
    NullAllocator test_alloc;
    MemoryArena mem_arena = test_alloc.allocate(1);

    REQUIRE(memarena_isEmpty(mem_arena));
    REQUIRE(!test_alloc.owns(mem_arena.data));
}

TEST_CASE("Null Allocator aligned allocate", "[null_allocator]")
{
    NullAllocator test_alloc;
    MemoryArena mem_arena = test_alloc.allocate(ALIGNMENT_16B, 1);

    REQUIRE(memarena_isEmpty(mem_arena));
    REQUIRE(!test_alloc.owns(mem_arena.data));
}

#include <extern_epilog.h>
