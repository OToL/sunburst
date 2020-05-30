#include <core/bitwise.h>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

namespace sb {

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
        REQUIRE(nextPowerOf2(0U) == 0U);
        REQUIRE(nextPowerOf2(1U) == 1U);
        REQUIRE(nextPowerOf2(2U) == 2U);
        REQUIRE(nextPowerOf2(4U) == 4U);
        REQUIRE(nextPowerOf2(128U) == 128U);
     
        REQUIRE(prevPowerOf2(0U) == 0U);
        REQUIRE(prevPowerOf2(1U) == 1U);
        REQUIRE(prevPowerOf2(2U) == 2U);
        REQUIRE(prevPowerOf2(4U) == 4U);
        REQUIRE(prevPowerOf2(128U) == 128U);
    }

    SECTION("Rounding to next power of 2")
    {
        REQUIRE(nextPowerOf2(3U) == 4U);
        REQUIRE(nextPowerOf2(123U) == 128U);
        REQUIRE(nextPowerOf2(129U) == 256U);
    }

    SECTION("Rounding to previous power of 2")
    {
        REQUIRE(prevPowerOf2(3U) == 2U);
        REQUIRE(prevPowerOf2(123U) == 64U);
        REQUIRE(prevPowerOf2(129U) == 128U);
    } 
}

}

#include <catch2/test_epilog.h>