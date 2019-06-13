#include <core/hash/hash_str.h>

#include <gtest/gtest_common.h>

namespace sb { namespace test {

static constexpr char const * const HASH_STR_TEST_STRING = "Hello Hash";

TEST(HASH_STR, DefaultHash)
{
    HashStr const hash{};
    EXPECT_TRUE(hash.isNull());
}

TEST(HASH_STR, SetFromStr)
{
    HashStr const hash(HASH_STR_TEST_STRING);

    EXPECT_EQ(hash.m_value, HashStr::HashPolicy::compute(HASH_STR_TEST_STRING));
    EXPECT_FALSE(hash.isNull());
}

TEST(HASH_STR, SetFromValue)
{
    HashStr::ValueType const HASH_TEST_VALUE = HashStr::HashPolicy::compute(HASH_STR_TEST_STRING);
    HashStr const hash(HASH_STR_TEST_STRING);

    EXPECT_EQ(hash.m_value, HASH_TEST_VALUE);
    EXPECT_FALSE(hash.isNull());
}

TEST(HASH_STR, AssignHash)
{
    HashStr hash(HASH_STR_TEST_STRING);
    HashStr const hash2("Test");

    EXPECT_NE(hash, hash2);

    hash = hash2;

    EXPECT_EQ(hash, hash2);
}

/// constexpr tests
static_assert((HashStr::HashPolicy::compute(HASH_STR_TEST_STRING) == HashStr(HASH_STR_TEST_STRING).m_value), "HashStr constexpr construction failed");

}} // namespace sb::test
