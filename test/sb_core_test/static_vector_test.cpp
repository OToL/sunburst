#include <sb_core/container/static_vector.h>
#include <sb_core/bit.h>

#include <gtest/gtest_common.h>
#include <gtest/object_tracker.h>

#include <sb_std/type_traits>

using namespace sb;

class StaticVectorTestFixture : public testing::Test
{
public:
    typedef StaticVector<usize, 10> VectorOfPOD;
    typedef StaticVector<gtestx::ObjectTracker, 10> VectorOfNonPOD;

    void FillTestArray(VectorOfPOD & test_vector, sbstd::true_type const & /* push */ = sbstd::true_type{})
    {
        for (usize idx = 0; idx != test_vector.capacity(); ++idx)
        {
            test_vector.push_back(idx);
        }

        EXPECT_EQ(test_vector.size(), 10ULL);
        EXPECT_FALSE(test_vector.empty());
    }

    void FillTestArray(VectorOfPOD & test_vector, sbstd::false_type const &)
    {
        for (usize idx = 0; idx != test_vector.capacity(); ++idx)
        {
            test_vector.emplace_back(idx);
        }

        EXPECT_EQ(test_vector.size(), 10ULL);
        EXPECT_FALSE(test_vector.empty());
    }

    void FillTestArray(VectorOfNonPOD & test_vector, sbstd::true_type const & /* push */ = sbstd::true_type{})
    {
        for (usize idx = 0; idx != test_vector.capacity(); ++idx)
        {
            test_vector.push_back(gtestx::ObjectTracker{idx});
        }

        EXPECT_EQ(test_vector.size(), 10ULL);
        EXPECT_FALSE(test_vector.empty());
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10ULL);
    }

    void FillTestArray(VectorOfNonPOD & test_vector, sbstd::false_type const &)
    {
        for (usize idx = 0; idx != test_vector.capacity(); ++idx)
        {
            test_vector.emplace_back(gtestx::ObjectTracker{idx});
        }

        EXPECT_EQ(test_vector.size(), 10ULL);
        EXPECT_FALSE(test_vector.empty());
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10ULL);
    }

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

typedef StaticVectorTestFixture STATIC_ARRAY;

TEST_F(STATIC_ARRAY, DEFAULT_CTOR)
{
    VectorOfPOD test_vector;

    EXPECT_EQ(test_vector.size(), 0ULL);
    EXPECT_TRUE(test_vector.empty());
}

TEST_F(STATIC_ARRAY, PUSH_BACK_POD)
{
    VectorOfPOD test_vector;

    FillTestArray(test_vector);

    usize const * const test_value = test_vector.data();

    for (usize idx = 0; idx != test_vector.capacity(); ++idx)
    {
        EXPECT_EQ(idx, test_value[idx]);
    }
}

TEST_F(STATIC_ARRAY, BACK)
{
    VectorOfPOD test_vector;

    test_vector.push_back(0U);
    test_vector.push_back(1U);
    test_vector.push_back(2U);

    EXPECT_EQ(test_vector.back(), 2U);
}

TEST_F(STATIC_ARRAY, PUSH_BACK_CTOR)
{
    {
        VectorOfNonPOD test_vector;

        FillTestArray(test_vector);
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0ULL);
}

TEST_F(STATIC_ARRAY, EMPLACE_BACK_CTOR)
{
    {
        VectorOfNonPOD test_vector;

        FillTestArray(test_vector, sbstd::false_type{});
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0ULL);
}

TEST_F(STATIC_ARRAY, ERASE_CTOR_BEGIN)
{
    {
        VectorOfNonPOD test_vector;

        FillTestArray(test_vector);

        test_vector.erase(test_vector.begin());

        EXPECT_EQ(test_vector.size(), 9ULL);
        EXPECT_FALSE(test_vector.empty());
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 9ULL);

        usize idx = 1;
        for (auto const & iter : test_vector)
        {
            EXPECT_EQ(idx, iter.getId());
            ++idx;
        }
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0ULL);
}

TEST_F(STATIC_ARRAY, ERASE_CTOR_LAST)
{
    {
        VectorOfNonPOD test_vector;

        FillTestArray(test_vector);

        test_vector.erase(test_vector.end() - 1);

        EXPECT_EQ(test_vector.size(), 9ULL);
        EXPECT_FALSE(test_vector.empty());
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 9ULL);

        usize idx = 0;
        for (auto const & iter : test_vector)
        {
            EXPECT_EQ(idx, iter.getId());
            ++idx;
        }
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0ULL);
}

TEST_F(STATIC_ARRAY, ERASE_CTOR_MIDDLE)
{
    {
        VectorOfNonPOD test_vector;

        FillTestArray(test_vector);

        usize const ERASE_OFFSET = 2;
        test_vector.erase(test_vector.begin() + ERASE_OFFSET);

        EXPECT_EQ(test_vector.size(), 9ULL);
        EXPECT_FALSE(test_vector.empty());
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 9ULL);

        usize idx = 0;
        for (auto const & iter : test_vector)
        {
            if (ERASE_OFFSET == idx)
            {
                ++idx;
            }

            EXPECT_EQ(idx, iter.getId());

            ++idx;
        }
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0ULL);
}

TEST_F(STATIC_ARRAY, ALIGNED_STORAGE)
{
    struct alignas(32) AlignedType
    {
        u32 m_x;
    };

    StaticVector<AlignedType, 10> aligned_array;

    for (u32 idx = 0; idx < 5; ++idx)
    {
        aligned_array.emplace_back(idx);
    }

    EXPECT_TRUE(isAlignedTo(aligned_array.data(), 32));

    for (u32 idx = 0; idx < 5; ++idx)
    {
        EXPECT_EQ(aligned_array[idx].m_x, idx);
    }
}
