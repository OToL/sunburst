#include <sb_core/memory/allocator/null_allocator.h>
#include <sb_core/error.h>

sb::NullAllocator::NullAllocator(InitParams const &) { }

void * sb::NullAllocator::allocate(usize const)
{
    return nullptr;
}

void * sb::NullAllocator::allocate(usize const, Alignment const)
{
    return nullptr;
}

void sb::NullAllocator::deallocate([[maybe_unused]] void * ptr)
{
    sbWarn(nullptr == ptr);
}

void sb::NullAllocator::deallocateAll() { }

sb::b8 sb::NullAllocator::owns(void const * ptr) const
{
    return (nullptr == ptr);
}
