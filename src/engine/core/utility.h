#pragma once

#include "platform.h"

namespace sb {

// Returns number of characters copied without the terminating '\0'
usize strCpyT(char * sb_restrict output, usize capacity, char const * sb_restrict src);

/// Returns number of characters copied without the terminating '\0'
template <usize COUNT>
inline usize strCpyT(char (&output)[COUNT], char const * sb_restrict src)
{
    return strCpyT(&output[0], COUNT, src);
}

// Returns true if 'str' begins with 'start_str'
bool strStartWith(char const * sb_restrict str, char const * sb_restrict start_str);

void zeroMemory(void * const memPtr, usize const byteCount);

template <typename T>
void zeroStruct(T * const objPtr)
{
    zeroMemory(objPtr, sizeof(T));
}

template <typename T, usize COUNT>
void zeroStructArray(T (&objPtr) [COUNT])
{
    zeroMemory(objPtr, sizeof(T) * COUNT);
}

template <typename T>
void zeroStruct(T & obj)
{
    zeroMemory(&obj, sizeof(T));
}

[[noreturn]] void notifyOOM(usize requestedSize, char const * message);

} // namespace sb
