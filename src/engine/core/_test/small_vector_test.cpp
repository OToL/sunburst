#include <core/container/small_vector.h>
#include <core/memory/allocator/stl_allocator.h>

#include <libc++/limits>

#include <gtest/gtest_common.h>
#include <gtest/object_tracker.h>
#include <gtest/allocator_stats.h>

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
            obj.setId(base_idx++);
        }
    }

    b8 objSeqsEq(wstd::span<gtestx::ObjectTracker> seq1, wstd::span<gtestx::ObjectTracker> seq2)
    {
        if (seq1.size() == seq2.size())
        {
            usize idx = 0;
            for (auto const & obj1 : seq1)
            {
                if (obj1.getId() != seq2[idx].getId())
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

    b8 objSeqEq(wstd::span<gtestx::ObjectTracker> seq, gtestx::ObjectTracker const & obj_test = gtestx::ObjectTracker{})
    {
        for (auto const & obj : seq)
        {
            if (obj.getId() != obj_test.getId())
            {
                return false;
            }
        }

        return true;
    }

    b8 objSeqEqIdRange(wstd::span<gtestx::ObjectTracker> seq, usize start_id = 0U)
    {
        usize id = start_id;

        for (auto const & obj : seq)
        {
            if (obj.getId() != id)
            {
                return false;
            }

            ++id;
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

template <usize CAPACITY>
using SmallVectorTestA = SmallVector<gtestx::ObjectTracker, CAPACITY, STLAllocatorView<gtestx::ObjectTracker>>;
using SmallVectorTestAlloc = STLAllocatorView<gtestx::ObjectTracker>;

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
        EXPECT_TRUE(small_vect.isSmallStorage());
        EXPECT_TRUE(objSeqEq(small_vect));
    }

    {
        gtestx::AllocatorStats alloc_stats;
        SmallVectorTestAlloc stl_alloc{alloc_stats};

        SmallVectorTestA<5> small_vect(5, stl_alloc);

        EXPECT_FALSE(small_vect.empty());
        EXPECT_EQ(small_vect.size(), 5);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 5U);
        EXPECT_TRUE(small_vect.isSmallStorage());
        EXPECT_TRUE(objSeqEq(small_vect));

        EXPECT_EQ(alloc_stats.getStats().m_alloc_count, 0ULL);
        EXPECT_FALSE(alloc_stats.owns(small_vect.data()));
    }

    {
        SmallVectorTest<5> small_vect(10);

        EXPECT_FALSE(small_vect.empty());
        EXPECT_EQ(small_vect.size(), 10);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
        EXPECT_TRUE(objSeqEq(small_vect));
        EXPECT_FALSE(small_vect.isSmallStorage());
    }

    {
        gtestx::AllocatorStats alloc_stats;
        SmallVectorTestAlloc stl_alloc{alloc_stats};
        SmallVectorTestA<5> small_vect(10, stl_alloc);

        EXPECT_FALSE(small_vect.empty());
        EXPECT_EQ(small_vect.size(), 10);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
        EXPECT_TRUE(objSeqEq(small_vect));
        EXPECT_FALSE(small_vect.isSmallStorage());

        EXPECT_NE(alloc_stats.getStats().m_alloc_count, 0ULL);
        EXPECT_TRUE(alloc_stats.owns(small_vect.data()));
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
        EXPECT_TRUE(objSeqEq(small_vect, init_obj));
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);

    {
        gtestx::AllocatorStats alloc_stats;
        SmallVectorTestAlloc stl_alloc{alloc_stats};

        SmallVectorTestA<5> small_vect(5, init_obj, stl_alloc);

        EXPECT_FALSE(small_vect.empty());
        EXPECT_EQ(small_vect.size(), 5);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 6U);
        EXPECT_TRUE(objSeqEq(small_vect, init_obj));

        EXPECT_EQ(alloc_stats.getStats().m_alloc_count, 0ULL);
        EXPECT_FALSE(alloc_stats.owns(small_vect.data()));
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);

    {
        SmallVectorTest<5> small_vect(10, init_obj);

        EXPECT_FALSE(small_vect.empty());
        EXPECT_EQ(small_vect.size(), 10);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 11U);
        EXPECT_TRUE(objSeqEq(small_vect, init_obj));
    }

    {
        gtestx::AllocatorStats alloc_stats;
        SmallVectorTestAlloc stl_alloc{alloc_stats};

        SmallVectorTestA<5> small_vect(10, init_obj, stl_alloc);

        EXPECT_FALSE(small_vect.empty());
        EXPECT_EQ(small_vect.size(), 10);
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 11U);
        EXPECT_TRUE(objSeqEq(small_vect, init_obj));

        EXPECT_NE(alloc_stats.getStats().m_alloc_count, 0ULL);
        EXPECT_TRUE(alloc_stats.owns(small_vect.data()));
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 1U);
}

