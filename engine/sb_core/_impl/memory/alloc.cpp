#include <sb_core/memory/allocator/global_heap_allocator.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/alloc.h>
#include <sb_core/memory/global_heap.h>

void * operator new(sb::usize byte_count)
{
    return sb::getGlobalHeap()->allocate(byte_count);
}

void * operator new(sb::usize byte_count, sbstd::nothrow_t const &) noexcept
{
    return sb::getGlobalHeap()->allocate(byte_count);
}

void * operator new(sb::usize byte_count, sbstd::align_val_t alignment)
{
    return sb::getGlobalHeap()->allocate(byte_count, (sb::Alignment)alignment);
}

void * operator new[](sb::usize byte_count, sbstd::align_val_t alignment)
{
    return sb::getGlobalHeap()->allocate(byte_count, (sb::Alignment)alignment);
}

void * operator new[](sb::usize byte_count)
{
    return sb::getGlobalHeap()->allocate(byte_count);
}

void * operator new[](sb::usize byte_count, sbstd::nothrow_t const &) noexcept
{
    return sb::getGlobalHeap()->allocate(byte_count);
}

void operator delete(void * ptr) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap()->deallocate(ptr);
    }
}

void operator delete(void * ptr, sbstd::nothrow_t const &) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap()->deallocate(ptr);
    }
}

void operator delete[](void * ptr) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap()->deallocate(ptr);
    }
}

void operator delete[](void * ptr, sbstd::nothrow_t const &) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap()->deallocate(ptr);
    }
}

void operator delete(void * ptr, sbstd::align_val_t) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap()->deallocate(ptr);
    }
}

void operator delete[](void * ptr, sbstd::align_val_t) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap()->deallocate(ptr);
    }
}
