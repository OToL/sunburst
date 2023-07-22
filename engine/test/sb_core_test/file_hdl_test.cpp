#include <sb_core/io/file_hdl.h>

#include <extern_prolog.h>
#include <catch2/catch.hpp>
#include <extern_epilog.h>

using namespace sb;

TEST_CASE("File default ctor invalid", "[file_hdl]")
{
    FileHdl hdl = {};

    REQUIRE_FALSE(hdl.isValid());
}

TEST_CASE("File compare", "[file_hdl]")
{
    FileHdl hdl1 = {1};
    FileHdl hdl1b = {1};
    FileHdl hdl2 = {2};

    REQUIRE(hdl1 == hdl1b);
    REQUIRE(hdl1 != hdl2);
}
