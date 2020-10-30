#pragma once

#include <sb_core/core.h>

namespace sb {

enum Alignment : usize
{
    ALIGNMENT_1B = 1,
    ALIGNMENT_2B = 2,
    ALIGNMENT_4B = 4,
    ALIGNMENT_8B = 8,
    ALIGNMENT_16B = 16,
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

template <typename TType>
constexpr inline Alignment alignOf()
{
    return (Alignment)alignof(TType);
}

} // namespace sb
