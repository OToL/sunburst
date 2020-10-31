#include <sb_core/core.h>
#include <sb_core/memory/allocator/stl_allocator_wrapper.h>
#include <sb_core/container/dynamic_array.h>

#include <sb_unit/test_allocator.h>

#include <sb_std/iterator>

#include <sb_std/algorithm>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

TEST_CASE("STL Allocator Wrapper", "[stl_allocator]")
{
    TestAllocator test_alloc;
    
    {        
        DArray<u32, STLAllocatorWrapper<u32>> vector_test{STLAllocatorWrapper<u32>{test_alloc}};

        u32 const array_test [] = {0, 1, 2, 3, 4};

        sbstd::copy(sbstd::begin(array_test), sbstd::end(array_test), back_inserter(vector_test));

        REQUIRE(test_alloc.getStats().m_allocated_byte != 0U);
        REQUIRE(test_alloc.getStats().m_allocated_byte >= sizeof(array_test));

        for (u32 idx = 0 ; idx != sbstd::size(array_test) ; ++idx)
        {
            REQUIRE(array_test[idx] == idx);
            REQUIRE(array_test[idx] == vector_test[idx]);
        }
    }

    REQUIRE(test_alloc.getStats().m_alloc_count == 0U);
    REQUIRE(test_alloc.getStats().m_allocated_byte == 0U);
}

#include <catch2/test_epilog.h>

#if 0

TEST(STL_ALLOCATOR, default)
{

}

#endif
