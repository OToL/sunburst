#include <core/conversion.h>

#include <gtest/gtest_common.h>

#include <charconv>

namespace sb { namespace test {

TEST(CONVERSION, StringToSpan)
{
    char test_buffer[255];

    auto byte_cnt = stringCastT("test", test_buffer);

    EXPECT_EQ(byte_cnt, 4U);
    EXPECT_STREQ("test", test_buffer);
}

TEST(CONVERSION, SignedIntToSpan)
{
    char test_buffer[255];

    // si8
    auto byte_cnt = stringCastT((si8)1, test_buffer);
    EXPECT_STREQ("1", test_buffer);
    EXPECT_EQ(byte_cnt, 1U);

    byte_cnt = stringCastT((si8)-1, test_buffer);
    EXPECT_STREQ("-1", test_buffer);
    EXPECT_EQ(byte_cnt, 2U);

    // si16
    byte_cnt = stringCastT((si16)256, test_buffer);
    EXPECT_STREQ("256", test_buffer);
    EXPECT_EQ(byte_cnt, 3U);

    byte_cnt = stringCastT((si16)-256, test_buffer);
    EXPECT_STREQ("-256", test_buffer);
    EXPECT_EQ(byte_cnt, 4U);

    // si32
    byte_cnt = stringCastT((si32)65536, test_buffer);
    EXPECT_STREQ("65536", test_buffer);
    EXPECT_EQ(byte_cnt, 5U);

    byte_cnt = stringCastT((si32)-65536, test_buffer);
    EXPECT_STREQ("-65536", test_buffer);
    EXPECT_EQ(byte_cnt, 6U);

    // si64
    byte_cnt = stringCastT((si64)4294967296, test_buffer);
    EXPECT_STREQ("4294967296", test_buffer);
    EXPECT_EQ(byte_cnt, 10U);

    byte_cnt = stringCastT((si64)-4294967296, test_buffer);
    EXPECT_STREQ("-4294967296", test_buffer);
    EXPECT_EQ(byte_cnt, 11U);
}

TEST(CONVERSION, UnsignedIntToSpan)
{
    char test_buffer[255];

    // ui8
    auto byte_cnt = stringCastT((ui8)255, test_buffer);
    EXPECT_STREQ("255", test_buffer);
    EXPECT_EQ(byte_cnt, 3U);

    // ui16
    byte_cnt = stringCastT((ui16)65535, test_buffer);
    EXPECT_STREQ("65535", test_buffer);
    EXPECT_EQ(byte_cnt, 5U);

    // ui32
    byte_cnt = stringCastT((ui32)4294967295U, test_buffer);
    EXPECT_STREQ("4294967295", test_buffer);
    EXPECT_EQ(byte_cnt, 10U);

    // ui64
    byte_cnt = stringCastT((ui64)18446744073709551615ULL, test_buffer);
    EXPECT_STREQ("18446744073709551615", test_buffer);
    EXPECT_EQ(byte_cnt, 20U);
}

}} // namespace sb::test
