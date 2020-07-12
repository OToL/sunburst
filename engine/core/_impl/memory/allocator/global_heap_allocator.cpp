#include <core/error.h>
#include <core/memory/allocator/allocator_view.h>
#include <core/memory/allocator/global_heap_allocator.h>
#include <core/base.h>
#include <core/memory/memory.h>
#include <core/memory/alloc.h>

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
