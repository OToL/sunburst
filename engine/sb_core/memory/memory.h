#pragma once

#include <sb_core/core.h>

#include <sb_std/new>

namespace sb {

enum Alignment : usize
{
    ALIGNMENT_1B = 1,
    ALIGNMENT_2B = 2,
    ALIGNMENT_4B = 4,
    ALIGNMENT_8B = 8,
    ALIGNMENT_16B = 16,
    ALIGNMENT_32B = 32,
    ALIGNMENT_64B = 64,
    ALIGNMENT_128B = 128,

    ALIGNMENT_DEFAULT = ALIGNMENT_8B
};

void zeroMemory(void * const memPtr, usize const byteCount);

template <typename T>
void zeroStruct(T * const objPtr)
{
    zeroMemory(objPtr, sizeof(T));
}

template <typename T>
void zeroStruct(T & obj)
{
    zeroMemory(&obj, sizeof(T));
}

template <typename T, usize COUNT>
void zeroStruct(T (&objPtr)[COUNT])
{
    zeroMemory(objPtr, sizeof(T) * COUNT);
}

[[noreturn]] void notifyOOM(usize requestedSize, char const * message);

// @todo: not sure to like it ... we should use alignof directly in the code
// and make sure Alignment covers all possible values
template <typename TType>
constexpr inline Alignment alignOf()
{
    return (Alignment)alignof(TType);
}

} // namespace sb

#define sbMalloc(heap, ...) heap.allocate(__VA_ARGS__)
#define sbMallocUsabeSize(heap, ptr) heap.getBlockSize(ptr)
#define sbFree(heap, ptr) heap.deallocate(ptr)

#define sbNew(heap, type, ...) sb::internal::newImpl<type>(heap, ##__VA_ARGS__)
#define sbDelete(heap, obj) sb::internal::deleteImpl(heap, obj)

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
