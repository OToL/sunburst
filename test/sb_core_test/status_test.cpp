#include <sb_core/error/status.h>

#include <catch2/test_prolog.h>
#include <catch2/xcatch.hpp>

using namespace sb;

TEST_CASE("Default Status is ok", "[status]")
{
    Status status = {};

    REQUIRE(status_isOk(status));
}

#include <catch2/test_epilog.h>
