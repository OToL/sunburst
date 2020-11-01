#include <sb_core/os.h>
#include <sb_core/error.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>

#include <sb_unit/test_allocator.h>

#include <sb_std/algorithm>

namespace sb {

TestAllocator::~TestAllocator()
{
    sbAssert(_allocs.empty());
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
        _stats._allocated_byte += size;
        ++_stats._alloc_count;

        auto const alloc_desc = sbstd::find_if(begin(_allocs), end(_allocs), [mem_arena](AllocDesc const & val) {
            return (val._mem <= mem_arena.data) &&
                   (reinterpret_cast<uintptr_t>(mem_arena.data) <= (reinterpret_cast<uintptr_t>(val._mem) + val.size));
        });
        sbAssert(alloc_desc == end(_allocs));
        _allocs.emplace_back(mem_arena.data, size);
    }

    return mem_arena;
}

sb::MemoryArena TestAllocator::allocate(size_t const size, sb::Alignment alignment)
{
    MemoryArena mem_arena = getGlobalHeap().allocate(size, alignment);

    if (!mem_arena.isEmpty())
    {
        _stats._allocated_byte += size;
        ++_stats._alloc_count;

        auto const alloc_desc = sbstd::find_if(begin(_allocs), end(_allocs), [mem_arena](AllocDesc const & val) {
            return (val._mem <= mem_arena.data) &&
                   (reinterpret_cast<uintptr_t>(mem_arena.data) <= (reinterpret_cast<uintptr_t>(val._mem) + val.size));
        });
        sbAssert(alloc_desc == end(_allocs));
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
        _stats._allocated_byte -= global_heap.getBlockSize(ptr);
        --_stats._alloc_count;

        auto const alloc_desc =
            sbstd::find_if(begin(_allocs), end(_allocs), [ptr](AllocDesc const & val) { return val._mem == ptr; });
        sbAssert(alloc_desc != end(_allocs));
        _allocs.erase(alloc_desc);
    }

    global_heap.deallocate(ptr);
}

bool TestAllocator::owns(void const * ptr) const
{
    auto const alloc_desc = sbstd::find_if(begin(_allocs), end(_allocs), [ptr](AllocDesc const & val) {
        return (val._mem <= ptr) &&
               (reinterpret_cast<uintptr_t>(ptr) <= (reinterpret_cast<uintptr_t>(val._mem) + val.size));
    });

    return alloc_desc != end(_allocs);
}

} // namespace sb