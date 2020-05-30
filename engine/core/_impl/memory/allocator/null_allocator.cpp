#include <core/memory/allocator/null_allocator.h>
#include <core/error.h>

namespace sb {

NullAllocator::NullAllocator(InitParams const &)
{
}

void * NullAllocator::allocate(usize const)
{
    return nullptr;
}

void * NullAllocator::allocate(usize const, Alignment const)
{
    return nullptr;
}

void NullAllocator::deallocate([[maybe_unused]] void * ptr)
{
    sbWarn(nullptr == ptr);
}

void NullAllocator::deallocateAll() 
{
}

b8 NullAllocator::owns(void const * ptr) const
{
    return (nullptr == ptr);
}

}
