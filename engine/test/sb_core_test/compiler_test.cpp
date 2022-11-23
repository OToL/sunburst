#include <sb_core/compiler.h>

#include <extern_prolog.h>
#include <catch2/catch.hpp>
#include <extern_epilog.h>

#include <sb_core/error.h>
#include <sb_core/extern/intrinsics.h>

using namespace sb;

TEST_CASE("Count leading zeros with bit set", "[compiler]")
{
    REQUIRE(29U == sb_lzcnt((sb::u32)0b100));
}

TEST_CASE("Count leading zeros no bit set", "[compiler]")
{
    REQUIRE(32U == sb_lzcnt(0U));
}

