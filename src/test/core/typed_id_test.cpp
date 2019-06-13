#include <core/typed_id.h>

#include <gtest/gtest_common.h>

namespace sb::test {

struct TestId {};

using TestTypedId = TypedId<TestId, ui32>;

TEST(TYPED_ID, DefaultCtor)
{
    EXPECT_TRUE(TestTypedId{}.isNull());
}

TEST(TYPED_ID, Reset)
{
    TestTypedId test_id(2);

    EXPECT_FALSE(test_id.isNull()); 

    test_id.reset();

    EXPECT_TRUE(TestTypedId{}.isNull());
}


TEST(TYPED_ID, Operator)
{
    TestTypedId test_id(2);
    TestTypedId test_id2(3);

    EXPECT_EQ(test_id, test_id);
    EXPECT_NE(test_id, test_id2);
}

}
