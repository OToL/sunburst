#pragma once

#include <sb_core/core.h>

#include <sb_slw/bit>

inline sb::i32 sb::countLeadingZeros(u32 val)
{
    return slw::countl_zero(val);
}

inline sb::i32 sb::countLeadingZeros(u64 val)
{
    return slw::countl_zero(val);
}
