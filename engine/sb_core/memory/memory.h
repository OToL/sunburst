#pragma once

#include <sb_core/core.h>

#include <sb_std/new>

namespace sb {

inline constexpr Alignment DEFAULT_MEMORY_ALIGNMENT = 8ULL;

} // namespace sb

#define sb_malloc(heap, ...) heap.allocate(__VA_ARGS__)
#define sb_malloc_usable_size(heap, ptr) heap.getBlockSize(ptr)
#define sb_free(heap, ptr) heap.deallocate(ptr)

#define sb_new(heap, type, ...) sb::internal::newImpl<type>(heap, ##__VA_ARGS__)
#define sb_delete(heap, obj) sb::internal::deleteImpl(heap, obj)

void * operator new(sb::usize byte_count);
void * operator new[](sb::usize byte_count);

void * operator new(sb::usize byte_count, sbstd::align_val_t alignment);
void * operator new[](sb::usize byte_count, sbstd::align_val_t alignment);

void * operator new(sb::usize byte_count, sbstd::nothrow_t const & tag) noexcept;
void * operator new[](sb::usize byte_count, sbstd::nothrow_t const & tag) noexcept;

void * operator new(sb::usize byte_count, sbstd::align_val_t alignment, sbstd::nothrow_t const & tag) noexcept;
void * operator new[](sb::usize byte_count, sbstd::align_val_t alignment, sbstd::nothrow_t const & tag) noexcept;

// placement
// void* operator new  ( sb::usize count, void* ptr );
// void* operator new[]( sb::usize count, void* ptr );

void operator delete(void * ptr) noexcept;
void operator delete[](void * ptr) noexcept;

void operator delete(void * ptr, sbstd::align_val_t al) noexcept;
void operator delete[](void * ptr, sbstd::align_val_t al) noexcept;

void operator delete(void * ptr, sb::usize sz) noexcept;
void operator delete[](void * ptr, sb::usize sz) noexcept;

void operator delete(void * ptr, sb::usize sz, sbstd::align_val_t al) noexcept;
void operator delete[](void * ptr, sb::usize sz, sbstd::align_val_t al) noexcept;

void operator delete(void * ptr, const sbstd::nothrow_t & tag) noexcept;
void operator delete[](void * ptr, const sbstd::nothrow_t & tag) noexcept;

void operator delete(void * ptr, sbstd::align_val_t al, const sbstd::nothrow_t & tag) noexcept;
void operator delete[](void * ptr, sbstd::align_val_t al, const sbstd::nothrow_t & tag) noexcept;

// placement
// void* operator new  ( sb::usize count, void* ptr ) noexcept;
// void* operator new[]( sb::usize count, void* ptr ) noexcept;
// void operator delete  ( void* ptr, void* place ) noexcept;
// void operator delete[]( void* ptr, void* place ) noexcept;

#include <sb_core/_impl/memory/memory.h>
