#include <core/_impl/memory/malloc.h>
#include <core/error.h>
#include <core/memory/allocator/allocator_view.h>
#include <core/memory/allocator/global_heap_allocator.h>
#include <core/platform.h>

namespace sb {

void * GlobalHeapAllocator::allocate(usize const size)
{
    return sb::malloc(size);
}

void * GlobalHeapAllocator::allocate(usize const size, usize const alignment)
{
    return sb::malloc(size, alignment);
}

void GlobalHeapAllocator::deallocate(void * ptr)
{
    sb::free(ptr);
}

usize GlobalHeapAllocator::getBlockSize(void * ptr) const
{
    return sb::mallocUsableSize(ptr);
}

void GlobalHeapAllocator::deallocateAll()
{
    sbNotImplemented(__FUNCTION__);
}

b8 GlobalHeapAllocator::owns(void const *) const
{
    sbNotImplemented(__FUNCTION__);

    return false;
}

} // namespace sb
