
#include <sb_core/io/file.h>

#include <sb_core/extern_prolog.h>
#include <catch2/catch.hpp>

using namespace sb;

TEST_CASE("File default ctor invalid", "[file]")
{
    File hdl = {};

    REQUIRE_FALSE(file_isValid(hdl));
}

TEST_CASE("File compare", "[file]")
{
    File hdl1 = {1};
    File hdl1b = {1};
    File hdl2 = {2};

    REQUIRE(hdl1 == hdl1b);
    REQUIRE(hdl1 != hdl2);
}

#include <sb_core/extern_epilog.h>
