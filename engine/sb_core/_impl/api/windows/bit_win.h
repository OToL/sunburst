#pragma once

#include <intrin.h>

inline sb::u32 sb::countLeadingZeros(u32 val)
{
    unsigned long ret;
    _BitScanForward(&ret, (unsigned long)val);

    return ret;
}

inline sb::u32 sb::countLeadingZeros(u64 val)
{
    unsigned long ret;
    _BitScanForward64(&ret, val);

    return ret;
}
