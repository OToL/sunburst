#include <core/string/string_format.h>
#include <libc++/iterator>

#include <gtest/gtest_common.h>

namespace sb::test {

TEST(STRING_FORMAT, NoArg)
{
    char test[255];

    usize const byte_cnt = stringFormat(test, "Test Format");

    EXPECT_STREQ("Test Format", test);
    EXPECT_EQ(byte_cnt, 11U);
}

TEST(STRING_FORMAT, EscapeArgToken)
{
    char test[255];

    {
        usize const byte_cnt = stringFormat(test, "{{Test {{ Format{{");

        EXPECT_STREQ("{Test { Format{", test);
        EXPECT_EQ(byte_cnt, 15U);
    }

    {
        usize const byte_cnt = stringFormat(test, "}}Test }} Format}}");

        EXPECT_STREQ("}Test } Format}", test);
        EXPECT_EQ(byte_cnt, 15U);
    }

    {
        usize const byte_cnt = stringFormat(test, "{{}}Test {{}} Format{{}}");

        EXPECT_STREQ("{}Test {} Format{}", test);
        EXPECT_EQ(byte_cnt, 18U);
    }
}

TEST(STRING_FORMAT, ArgAutoIncrement)
{
    char test[255];

    usize const byte_cnt = stringFormat(test, "{} {}", "Test", "Format");

    EXPECT_STREQ(test, "Test Format");
    EXPECT_EQ(byte_cnt, 11U);
}

TEST(STRING_FORMAT, ArgIndexed)
{
    char test[255];

    {
        usize const byte_cnt = stringFormat(test, "{0} {1}", "Test", "Format");

        EXPECT_STREQ(test, "Test Format");
        EXPECT_EQ(byte_cnt, 11U);
    }

    {
        usize const byte_cnt = stringFormat(test, "{1} {0}", "Test", "Format");

        EXPECT_STREQ(test, "Format Test");
        EXPECT_EQ(byte_cnt, 11U);
    }

    {
        usize const byte_cnt = stringFormat(test, "{1} {1}", "Test", "Format");

        EXPECT_STREQ(test, "Format Format");
        EXPECT_EQ(byte_cnt, 13U);
    }
}

TEST(STRING_FORMAT, ArgUI8)
{
    char test[255];

    ui8 const value = 10;
    usize const byte_cnt = stringFormat(test, "{}", value);

    EXPECT_STREQ(test, "10");
    EXPECT_EQ(byte_cnt, 2U);
}

TEST(STRING_FORMAT, ArgSI8)
{
    char test[255];

    si8 const value = -10;
    usize const byte_cnt = stringFormat(test, "{}", value);

    EXPECT_STREQ(test, "-10");
    EXPECT_EQ(byte_cnt, 3U);
}

TEST(STRING_FORMAT, ArgUI16)
{
    char test[255];

    ui16 const value = 512;
    usize const byte_cnt = stringFormat(test, "{}", value);

    EXPECT_STREQ(test, "512");
    EXPECT_EQ(byte_cnt, 3U);
}

TEST(STRING_FORMAT, ArgSI16)
{
    char test[255];

    si16 const value = -512;
    usize const byte_cnt = stringFormat(test, "{}", value);

    EXPECT_STREQ(test, "-512");
    EXPECT_EQ(byte_cnt, 4U);
}

TEST(STRING_FORMAT, ArgUI32)
{
    char test[255];

    ui32 const value = 16777215;
    usize const byte_cnt = stringFormat(test, "{}", value);

    EXPECT_STREQ(test, "16777215");
    EXPECT_EQ(byte_cnt, 8U);
}

TEST(STRING_FORMAT, ArgSI32)
{
    char test[255];

    si32 const value = -16777215;
    usize const byte_cnt = stringFormat(test, "{}", value);

    EXPECT_STREQ(test, "-16777215");
    EXPECT_EQ(byte_cnt, 9U);
}

TEST(STRING_FORMAT, ArgUI64)
{
    char test[255];

    ui64 const value = 1099511627775;
    usize const byte_cnt = stringFormat(test, "{}", value);

    EXPECT_STREQ(test, "1099511627775");
    EXPECT_EQ(byte_cnt, 13U);
}

TEST(STRING_FORMAT, ArgSI64)
{
    char test[255];

    si64 const value = -1099511627775;
    usize const byte_cnt = stringFormat(test, "{}", value);

    EXPECT_STREQ(test, "-1099511627775");
    EXPECT_EQ(byte_cnt, 14U);
}

}
