
#include <core/string/utility.h>
#include <core/memory/allocator/allocator_view.h>
#include <core/memory/allocator/global_heap_allocator.h>
#include <core/memory/memory.h>
#include <core/memory/global_heap.h>

#include <libc++/cstdlib>
#include <libc++/cstring>

namespace sb {

void zeroMemory(void * const memPtr, usize const byteCount)
{
    wstd::memset(memPtr, 0, byteCount);
}

[[noreturn]] void notifyOOM([[maybe_unused]] usize  requestedSize, char const * message)
{
    outputDebugString(message);

    wstd::exit(-1);
}

} // namespace sb

void * operator new(sb::usize byte_count)
{
    return sb::getGlobalHeap()->allocate(byte_count);
}

void * operator new(sb::usize byte_count, wstd::nothrow_t const &) noexcept
{
    return sb::getGlobalHeap()->allocate(byte_count);
}

void * operator new(sb::usize byte_count, wstd::align_val_t alignment)
{
    return sb::getGlobalHeap()->allocate(byte_count, (sb::usize)alignment);
}

void * operator new[](sb::usize byte_count, wstd::align_val_t alignment)
{
    return sb::getGlobalHeap()->allocate(byte_count, (sb::usize)alignment);
}

void * operator new[](sb::usize byte_count)
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
