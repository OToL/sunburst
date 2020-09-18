#pragma once

#include <intrin.h>

inline sb::ui32 sb::countLeadingZeros(ui32 val)
{
    unsigned long ret;
    _BitScanForward(&ret, (unsigned long)val);

    return ret;
}

inline sb::ui32 sb::countLeadingZeros(ui64 val)
{
    unsigned long ret;
    _BitScanForward64(&ret, val);

    return ret;
}
