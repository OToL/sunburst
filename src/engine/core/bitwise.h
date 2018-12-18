#pragma once

#include "platform.h"

namespace sb {

inline ui32 countLeadingZeros(ui32 val);

inline ui32 countLeadingZeros(ui64 val);

// returns true for 'val = 0' to avoid a branch
template <typename T>
inline constexpr bool isPowerOf2(T val)
{
    return (0 == ((val - 1) & val));
}

// http://asawicki.info/news_1688_operations_on_power_of_two_numbers.html
inline ui32 nextPowerOf2(ui32 val)
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

inline ui64 nextPowerOf2(ui64 val)
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

inline ui32 prevPowerOf2(ui32 val)
{
    val |= val >> 1; 
    val |= val >> 2; 
    val |= val >> 4; 
    val |= val >> 8;
    val |= val >> 16;
    val = val ^ (val >> 1);

    return val;
}

inline ui64 prevPowerOf2(ui64 val)
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
}

#if defined(SB_TARGET_WINDOWS)
#    include "_pimpl/windows/bitwise_win.h"
#else
#    error "Unsupported compiler"
#endif
