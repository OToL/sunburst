#pragma once

#include <sb_core/core.h>

namespace sb {

inline i32 countLeadingZeros(u32 val);

inline i32 countLeadingZeros(u64 val);

// returns true for 'val = 0' to avoid a branch
template <typename T>
inline constexpr b8 isPowerOf2(T val)
{
    return (0 == ((val - 1) & val));
}

// http://asawicki.info/news_1688_operations_on_power_of_two_numbers.html
inline u32 getNextPowerOf2(u32 val)
{
    val--;
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    val |= val >> 8;
    val |= val >> 16;
    val++;

    return val;
}

inline u64 getNextPowerOf2(u64 val)
{
    val--;
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    val |= val >> 8;
    val |= val >> 16;
    val |= val >> 32;
    val++;

    return val;
}

inline u32 getPreviousPowerOf2(u32 val)
{
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    val |= val >> 8;
    val |= val >> 16;
    val = val ^ (val >> 1);

    return val;
}

inline u64 getPreviousPowerOf2(u64 val)
{
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    val |= val >> 8;
    val |= val >> 16;
    val |= val >> 32;
    val = val ^ (val >> 1);

    return val;
}

// 'align' has to be a power of 2
template <typename T>
inline constexpr T alignUp(T value, usize align)
{
    return (T)((((usize)value) + align - 1) & ~(align - 1));
}

template <typename T>
inline constexpr b8 isAlignedTo(T val, usize alignment)
{
    return (0 == (((usize)val) & (alignment - 1)));
}
} // namespace sb

#if defined(SB_PLATFORM_WINDOWS)
#    include "_impl/bit_win.h"
#elif defined(SB_PLATFORM_LINUX)
#    include "_impl/bit_linux.h"
#else
#    error "Unsupported platform"
#endif
