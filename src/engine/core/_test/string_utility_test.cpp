#include <core/utility.h>

#include <libc++/iterator>

#include <gtest/gtest_common.h>

namespace sb { namespace test {

static char STRCPYT_TEST_SRC[] = "Hello World";
static const usize STRCPYT_TEST_LEN = wstd::size(STRCPYT_TEST_SRC) - 1;
static const usize STRCPYT_TEST_CAPACITY = wstd::size(STRCPYT_TEST_SRC);

TEST(STRING_UTILITY, StrCpyTFit)
{
    char buffer[20];
    memset(&buffer[0], 0xFF, wstd::size(buffer));

    usize const copy_cnt = strCpyT(buffer, STRCPYT_TEST_SRC);

    EXPECT_EQ(copy_cnt, STRCPYT_TEST_LEN);
    EXPECT_EQ('\xFF', buffer[STRCPYT_TEST_LEN + 1]);
}

TEST(STRING_UTILITY, StrCpyTPerfectFit)
{
    char buffer[STRCPYT_TEST_CAPACITY];

    usize const copy_cnt = strCpyT(buffer, STRCPYT_TEST_SRC);

    EXPECT_EQ(copy_cnt, STRCPYT_TEST_LEN);
}

TEST(STRING_UTILITY, StrCpyTOverflow)
{
    char buffer[STRCPYT_TEST_CAPACITY - 1];

    usize const copy_cnt = strCpyT(buffer, STRCPYT_TEST_SRC);

    EXPECT_EQ(copy_cnt, STRCPYT_TEST_LEN - 1);
    EXPECT_STREQ(buffer, "Hello Worl");
}

TEST(STRING_UTILITY, strStartWithExactMatch)
{
    EXPECT_TRUE(strStartWith("Hello", "Hello"));
}

TEST(STRING_UTILITY, strStartWithExactSubMatch)
{
    EXPECT_TRUE(strStartWith("Hello World", "Hello"));
}

TEST(STRING_UTILITY, strStartWithNoMatch)
{
    EXPECT_FALSE(strStartWith("Hello", "Helli"));
}

}} // namespace sb::test
