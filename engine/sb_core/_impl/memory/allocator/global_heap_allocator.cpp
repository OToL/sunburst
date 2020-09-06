#include <sb_core/error.h>
#include <sb_core/memory/allocator/allocator_view.h>
#include <sb_core/memory/allocator/global_heap_allocator.h>
#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/alloc.h>

namespace sb {

void * GlobalHeapAllocator::allocate(usize const size)
{
    return sbMalloc(size);
}

void * GlobalHeapAllocator::allocate(usize const size, Alignment const alignment)
{
    return sbMalloc(size, alignment);
}

void GlobalHeapAllocator::deallocate(void * ptr)
{
    sbFree(ptr);
}

usize GlobalHeapAllocator::getBlockSize(void * ptr) const
{
    return sbMallocUsabeSize(ptr);
}

b8 GlobalHeapAllocator::owns(void const *) const
{
    sbNotImplemented(__FUNCTION__);

    return false;
}

} // namespace sb