TEST_F(SMALL_VECTOR, DEFAULT_CTOR_RANGE_INIT)
{
    {
        gtestx::ObjectTracker objs[5];
        initObjSeq(objs);

        {
            SmallVectorTest<5> small_vect(wstd::begin(objs), wstd::end(objs));

            EXPECT_FALSE(small_vect.empty());
            EXPECT_EQ(small_vect.size(), 5U);
            EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
            EXPECT_TRUE(objSeqsEq(objs, small_vect));
        }

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 5U);

        {
            gtestx::AllocatorStats alloc_stats;
            SmallVectorTestAlloc stl_alloc{alloc_stats};

            SmallVectorTestA<5> small_vect(wstd::begin(objs), wstd::end(objs), stl_alloc);

            EXPECT_FALSE(small_vect.empty());
            EXPECT_EQ(small_vect.size(), 5U);
            EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
            EXPECT_TRUE(objSeqsEq(objs, small_vect));

            EXPECT_EQ(alloc_stats.getStats().m_alloc_count, 0ULL);
            EXPECT_FALSE(alloc_stats.owns(small_vect.data()));
        }
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);

    {
        gtestx::ObjectTracker objs[10];
        initObjSeq(objs);

        {
            SmallVectorTest<5> vect(wstd::begin(objs), wstd::end(objs));

            EXPECT_FALSE(vect.empty());
            EXPECT_EQ(vect.size(), 10U);
            EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 20U);
            EXPECT_TRUE(objSeqsEq(objs, vect));
        }

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);

        {
            gtestx::AllocatorStats alloc_stats;
            SmallVectorTestAlloc stl_alloc{alloc_stats};

            SmallVectorTestA<5> vect(wstd::begin(objs), wstd::end(objs), stl_alloc);

            EXPECT_FALSE(vect.empty());
            EXPECT_EQ(vect.size(), 10U);
            EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 20U);
            EXPECT_TRUE(objSeqsEq(objs, vect));

            EXPECT_NE(alloc_stats.getStats().m_alloc_count, 0ULL);
            EXPECT_TRUE(alloc_stats.owns(vect.data()));
        }
    }

}

TEST_F(SMALL_VECTOR, DEFAULT_CTOR_COPY)
{
    {
        SmallVectorTest<4> src_vec(3);
        initObjSeq(src_vec);
        EXPECT_TRUE(src_vec.isSmallStorage());

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);

        SmallVectorTest<5> dst_vec(src_vec);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 6U);
        EXPECT_EQ(dst_vec.size(), src_vec.size());
        EXPECT_TRUE(objSeqsEq(src_vec, dst_vec));
        EXPECT_TRUE(dst_vec.isSmallStorage());
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);

    {
        gtestx::AllocatorStats src_alloc_stats;
        SmallVectorTestAlloc src_stl_alloc{src_alloc_stats};

        gtestx::AllocatorStats dst_alloc_stats;
        SmallVectorTestAlloc dst_stl_alloc{dst_alloc_stats};

        {
            SmallVectorTestA<4> src_vec(3, src_stl_alloc);
            initObjSeq(src_vec);
            EXPECT_TRUE(src_vec.isSmallStorage());
            EXPECT_EQ(src_alloc_stats.getStats().m_alloc_count, 0);
            EXPECT_FALSE(src_alloc_stats.owns(src_vec.data()));

            EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);

            SmallVectorTestA<5> dst_vec(src_vec, dst_stl_alloc);

            EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 6U);
            EXPECT_EQ(dst_vec.size(), src_vec.size());
            EXPECT_TRUE(objSeqsEq(src_vec, dst_vec));
            EXPECT_TRUE(dst_vec.isSmallStorage());
            EXPECT_EQ(dst_alloc_stats.getStats().m_alloc_count, 0);
            EXPECT_FALSE(dst_alloc_stats.owns(dst_vec.data()));
        }

        EXPECT_EQ(dst_alloc_stats.getStats().m_alloc_count, 0);
        EXPECT_EQ(src_alloc_stats.getStats().m_alloc_count, 0);
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);

    {
        SmallVectorTest<10> src_vec(6);
        initObjSeq(src_vec);
        EXPECT_TRUE(src_vec.isSmallStorage());

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 6U);

        SmallVectorTest<5> dst_vec(src_vec);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 12U);
        EXPECT_EQ(dst_vec.size(), src_vec.size());
        EXPECT_TRUE(objSeqsEq(src_vec, dst_vec));
        EXPECT_FALSE(dst_vec.isSmallStorage());
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);

    {
        gtestx::AllocatorStats src_alloc_stats;
        SmallVectorTestAlloc src_stl_alloc{src_alloc_stats};

        gtestx::AllocatorStats dst_alloc_stats;
        SmallVectorTestAlloc dst_stl_alloc{dst_alloc_stats};

        {
            SmallVectorTestA<10> src_vec(6, src_stl_alloc);
            initObjSeq(src_vec);
            EXPECT_TRUE(src_vec.isSmallStorage());
            EXPECT_EQ(src_alloc_stats.getStats().m_alloc_count, 0);
            EXPECT_FALSE(src_alloc_stats.owns(src_vec.data()));

            EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 6U);

            SmallVectorTestA<5> dst_vec(src_vec, dst_stl_alloc);

            EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 12U);
            EXPECT_EQ(dst_vec.size(), src_vec.size());
            EXPECT_TRUE(objSeqsEq(src_vec, dst_vec));
            EXPECT_FALSE(dst_vec.isSmallStorage());
            EXPECT_NE(dst_alloc_stats.getStats().m_alloc_count, 0);
            EXPECT_TRUE(dst_alloc_stats.owns(dst_vec.data()));
        }

        EXPECT_EQ(dst_alloc_stats.getStats().m_alloc_count, 0);
        EXPECT_EQ(src_alloc_stats.getStats().m_alloc_count, 0);
    }
}

