#include <core/memory/allocator/null_allocator.h>

#include <gtest/gtest_common.h>

namespace sb::test {

TEST(NULL_ALLOCATOR, Allocate)
{
    NullAllocator alloc;

    void * const mem_ptr = alloc.allocate(10);

    EXPECT_EQ(nullptr, mem_ptr);

    alloc.deallocate(mem_ptr);
}

TEST(NULL_ALLOCATOR, AlignedAllocate)
{
    NullAllocator alloc;

    void * const mem_ptr = alloc.allocate(10, 16);

    EXPECT_EQ(nullptr, mem_ptr);

    alloc.deallocate(mem_ptr);
}

} // namespace sb::test
