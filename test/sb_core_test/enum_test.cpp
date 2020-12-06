#include <sb_core/enum.h>
#include <sb_core/core.h>

#include <catch2/test_prolog.h>
#include <catch2/xcatch.hpp>

using namespace sb;

enum class TestMask : u8
{
    VALUE0,
    VALUE1
};

TEST_CASE("Single value mask", "[enum]")
{
    REQUIRE(0b01U == makeEnumMaskValue(TestMask::VALUE0));
}

TEST_CASE("Multi value mask", "[enum]")
{
    REQUIRE(makeEnumMaskValue(TestMask::VALUE0, TestMask::VALUE1) == 0b11U);
}

TEST_CASE("Constexpr masks", "[enum]")
{
    STATIC_REQUIRE(makeEnumMaskValue(TestMask::VALUE0, TestMask::VALUE1) == 0b11U);
    STATIC_REQUIRE(makeEnumMaskValue(TestMask::VALUE1) == 0b10U);
    STATIC_REQUIRE(makeEnumMaskValue(TestMask::VALUE0) == 0b01U);
}

TEST_CASE("Enum mask", "[enum]")
{
    STATIC_REQUIRE(
        enummask_hasValues(makeEnumMask(TestMask::VALUE0, TestMask::VALUE1), TestMask::VALUE0, TestMask::VALUE1));
    STATIC_REQUIRE(
        enummask_hasAnyValue(makeEnumMask(TestMask::VALUE0, TestMask::VALUE1), TestMask::VALUE0, TestMask::VALUE1));
    STATIC_REQUIRE(enummask_hasAnyValue(makeEnumMask(TestMask::VALUE0, TestMask::VALUE1), TestMask::VALUE1));
    STATIC_REQUIRE(enummask_hasAnyValue(makeEnumMask(TestMask::VALUE0, TestMask::VALUE1), TestMask::VALUE0));
}

#include <catch2/test_epilog.h>
