#include <core/container/small_vector.h>

#include <gtest/gtest_common.h>
#include <gtest/object_tracker.h>

namespace sb::test {

class SmallVectorFixture : public testing::Test
{
protected:
    SmallVectorFixture() {}

    ~SmallVectorFixture() override {}

    void initObjSeq(wstd::span<gtestx::ObjectTracker> objs, usize base_idx = 0ULL)
    {
        for (auto & obj : objs)
        {
            obj.setIdx(base_idx++);
        }
    }

    b8 objSeqsEq(wstd::span<gtestx::ObjectTracker> seq1, wstd::span<gtestx::ObjectTracker> seq2)
    {
        if (seq1.size() == seq2.size())
        {
            usize idx = 0;
            for (auto const & obj1 : seq1)
            {
                if (obj1.getIdx() != seq2[idx].getIdx())
                {
                    return false;
                }

                ++idx;
            }
        }
        else
        {
            return false;
        }

        return true;
    }

    void SetUp() override
    {
        gtestx::ObjectTracker::resetStats();
    }

    void TearDown() override 
    {
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
        gtestx::ObjectTracker::resetStats();
    }
};

template <usize CAPACITY>
using SmallVectorTest = SmallVector<gtestx::ObjectTracker, CAPACITY>; 

using SMALL_VECTOR = SmallVectorFixture;

TEST_F(SMALL_VECTOR, DEFAULT_CTOR)
{
    SmallVectorTest<5> vect;
    EXPECT_TRUE(vect.empty());
    EXPECT_EQ(5U, vect.capacity());
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
}

TEST_F(SMALL_VECTOR, DEFAULT_CTOR_SIZE)
{
    {
        SmallVectorTest<5> small_vect(5);
        EXPECT_FALSE(small_vect.empty());
        EXPECT_EQ(small_vect.size(), 5);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 5U);

        for (auto const & obj : small_vect)
        {
            EXPECT_EQ(obj.getIdx(), 0U);
        }
    }

    {
        SmallVectorTest<5> vect(10);
        EXPECT_FALSE(vect.empty());
        EXPECT_EQ(vect.size(), 10);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);

        for (auto const & obj : vect)
        {
            EXPECT_EQ(obj.getIdx(), 0U);
        }
    }
}

TEST_F(SMALL_VECTOR, DEFAULT_CTOR_SIZE_INIT)
{
    gtestx::ObjectTracker init_obj{1};

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);

    {
        SmallVectorTest<5> small_vect(5, init_obj);
        EXPECT_FALSE(small_vect.empty());
        EXPECT_EQ(small_vect.size(), 5);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 6U);

        for (auto const & obj : small_vect)
        {
            EXPECT_EQ(obj.getIdx(), 1U);
        }
    }

    {
        SmallVectorTest<5> vect(10, init_obj);
        EXPECT_FALSE(vect.empty());
        EXPECT_EQ(vect.size(), 10);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 11U);

        for (auto const & obj : vect)
        {
            EXPECT_EQ(obj.getIdx(), 1U);
        }
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);
}

TEST_F(SMALL_VECTOR, DEFAULT_CTOR_RANGE_INIT)
{
    {
        gtestx::ObjectTracker objs[5];
        initObjSeq(objs);
        
        SmallVectorTest<5> vect(wstd::begin(objs), wstd::end(objs));
        EXPECT_FALSE(vect.empty());
        EXPECT_EQ(vect.size(), 5U);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
        EXPECT_TRUE(objSeqsEq(objs, vect));
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);

    {
        gtestx::ObjectTracker objs[10];
        initObjSeq(objs);
        
        SmallVectorTest<5> vect(wstd::begin(objs), wstd::end(objs));
        EXPECT_FALSE(vect.empty());
        EXPECT_EQ(vect.size(), 10U);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 20U);
        EXPECT_TRUE(objSeqsEq(objs, vect));
    }
}

TEST_F(SMALL_VECTOR, DEFAULT_CTOR_COPY)
{
    {
        SmallVectorTest<4> src_vec(3);
        initObjSeq(src_vec);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);

        SmallVectorTest<5> dst_vec(src_vec);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 6U);
        EXPECT_EQ(dst_vec.size(), 3U);
        EXPECT_TRUE(objSeqsEq(src_vec, dst_vec));
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);
}

TEST_F(SMALL_VECTOR, EMPLACE_BACK)
{
    SmallVectorTest<5> vect;

    auto & new_value = vect.emplace_back(0xFFU);
    EXPECT_FALSE(vect.empty());
    EXPECT_EQ(1U, vect.size());
    EXPECT_EQ(new_value.getIdx(), 0xFFU);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);
}

TEST_F(SMALL_VECTOR, PUSH_BACK_COPY)
{
    SmallVectorTest<5> vect;

    gtestx::ObjectTracker const obj{0xFFU};
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);

    vect.push_back(obj);
    EXPECT_FALSE(vect.empty());
    EXPECT_EQ(1U, vect.size());
    EXPECT_EQ(vect.back().getIdx(), 0xFFU);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 2U);
}

TEST_F(SMALL_VECTOR, PUSH_BACK_MOVE)
{
    SmallVectorTest<5> vect;

    vect.push_back(gtestx::ObjectTracker{0xFFU});

    EXPECT_FALSE(vect.empty());
    EXPECT_EQ(1U, vect.size());
    EXPECT_EQ(vect.back().getIdx(), 0xFFU);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);
}

TEST_F(SMALL_VECTOR, CLEAR)
{
    SmallVectorTest<5> vect;

    vect.emplace_back(0xFFU);
    vect.emplace_back(0xFFU);
    vect.emplace_back(0xFFU);

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);

    vect.clear();

    EXPECT_TRUE(vect.empty());
    EXPECT_EQ(vect.size(), 0U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);

    vect.emplace_back(0U);
    vect.emplace_back(1U);
    vect.emplace_back(2U);
    vect.emplace_back(3U);
    vect.emplace_back(4U);

    EXPECT_FALSE(vect.empty());
    EXPECT_EQ(5U, vect.size());
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 5U);

    EXPECT_EQ(vect[0].getIdx(), 0U);
    EXPECT_EQ(vect[1].getIdx(), 1U);
    EXPECT_EQ(vect[2].getIdx(), 2U);
    EXPECT_EQ(vect[3].getIdx(), 3U);
    EXPECT_EQ(vect[4].getIdx(), 4U);
}

TEST_F(SMALL_VECTOR, ACCESSOR)
{
    SmallVectorTest<5> vect;

    for (usize idx = 0 ; 5 != idx ; ++idx)
    {
        vect.emplace_back(idx);
    }
    
    EXPECT_EQ(5U, vect.size());

    for (usize idx = 0 ; 5 != idx ; ++idx)
    {
        EXPECT_EQ(idx, vect[idx].getIdx());
        EXPECT_EQ(idx, (vect.data() + idx)->getIdx());
    }
}

TEST_F(SMALL_VECTOR, TEMP)
{
    SmallVectorTest<5> vect;

    for (ui32 idx = 0 ; idx != 5; ++idx)
    {
        gtestx::ObjectTracker obj{idx};
        vect.push_back(obj);
    }

    gtestx::ObjectTracker obj{5};
    vect.push_back(obj);
}

}
