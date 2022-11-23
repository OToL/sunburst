#include <sb_core/memory/memory.h>

#include <extern_prolog.h>
#include <catch2/catch.hpp>
#include <extern_epilog.h>

#include <map>
#include <iterator>
#include <sb_core/error.h>

#include <sb_core/memory/memory_system.h>

#include <iostream>

using namespace sb;

TEST_CASE("Align a value", "[memory]")
{
    SECTION("Basic alignment")
    {
        REQUIRE(2U == alignUp(1U, 2U));
        REQUIRE(4U == alignUp(1U, 4U));
        REQUIRE(8U == alignUp(1U, 8U));
        REQUIRE(16U == alignUp(1U, 16U));
    }

    SECTION("Aligning a value to itself results to the same value")
    {
        REQUIRE(2U == alignUp(2U, 2U));
        REQUIRE(4U == alignUp(4U, 4U));
        REQUIRE(8U == alignUp(8U, 8U));
        REQUIRE(16U == alignUp(16U, 16U));
    }
}

TEST_CASE("Alignment check", "[memory]")
{
    REQUIRE(isAlignedTo(0U, 2U));
    REQUIRE(isAlignedTo(4U, 2U));
    REQUIRE(isAlignedTo(32U, 16U));

    REQUIRE(!isAlignedTo(3U, 2U));
}

