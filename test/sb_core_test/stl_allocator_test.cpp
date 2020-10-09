#include <sb_core/platform.h>
#include <sb_core/memory/allocator/stl_allocator.h>
#include <sb_core/container/vector.h>

#include <sb_std/iterator>

#include <gtest/gtest_common.h>
#include <gtest/allocator_stats.h>

using namespace sb;

TEST(STL_ALLOCATOR, AllocatorView)
{
    gtestx::AllocatorStats allocator_test;
    
    {        
        Vector<u32, STLAllocatorView<u32>> vector_test{STLAllocatorView<u32>{allocator_test}};

        u32 const array_test [] = {0, 1, 2, 3, 4};

        sbstd::copy(sbstd::begin(array_test), sbstd::end(array_test), back_inserter(vector_test));

        EXPECT_NE(allocator_test.getStats().m_alloc_count, 0U);
        EXPECT_GE(allocator_test.getStats().m_allocated_byte, sizeof(array_test));

        for (u32 idx = 0 ; idx != sbstd::size(array_test) ; ++idx)
        {
            EXPECT_EQ(array_test[idx], idx);
            EXPECT_EQ(array_test[idx], vector_test[idx]);
        }
    }

    EXPECT_EQ(allocator_test.getStats().m_alloc_count, 0U);
    EXPECT_GE(allocator_test.getStats().m_allocated_byte, 0U);
}
