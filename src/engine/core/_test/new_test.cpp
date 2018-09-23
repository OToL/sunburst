#include <core/memory/allocator/global_heap_allocator.h>
#include <core/memory/new.h>

#include <gtest/allocator_stats.h>
#include <gtest/gtest_common.h>
#include <gtest/object_tracker.h>

namespace sb::test {

class MemoryOperatorFixture : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        gtestx::ObjectTracker::resetStats();
    }

    virtual void TearDown() override
    {
        gtestx::ObjectTracker::resetStats();
    }
};

using MEMORY_OPERATOR = MemoryOperatorFixture;

TEST_F(MEMORY_OPERATOR, NEW_DELETE_DEFAULT)
{
    gtestx::ObjectTracker * test_obj = sbNew(gtestx::ObjectTracker)();

    EXPECT_NE(test_obj, nullptr);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);
    EXPECT_TRUE(isAlignedTo(test_obj, GLOBAL_HEAP_MIN_ALIGNMENT));

    sbDelete(test_obj);

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
}

TEST_F(MEMORY_OPERATOR, NEW_DELETE_PARAM)
{
    class ParamObject : public gtestx::ObjectTracker
    {
        sbBaseClass(gtestx::ObjectTracker);

    public:
        ParamObject(ui32 value)
            : BaseClass()
            , m_value(value)
        {
        }

        ui32 m_value;
    };

    {
        ParamObject * test_obj = sbNew(ParamObject)(100U);

        EXPECT_NE(test_obj, nullptr);
        EXPECT_EQ(test_obj->m_value, 100U);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);
        EXPECT_TRUE(isAlignedTo(test_obj, GLOBAL_HEAP_MIN_ALIGNMENT));

        sbDelete(test_obj);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
    }
}

TEST_F(MEMORY_OPERATOR, NEW_DELETE_ALIGNED)
{
    class alignas(128) AlignedObject : public gtestx::ObjectTracker
    {
    };

    AlignedObject * test_obj = sbNew(AlignedObject)();

    EXPECT_NE(test_obj, nullptr);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);
    EXPECT_TRUE(isAlignedTo(test_obj, 128));

    sbDelete(test_obj);

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
}

TEST_F(MEMORY_OPERATOR, NEW_DELETE_ARRAY_ALIGNED)
{
    class alignas(128) AlignedObject : public gtestx::ObjectTracker
    {
    };

    AlignedObject * test_obj = sbNewArray(AlignedObject)[10];

    EXPECT_NE(test_obj, nullptr);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
    EXPECT_TRUE(isAlignedTo(test_obj, 128));

    sbDeleteArray(test_obj);

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
}

TEST_F(MEMORY_OPERATOR, NEW_DELETE_ALLOCATOR)
{
    gtestx::AllocatorStats test_allocator;

    gtestx::ObjectTracker * test_obj = sbNew(gtestx::ObjectTracker, test_allocator)();

    EXPECT_NE(test_obj, nullptr);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);
    EXPECT_EQ(test_allocator.getStats().m_alloc_count, 1U);
    EXPECT_TRUE(isAlignedTo(test_obj, GLOBAL_HEAP_MIN_ALIGNMENT));

    sbDelete(test_obj, test_allocator);

    EXPECT_EQ(test_allocator.getStats().m_alloc_count, 0U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
}

TEST_F(MEMORY_OPERATOR, NEW_DELETE_ARRAY_ALLOCATOR)
{
    gtestx::AllocatorStats test_allocator;

    gtestx::ObjectTracker * test_obj = sbNewArray(gtestx::ObjectTracker, test_allocator)[10];

    EXPECT_NE(test_obj, nullptr);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
    EXPECT_EQ(test_allocator.getStats().m_alloc_count, 1U);
    EXPECT_TRUE(isAlignedTo(test_obj, GLOBAL_HEAP_MIN_ALIGNMENT));

    sbDeleteArray(test_obj, test_allocator);

    EXPECT_EQ(test_allocator.getStats().m_alloc_count, 0U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
}

TEST_F(MEMORY_OPERATOR, NEW_DELETE_ALLOCATOR_ALIGNED)
{
    class alignas(128) AlignedObject : public gtestx::ObjectTracker
    {
    };

    gtestx::AllocatorStats test_allocator;

    AlignedObject * test_obj = sbNew(AlignedObject, test_allocator)();

    EXPECT_NE(test_obj, nullptr);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);
    EXPECT_EQ(test_allocator.getStats().m_alloc_count, 1U);
    EXPECT_TRUE(isAlignedTo(test_obj, 128));

    sbDelete(test_obj, test_allocator);

    EXPECT_EQ(test_allocator.getStats().m_alloc_count, 0U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
}

TEST_F(MEMORY_OPERATOR, NEW_DELETE_ARRAY_ALLOCATOR_ALIGNED)
{
    class alignas(128) AlignedObject : public gtestx::ObjectTracker
    {
    };

    gtestx::AllocatorStats test_allocator;

    AlignedObject * test_obj = sbNewArray(AlignedObject, test_allocator)[10];

    EXPECT_NE(test_obj, nullptr);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
    EXPECT_EQ(test_allocator.getStats().m_alloc_count, 1U);
    EXPECT_TRUE(isAlignedTo(test_obj, 128));

    sbDeleteArray(test_obj, test_allocator);

    EXPECT_EQ(test_allocator.getStats().m_alloc_count, 0U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
}

TEST_F(MEMORY_OPERATOR, NEW_DELETE_ARRAY_TIVIAL)
{
    struct TrivialType
    {
        ui32 m_value;
    };

    gtestx::AllocatorStats test_allocator;

    TrivialType * array_obj = sbNewArray(TrivialType, test_allocator)[10];

    EXPECT_NE(array_obj, nullptr);
    EXPECT_EQ(test_allocator.getStats().m_alloc_count, 1U);
    EXPECT_EQ(test_allocator.getStats().m_allocated_byte, 10U * sizeof(TrivialType));
    EXPECT_TRUE(isAlignedTo(array_obj, alignof(TrivialType)));

    sbDeleteArray(array_obj, test_allocator);

    EXPECT_EQ(test_allocator.getStats().m_alloc_count, 0U);
    EXPECT_EQ(test_allocator.getStats().m_allocated_byte, 0U);
}

}
