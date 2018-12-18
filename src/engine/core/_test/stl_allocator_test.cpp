#include <core/platform.h>
#include <core/memory/allocator/stl_allocator.h>
#include <core/container/vector.h>

#include <libc++/iterator>

#include <gtest/gtest_common.h>
#include <gtest/allocator_stats.h>

namespace sb::test {

TEST(STL_ALLOCATOR, AllocatorView)
{
    gtestx::AllocatorStats allocator_test;
    
    {        
        Vector<ui32, STLAllocatorView<ui32>> vector_test{STLAllocatorView<ui32>{allocator_test}};

        ui32 const array_test [] = {0, 1, 2, 3, 4};

        wstd::copy(wstd::begin(array_test), wstd::end(array_test), back_inserter(vector_test));

        EXPECT_NE(allocator_test.getStats().m_alloc_count, 0U);
        EXPECT_GE(allocator_test.getStats().m_allocated_byte, sizeof(array_test));

        for (ui32 idx = 0 ; idx != wstd::size(array_test) ; ++idx)
        {
            EXPECT_EQ(array_test[idx], idx);
            EXPECT_EQ(array_test[idx], vector_test[idx]);
        }
    }

    EXPECT_EQ(allocator_test.getStats().m_alloc_count, 0U);
    EXPECT_GE(allocator_test.getStats().m_allocated_byte, 0U);
}

}
