#include <core/platform.h>

#include <gtest/gtest_common.h>

namespace sb { namespace test {

class ConcatIDTest
{
};

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