TEST_F(SMALL_VECTOR, DEFAULT_CTOR_MOVE)
{
    // small storage --> small storage
    {
        SmallVectorTest<3> src_vec(3);
        void const * const src_data = src_vec.data();
        initObjSeq(src_vec);
        EXPECT_TRUE(src_vec.isSmallStorage());

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);

        SmallVectorTest<5> dst_vec(wstd::move(src_vec));
        EXPECT_EQ(dst_vec.size(), 3U);
        EXPECT_EQ(src_vec.size(), 0U);
        EXPECT_TRUE(dst_vec.isSmallStorage());
        EXPECT_NE(src_data, dst_vec.data());

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);

    {
        gtestx::AllocatorStats alloc_stats;
        SmallVectorTestAlloc src_stl_alloc{alloc_stats};
        SmallVectorTestAlloc dst_stl_alloc{alloc_stats};
        
        SmallVectorTestA<3> src_vec(3, src_stl_alloc);
        void const * const src_data = src_vec.data();
        initObjSeq(src_vec);
        EXPECT_TRUE(src_vec.isSmallStorage());
        EXPECT_FALSE(alloc_stats.owns(src_vec.data()));
        EXPECT_EQ(alloc_stats.getStats().m_alloc_count, 0U);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);

        SmallVectorTestA<5> dst_vec(wstd::move(src_vec), dst_stl_alloc);
        EXPECT_EQ(dst_vec.size(), 3U);
        EXPECT_EQ(src_vec.size(), 0U);
        EXPECT_TRUE(dst_vec.isSmallStorage());
        EXPECT_NE(src_data, dst_vec.data());
        EXPECT_FALSE(alloc_stats.owns(dst_vec.data()));
        EXPECT_EQ(alloc_stats.getStats().m_alloc_count, 0U);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);

    // small storage --> heap storage
    {
        SmallVectorTest<5> src_vec(5);
        void const * const src_data = src_vec.data();
        initObjSeq(src_vec);
        EXPECT_TRUE(src_vec.isSmallStorage());

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 5U);

        SmallVectorTest<3> dst_vec(wstd::move(src_vec));
        EXPECT_EQ(dst_vec.size(), 5U);
        EXPECT_EQ(src_vec.size(), 0U);
        EXPECT_FALSE(dst_vec.isSmallStorage());
        EXPECT_NE(src_data, dst_vec.data());

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 5U);
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);

    {
        gtestx::AllocatorStats alloc_stats;
        
        {
            SmallVectorTestAlloc src_stl_alloc{alloc_stats};
            SmallVectorTestAlloc dst_stl_alloc{alloc_stats};

            SmallVectorTestA<5> src_vec(5, src_stl_alloc);
            void const * const src_data = src_vec.data();
            initObjSeq(src_vec);
            EXPECT_TRUE(src_vec.isSmallStorage());
            EXPECT_FALSE(alloc_stats.owns(src_vec.data()));
            EXPECT_EQ(alloc_stats.getStats().m_alloc_count, 0U);

            EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 5U);

            SmallVectorTestA<3> dst_vec(wstd::move(src_vec), dst_stl_alloc);
            EXPECT_EQ(dst_vec.size(), 5U);
            EXPECT_EQ(src_vec.size(), 0U);
            EXPECT_FALSE(dst_vec.isSmallStorage());
            EXPECT_TRUE(alloc_stats.owns(dst_vec.data()));
            EXPECT_NE(src_data, dst_vec.data());
            EXPECT_NE(alloc_stats.getStats().m_alloc_count, 0U);

            EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 5U);
        }

        EXPECT_EQ(alloc_stats.getStats().m_alloc_count, 0U);
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);

    // heap storage --> heap storage
    {
        SmallVectorTest<3> src_vec(10);
        void const * const src_data = src_vec.data();
        initObjSeq(src_vec);
        EXPECT_FALSE(src_vec.isSmallStorage());

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);

        SmallVectorTest<5> dst_vec(wstd::move(src_vec));
        EXPECT_EQ(dst_vec.size(), 10U);
        EXPECT_EQ(src_vec.size(), 0U);
        EXPECT_FALSE(dst_vec.isSmallStorage());
        EXPECT_EQ(src_data, dst_vec.data());

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);

    {
        gtestx::AllocatorStats alloc_stats;

        {
            SmallVectorTestAlloc src_stl_alloc{alloc_stats};
            SmallVectorTestAlloc dst_stl_alloc{alloc_stats};

            SmallVectorTestA<3> src_vec(10, src_stl_alloc);
            void const * const src_data = src_vec.data();
            initObjSeq(src_vec);
            EXPECT_FALSE(src_vec.isSmallStorage());
            EXPECT_TRUE(alloc_stats.owns(src_vec.data()));
            EXPECT_NE(alloc_stats.getStats().m_alloc_count, 0U);

            EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);

            SmallVectorTestA<5> dst_vec(wstd::move(src_vec), dst_stl_alloc);
            EXPECT_EQ(dst_vec.size(), 10U);
            EXPECT_EQ(src_vec.size(), 0U);
            EXPECT_FALSE(dst_vec.isSmallStorage());
            EXPECT_EQ(src_data, dst_vec.data());
            EXPECT_TRUE(alloc_stats.owns(dst_vec.data()));
            EXPECT_NE(alloc_stats.getStats().m_alloc_count, 0U);

            EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
        }

        EXPECT_EQ(alloc_stats.getStats().m_alloc_count, 0U);        
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);

    // heap storage --> small storage
    {
        SmallVectorTest<5> src_vec(10);
        void const * const src_data = src_vec.data();
        initObjSeq(src_vec);
        EXPECT_FALSE(src_vec.isSmallStorage());

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);

        SmallVectorTest<15> dst_vec(wstd::move(src_vec));
        EXPECT_EQ(dst_vec.size(), 10U);
        EXPECT_EQ(src_vec.size(), 0U);
        EXPECT_TRUE(dst_vec.isSmallStorage());
        EXPECT_NE(src_data, dst_vec.data());

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
    }

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 0U);

    {
        gtestx::AllocatorStats alloc_stats;

        {
            SmallVectorTestAlloc src_stl_alloc{alloc_stats};
            SmallVectorTestAlloc dst_stl_alloc{alloc_stats};

            SmallVectorTestA<5> src_vec(10, src_stl_alloc);
            void const * const src_data = src_vec.data();
            initObjSeq(src_vec);
            EXPECT_FALSE(src_vec.isSmallStorage());
            EXPECT_TRUE(alloc_stats.owns(src_vec.data()));

            EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);

            SmallVectorTestA<15> dst_vec(wstd::move(src_vec), dst_stl_alloc);
            EXPECT_EQ(dst_vec.size(), 10U);
            EXPECT_EQ(src_vec.size(), 0U);
            EXPECT_TRUE(dst_vec.isSmallStorage());
            EXPECT_NE(src_data, dst_vec.data());
            EXPECT_FALSE(alloc_stats.owns(dst_vec.data()));

            EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
        }

        EXPECT_EQ(alloc_stats.getStats().m_alloc_count, 0U);
    }
}

