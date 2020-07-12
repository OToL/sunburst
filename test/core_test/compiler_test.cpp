#include <core/compiler.h>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb; 

#define SB_CTF_TEST SB_ENABLED

TEST_CASE("Feature enabled", "[compiler]")
{
    STATIC_REQUIRE(sbIsEnabled(TEST));
}

#undef SB_CTF_TEST
#define SB_CTF_TEST SB_DISABLED

TEST_CASE("Compile Time Feature disabled", "[compiler]")
{
    STATIC_REQUIRE(!sbIsEnabled(TEST));
}

#include <catch2/test_epilog.h>
