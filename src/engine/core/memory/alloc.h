#pragma once

namespace sb
{

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

}

#include <core/_impl/memory/alloc.h>