TEST_F(SMALL_VECTOR, EMPLACE_BACK)
{
    SmallVectorTest<5> vect;

    auto & new_value = vect.emplace_back(0xFFU);
    EXPECT_FALSE(vect.empty());
    EXPECT_EQ(1U, vect.size());
    EXPECT_EQ(new_value.getId(), 0xFFU);
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
    EXPECT_EQ(vect.back().getId(), 0xFFU);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 2U);
}

TEST_F(SMALL_VECTOR, PUSH_BACK_MOVE)
{
    SmallVectorTest<5> vect;

    vect.push_back(gtestx::ObjectTracker{0xFFU});

    EXPECT_FALSE(vect.empty());
    EXPECT_EQ(1U, vect.size());
    EXPECT_EQ(vect.back().getId(), 0xFFU);
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

    EXPECT_EQ(vect[0].getId(), 0U);
    EXPECT_EQ(vect[1].getId(), 1U);
    EXPECT_EQ(vect[2].getId(), 2U);
    EXPECT_EQ(vect[3].getId(), 3U);
    EXPECT_EQ(vect[4].getId(), 4U);
}

TEST_F(SMALL_VECTOR, TEMP)
{
    SmallVectorTest<5> vect{5};
    initObjSeq(vect);

    gtestx::ObjectTracker obj{5};
    vect.push_back(obj);
}

