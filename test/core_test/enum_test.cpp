#include <core/enum.h>
#include <core/base.h>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

enum class TestMask : ui8
{
    VALUE0 = 1,
    VALUE1 = 2
}; 

TEST_CASE("Single value mask", "[enum]")
{
    REQUIRE(getEnumValue(TestMask::VALUE0) == makeEnumMask(TestMask::VALUE0));
}

TEST_CASE("Multi value mask", "[enum]")
{
    REQUIRE(0b11U == makeEnumMask(TestMask::VALUE0, TestMask::VALUE1));
}

TEST_CASE("Constexpr masks", "[enum]")
{
    STATIC_REQUIRE(getEnumValue(TestMask::VALUE0) == 1);
    STATIC_REQUIRE(makeEnumMask(TestMask::VALUE0, TestMask::VALUE1) == 0b11U);
}

#include <catch2/test_epilog.h>
