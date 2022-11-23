#pragma once

#include <sb_core/core.h>

namespace sb {

// Custom implementation because of frequent 'min' name clash
template <class T>
const T & minValue(const T & a, const T & b)
{
    return (b < a) ? b : a;
}

// Custom implementation because of frequent 'max' name clash
template <class T>
const T & maxValue(const T & a, const T & b)
{
    return (b > a) ? b : a;
}


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

} // namespace sb

