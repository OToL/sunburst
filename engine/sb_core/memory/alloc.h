#pragma once

#include "memory_tag.h"

#include <sb_std/new>

// TODO: return arena
#define sbMalloc(size, ...) sb::internal::malloc(size, ##__VA_ARGS__)
#define sbMallocUsabeSize(ptr) sb::internal::mallocUsableSize(ptr)
#define sbFree(ptr) sb::internal::free(ptr)

#define sbNew new
#define sbDelete delete

void * operator new(sb::usize byte_count);
void * operator new(sb::usize byte_count, sb::MemoryTag tag);
void * operator new(sb::usize byte_count, sbstd::nothrow_t const & tag) noexcept;
void * operator new(sb::usize byte_count, sbstd::align_val_t alignment);
void * operator new[](sb::usize byte_count, sb::MemoryTag tag);
void * operator new[](sb::usize byte_count, sbstd::nothrow_t const & tag) noexcept;
void * operator new[](sb::usize byte_count, sbstd::align_val_t alignment);

void operator delete(void * ptr) noexcept;
void operator delete(void * ptr, sb::MemoryTag tag) noexcept;
void operator delete(void * ptr, sbstd::nothrow_t const & tag) noexcept;
void operator delete(void *, sbstd::align_val_t alignment, sb::MemoryTag tag) noexcept;
void operator delete[](void * ptr) noexcept;
void operator delete[](void * ptr, sbstd::nothrow_t const & tag) noexcept;
void operator delete[](void *, sbstd::align_val_t alignment) noexcept;

#include <sb_core/_impl/memory/alloc.h>
