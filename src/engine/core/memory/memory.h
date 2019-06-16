#pragma once

#include <core/platform.h>

namespace sb {

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
void zeroStructArray(T (&objPtr) [COUNT])
{
    zeroMemory(objPtr, sizeof(T) * COUNT);
}

[[noreturn]] void notifyOOM(usize requestedSize, char const * message);

}

#define sbMalloc(size, ...) sb::detail::malloc(size, ##__VA_ARGS__)

#define sbMallocUsabeSize(ptr) sb::detail::mallocUsableSize(ptr)

#define sbFree(ptr) sb::detail::free(ptr)

#define sbNew(type, ...)              \
    sb::detail::MemoryOperatorHelperNew<type> \
    {                                 \
        __VA_ARGS__                   \
    }

#define sbDelete(ptr, ...) sb::detail::MemoryOperatorHelperDelete<typename wstd::remove_pointer<decltype(ptr)>::type>::Destroy(ptr, ##__VA_ARGS__);

#define sbNewArray(type, ...)              \
    sb::detail::MemoryOperatorHelperNewArray<type> \
    {                                      \
        __VA_ARGS__                        \
    }

#define sbDeleteArray(ptr, ...) sb::detail::MemoryOperatorHelperDeleteArray<typename wstd::remove_pointer<decltype(ptr)>::type>::DestroyArray(ptr, ##__VA_ARGS__)

#include <core/_impl/memory/memory.h>
