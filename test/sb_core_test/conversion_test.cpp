#include <sb_core/conversion.h>

#include <extern_prolog.h>
#include <catch2/catch.hpp>

using namespace sb;

TEST_CASE("String to buffer", "[conversion]")
{
    char test_buffer[255];

    auto byte_cnt = convertToString("test", test_buffer);

    REQUIRE(byte_cnt == 4U);
    REQUIRE(0 == strcmp("test", &test_buffer[0]));
}

TEST_CASE("Integral to string", "[conversion]")
{
    char test_buffer[255];

    // i8
    auto byte_cnt = convertToString((i8)1, test_buffer);
    REQUIRE(0 == strcmp("1", &test_buffer[0]));
    REQUIRE(byte_cnt == 1U);

    byte_cnt = convertToString((i8)-1, test_buffer);
    REQUIRE(0 == strcmp("-1", &test_buffer[0]));
    REQUIRE(byte_cnt == 2U);

    // i16
    byte_cnt = convertToString((i16)256, test_buffer);
    REQUIRE(0 == strcmp("256", &test_buffer[0]));
    REQUIRE(byte_cnt == 3U);

    byte_cnt = convertToString((i16)-256, test_buffer);
    REQUIRE(0 == strcmp("-256", &test_buffer[0]));
    REQUIRE(byte_cnt == 4U);

    // i32
    byte_cnt = convertToString((i32)65536, test_buffer);
    REQUIRE(0 == strcmp("65536", &test_buffer[0]));
    REQUIRE(byte_cnt == 5U);

    byte_cnt = convertToString((i32)-65536, test_buffer);
    REQUIRE(0 == strcmp("-65536", &test_buffer[0]));
    REQUIRE(byte_cnt == 6U);

    // i64
    byte_cnt = convertToString((i64)4294967296, test_buffer);
    REQUIRE(0 == strcmp("4294967296", &test_buffer[0]));
    REQUIRE(byte_cnt == 10U);

    byte_cnt = convertToString((i64)-4294967296, test_buffer);
    REQUIRE(0 == strcmp("-4294967296", &test_buffer[0]));
    REQUIRE(byte_cnt == 11U);
}

TEST_CASE("Unsigned integral to buffer", "[conversion]")
{
    char test_buffer[255];

    // u8
    auto byte_cnt = convertToString((u8)255, test_buffer);
    REQUIRE(0 == strcmp("255", &test_buffer[0]));
    REQUIRE(byte_cnt == 3U);

    // u16
    byte_cnt = convertToString((u16)65535, test_buffer);
    REQUIRE(0 == strcmp("65535", &test_buffer[0]));
    REQUIRE(byte_cnt == 5U);

    // u32
    byte_cnt = convertToString((u32)4294967295U, test_buffer);
    REQUIRE(0 == strcmp("4294967295", &test_buffer[0]));
    REQUIRE(byte_cnt == 10U);

    // u64
    byte_cnt = convertToString((u64)18446744073709551615ULL, test_buffer);
    REQUIRE(0 == strcmp("18446744073709551615", &test_buffer[0]));
    REQUIRE(byte_cnt == 20U);
}

#include <extern_epilog.h>
