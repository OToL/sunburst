#include <sb_core/bit.h>

#include <catch2/test_prolog.h>
#include <catch2/xcatch.hpp>

#include <map>
#include <iterator>
#include <sb_core/error.h>

using namespace sb;

TEST_CASE("Power of 2 chek", "[bitwise]")
{
    SECTION("0 is a power of 2")
    {
        std::map<size_t, size_t> my_map;

        auto res = my_map.emplace(10, 100);
        sbAssert(res.second);

        res = my_map.emplace(0, 5);
        sbAssert(res.second);

        [[maybe_unused]] auto hbound = my_map.upper_bound(5);

        size_t base_addr = 5;
        size_t my_size = 5;

        auto lower_addr_iter = hbound;

        if (hbound != my_map.begin())
        {
            lower_addr_iter--;

            if ((lower_addr_iter->first + lower_addr_iter->second) == base_addr)
            {
                lower_addr_iter->second += my_size;
            }
            else
            {
                lower_addr_iter = my_map.emplace_hint(lower_addr_iter, base_addr, my_size);
            }
        }
        else
        {
            lower_addr_iter = my_map.emplace(base_addr, my_size).first;
        }

        if ((lower_addr_iter->first + lower_addr_iter->second) == hbound->first)
        {
            lower_addr_iter->second += hbound->second;
            my_map.erase(hbound);
        }

        if ((base_addr + my_size) == hbound->first)
        {
            my_size += hbound->second;
            my_map.erase(hbound);
            my_map.emplace(base_addr, my_size);
        }

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

#include <catch2/test_epilog.h>
