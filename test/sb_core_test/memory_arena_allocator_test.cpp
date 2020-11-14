#include <sb_core/memory/allocator/memory_arena_allocator.h>

#include <catch2/test_prolog.h>
#include <catch2/xcatch.hpp>

using namespace sb;

TEST_CASE("Memory Arena Allocator empty", "[memory_arena_allocator]")
{
    MemoryArenaAllocator alloc;

    REQUIRE(isEmpty(alloc.allocate(1)));
    REQUIRE(isEmpty(alloc.allocate(1, ALIGNMENT_8B)));
}

TEST_CASE("Memory Arena Allocator init", "[memory_arena_allocator]")
{
    SECTION("Local buffer init")
    {
        u8 my_array[8];
        MemoryArenaAllocator alloc(my_array);

        REQUIRE(alloc.getArena().size == sizeof(my_array));
        REQUIRE(alloc.getArena().data == my_array);
    }

    SECTION("Arena init")
    {
        MemoryArena arena = {(void *)8, 8};
        MemoryArenaAllocator alloc(arena);

        REQUIRE(alloc.getArena().size == 8U);
        REQUIRE(alloc.getArena().data == (void *)8);
    }
}

TEST_CASE("Memory Arena Allocator aligned allocate", "[memory_arena_allocator]")
{
    alignas(8) u8 my_array[8];

    SECTION("Perfect fit")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(alloc.getArena().size, ALIGNMENT_8B);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == my_array);
        REQUIRE(isEmpty(alloc.allocate(1)));
    }

    SECTION("Smaller allocation")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(4, ALIGNMENT_8B);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == my_array);
        REQUIRE(isEmpty(alloc.allocate(1)));
    }

    SECTION("Larger allocation")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(alloc.getArena().size + 1, ALIGNMENT_8B);
        REQUIRE(isEmpty(arena));
        REQUIRE(!isEmpty(alloc.allocate(alloc.getArena().size)));
    }

    SECTION("Alignment overflow")
    {
        MemoryArenaAllocator alloc(MemoryArena{(void *)8, 8});

        MemoryArena arena = alloc.allocate(8, ALIGNMENT_16B);
        REQUIRE(isEmpty(arena));

        arena = alloc.allocate(8, ALIGNMENT_8B);
        REQUIRE(!isEmpty(arena));
    }

    SECTION("Reset")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(4, ALIGNMENT_8B);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == my_array);
        REQUIRE(isEmpty(alloc.allocate(1)));

        alloc.deallocateAll();
        arena = alloc.allocate(4, ALIGNMENT_8B);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == my_array);
    }

    SECTION("Null alloc")
    {
        MemoryArenaAllocator alloc{};

        REQUIRE(isEmpty(alloc.allocate(1, ALIGNMENT_1B)));
    }
}

TEST_CASE("Memory Arena Allocator allocate", "[memory_arena_allocator]")
{
    alignas(8) u8 my_array[8];

    SECTION("Perfect fit")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(alloc.getArena().size);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == my_array);
        REQUIRE(isEmpty(alloc.allocate(1)));
    }

    SECTION("Smaller allocation")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(1);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == my_array);
        REQUIRE(isEmpty(alloc.allocate(1)));
    }

    SECTION("Larger allocation")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(alloc.getArena().size + 1);
        REQUIRE(isEmpty(arena));
        REQUIRE(!isEmpty(alloc.allocate(alloc.getArena().size)));
    }

    SECTION("Null alloc")
    {
        MemoryArenaAllocator alloc{};

        REQUIRE(isEmpty(alloc.allocate(1)));
    }

    SECTION("Reset")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(4);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == my_array);
        REQUIRE(isEmpty(alloc.allocate(1)));

        alloc.deallocateAll();
        arena = alloc.allocate(4);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == my_array);
    }
}

TEST_CASE("Memory Arena Allocator owns", "[memory_arena_allocator]")
{
    alignas(8) u8 my_array[8];
    MemoryArenaAllocator alloc(my_array);

    for (auto const & val : my_array)
    {
        REQUIRE(alloc.owns(&val));
    }

    REQUIRE(!alloc.owns(my_array + sizeof(my_array)));
}

#include <catch2/test_epilog.h>
