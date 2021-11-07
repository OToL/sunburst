#include <sb_core/core.h>

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
void zeroStruct(T (&objPtr)[COUNT])
{
    zeroMemory(objPtr, sizeof(T) * COUNT);
}

[[noreturn]] void notifyOOM(usize requestedSize, char const * message);

}
