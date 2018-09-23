#pragma once

#define sbNew(type, ...)              \
    sb::MemoryOperatorHelperNew<type> \
    {                                 \
        __VA_ARGS__                   \
    }

#define sbDelete(ptr, ...) sb::MemoryOperatorHelperDelete<typename wstd::remove_pointer<decltype(ptr)>::type>::Destroy(ptr, ##__VA_ARGS__);

#define sbNewArray(type, ...)              \
    sb::MemoryOperatorHelperNewArray<type> \
    {                                      \
        __VA_ARGS__                        \
    }

#define sbDeleteArray(ptr, ...) sb::MemoryOperatorHelperDeleteArray<typename wstd::remove_pointer<decltype(ptr)>::type>::DestroyArray(ptr, ##__VA_ARGS__)

#include <core/_impl/memory/new.h>
