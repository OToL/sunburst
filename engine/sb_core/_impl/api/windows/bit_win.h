#pragma once

#include <intrin.h>

namespace sb::detail {

inline ui32 countLeadingZeros(ui32 val)
{
    unsigned long ret;
    _BitScanForward(&ret, (unsigned long)val);

    return ret;
}

inline ui32 countLeadingZeros(ui64 val)
{
    unsigned long ret;
    _BitScanForward64(&ret, val);

    return ret;
}
} // namespace sb::detail
