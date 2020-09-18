
#include <sb_core/io/file_hdl.h>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

TEST_CASE("FileHdl default ctor invalid", "[file_hdl]")
{
    FileHdl hdl;

    REQUIRE_FALSE(isValid(hdl));
}

TEST_CASE("FileHdl compare", "[file_hdl]")
{
    FileHdl hdl1 = {1};
    FileHdl hdl1b = {1};
    FileHdl hdl2 = {2};

    REQUIRE(hdl1 == hdl1b);
    REQUIRE(hdl1 != hdl2);
}

#include <catch2/test_epilog.h>