#pragma once

#include <sb_core/core.h>

#if defined(SB_PLATFORM_WINDOWS)
#    include <sb_core/_impl/memory/memory_win.h>
#else
#    error "Unsupported platform"
#endif

#include <sb_slw/new>

namespace sb {

inline constexpr Alignment DEFAULT_MEMORY_ALIGNMENT = 8ULL;
using internal::MEMORY_PAGE_SIZE;

// 'align' has to be a power of 2
template <typename T>
inline constexpr T alignUp(T value, usize align)
{
    return (T)((((usize)value) + align - 1) & ~(align - 1));
}

template <typename T>
inline constexpr b8 isAlignedTo(T val, usize alignment)
{
    return (0 == (((usize)val) & (alignment - 1)));
}

void zeroMemory(void * const memPtr, usize const byteCount);

template <typename T>
void zeroMemory(T * const objPtr)
{
    zeroMemory(objPtr, sizeof(T));
}

template <typename T>
void zeroMemory(T & obj)
{
    zeroMemory(&obj, sizeof(T));
}

template <typename T, usize COUNT>
void zeroMemory(T (&objPtr)[COUNT])
{
    zeroMemory(objPtr, sizeof(T) * COUNT);
}

[[noreturn]] void notifyOOM(usize requestedSize, char const * message);

} // namespace sb

#define sb_malloc(heap, ...) heap.allocate(__VA_ARGS__)
#define sb_malloc_usable_size(heap, ptr) heap.getBlockSize(ptr)
#define sb_free(heap, ptr) heap.deallocate(ptr)

#define sb_new(heap, type, ...) sb::internal::newImpl<type>(heap, ##__VA_ARGS__)
#define sb_delete(heap, obj) sb::internal::deleteImpl(heap, obj)

void * operator new(sb::usize byte_count);
void * operator new[](sb::usize byte_count);

void * operator new(sb::usize byte_count, slw::align_val_t alignment);
void * operator new[](sb::usize byte_count, slw::align_val_t alignment);

void * operator new(sb::usize byte_count, slw::nothrow_t const & tag) noexcept;
void * operator new[](sb::usize byte_count, slw::nothrow_t const & tag) noexcept;

void * operator new(sb::usize byte_count, slw::align_val_t alignment, slw::nothrow_t const & tag) noexcept;
void * operator new[](sb::usize byte_count, slw::align_val_t alignment, slw::nothrow_t const & tag) noexcept;

// placement
// void* operator new  ( sb::usize count, void* ptr );
// void* operator new[]( sb::usize count, void* ptr );

void operator delete(void * ptr) noexcept;
void operator delete[](void * ptr) noexcept;

void operator delete(void * ptr, slw::align_val_t al) noexcept;
void operator delete[](void * ptr, slw::align_val_t al) noexcept;

void operator delete(void * ptr, sb::usize sz) noexcept;
void operator delete[](void * ptr, sb::usize sz) noexcept;

void operator delete(void * ptr, sb::usize sz, slw::align_val_t al) noexcept;
void operator delete[](void * ptr, sb::usize sz, slw::align_val_t al) noexcept;

void operator delete(void * ptr, const slw::nothrow_t & tag) noexcept;
void operator delete[](void * ptr, const slw::nothrow_t & tag) noexcept;

void operator delete(void * ptr, slw::align_val_t al, const slw::nothrow_t & tag) noexcept;
void operator delete[](void * ptr, slw::align_val_t al, const slw::nothrow_t & tag) noexcept;

// placement
// void* operator new  ( sb::usize count, void* ptr ) noexcept;
// void* operator new[]( sb::usize count, void* ptr ) noexcept;
// void operator delete  ( void* ptr, void* place ) noexcept;
// void operator delete[]( void* ptr, void* place ) noexcept;

#include <sb_core/_impl/memory/memory.h>
