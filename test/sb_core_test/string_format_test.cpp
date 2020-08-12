#include <sb_core/string/string_format.h>
#include <sb_std/iterator>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

TEST_CASE("String format without argument", "[string_format]")
{
    char test[255];

    usize const byte_cnt = stringFormat(test, "Test Format");

    REQUIRE_THAT(test, Catch::Equals("Test Format"));
    REQUIRE(byte_cnt == 11U);
}

TEST_CASE("String format escape characters", "[string_format]")
{
    char test[255];

    SECTION("{{ escape")
    {
        usize const byte_cnt = stringFormat(test, "{{Test {{ Format{{");

        REQUIRE_THAT(test, Catch::Equals("{Test { Format{"));
        REQUIRE(byte_cnt == 15U);
    }

    SECTION("}} escape")
    {
        usize const byte_cnt = stringFormat(test, "}}Test }} Format}}");

        REQUIRE_THAT(test, Catch::Equals("}Test } Format}"));
        REQUIRE(byte_cnt == 15U);
    }

    SECTION("{{ and }} escape")
    {
        usize const byte_cnt = stringFormat(test, "{{}}Test {{}} Format{{}}");

        REQUIRE_THAT(test, Catch::Equals("{}Test {} Format{}"));
        REQUIRE(byte_cnt == 18U);
    }
}

TEST_CASE("String format auto incremented argument", "[string_format]")
{
    char test[255];

    usize const byte_cnt = stringFormat(test, "{} {}", "Test", "Format");

    REQUIRE_THAT(test, Catch::Equals("Test Format"));
    REQUIRE(byte_cnt == 11U);
}

TEST_CASE("String format indexed argument", "[string_format]")
{
    char test[255];

    SECTION("Sequencial arguments")
    {
        usize const byte_cnt = stringFormat(test, "{0} {1}", "Test", "Format");

        REQUIRE_THAT(test, Catch::Equals("Test Format"));
        REQUIRE(byte_cnt == 11U);
    }

    SECTION("Inverted arguments")
    {
        usize const byte_cnt = stringFormat(test, "{1} {0}", "Test", "Format");

        REQUIRE_THAT(test, Catch::Equals("Format Test"));
        REQUIRE(byte_cnt == 11U);
    }

    SECTION("Repeated argument")
    {
        usize const byte_cnt = stringFormat(test, "{1} {1}", "Test", "Format");

        REQUIRE_THAT(test, Catch::Equals("Format Format"));
        REQUIRE(byte_cnt == 13U);
    }
}

TEST_CASE("String format numeric arguments", "[string_format]")
{
    char test[255];

    SECTION("ui8 argument")
    {
        ui8 const value = 10;
        usize const byte_cnt = stringFormat(test, "{}", value);

        REQUIRE_THAT(test, Catch::Equals("10"));
        REQUIRE(byte_cnt == 2U);
    }

    SECTION("si8 argument")
    {
        si8 const value = -10;
        usize const byte_cnt = stringFormat(test, "{}", value);

        REQUIRE_THAT(test, Catch::Equals("-10"));
        REQUIRE(byte_cnt== 3U);
    }

    SECTION("ui16 argument")
    {
        ui16 const value = 512;
        usize const byte_cnt = stringFormat(test, "{}", value);

        REQUIRE_THAT(test, Catch::Equals("512"));
        REQUIRE(byte_cnt == 3U);
    }

    SECTION("si16 argument")
    {
        si16 const value = -512;
        usize const byte_cnt = stringFormat(test, "{}", value);

        REQUIRE_THAT(test, Catch::Equals("-512"));
        REQUIRE(byte_cnt == 4U);
    }

    SECTION("ui32 argument")
    {
        ui32 const value = 16777215U;
        usize const byte_cnt = stringFormat(test, "{}", value);

        REQUIRE_THAT(test, Catch::Equals("16777215"));
        REQUIRE(byte_cnt == 8U);
    }

    SECTION("si32 argument")
    {
        si32 const value = -16777215;
        usize const byte_cnt = stringFormat(test, "{}", value);

        REQUIRE_THAT(test, Catch::Equals("-16777215"));
        REQUIRE(byte_cnt == 9U);
    }

    SECTION("ui64 argument")
    {
        ui64 const value = 1099511627775;
        usize const byte_cnt = stringFormat(test, "{}", value);

        REQUIRE_THAT(test, Catch::Equals("1099511627775"));
        REQUIRE(byte_cnt == 13U);
    }

    SECTION("si64 argument")
    {
        si64 const value = -1099511627775;
        usize const byte_cnt = stringFormat(test, "{}", value);

        REQUIRE_THAT(test, Catch::Equals("-1099511627775"));
        REQUIRE(byte_cnt == 14U);
    }
}

#include <catch2/test_epilog.h>
