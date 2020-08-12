#include <sb_core/handle.h>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

struct TestHdl {};

using TestHandle = Handle<TestHdl, ui32>;

TEST_CASE("Handle default ctor", "[handle]")
{
    REQUIRE_FALSE(TestHandle{}.isValid());
}

TEST_CASE("Handle reset", "[handle]")
{
    TestHandle test_hdl(2);

    REQUIRE(test_hdl.isValid()); 

    test_hdl.reset();

    REQUIRE_FALSE(TestHandle{}.isValid());
}

TEST_CASE("Handle equality", "[handle]")
{
    TestHandle test_hdl(2);
    TestHandle test_hdl2(3);

    REQUIRE(test_hdl == test_hdl);
    REQUIRE_FALSE(test_hdl == test_hdl2);
}

#include <catch2/test_epilog.h>