TEST_F(SMALL_VECTOR, ACCESSOR)
{
    SmallVectorTest<5> vect{5};
    initObjSeq(vect);

    for (usize idx = 0 ; idx != 5; ++idx)
    {
        EXPECT_EQ(vect.at(idx).getId(), idx);
        EXPECT_EQ(vect[idx].getId(), idx);
    }

    EXPECT_EQ(vect.front().getId(), 0U);
    EXPECT_EQ(vect.back().getId(), 4U);
}

TEST_F(SMALL_VECTOR, RESERVATION)
{
    SmallVectorTest<5> vect;
    auto const small_storage_data = vect.data();

    SmallVectorTest<5> vect_test(2);
    initObjSeq(vect_test);

    vect.emplace_back(0);
    vect.emplace_back(1);

    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 4U);
    EXPECT_EQ(5U, vect.capacity());
    EXPECT_EQ(2U, vect.size());
    EXPECT_TRUE(objSeqsEq(vect, vect_test));

    vect.reserve(2U);

    EXPECT_EQ(vect.data(), small_storage_data);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 4U);
    EXPECT_EQ(2U, vect.size());
    EXPECT_EQ(5U, vect.capacity());
    EXPECT_TRUE(objSeqsEq(vect, vect_test));    
    EXPECT_TRUE(vect.isSmallStorage());

    vect.reserve(10U);
    auto const heap_storage_data = vect.data();

    EXPECT_NE(small_storage_data, heap_storage_data);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 4U);
    EXPECT_EQ(2U, vect.size());    
    EXPECT_EQ(10U, vect.capacity());
    EXPECT_TRUE(objSeqsEq(vect, vect_test));    
    EXPECT_FALSE(vect.isSmallStorage());
}

TEST_F(SMALL_VECTOR, SHRINK_TO_FIT)
{
    SmallVectorTest<5> vect(3);
    initObjSeq(vect);

    EXPECT_TRUE(vect.isSmallStorage());
    EXPECT_EQ(vect.capacity(), 5U);
    EXPECT_EQ(vect.size(), 3U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);

    vect.shrink_to_fit();

    EXPECT_TRUE(vect.isSmallStorage());
    EXPECT_EQ(vect.capacity(), 5U);
    EXPECT_EQ(vect.size(), 3U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);

    vect.reserve(10U);

    EXPECT_FALSE(vect.isSmallStorage());
    EXPECT_EQ(vect.capacity(), 10U);
    EXPECT_EQ(vect.size(), 3U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);


    vect.shrink_to_fit();

    EXPECT_FALSE(vect.isSmallStorage());
    EXPECT_EQ(vect.capacity(), 3U);
    EXPECT_EQ(vect.size(), 3U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);
}

