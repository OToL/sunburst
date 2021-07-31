#include <sb_core/bit_utility.h>

#include <extern_prolog.h>
#include <catch2/catch.hpp>

#include <map>
#include <iterator>
#include <sb_core/error/error.h>

#include <sb_core/memory/virtual_memory_backstore.h>

using namespace sb;

TEST_CASE("Power of 2 chek", "[bitwise]")
{
    SECTION("0 is a power of 2")
    {
        REQUIRE(isPowerOf2(0));
    }

    SECTION("Negative numbers are not a power of 2")
    {
        REQUIRE(!isPowerOf2(-2));
    }

    SECTION("Negative numbers are not a power of 2")
    {
        REQUIRE(!isPowerOf2(-2));
    }

    SECTION("Odd numbers are not a power of 2")
    {
        REQUIRE(!isPowerOf2(3));
    }

    SECTION("Power of 2 numbers are power of 2")
    {
        REQUIRE(isPowerOf2(8));
    }
}

TEST_CASE("Align a value", "[bitwise]")
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

TEST_CASE("Alignment check", "[bitwise]")
{
    REQUIRE(isAlignedTo(0U, 2U));
    REQUIRE(isAlignedTo(4U, 2U));
    REQUIRE(isAlignedTo(32U, 16U));

    REQUIRE(!isAlignedTo(3U, 2U));
}

TEST_CASE("Power of 2 rounding", "[bitwise]")
{
    SECTION("Rounding a power of 2 results to the same value")
    {
        REQUIRE(getNextPowerOf2(0U) == 0U);
        REQUIRE(getNextPowerOf2(1U) == 1U);
        REQUIRE(getNextPowerOf2(2U) == 2U);
        REQUIRE(getNextPowerOf2(4U) == 4U);
        REQUIRE(getNextPowerOf2(128U) == 128U);

        REQUIRE(getPreviousPowerOf2(0U) == 0U);
        REQUIRE(getPreviousPowerOf2(1U) == 1U);
        REQUIRE(getPreviousPowerOf2(2U) == 2U);
        REQUIRE(getPreviousPowerOf2(4U) == 4U);
        REQUIRE(getPreviousPowerOf2(128U) == 128U);
    }

    SECTION("Rounding to next power of 2")
    {
        REQUIRE(getNextPowerOf2(3U) == 4U);
        REQUIRE(getNextPowerOf2(123U) == 128U);
        REQUIRE(getNextPowerOf2(129U) == 256U);
    }

    SECTION("Rounding to previous power of 2")
    {
        REQUIRE(getPreviousPowerOf2(3U) == 2U);
        REQUIRE(getPreviousPowerOf2(123U) == 64U);
        REQUIRE(getPreviousPowerOf2(129U) == 128U);
    }
}

#include <extern_epilog.h>
