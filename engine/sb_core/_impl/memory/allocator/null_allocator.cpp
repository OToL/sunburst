#include <sb_core/memory/allocator/null_allocator.h>
#include <sb_core/error.h>

sb::NullAllocator::NullAllocator(InitParams const &) { }

sb::MemoryArena sb::NullAllocator::allocate(usize const)
{
    return {};
}

sb::MemoryArena sb::NullAllocator::allocate(usize const, Alignment const)
{
    return {};
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
