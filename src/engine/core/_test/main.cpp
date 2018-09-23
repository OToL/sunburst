#include <gtest/gtest_common.h>

int main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    auto const res = RUN_ALL_TESTS();

    return res;
}