TEST_F(SMALL_VECTOR, POP_BACK)
{
    SmallVectorTest<5> vect(3);
    initObjSeq(vect);

    EXPECT_EQ(vect.size(), 3U);
    EXPECT_EQ(vect.capacity(), 5U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);

    vect.pop_back();
    EXPECT_EQ(vect.size(), 2U);
    EXPECT_EQ(vect.capacity(), 5U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 2U);

    for (usize idx = 0 ; idx != vect.size() ; ++idx)
    {
        EXPECT_EQ(idx, vect[idx].getId());
    }
}

TEST_F(SMALL_VECTOR, MAX_SIZE)
{
    SmallVectorTest<5> vect;
    EXPECT_EQ(wstd::numeric_limits<SmallVectorTest<5>::size_type>::max(), vect.max_size());
}

TEST_F(SMALL_VECTOR, RESIZE)
{
    SmallVectorTest<5> vect(2);
    initObjSeq(vect);

    EXPECT_EQ(vect.size(), 2U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 2U);
    EXPECT_TRUE(vect.isSmallStorage());
    EXPECT_TRUE(objSeqEqIdRange(vect));

    vect.resize(2U);

    EXPECT_EQ(vect.size(), 2U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 2U);
    EXPECT_TRUE(vect.isSmallStorage());
    EXPECT_TRUE(objSeqEqIdRange(vect));

    vect.resize(4U);

    EXPECT_EQ(vect.size(), 4U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 4U);
    EXPECT_TRUE(vect.isSmallStorage());
    EXPECT_TRUE(objSeqEqIdRange({vect.data(), 2}));
    EXPECT_TRUE(objSeqEq({vect.data() + 2, 2}, gtestx::ObjectTracker{}));

    initObjSeq({vect.data() + 2, 2}, 2U);

    vect.resize(6U);

    EXPECT_EQ(vect.size(), 6U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 6U);
    EXPECT_FALSE(vect.isSmallStorage());
    EXPECT_TRUE(objSeqEqIdRange({vect.data(), 4}));
    EXPECT_TRUE(objSeqEq({vect.data() + 4, 2}, gtestx::ObjectTracker{}));

    initObjSeq({vect.data() + 4, 2}, 4U);

    vect.resize(2U);

    EXPECT_EQ(vect.size(), 2U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 2U);
    EXPECT_FALSE(vect.isSmallStorage());
    EXPECT_TRUE(objSeqEqIdRange({vect.data(), 2}));
}

TEST_F(SMALL_VECTOR, RESIZE_VALUE)
{
    SmallVectorTest<5> vect(2);
    initObjSeq(vect);

    EXPECT_EQ(vect.size(), 2U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 2U);
    EXPECT_TRUE(vect.isSmallStorage());
    EXPECT_TRUE(objSeqEqIdRange(vect));

    vect.resize(2U, gtestx::ObjectTracker{0xFFFFU});

    EXPECT_EQ(vect.size(), 2U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 2U);
    EXPECT_TRUE(vect.isSmallStorage());
    EXPECT_TRUE(objSeqEqIdRange(vect));

    vect.resize(4U, gtestx::ObjectTracker{0xFFFFU});

    EXPECT_EQ(vect.size(), 4U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 4U);
    EXPECT_TRUE(vect.isSmallStorage());
    EXPECT_TRUE(objSeqEqIdRange({vect.data(), 2}));
    EXPECT_TRUE(objSeqEq({vect.data() + 2, 2}, gtestx::ObjectTracker{0xFFFFU}));

    initObjSeq({vect.data() + 2, 2}, 2U);

    vect.resize(6U, gtestx::ObjectTracker{0xFFFFU});

    EXPECT_EQ(vect.size(), 6U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 6U);
    EXPECT_FALSE(vect.isSmallStorage());
    EXPECT_TRUE(objSeqEqIdRange({vect.data(), 4}));
    EXPECT_TRUE(objSeqEq({vect.data() + 4, 2}, gtestx::ObjectTracker{0xFFFFU}));

    initObjSeq({vect.data() + 4, 2}, 4U);

    vect.resize(2U, gtestx::ObjectTracker{0xFFFFU});

    EXPECT_EQ(vect.size(), 2U);
    EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 2U);
    EXPECT_FALSE(vect.isSmallStorage());
    EXPECT_TRUE(objSeqEqIdRange({vect.data(), 2}));
}

