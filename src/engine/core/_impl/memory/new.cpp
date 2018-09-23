#include <core/memory/allocator/allocator_view.h>
#include <core/memory/allocator/global_heap_allocator.h>
#include <core/memory/new.h>
#include <core/memory/global_heap.h>

using namespace sb;

void * operator new(usize byte_count)
{
    return sb::getGlobalHeap()->allocate(byte_count);
}

void * operator new(sb::usize byte_count, wstd::nothrow_t const &) noexcept
{
    return sb::getGlobalHeap()->allocate(byte_count);
}

void * operator new(sb::usize byte_count, wstd::align_val_t alignment)
{
    return sb::getGlobalHeap()->allocate(byte_count, (usize)alignment);
}

void * operator new[](sb::usize byte_count, wstd::align_val_t alignment)
{
    return sb::getGlobalHeap()->allocate(byte_count, (usize)alignment);
}

void * operator new[](usize byte_count)
{
    return sb::getGlobalHeap()->allocate(byte_count);
}

void * operator new[](sb::usize byte_count, wstd::nothrow_t const &) noexcept
{
    return sb::getGlobalHeap()->allocate(byte_count);
}

void operator delete(void * ptr)noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap()->deallocate(ptr);
    }
}

void operator delete(void * ptr, wstd::nothrow_t const &)noexcept
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

void operator delete[](void * ptr, wstd::nothrow_t const &) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap()->deallocate(ptr);
    }
}

void operator delete(void * ptr, wstd::align_val_t)noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap()->deallocate(ptr);
    }
}

void operator delete[](void * ptr, wstd::align_val_t) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap()->deallocate(ptr);
    }
}
