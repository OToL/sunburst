#include <sb_core/core.h>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb; 

#define SB_CTF_TEST SB_ENABLED

TEST_CASE("Feature enabled", "[core]")
{
    STATIC_REQUIRE(sbIsEnabled(TEST));
}

#undef SB_CTF_TEST
#define SB_CTF_TEST SB_DISABLED

TEST_CASE("Compile Time Feature disabled", "[core]")
{
    STATIC_REQUIRE(!sbIsEnabled(TEST));
}

TEST_CASE("Check types size", "[core]")
{
    STATIC_REQUIRE(1 == sizeof(b8));
    STATIC_REQUIRE(4 == sizeof(b32));
    STATIC_REQUIRE(1 == sizeof(ui8));
    STATIC_REQUIRE(1 == sizeof(si8));
    STATIC_REQUIRE(2 == sizeof(ui16));
    STATIC_REQUIRE(2 == sizeof(si16));
    STATIC_REQUIRE(4 == sizeof(ui32));
    STATIC_REQUIRE(4 == sizeof(si32));
    STATIC_REQUIRE(8 == sizeof(ui64));
    STATIC_REQUIRE(8 == sizeof(si64));
    STATIC_REQUIRE(8 == sizeof(f64));
    STATIC_REQUIRE(4 == sizeof(f32));
    STATIC_REQUIRE(8 == sizeof(usize));
    STATIC_REQUIRE(8 == sizeof(uptr));
}

#include <catch2/test_epilog.h>
