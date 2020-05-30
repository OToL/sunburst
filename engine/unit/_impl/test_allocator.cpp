#include <unit/test_allocator.h>

#include <core/os.h>
#include <core/error.h>

#include <libc++/algorithm>

namespace sb {

TestAllocator::~TestAllocator()
{
    sbAssert(m_allocs.empty());
}

usize TestAllocator::getAlignment() const
{
    return GLOBAL_HEAP_ALIGNMENT;
}

void * TestAllocator::allocate(size_t const size)
{
    void * const mem_ptr = sb::getGlobalHeap()->allocate(size);

    if (nullptr != mem_ptr)
    {
        m_stats.m_allocated_byte += size;
        ++m_stats.m_alloc_count;

        auto const alloc_desc =
            wstd::find_if(begin(m_allocs), end(m_allocs), [mem_ptr](AllocDesc const & val) {
                return (val.m_mem <= mem_ptr) &&
                       (reinterpret_cast<uintptr_t>(mem_ptr) <=
                        (reinterpret_cast<uintptr_t>(val.m_mem) + val.m_size));
            });
        sbAssert(alloc_desc == end(m_allocs));
        m_allocs.emplace_back(mem_ptr, size);
    }

    return mem_ptr;
}

void * TestAllocator::allocate(size_t const size, sb::Alignment alignment)
{
    void * mem_ptr = getGlobalHeap()->allocate(size, alignment);

    if (nullptr != mem_ptr)
    {
        m_stats.m_allocated_byte += size;
        ++m_stats.m_alloc_count;

        auto const alloc_desc =
            wstd::find_if(begin(m_allocs), end(m_allocs), [mem_ptr](AllocDesc const & val) {
                return (val.m_mem <= mem_ptr) &&
                       (reinterpret_cast<uintptr_t>(mem_ptr) <=
                        (reinterpret_cast<uintptr_t>(val.m_mem) + val.m_size));
            });
        sbAssert(alloc_desc == end(m_allocs));
        m_allocs.emplace_back(mem_ptr, size);
    }

    return mem_ptr;
}

void TestAllocator::deallocate(void * ptr)
{
    auto const global_heap = sb::getGlobalHeap();

    if (ptr)
    {
        m_stats.m_allocated_byte -= global_heap->getBlockSize(ptr);
        --m_stats.m_alloc_count;

        auto const alloc_desc =
            wstd::find_if(begin(m_allocs), end(m_allocs),
                          [ptr](AllocDesc const & val) { return val.m_mem == ptr; });
        sbAssert(alloc_desc != end(m_allocs));
        m_allocs.erase(alloc_desc);
    }

    global_heap->deallocate(ptr);
}

bool TestAllocator::owns(void const * ptr) const
{
    auto const alloc_desc =
        wstd::find_if(begin(m_allocs), end(m_allocs), [ptr](AllocDesc const & val) {
            return (val.m_mem <= ptr) && (reinterpret_cast<uintptr_t>(ptr) <=
                                          (reinterpret_cast<uintptr_t>(val.m_mem) + val.m_size));
        });

    return alloc_desc != end(m_allocs);
}

} // namespace sb