TEST_F(SMALL_VECTOR, ASSIGN_VALUE_RANGE)
{
    {
        SmallVectorTest<5> vect(3);
        gtestx::ObjectTracker const REF_OBJ(0xFF);

        initObjSeq(vect);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 4U);
        EXPECT_TRUE(vect.isSmallStorage());
        EXPECT_TRUE(objSeqEqIdRange(vect));
        EXPECT_EQ(vect.size(), 3U);

        vect.assign(4, REF_OBJ);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 5U);
        EXPECT_TRUE(vect.isSmallStorage());
        EXPECT_TRUE(objSeqEq(vect, REF_OBJ));
        EXPECT_EQ(vect.size(), 4U);
    }

    {
        SmallVectorTest<5> vect(3);
        gtestx::ObjectTracker const REF_OBJ(0xFF);

        initObjSeq(vect);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 4U);
        EXPECT_TRUE(vect.isSmallStorage());
        EXPECT_TRUE(objSeqEqIdRange(vect));
        EXPECT_EQ(vect.size(), 3U);

        vect.assign(2, REF_OBJ);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);
        EXPECT_TRUE(vect.isSmallStorage());
        EXPECT_TRUE(objSeqEq(vect, REF_OBJ));
        EXPECT_EQ(vect.size(), 2U);
    }

    {
        SmallVectorTest<5> vect(3);
        gtestx::ObjectTracker const REF_OBJ(0xFF);

        initObjSeq(vect);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 4U);
        EXPECT_TRUE(vect.isSmallStorage());
        EXPECT_TRUE(objSeqEqIdRange(vect));
        EXPECT_EQ(vect.size(), 3U);

        vect.assign(15, REF_OBJ);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 16U);
        EXPECT_FALSE(vect.isSmallStorage());
        EXPECT_TRUE(objSeqEq(vect, REF_OBJ));
        EXPECT_EQ(vect.size(), 15U);
    }
}

TEST_F(SMALL_VECTOR, ASSIGN_OP)
{
    {
        SmallVectorTest<3> vect_src(3);
        initObjSeq(vect_src);

        EXPECT_TRUE(objSeqEqIdRange(vect_src));
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 3U);
        EXPECT_TRUE(vect_src.isSmallStorage());

        SmallVectorTest<5> vect(5);
        initObjSeq(vect, 10);

        EXPECT_TRUE(objSeqEqIdRange(vect, 10));
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 8U);
        EXPECT_TRUE(vect.isSmallStorage());

        vect = vect_src;
        
        EXPECT_EQ(vect.size(), 3);
        EXPECT_TRUE(objSeqEqIdRange(vect));
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 6U);
        EXPECT_TRUE(vect.isSmallStorage());
    }

    {
        SmallVectorTest<5> vect_src(5);
        initObjSeq(vect_src);

        EXPECT_TRUE(objSeqEqIdRange(vect_src));
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 5U);
        EXPECT_TRUE(vect_src.isSmallStorage());

        SmallVectorTest<3> vect(3);
        initObjSeq(vect, 10);

        EXPECT_TRUE(objSeqEqIdRange(vect, 10));
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 8U);
        EXPECT_TRUE(vect.isSmallStorage());

        vect = vect_src;
        
        EXPECT_EQ(vect.size(), 5);
        EXPECT_TRUE(objSeqEqIdRange(vect));
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
        EXPECT_FALSE(vect.isSmallStorage());
    }

    {
        SmallVectorTest<5> vect_src(5);
        initObjSeq(vect_src);

        EXPECT_TRUE(objSeqEqIdRange(vect_src));
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 5U);
        EXPECT_TRUE(vect_src.isSmallStorage());

        SmallVectorTest<10> vect(3);
        initObjSeq(vect, 10);

        EXPECT_TRUE(objSeqEqIdRange(vect, 10));
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 8U);
        EXPECT_TRUE(vect.isSmallStorage());

        vect = vect_src;
        
        EXPECT_EQ(vect.size(), 5);
        EXPECT_TRUE(objSeqEqIdRange(vect));
        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 10U);
        EXPECT_TRUE(vect.isSmallStorage());
    }
}

