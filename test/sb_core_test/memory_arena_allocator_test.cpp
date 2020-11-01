#include <sb_core/memory/allocator/memory_arena_allocator.h>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

TEST_CASE("Memory Arena Allocator empty", "[memory_arena_allocator]")
{
    MemoryArenaAllocator alloc;

    REQUIRE(alloc.allocate(1).isEmpty());
    REQUIRE(alloc.allocate(1, ALIGNMENT_8B).isEmpty());
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
        REQUIRE(alloc.allocate(1).isEmpty());
    }
    
    SECTION("Smaller allocation")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(4, ALIGNMENT_8B);
        REQUIRE(arena.size == sizeof(my_array));    
        REQUIRE(arena.data == my_array);
        REQUIRE(alloc.allocate(1).isEmpty());
    }

    SECTION("Larger allocation")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(alloc.getArena().size + 1, ALIGNMENT_8B);
        REQUIRE(arena.isEmpty());
        REQUIRE(!alloc.allocate(alloc.getArena().size).isEmpty());
    }

    SECTION("Alignment overflow")
    {
        MemoryArenaAllocator alloc(MemoryArena{(void *)8, 8});

        MemoryArena arena = alloc.allocate(8, ALIGNMENT_16B);
        REQUIRE(arena.isEmpty());

        arena = alloc.allocate(8, ALIGNMENT_8B);
        REQUIRE(!arena.isEmpty());
    }

    SECTION("Reset")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(4, ALIGNMENT_8B);
        REQUIRE(arena.size == sizeof(my_array));    
        REQUIRE(arena.data == my_array);
        REQUIRE(alloc.allocate(1).isEmpty());

        alloc.deallocateAll();
        arena = alloc.allocate(4, ALIGNMENT_8B);
        REQUIRE(arena.size == sizeof(my_array));    
        REQUIRE(arena.data == my_array);
    }

    SECTION("Null alloc")
    {
        MemoryArenaAllocator alloc{};

        REQUIRE(alloc.allocate(1, ALIGNMENT_1B).isEmpty());
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
        REQUIRE(alloc.allocate(1).isEmpty());
    }

    SECTION("Smaller allocation")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(1);
        REQUIRE(arena.size == sizeof(my_array));    
        REQUIRE(arena.data == my_array);
        REQUIRE(alloc.allocate(1).isEmpty());
    }

    SECTION("Larger allocation")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(alloc.getArena().size + 1);
        REQUIRE(arena.isEmpty());
        REQUIRE(!alloc.allocate(alloc.getArena().size).isEmpty());
    }

    SECTION("Null alloc")
    {
        MemoryArenaAllocator alloc{};

        REQUIRE(alloc.allocate(1).isEmpty());
    }

    SECTION("Reset")
    {
        MemoryArenaAllocator alloc(my_array);

        MemoryArena arena = alloc.allocate(4);
        REQUIRE(arena.size == sizeof(my_array));    
        REQUIRE(arena.data == my_array);
        REQUIRE(alloc.allocate(1).isEmpty());

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
