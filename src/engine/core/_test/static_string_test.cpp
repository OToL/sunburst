#include <core/string/static_string.h>

#include <gtest/allocator_stats.h>
#include <gtest/gtest_common.h>

namespace sb { namespace test {

TEST(STATIC_STRING, DefaultCtor)
{
    StaticString<10> testStr;

    EXPECT_TRUE(testStr.empty());
    EXPECT_EQ(0U, testStr.length());
    EXPECT_STREQ(testStr.c_str(), "");
}

TEST(STATIC_STRING, CtorRawStr)
{
    StaticString<10> testStr("Hello");

    EXPECT_FALSE(testStr.empty());
    EXPECT_EQ(5U, testStr.length());
    EXPECT_STREQ(testStr.c_str(), "Hello");
}

TEST(STATIC_STRING, CtorStaticString)
{
    StaticString<10> testStr("Hello");
    StaticString<10> testStr2(testStr);

    EXPECT_FALSE(testStr2.empty());
    EXPECT_EQ(5U, testStr2.length());
    EXPECT_STREQ(testStr2.c_str(), "Hello");
}

TEST(STATIC_STRING, AssignRawStr)
{
    StaticString<10> testStr;
    EXPECT_TRUE(testStr.empty());

    testStr = "Hello";
    EXPECT_FALSE(testStr.empty());
    EXPECT_EQ(5U, testStr.length());
    EXPECT_STREQ(testStr.c_str(), "Hello");
}

TEST(STATIC_STRING, AssignStaticString)
{
    StaticString<10> testStr("Hello");
    StaticString<10> testStr2;

    EXPECT_FALSE(testStr.empty());
    EXPECT_TRUE(testStr2.empty());

    testStr2 = testStr;

    EXPECT_FALSE(testStr2.empty());
    EXPECT_EQ(5U, testStr2.length());
    EXPECT_STREQ(testStr2.c_str(), "Hello");
}

TEST(STATIC_STRING, AssignRawStrExceed)
{
    StaticString<3> testStr;
    EXPECT_TRUE(testStr.empty());

    testStr = "Hello";
    EXPECT_FALSE(testStr.empty());
    EXPECT_EQ(2U, testStr.length());
    EXPECT_STREQ(testStr.c_str(), "He");
}

TEST(STATIC_STRING, AssignExceed)
{
    StaticString<10> testStr("Hello");
    StaticString<3> testStr2;

    EXPECT_FALSE(testStr.empty());
    EXPECT_TRUE(testStr2.empty());

    testStr2 = testStr;

    EXPECT_FALSE(testStr2.empty());
    EXPECT_EQ(2U, testStr2.length());
    EXPECT_STREQ(testStr2.c_str(), "He");
}

TEST(STATIC_STRING, ConcatRawStr)
{
    StaticString<20> testStr("Hello ");
    testStr += "world";

    EXPECT_STREQ(testStr.c_str(), "Hello world");
}

TEST(STATIC_STRING, ConcatRawStrExceed)
{
    StaticString<9> testStr("Hello ");
    testStr += "world";

    EXPECT_STREQ(testStr.c_str(), "Hello wo");
}

TEST(STATIC_STRING, ConcatStaticString)
{
    StaticString<20> testStr("Hello ");
    StaticString<20> testStr2("world");
    testStr += testStr2;

    EXPECT_STREQ(testStr.c_str(), "Hello world");
}

TEST(STATIC_STRING, ConcatStaticStringExceed)
{
    StaticString<9> testStr("Hello ");
    StaticString<20> testStr2("world");
    testStr += testStr2;

    EXPECT_STREQ(testStr.c_str(), "Hello wo");
}

TEST(STATIC_STRING, Back)
{
    StaticString<10> testStr("Hello");

    EXPECT_EQ(testStr.back(), 'o');
}

TEST(STATIC_STRING, PushBack)
{
    StaticString<10> testStr("/hello");

    EXPECT_EQ(testStr.back(), 'o');
    EXPECT_EQ(testStr.length(), 6ULL);

    testStr.push_back('/');

    EXPECT_EQ(testStr.back(), '/');
    EXPECT_EQ(testStr.length(), 7ULL);
}

}} // namespace sb::test
