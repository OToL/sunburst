#include <core/enum.h>
#include <core/platform.h>

#include <gtest/gtest_common.h>

namespace sb::test {

enum class TestMask : ui8
{
    VALUE0 = 1,
    VALUE1 = 2
}; 

TEST(ENUM, MakeMaskSingleValue)
{
    EXPECT_EQ(getEnumValue(TestMask::VALUE0), makeEnumMask(TestMask::VALUE0));
}

TEST(ENUM, MakMaskMultiValue)
{
    EXPECT_EQ(0b11U, makeEnumMask(TestMask::VALUE0, TestMask::VALUE1));
}

// constexpr tests
static_assert(getEnumValue(TestMask::VALUE0) == 1, "Compile time getEnumValue broken");
static_assert(makeEnumMask(TestMask::VALUE0, TestMask::VALUE1) == 0b11U, "Compile time makeEnumMask broken");

}
