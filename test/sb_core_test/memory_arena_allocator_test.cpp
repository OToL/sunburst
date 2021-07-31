#include <sb_core/memory/allocator/memory_arena_allocator.h>

#include <extern_prolog.h>
#include <catch2/catch.hpp>

using namespace sb;

TEST_CASE("Memory Arena Allocator empty", "[memory_arena_allocator]")
{
    MemoryArenaAllocator alloc;

    REQUIRE(memory_arena::isEmpty(alloc.allocate(1)));
    REQUIRE(memory_arena::isEmpty(alloc.allocate(ALIGNMENT_8B, 1)));
}

TEST_CASE("Memory Arena Allocator init", "[memory_arena_allocator]")
{
    SECTION("Local buffer init")
    {
        u8 my_array[8];
        MemoryArenaAllocator alloc(my_array);

        REQUIRE(alloc.getArena().size == sizeof(my_array));
        REQUIRE(alloc.getArena().data == &my_array[0]);
    }

    SECTION("Arena init")
    {
        MemoryArena arena = {reinterpret_cast<void *>(8), 8};
        MemoryArenaAllocator alloc(arena);

        REQUIRE(alloc.getArena().size == 8U);
        REQUIRE(alloc.getArena().data == reinterpret_cast<void *>(8));
    }
}

TEST_CASE("Memory Arena Allocator aligned allocate", "[memory_arena_allocator]")
{
    alignas(8) u8 my_array[8];

    SECTION("Perfect fit")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(ALIGNMENT_8B, alloc.getArena().size);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == &my_array[0]);
        REQUIRE(memory_arena::isEmpty(alloc.allocate(1)));
    }

    SECTION("Smaller allocation")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(ALIGNMENT_8B, 4);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == &my_array[0]);
        REQUIRE(memory_arena::isEmpty(alloc.allocate(1)));
    }

    SECTION("Larger allocation")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(ALIGNMENT_8B, alloc.getArena().size + 1);
        REQUIRE(memory_arena::isEmpty(arena));
        REQUIRE(!memory_arena::isEmpty(alloc.allocate(alloc.getArena().size)));
    }

    SECTION("Alignment overflow")
    {
        MemoryArenaAllocator alloc(MemoryArena{reinterpret_cast<void *>(8), 8});

        MemoryArena arena = alloc.allocate(ALIGNMENT_16B, 8);
        REQUIRE(memory_arena::isEmpty(arena));

        arena = alloc.allocate(ALIGNMENT_8B, 8);
        REQUIRE(!memory_arena::isEmpty(arena));
    }

    SECTION("Reset")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(ALIGNMENT_8B, 4);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == &my_array[0]);
        REQUIRE(memory_arena::isEmpty(alloc.allocate(1)));

        alloc.deallocateAll();
        arena = alloc.allocate(ALIGNMENT_8B, 4);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == &my_array[0]);
    }

    SECTION("Null alloc")
    {
        MemoryArenaAllocator alloc{};

        REQUIRE(memory_arena::isEmpty(alloc.allocate(ALIGNMENT_1B, 1)));
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
        REQUIRE(arena.data == &my_array[0]);
        REQUIRE(memory_arena::isEmpty(alloc.allocate(1)));
    }

    SECTION("Smaller allocation")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(1);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == &my_array[0]);
        REQUIRE(memory_arena::isEmpty(alloc.allocate(1)));
    }

    SECTION("Larger allocation")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(alloc.getArena().size + 1);
        REQUIRE(memory_arena::isEmpty(arena));
        REQUIRE(!memory_arena::isEmpty(alloc.allocate(alloc.getArena().size)));
    }

    SECTION("Null alloc")
    {
        MemoryArenaAllocator alloc{};

        REQUIRE(memory_arena::isEmpty(alloc.allocate(1)));
    }

    SECTION("Reset")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(4);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == &my_array[0]);
        REQUIRE(memory_arena::isEmpty(alloc.allocate(1)));

        alloc.deallocateAll();
        arena = alloc.allocate(4);
        REQUIRE(arena.size == sizeof(my_array));
        REQUIRE(arena.data == &my_array[0]);
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

    REQUIRE(!alloc.owns(&my_array[0] + sizeof(my_array)));
}

#include <extern_epilog.h>
