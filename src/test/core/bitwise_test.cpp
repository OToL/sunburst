#include <core/bitwise.h>

#include <gtest/gtest_common.h>

namespace sb::test {

TEST(MATH, IsPowerOf20)
{
    EXPECT_TRUE(isPowerOf2(0));
}

TEST(MATH, IsPowerOf2Neg)
{
    EXPECT_FALSE(isPowerOf2(-2));
}

TEST(MATH, IsPowerOf2False)
{
    EXPECT_FALSE(isPowerOf2(3));
}

TEST(MATH, IsPowerOf2True)
{
    EXPECT_TRUE(isPowerOf2(8));
}

TEST(MATH, AlignUp)
{
    EXPECT_EQ(2U, alignUp(1U, 2U));
    EXPECT_EQ(4U, alignUp(1U, 4U));
    EXPECT_EQ(8U, alignUp(1U, 8U));
    EXPECT_EQ(16U, alignUp(1U, 16U));
}

TEST(MATH, AlignUpExact)
{
    EXPECT_EQ(2U, alignUp(2U, 2U));
    EXPECT_EQ(4U, alignUp(4U, 4U));
    EXPECT_EQ(8U, alignUp(8U, 8U));
    EXPECT_EQ(16U, alignUp(16U, 16U));
}

TEST(MATH, AlignedTo)
{
    EXPECT_TRUE(isAlignedTo(0U, 2U));
    EXPECT_TRUE(isAlignedTo(4U, 2U));
    EXPECT_TRUE(isAlignedTo(32U, 16U));
}

TEST(MATH, NotAlignedTo)
{
    EXPECT_FALSE(isAlignedTo(3U, 2U));
}

TEST(MATH, NextPowerOf2)
{
    EXPECT_EQ(nextPowerOf2(0U), 0U);
    EXPECT_EQ(nextPowerOf2(1U), 1U);
    EXPECT_EQ(nextPowerOf2(2U), 2U);
    EXPECT_EQ(nextPowerOf2(3U), 4U);
    EXPECT_EQ(nextPowerOf2(4U), 4U);
    EXPECT_EQ(nextPowerOf2(123U), 128U);
    EXPECT_EQ(nextPowerOf2(128U), 128U);
    EXPECT_EQ(nextPowerOf2(129U), 256U);
};

TEST(MATH, PrevtPowerOf2)
{
    EXPECT_EQ(prevPowerOf2(0U), 0U);
    EXPECT_EQ(prevPowerOf2(1U), 1U);
    EXPECT_EQ(prevPowerOf2(2U), 2U);
    EXPECT_EQ(prevPowerOf2(3U), 2U);
    EXPECT_EQ(prevPowerOf2(4U), 4U);
    EXPECT_EQ(prevPowerOf2(123U), 64U);
    EXPECT_EQ(prevPowerOf2(128U), 128U);
    EXPECT_EQ(prevPowerOf2(129U), 128U);
}

}
