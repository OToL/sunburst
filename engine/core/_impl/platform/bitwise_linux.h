#pragma once

namespace sb::detail {

inline ui32 countLeadingZeros(ui32 val)
{
    // __builtin_clz(0) is an undefined behavior
    return (ui32)(val ? __builtin_clz(val) : 32);
}

inline ui32 countLeadingZeros(ui64 val)
{
    // __builtin_clz(0) is an undefined behavior
    return (ui32)(val ? __builtin_clzl(val) : 64);
}
} // namespace sb::detail
