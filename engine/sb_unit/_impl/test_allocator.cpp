#include <sb_core/os.h>
#include <sb_core/error.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>

#include <sb_unit/test_allocator.h>

#include <sb_std/algorithm>

namespace sb {

TestAllocator::~TestAllocator()
{
    sbAssert(m_allocs.empty());
}

usize TestAllocator::getAlignment() const
{
    return ALIGNMENT_DEFAULT;
}

sb::MemoryArena TestAllocator::allocate(size_t const size)
{
    MemoryArena mem_arena = sb::getGlobalHeap().allocate(size);

    if (!mem_arena.isEmpty())
    {
        m_stats.m_allocated_byte += size;
        ++m_stats.m_alloc_count;

        auto const alloc_desc = sbstd::find_if(begin(m_allocs), end(m_allocs), [mem_arena](AllocDesc const & val) {
            return (val.m_mem <= mem_arena.m_ptr) && (reinterpret_cast<uintptr_t>(mem_arena.m_ptr) <=
                                                      (reinterpret_cast<uintptr_t>(val.m_mem) + val.m_size));
        });
        sbAssert(alloc_desc == end(m_allocs));
        m_allocs.emplace_back(mem_arena.m_ptr, size);
    }

    return mem_arena;
}

sb::MemoryArena TestAllocator::allocate(size_t const size, sb::Alignment alignment)
{
    MemoryArena mem_arena = getGlobalHeap().allocate(size, alignment);

    if (!mem_arena.isEmpty())
    {
        m_stats.m_allocated_byte += size;
        ++m_stats.m_alloc_count;

        auto const alloc_desc = sbstd::find_if(begin(m_allocs), end(m_allocs), [mem_arena](AllocDesc const & val) {
            return (val.m_mem <= mem_arena.m_ptr) && (reinterpret_cast<uintptr_t>(mem_arena.m_ptr) <=
                                                      (reinterpret_cast<uintptr_t>(val.m_mem) + val.m_size));
        });
        sbAssert(alloc_desc == end(m_allocs));
        m_allocs.emplace_back(mem_arena.m_ptr, size);
    }

    return mem_arena;
}

void TestAllocator::deallocate(MemoryArena arena)
{
    deallocate(arena.m_ptr);
}

void TestAllocator::deallocate(void * ptr)
{
    auto & global_heap = sb::getGlobalHeap();

    if (ptr)
    {
        m_stats.m_allocated_byte -= global_heap.getBlockSize(ptr);
        --m_stats.m_alloc_count;

        auto const alloc_desc =
            sbstd::find_if(begin(m_allocs), end(m_allocs), [ptr](AllocDesc const & val) { return val.m_mem == ptr; });
        sbAssert(alloc_desc != end(m_allocs));
        m_allocs.erase(alloc_desc);
    }

    global_heap.deallocate(ptr);
}

bool TestAllocator::owns(void const * ptr) const
{
    auto const alloc_desc = sbstd::find_if(begin(m_allocs), end(m_allocs), [ptr](AllocDesc const & val) {
        return (val.m_mem <= ptr) &&
               (reinterpret_cast<uintptr_t>(ptr) <= (reinterpret_cast<uintptr_t>(val.m_mem) + val.m_size));
    });

    return alloc_desc != end(m_allocs);
}

} // namespace sb