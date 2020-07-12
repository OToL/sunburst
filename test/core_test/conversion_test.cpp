#include <core/conversion.h>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

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

    // si8
    auto byte_cnt = stringConvT((si8)1, test_buffer);
    REQUIRE(0 == strcmp("1", test_buffer));
    REQUIRE(byte_cnt == 1U);

    byte_cnt = stringConvT((si8)-1, test_buffer);
    REQUIRE(0 == strcmp("-1", test_buffer));
    REQUIRE(byte_cnt == 2U);

    // si16
    byte_cnt = stringConvT((si16)256, test_buffer);
    REQUIRE(0 == strcmp("256", test_buffer));
    REQUIRE(byte_cnt == 3U);

    byte_cnt = stringConvT((si16)-256, test_buffer);
    REQUIRE(0 == strcmp("-256", test_buffer));
    REQUIRE(byte_cnt == 4U);

    // si32
    byte_cnt = stringConvT((si32)65536, test_buffer);
    REQUIRE(0 == strcmp("65536", test_buffer));
    REQUIRE(byte_cnt == 5U);

    byte_cnt = stringConvT((si32)-65536, test_buffer);
    REQUIRE(0 == strcmp("-65536", test_buffer));
    REQUIRE(byte_cnt == 6U);

    // si64
    byte_cnt = stringConvT((si64)4294967296, test_buffer);
    REQUIRE(0 == strcmp("4294967296", test_buffer));
    REQUIRE(byte_cnt == 10U);

    byte_cnt = stringConvT((si64)-4294967296, test_buffer);
    REQUIRE(0 == strcmp("-4294967296", test_buffer));
    REQUIRE(byte_cnt == 11U);
}

TEST_CASE("Unsigned integral to buffer", "[conversion]")
{
    char test_buffer[255];

    // ui8
    auto byte_cnt = stringConvT((ui8)255, test_buffer);
    REQUIRE(0 == strcmp("255", test_buffer));
    REQUIRE(byte_cnt == 3U);

    // ui16
    byte_cnt = stringConvT((ui16)65535, test_buffer);
    REQUIRE(0 == strcmp("65535", test_buffer));
    REQUIRE(byte_cnt == 5U);

    // ui32
    byte_cnt = stringConvT((ui32)4294967295U, test_buffer);
    REQUIRE(0 == strcmp("4294967295", test_buffer));
    REQUIRE(byte_cnt == 10U);

    // ui64
    byte_cnt = stringConvT((ui64)18446744073709551615ULL, test_buffer);
    REQUIRE(0 == strcmp("18446744073709551615", test_buffer));
    REQUIRE(byte_cnt == 20U);
}

#include <catch2/test_epilog.h>