TEST_F(SMALL_VECTOR, SWAP)
{
    {
        SmallVectorTest<3> vect1(10), vect2(5);

        initObjSeq(vect1);
        initObjSeq(vect2, 20U);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 15U);
        EXPECT_EQ(vect1.size(), 10U);
        EXPECT_EQ(vect2.size(), 5U);
        EXPECT_FALSE(vect1.isSmallStorage());
        EXPECT_FALSE(vect2.isSmallStorage());
        EXPECT_TRUE(objSeqEqIdRange(vect1));
        EXPECT_TRUE(objSeqEqIdRange(vect2, 20U));

        auto const * vect1_data = vect1.data();
        auto const * vect2_data = vect2.data();

        vect1.swap(vect2);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 15U);
        EXPECT_EQ(vect2.size(), 10U);
        EXPECT_EQ(vect1.size(), 5U);
        EXPECT_FALSE(vect1.isSmallStorage());
        EXPECT_FALSE(vect2.isSmallStorage());
        EXPECT_TRUE(objSeqEqIdRange(vect2));
        EXPECT_TRUE(objSeqEqIdRange(vect1, 20U));

        EXPECT_EQ(vect1_data, vect2.data());
        EXPECT_EQ(vect2_data, vect1.data());
    }    

    {
        SmallVectorTest<20> vect1(10), vect2(5);

        initObjSeq(vect1);
        initObjSeq(vect2, 20U);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 15U);
        EXPECT_EQ(vect1.size(), 10U);
        EXPECT_EQ(vect2.size(), 5U);
        EXPECT_TRUE(vect1.isSmallStorage());
        EXPECT_TRUE(vect2.isSmallStorage());
        EXPECT_TRUE(objSeqEqIdRange(vect1));
        EXPECT_TRUE(objSeqEqIdRange(vect2, 20U));

        vect1.swap(vect2);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 15U);
        EXPECT_EQ(vect2.size(), 10U);
        EXPECT_EQ(vect1.size(), 5U);
        EXPECT_TRUE(vect2.isSmallStorage());
        EXPECT_TRUE(vect1.isSmallStorage());
        EXPECT_TRUE(objSeqEqIdRange(vect2));
        EXPECT_TRUE(objSeqEqIdRange(vect1, 20U));
    }

    {
        SmallVectorTest<5> vect1(5), vect2(10);

        initObjSeq(vect1);
        initObjSeq(vect2, 20U);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 15U);
        EXPECT_EQ(vect1.size(), 5U);
        EXPECT_EQ(vect2.size(), 10U);
        EXPECT_TRUE(vect1.isSmallStorage());
        EXPECT_FALSE(vect2.isSmallStorage());
        EXPECT_TRUE(objSeqEqIdRange(vect1));
        EXPECT_TRUE(objSeqEqIdRange(vect2, 20U));

        auto const * const vect1_data = vect1.data();
        auto const * const vect2_data = vect2.data();

        vect1.swap(vect2);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 15U);
        EXPECT_EQ(vect2.size(), 5U);
        EXPECT_EQ(vect1.size(), 10U);
        EXPECT_FALSE(vect1.isSmallStorage());
        EXPECT_FALSE(vect2.isSmallStorage());
        EXPECT_TRUE(objSeqEqIdRange(vect2));
        EXPECT_TRUE(objSeqEqIdRange(vect1, 20U));
        EXPECT_EQ(vect2.data(), vect2_data);
        EXPECT_NE(vect1.data(), vect1_data);
    }

    {
        SmallVectorTest<5> vect2(5), vect1(10);

        initObjSeq(vect2);
        initObjSeq(vect1, 20U);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 15U);
        EXPECT_EQ(vect2.size(), 5U);
        EXPECT_EQ(vect1.size(), 10U);
        EXPECT_TRUE(vect2.isSmallStorage());
        EXPECT_FALSE(vect1.isSmallStorage());
        EXPECT_TRUE(objSeqEqIdRange(vect2));
        EXPECT_TRUE(objSeqEqIdRange(vect1, 20U));

        auto const * const vect2_data = vect2.data();
        auto const * const vect1_data = vect1.data();

        vect2.swap(vect1);

        EXPECT_EQ(gtestx::ObjectTracker::getStats().m_alive_obj_count, 15U);
        EXPECT_EQ(vect1.size(), 5U);
        EXPECT_EQ(vect2.size(), 10U);
        EXPECT_FALSE(vect2.isSmallStorage());
        EXPECT_FALSE(vect1.isSmallStorage());
        EXPECT_TRUE(objSeqEqIdRange(vect1));
        EXPECT_TRUE(objSeqEqIdRange(vect2, 20U));
        EXPECT_EQ(vect1.data(), vect1_data);
        EXPECT_NE(vect2.data(), vect2_data);
    }
}

// sizeof(m_begin) + sizeof(m_end) + sizeof(m_storage_end) + sizeof(small storage test = void *)
static_assert(sizeof(SmallVector<b8, sizeof(void *)>) == 4 * sizeof(void *), "SmallVector with empty allocator must have no extra overhead");

}
