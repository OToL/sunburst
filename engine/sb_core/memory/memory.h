#pragma once

#include <sb_core/core.h>

namespace sb {

enum Alignment : usize
{
    ALIGN_4B = 4,
    ALIGN_8B = 8,
    ALIGN_16B = 16,

    ALIGN_DEFAULT = ALIGN_8B
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
void zeroStructArray(T (&objPtr)[COUNT])
{
    zeroMemory(objPtr, sizeof(T) * COUNT);
}

[[noreturn]] void notifyOOM(usize requestedSize, char const * message);

template <typename TType>
constexpr inline Alignment alignOf()
{
    return (Alignment)alignof(TType);
}

} // namespace sb
