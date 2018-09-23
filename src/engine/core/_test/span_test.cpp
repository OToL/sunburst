#include <core/container/span.h>
#include <core/container/vector.h>

#include <libc++/array>
#include <libc++/vector>

#include <gtest/gtest_common.h>

namespace sb { namespace test {

TEST(SPAN, DefaultCtor)
{
    Span<si32> default_span;

    EXPECT_TRUE(default_span.empty());
    EXPECT_EQ(0U, default_span.size());
}

TEST(SPAN, MakeFromCArray)
{
    si32 test_array[] = {1, 2, 3, 4, 5};
    Span<si32> test_span(test_array);

    EXPECT_FALSE(test_span.empty());
    EXPECT_EQ(5U, test_span.size());

    si32 val_test = 1;
    for (si32 val : test_span)
    {
        EXPECT_EQ(val_test, val);
        ++val_test;
    }
}

TEST(SPAN, SubNoOffset)
{
    si32 test_array[] = {1, 2, 3, 4, 5};
    Span<si32> test_span(test_array);

    auto const sub_entire_span = test_span.sub(0);

    EXPECT_EQ(sub_entire_span.size(), test_span.size());
    for (usize idx = 0; idx != test_span.size(); ++idx)
    {
        EXPECT_EQ(sub_entire_span[idx], test_span[idx]);
    }
}

TEST(SPAN, SubOffset)
{
    si32 test_array[] = {1, 2, 3, 4, 5};
    Span<si32> test_span(test_array);

    usize const span_offset = 2;
    auto const sub_span = test_span.sub(span_offset);

    EXPECT_EQ(sub_span.size(), test_span.size() - span_offset);

    for (usize idx = 0; idx != sub_span.size(); ++idx)
    {
        EXPECT_EQ(sub_span[idx], test_span[span_offset + idx]);
    }
}

TEST(SPAN, SubToEmpty)
{
    si32 test_array[] = {1, 2, 3, 4, 5};
    Span<si32> const test_span(test_array);

    auto const empty_span = test_span.sub(test_span.size());

    EXPECT_TRUE(empty_span.empty());
}

TEST(SPAN, SubFromEmpty)
{
    Span<si32> const test_span;
    EXPECT_TRUE(test_span.empty());

    auto const empty_span = test_span.sub(test_span.size());

    EXPECT_TRUE(empty_span.empty());
}

TEST(SPAN, MakeFromContiguousContainer)
{
    Vector<si32> test_vector = {1, 2, 3, 4, 5};
    wstd::array<si32, 5> test_array{{1, 2, 3, 4, 5}};

    Span<si32> vector_span(test_vector);
    Span<si32> array_span(test_array);

    EXPECT_EQ(vector_span.size(), 5U);
    EXPECT_EQ(array_span.size(), 5U);

    si32 val_test = 1;
    for (usize idx = 0; idx < vector_span.size(); ++idx)
    {
        EXPECT_EQ(vector_span[idx], val_test);
        EXPECT_EQ(array_span[idx], val_test);

        ++val_test;
    }
}
}} // namespace sb::test
