#pragma once

#include <sb_std/bit>

inline sb::s32 sb::countLeadingZeros(u32 val)
{
    return sbstd::countl_zero(val);
}

inline sb::s32 sb::countLeadingZeros(u64 val)
{
    return sbstd::countl_zero(val);
}
