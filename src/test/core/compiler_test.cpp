#include <core/platform.h>

#include <gtest/gtest_common.h>

namespace sb { namespace test {

TEST(COMPILER, ConcatStr)
{
    char const * const pConcat = sbConcatStr("Hello ", "world");

    EXPECT_STREQ("Hello world", pConcat);
}

class ConcatIDTest
{
};

TEST(COMPILER, IdToStr)
{
    EXPECT_STREQ("world", sbIdToStr(world));
}

#define SB_CTF_TEST SB_CTF_ENABLED

TEST(COMPILER, CTFEnabled)
{
#if sbCTFIsEnabled(TEST)
    SUCCEED();
#else
    FAIL();
#endif
}

#undef SB_CTF_TEST
#define SB_CTF_TEST SB_CTF_DISABLED

TEST(COMPILER, CTFDisabled)
{
#if sbCTFIsEnabled(TEST)
    FAIL();
#else
    SUCCEED();
#endif
}
}} // namespace sb::test