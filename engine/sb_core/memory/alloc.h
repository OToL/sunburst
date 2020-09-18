#pragma once

namespace sb {

// TODO: return arena
#define sbMalloc(size, ...) sb::internal::malloc(size, ##__VA_ARGS__)

#define sbMallocUsabeSize(ptr) sb::internal::mallocUsableSize(ptr)

#define sbFree(ptr) sb::internal::free(ptr)

#define sbNew(type, ...)                                                                           \
    sb::internal::MemoryOperatorHelperNew<type>                                                    \
    {                                                                                              \
        __VA_ARGS__                                                                                \
    }

#define sbDelete(ptr, ...)                                                                         \
    sb::internal::MemoryOperatorHelperDelete<                                                      \
        typename sbstd::remove_pointer<decltype(ptr)>::type>::Destroy(ptr, ##__VA_ARGS__);

#define sbNewArray(type, ...)                                                                      \
    sb::internal::MemoryOperatorHelperNewArray<type>                                               \
    {                                                                                              \
        __VA_ARGS__                                                                                \
    }

#define sbDeleteArray(ptr, ...)                                                                    \
    sb::internal::MemoryOperatorHelperDeleteArray<                                                 \
        typename sbstd::remove_pointer<decltype(ptr)>::type>::DestroyArray(ptr, ##__VA_ARGS__)

} // namespace sb

#include <sb_core/_impl/memory/alloc.h>
