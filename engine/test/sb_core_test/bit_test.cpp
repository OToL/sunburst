#include <sb_core/core.h>
#include <sb_core/bit.h>

#include <extern_prolog.h>
#include <catch2/catch.hpp>
#include <extern_epilog.h>

TEST_CASE("Count leading zeros with bit set", "[bit]")
{
    REQUIRE(29U == sb::countLeadingZero((sb::u32)0b100));
}

TEST_CASE("Count leading zeros no bit set", "[bit]")
{
    REQUIRE(32U == sb::countLeadingZero(0U));
}

