#include <core/memory/allocator/allocator_view.h>
#include <core/unique_ptr.h>

#include <gtest/gtest_common.h>
#include <gtest/object_tracker.h>
#include <gtest/allocator_stats.h>

using namespace sb;

class UniquePtrFixture : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        gtestx::ObjectTracker::resetStats();
    }

    virtual void TearDown() override 
    {
    }
};

using UNIQUE_PTR = UniquePtrFixture;

TEST_F(UNIQUE_PTR, MakeUniqueObject)
{
    {
        auto test_ptr = makeUnique<gtestx::ObjectTracker>(10U);

        EXPECT_NE(nullptr, test_ptr);    
        EXPECT_EQ(test_ptr->getId(), 10U);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
}

TEST_F(UNIQUE_PTR, MakeUniqueArray)
{
    {
        auto test_ptr = makeUnique<gtestx::ObjectTracker []>(10U);

        EXPECT_NE(nullptr, test_ptr);    
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
}

TEST_F(UNIQUE_PTR, AllocateUniqueObject)
{
    gtestx::AllocatorStats alloc;

    {
        auto test_ptr = allocateUnique<gtestx::ObjectTracker, AllocatorView>(alloc, 10U);

        EXPECT_NE(nullptr, test_ptr);    
        EXPECT_EQ(test_ptr->getId(), 10U);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);
        EXPECT_EQ(alloc.getStats().m_alloc_count, 1U);
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
    EXPECT_EQ(alloc.getStats().m_alloc_count, 0U);
}

TEST_F(UNIQUE_PTR, AllocateUniqueArray)
{
    gtestx::AllocatorStats alloc;

    {
        auto test_ptr = allocateUnique<gtestx::ObjectTracker [], AllocatorView>(alloc, 10U);

        EXPECT_NE(nullptr, test_ptr);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
        EXPECT_GE(alloc.getStats().m_allocated_byte, 10U * sizeof(gtestx::ObjectTracker));
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
    EXPECT_EQ(alloc.getStats().m_alloc_count, 0U);
}

TEST_F(UNIQUE_PTR, AllocateRefUniqueObject)
{
    gtestx::AllocatorStats alloc;

    {
        auto test_ptr = allocateUnique<gtestx::ObjectTracker, gtestx::AllocatorStats>(wstd::reference_wrapper<gtestx::AllocatorStats>(alloc), 10U);

        EXPECT_NE(nullptr, test_ptr);    
        EXPECT_EQ(test_ptr->getId(), 10U);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);
        EXPECT_EQ(alloc.getStats().m_alloc_count, 1U);
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
    EXPECT_EQ(alloc.getStats().m_alloc_count, 0U);
}

TEST_F(UNIQUE_PTR, AllocateRefUniqueArray)
{
    gtestx::AllocatorStats alloc;

    {
        auto test_ptr = allocateUnique<gtestx::ObjectTracker [], gtestx::AllocatorStats>(wstd::reference_wrapper<gtestx::AllocatorStats>(alloc), 10U);

        EXPECT_NE(nullptr, test_ptr);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
        EXPECT_GE(alloc.getStats().m_allocated_byte, 10U * sizeof(gtestx::ObjectTracker));
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
    EXPECT_EQ(alloc.getStats().m_alloc_count, 0U);
}
