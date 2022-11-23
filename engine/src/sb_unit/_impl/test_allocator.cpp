#include <sb_core/system.h>
#include <sb_core/error.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>

#include <sb_unit/test_allocator.h>

#include <sb_slw/algorithm>

namespace sb {

TestAllocator::~TestAllocator()
{
    sb_assert(_allocs.empty());
}

usize TestAllocator::getAlignment() const
{
    return DEFAULT_MEMORY_ALIGNMENT;
}

sb::MemoryArena TestAllocator::allocate(size_t const size)
{
    MemoryArena mem_arena = sb::getGlobalHeap().allocate(size);

    if (!mem_arena.isEmpty())
    {
        _stats.allocated_byte += size;
        ++_stats.alloc_count;

        auto const alloc_desc = slw::find_if(begin(_allocs), end(_allocs), [mem_arena](AllocDesc const & val) {
            return (val.mem <= mem_arena.data) &&
                   (reinterpret_cast<uintptr_t>(mem_arena.data) <= (reinterpret_cast<uintptr_t>(val.mem) + val.size));
        });
        sb_assert(alloc_desc == end(_allocs));
        _allocs.emplace_back(mem_arena.data, size);
    }

    return mem_arena;
}

sb::MemoryArena TestAllocator::allocate(sb::Alignment alignment, size_t const size)
{
    MemoryArena mem_arena = getGlobalHeap().allocate(alignment, size);

    if (!mem_arena.isEmpty())
    {
        // @todo: we habe to do this because malloc does not return the MemArena of the proper capacity
        _stats.allocated_byte += getGlobalHeap().getBlockSize(mem_arena.data);
        ++_stats.alloc_count;

        auto const alloc_desc = slw::find_if(begin(_allocs), end(_allocs), [mem_arena](AllocDesc const & val) {
            return (val.mem <= mem_arena.data) &&
                   (reinterpret_cast<uintptr_t>(mem_arena.data) < (reinterpret_cast<uintptr_t>(val.mem) + val.size));
        });
        sb_assert(alloc_desc == end(_allocs));
        _allocs.emplace_back(mem_arena.data, size);
    }

    return mem_arena;
}

void TestAllocator::deallocate(MemoryArena arena)
{
    deallocate(arena.data);
}

void TestAllocator::deallocate(void * ptr)
{
    auto & global_heap = sb::getGlobalHeap();

    if (ptr)
    {
        _stats.allocated_byte -= global_heap.getBlockSize(ptr);
        --_stats.alloc_count;

        auto const alloc_desc =
            slw::find_if(begin(_allocs), end(_allocs), [ptr](AllocDesc const & val) { return val.mem == ptr; });
        sb_assert(alloc_desc != end(_allocs));
        _allocs.erase(alloc_desc);
    }

    global_heap.deallocate(ptr);
}

bool TestAllocator::owns(void const * ptr) const
{
    auto const alloc_desc = slw::find_if(begin(_allocs), end(_allocs), [ptr](AllocDesc const & val) {
        return (val.mem <= ptr) &&
               (reinterpret_cast<uintptr_t>(ptr) <= (reinterpret_cast<uintptr_t>(val.mem) + val.size));
    });

    return alloc_desc != end(_allocs);
}

} // namespace sb
