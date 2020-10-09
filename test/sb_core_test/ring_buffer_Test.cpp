#include <sb_core/container/ring_buffer.h>

#include <gtest/allocator_stats.h>
#include <gtest/gtest_common.h>

using namespace sb;

struct TestRingItem
{
    TestRingItem()
        : m_id(0)
    {
    }

    TestRingItem(u32 id)
        : m_id(id)
    {
    }

    u32 m_id;
};

struct TestRingItemCDtor
{
    TestRingItemCDtor()
    {
        ++ms_ctor_count;
    }

    TestRingItemCDtor(TestRingItemCDtor const &)
    {
        ++ms_ctor_count;
    }

    TestRingItemCDtor(TestRingItemCDtor &&)
    {
        ++ms_ctor_count;
    }

    TestRingItemCDtor & operator=(TestRingItemCDtor const &)
    {
        ++ms_ctor_count;

        return *this;
    }

    TestRingItemCDtor & operator=(TestRingItemCDtor &&)
    {
        ++ms_ctor_count;

        return *this;
    }

    ~TestRingItemCDtor()
    {
        ++ms_dtor_count;
    }

    static u32 ms_ctor_count;
    static u32 ms_dtor_count;
};

u32 TestRingItemCDtor::ms_ctor_count = 0;
u32 TestRingItemCDtor::ms_dtor_count = 0;

class RingBuffertemCDTorFixture : public testing::Test
{
protected:
    RingBuffertemCDTorFixture() {}

    ~RingBuffertemCDTorFixture() override {}

    void SetUp() override
    {
        TestRingItemCDtor::ms_ctor_count = 0;
        TestRingItemCDtor::ms_dtor_count = 0;
    }

    void TearDown() override {}
};

using RING_BUFFER = RingBuffertemCDTorFixture;

TEST_F(RING_BUFFER, DefaultCtor)
{
    RingBuffer<TestRingItem> test_ring_buffer(4);

    EXPECT_TRUE(test_ring_buffer.empty());
    EXPECT_EQ(test_ring_buffer.size(), 0U);
    EXPECT_EQ(test_ring_buffer.capacity(), 4U);
    EXPECT_FALSE(test_ring_buffer.full());
}

TEST_F(RING_BUFFER, PushBackToLimit)
{
    RingBuffer<TestRingItem> test_ring_buffer(4);

    for (u32 iter = 0; iter != 4; ++iter)
    {
        test_ring_buffer.push(TestRingItem{});

        EXPECT_FALSE(test_ring_buffer.empty());
        EXPECT_EQ(test_ring_buffer.size(), iter + 1U);

        if (iter != 3)
        {
            EXPECT_FALSE(test_ring_buffer.full());
        }
    }

    EXPECT_TRUE(test_ring_buffer.full());

    test_ring_buffer.push(TestRingItem{});
    EXPECT_TRUE(test_ring_buffer.full());
}

TEST_F(RING_BUFFER, SimplePushBackPop)
{
    RingBuffer<TestRingItem> test_ring_buffer(4);

    test_ring_buffer.push(TestRingItem{10});

    EXPECT_FALSE(test_ring_buffer.empty());
    EXPECT_EQ(test_ring_buffer.size(), 1U);

    TestRingItem val = test_ring_buffer.pop();

    EXPECT_TRUE(test_ring_buffer.empty());
    EXPECT_EQ(val.m_id, 10U);
}

TEST_F(RING_BUFFER, CirclePushBackPop)
{
    RingBuffer<TestRingItem> test_ring_buffer(4);

    for (u32 iter = 0; iter != 4; ++iter)
    {
        test_ring_buffer.push(TestRingItem{iter + 1});
    }

    EXPECT_TRUE(test_ring_buffer.full());

    for (u32 iter = 0; iter != 2; ++iter)
    {
        TestRingItem val = test_ring_buffer.pop();

        EXPECT_EQ(val.m_id, iter + 1);
    }

    EXPECT_FALSE(test_ring_buffer.full());

    for (u32 iter = 0; iter != 2; ++iter)
    {
        test_ring_buffer.push(TestRingItem{4 + iter + 1});
    }

    EXPECT_TRUE(test_ring_buffer.full());

    for (u32 iter = 0; iter != 4; ++iter)
    {
        TestRingItem val = test_ring_buffer.pop();

        EXPECT_EQ(val.m_id, 2 + iter + 1);
    }

    EXPECT_TRUE(test_ring_buffer.empty());
}

TEST_F(RingBuffertemCDTorFixture, BasicInit)
{
    RingBuffer<TestRingItemCDtor> test_ring_buffer(4);

    EXPECT_EQ(TestRingItemCDtor::ms_ctor_count, 0U);
    EXPECT_EQ(TestRingItemCDtor::ms_dtor_count, 0U);
}

TEST_F(RingBuffertemCDTorFixture, DtorCleanUp)
{
    {
        RingBuffer<TestRingItemCDtor> test_ring_buffer(4);

        EXPECT_EQ(TestRingItemCDtor::ms_ctor_count, 0U);
        EXPECT_EQ(TestRingItemCDtor::ms_dtor_count, 0U);

        for (u32 iter = 0; iter != 4; ++iter)
        {
            test_ring_buffer.push(TestRingItemCDtor{});
        }

        EXPECT_NE(TestRingItemCDtor::ms_ctor_count, 0U);
        EXPECT_NE(TestRingItemCDtor::ms_dtor_count, 0U);
    }

    EXPECT_EQ(TestRingItemCDtor::ms_ctor_count, TestRingItemCDtor::ms_dtor_count);
}
