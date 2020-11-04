#include <sb_core/conversion.h>

#include <catch2/test_prolog.h>
#include <catch2/xcatch.hpp>

using namespace sb;

TEST_CASE("String to buffer", "[conversion]")
{
    char test_buffer[255];

    auto byte_cnt = stringConvT("test", test_buffer);

    REQUIRE(byte_cnt == 4U);
    REQUIRE(0 == strcmp("test", test_buffer));
}

TEST_CASE("Integral to string", "[conversion]")
{
    char test_buffer[255];

    // s8
    auto byte_cnt = stringConvT((s8)1, test_buffer);
    REQUIRE(0 == strcmp("1", test_buffer));
    REQUIRE(byte_cnt == 1U);

    byte_cnt = stringConvT((s8)-1, test_buffer);
    REQUIRE(0 == strcmp("-1", test_buffer));
    REQUIRE(byte_cnt == 2U);

    // s16
    byte_cnt = stringConvT((s16)256, test_buffer);
    REQUIRE(0 == strcmp("256", test_buffer));
    REQUIRE(byte_cnt == 3U);

    byte_cnt = stringConvT((s16)-256, test_buffer);
    REQUIRE(0 == strcmp("-256", test_buffer));
    REQUIRE(byte_cnt == 4U);

    // s32
    byte_cnt = stringConvT((s32)65536, test_buffer);
    REQUIRE(0 == strcmp("65536", test_buffer));
    REQUIRE(byte_cnt == 5U);

    byte_cnt = stringConvT((s32)-65536, test_buffer);
    REQUIRE(0 == strcmp("-65536", test_buffer));
    REQUIRE(byte_cnt == 6U);

    // s64
    byte_cnt = stringConvT((s64)4294967296, test_buffer);
    REQUIRE(0 == strcmp("4294967296", test_buffer));
    REQUIRE(byte_cnt == 10U);

    byte_cnt = stringConvT((s64)-4294967296, test_buffer);
    REQUIRE(0 == strcmp("-4294967296", test_buffer));
    REQUIRE(byte_cnt == 11U);
}

TEST_CASE("Unsigned integral to buffer", "[conversion]")
{
    char test_buffer[255];

    // u8
    auto byte_cnt = stringConvT((u8)255, test_buffer);
    REQUIRE(0 == strcmp("255", test_buffer));
    REQUIRE(byte_cnt == 3U);

    // u16
    byte_cnt = stringConvT((u16)65535, test_buffer);
    REQUIRE(0 == strcmp("65535", test_buffer));
    REQUIRE(byte_cnt == 5U);

    // u32
    byte_cnt = stringConvT((u32)4294967295U, test_buffer);
    REQUIRE(0 == strcmp("4294967295", test_buffer));
    REQUIRE(byte_cnt == 10U);

    // u64
    byte_cnt = stringConvT((u64)18446744073709551615ULL, test_buffer);
    REQUIRE(0 == strcmp("18446744073709551615", test_buffer));
    REQUIRE(byte_cnt == 20U);
}

#include <catch2/test_epilog.h>
