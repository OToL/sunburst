#include <sb_core/memory/memory.h>
#include <sb_core/memory/allocators/global_heap_allocator.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/system.h>

#include <sb_slw/cstdlib>
#include <sb_slw/cstring>

void sb::zeroMemory(void * const memPtr, usize const byteCount)
{
    slw::memset(memPtr, 0, byteCount);
}

[[noreturn]] void sb::notifyOOM([[maybe_unused]] usize requestedSize, char const * message)
{
    outputDebugString(message);

    slw::exit(-1);
}

void * operator new(sb::usize byte_count)
{
    return sb::getGlobalHeap().allocate(byte_count).data;
}

void * operator new[](sb::usize byte_count)
{
    return sb::getGlobalHeap().allocate(byte_count).data;
}

void * operator new(sb::usize byte_count, slw::align_val_t alignment)
{
    return sb::getGlobalHeap().allocate(static_cast<sb::Alignment>(alignment), byte_count).data;
}

void * operator new[](sb::usize byte_count, slw::align_val_t alignment)
{
    return sb::getGlobalHeap().allocate(static_cast<sb::Alignment>(alignment), byte_count).data;
}

void * operator new(sb::usize byte_count, slw::nothrow_t const &) noexcept
{
    return sb::getGlobalHeap().allocate(byte_count).data;
}

void * operator new[](sb::usize byte_count, slw::nothrow_t const &) noexcept
{
    return sb::getGlobalHeap().allocate(byte_count).data;
}

void * operator new(sb::usize byte_count, slw::align_val_t alignment, slw::nothrow_t const &) noexcept
{
    return sb::getGlobalHeap().allocate(static_cast<sb::Alignment>(alignment), byte_count).data;
}

void * operator new[](sb::usize byte_count, slw::align_val_t alignment, slw::nothrow_t const &) noexcept
{
    return sb::getGlobalHeap().allocate(static_cast<sb::Alignment>(alignment), byte_count).data;
}

void operator delete(void * ptr) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap().deallocate(ptr);
    }
}

void operator delete[](void * ptr) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap().deallocate(ptr);
    }
}

void operator delete(void * ptr, slw::align_val_t) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap().deallocate(ptr);
    }
}

void operator delete[](void * ptr, slw::align_val_t) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap().deallocate(ptr);
    }
}

void operator delete(void * ptr, sb::usize) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap().deallocate(ptr);
    }
}

void operator delete[](void * ptr, sb::usize) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap().deallocate(ptr);
    }
}

void operator delete(void * ptr, sb::usize, slw::align_val_t) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap().deallocate(ptr);
    }
}

void operator delete[](void * ptr, sb::usize, slw::align_val_t) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap().deallocate(ptr);
    }
}

void operator delete(void * ptr, const slw::nothrow_t &) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap().deallocate(ptr);
    }
}

void operator delete[](void * ptr, const slw::nothrow_t &) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap().deallocate(ptr);
    }
}

void operator delete(void * ptr, slw::align_val_t, const slw::nothrow_t &) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap().deallocate(ptr);
    }
}

void operator delete[](void * ptr, slw::align_val_t, const slw::nothrow_t &) noexcept
{
    if (nullptr != ptr)
    {
        sb::getGlobalHeap().deallocate(ptr);
    }
}

