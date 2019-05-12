#include <gtest/allocator_stats.h>

#include <core/memory/allocator/global_heap_allocator.h>
#include <core/memory/global_heap.h>
#include <core/error.h>

#include <libc++/cassert>
#include <libc++/algorithm>

using namespace sb;

namespace gtest::ext {

AllocatorStats::~AllocatorStats()
{
    assert(m_allocs.empty());
}

void AllocatorStats::deallocate(void * ptr)
{
    auto const global_heap = sb::getGlobalHeap();

    if (ptr)
    {
        m_stats.m_allocated_byte -= global_heap->getBlockSize(ptr);
        --m_stats.m_alloc_count;

        auto const alloc_desc = wstd::find_if(begin(m_allocs), end(m_allocs), [ptr] (AllocDesc const & val) {return val.m_mem == ptr;} );
        assert(alloc_desc != end(m_allocs));
        m_allocs.erase(alloc_desc);
    }

    global_heap->deallocate(ptr);
}

void * AllocatorStats::allocate(usize const size)
{
    void * const mem_ptr = sb::getGlobalHeap()->allocate(size);

    if (nullptr != mem_ptr)
    {
        m_stats.m_allocated_byte += size;
        ++m_stats.m_alloc_count;

        auto const alloc_desc = wstd::find_if(begin(m_allocs), end(m_allocs), 
            [mem_ptr] (AllocDesc const & val) 
                {
                    return (val.m_mem <= mem_ptr) && (reinterpret_cast<uintptr_t>(mem_ptr) <= (reinterpret_cast<uintptr_t>(val.m_mem) + val.m_size));
                } 
            );
        assert(alloc_desc == end(m_allocs));
        m_allocs.emplace_back(mem_ptr, size);
    }

    return mem_ptr;
}

void * AllocatorStats::allocate(usize const size, usize const alignment)
{
    void * mem_ptr = sb::getGlobalHeap()->allocate(size, alignment);

    if (nullptr != mem_ptr)
    {
        m_stats.m_allocated_byte += size;
        ++m_stats.m_alloc_count;

        auto const alloc_desc = wstd::find_if(begin(m_allocs), end(m_allocs), 
            [mem_ptr] (AllocDesc const & val) 
                {
                    return (val.m_mem <= mem_ptr) && (reinterpret_cast<uintptr_t>(mem_ptr) <= (reinterpret_cast<uintptr_t>(val.m_mem) + val.m_size));
                } 
            );
        assert(alloc_desc == end(m_allocs));
        m_allocs.emplace_back(mem_ptr, size);
    }

    return mem_ptr;
}

void AllocatorStats::deallocateAll()
{
    sb::getGlobalHeap()->deallocateAll();
}

b8 AllocatorStats::owns(void const * mem_ptr) const
{
    auto const alloc_desc = wstd::find_if(begin(m_allocs), end(m_allocs), 
            [mem_ptr] (AllocDesc const & val) 
                {
                    return (val.m_mem <= mem_ptr) && (reinterpret_cast<uintptr_t>(mem_ptr) <= (reinterpret_cast<uintptr_t>(val.m_mem) + val.m_size));
                } 
            );

    return alloc_desc != end(m_allocs);
}

} // namespace wgtest
