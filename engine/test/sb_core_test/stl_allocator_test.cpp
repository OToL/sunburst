#include <sb_core/core.h>
#include <sb_core/memory/allocators/stl_allocator_wrapper.h>
#include <sb_core/containers/dynamic_array.h>

#include <sb_unit/test_allocator.h>

#include <sb_slw/iterator>
#include <sb_slw/algorithm>

#include <extern_prolog.h>
#include <catch2/catch.hpp>
#include <extern_epilog.h>

using namespace sb;

TEST_CASE("STL Allocator Wrapper", "[stl_allocator]")
{
    TestAllocator test_alloc;

    {
        DArray<u32, STLAllocatorWrapper<u32>> vector_test{STLAllocatorWrapper<u32>{test_alloc}};

        u32 const array_test[] = {0, 1, 2, 3, 4};

        slw::copy(slw::begin(array_test), slw::end(array_test), back_inserter(vector_test));

        REQUIRE(test_alloc.getStats().allocated_byte != 0U);
        REQUIRE(test_alloc.getStats().allocated_byte >= sizeof(array_test));

        for (u32 idx = 0; idx != slw::size(array_test); ++idx)
        {
            REQUIRE(array_test[idx] == idx);
            REQUIRE(array_test[idx] == vector_test[idx]);
        }
    }

    REQUIRE(test_alloc.getStats().alloc_count == 0U);
    REQUIRE(test_alloc.getStats().allocated_byte == 0U);
}


#if 0

TEST(STL_ALLOCATOR, default)
{

}

#endif