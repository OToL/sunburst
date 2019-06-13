#include <core/handle.h>

#include <gtest/gtest_common.h>

namespace sb::test {

struct TestHdl {};

using TestHandle = Handle<TestHdl, ui32>;

TEST(HANDLE, DefaultCtor)
{
    EXPECT_TRUE(TestHandle{}.isNull());
}

TEST(HANDLE, Reset)
{
    TestHandle test_hdl(2);

    EXPECT_FALSE(test_hdl.isNull()); 

    test_hdl.reset();

    EXPECT_TRUE(TestHandle{}.isNull());
}


TEST(HANDLE, Operator)
{
    TestHandle test_hdl(2);
    TestHandle test_hdl2(3);

    EXPECT_EQ(test_hdl, test_hdl);
    EXPECT_NE(test_hdl, test_hdl2);
}

}
