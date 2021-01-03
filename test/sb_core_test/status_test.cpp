#include <sb_core/error/status.h>

#include <sb_core/extern_prolog.h>
#include <catch2/catch.hpp>

using namespace sb;

TEST_CASE("Default Status is ok", "[status]")
{
    Status status = {};

    REQUIRE(status_isOk(status));
}

#include <sb_core/extern_epilog.h>
