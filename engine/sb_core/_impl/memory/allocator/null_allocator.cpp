#include <sb_core/memory/allocator/null_allocator.h>
#include <sb_core/error/error.h>

sb::NullAllocator::NullAllocator(InitParams const &) { }

sb::MemoryArena sb::NullAllocator::allocate(usize const)
{
    return {};
}

sb::MemoryArena sb::NullAllocator::allocate(Alignment const, usize const)
{
    return {};
}

void sb::NullAllocator::deallocate([[maybe_unused]] void * ptr)
{
    sb_warn(nullptr == ptr);
}

void sb::NullAllocator::deallocate(MemoryArena arena)
{
    sb_warn(memory_arena::isEmpty(arena));
}

void sb::NullAllocator::deallocateAll() { }

sb::b8 sb::NullAllocator::owns(void const * /*ptr*/) const
{
    return false;
}
