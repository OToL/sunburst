#include <gtest/allocator_stats.h>

#include <core/memory/allocator/global_heap_allocator.h>
#include <core/memory/global_heap.h>
#include <core/error.h>

#include <libc++/cstdlib>

using namespace sb;

namespace gtest::ext {

void AllocatorStats::deallocate(void * ptr)
{
    auto const global_heap = sb::getGlobalHeap();

    if (ptr)
    {
        m_stats.m_allocated_byte -= global_heap->getBlockSize(ptr);
        --m_stats.m_alloc_count;
    }

    global_heap->deallocate(ptr);
}

void * AllocatorStats::allocate(usize const size)
{
    void * const memPtr = sb::getGlobalHeap()->allocate(size);

    if (memPtr)
    {
        m_stats.m_allocated_byte += size;
        ++m_stats.m_alloc_count;
    }

    return memPtr;
}

void * AllocatorStats::allocate(usize const size, usize const alignment)
{
    void * memPtr = sb::getGlobalHeap()->allocate(size, alignment);

    if (memPtr)
    {
        m_stats.m_allocated_byte += size;
        ++m_stats.m_alloc_count;
    }

    return memPtr;
}

void AllocatorStats::deallocateAll()
{
    sb::getGlobalHeap()->deallocateAll();
}

b8 AllocatorStats::owns(void const *) const
{
    sbNotImplemented(__FUNCTION__);

    return false;
}

} // namespace wgtest
