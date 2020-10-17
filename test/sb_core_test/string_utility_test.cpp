#include <sb_core/string/string_utility.h>

#include <sb_std/iterator>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

static char STRCPYT_TEST_SRC[] = "Hello World";
static const usize STRCPYT_TEST_LEN = sbstd::size(STRCPYT_TEST_SRC) - 1;
static const usize STRCPYT_TEST_CAPACITY = sbstd::size(STRCPYT_TEST_SRC);

TEST_CASE("strCpyT", "[string_utility]")
{
    SECTION("StrCpyT fit")
    {
        char buffer[20];
        memset(&buffer[0], 0xFF, sbstd::size(buffer));

        auto const copy_cnt = strCpyT(buffer, STRCPYT_TEST_SRC);

        REQUIRE(copy_cnt == STRCPYT_TEST_LEN);
        REQUIRE('\xFF' == buffer[STRCPYT_TEST_LEN + 1]);
    }

    SECTION("Perfect fit")
    {
        char buffer[STRCPYT_TEST_CAPACITY];

        auto const copy_cnt = strCpyT(buffer, STRCPYT_TEST_SRC);

        REQUIRE(copy_cnt == STRCPYT_TEST_LEN);
    }

    SECTION("Overflow")
    {
        char buffer[STRCPYT_TEST_CAPACITY - 1];

        usize const copy_cnt = strCpyT(buffer, STRCPYT_TEST_SRC);

        REQUIRE(copy_cnt == STRCPYT_TEST_LEN - 1);
        REQUIRE_THAT(buffer, Catch::Equals("Hello Worl"));
    }
}

TEST_CASE("strStartsWith", "[string_utility]")
{
    SECTION("Exact match")
    {
        REQUIRE(strStartsWith("Hello", "Hello"));
    }

    SECTION("Exact sub-match")
    {
        REQUIRE(strStartsWith("Hello World", "Hello"));
    }

    SECTION("No match")
    {
        REQUIRE_FALSE(strStartsWith("Hello", "Helli"));
    }
}

#include <catch2/test_epilog.h>
