#pragma once

inline sb::ui32 sb::countLeadingZeros(ui32 val)
{
    // __builtin_clz(0) is an undefined behavior
    return (ui32)(val ? __builtin_clz(val) : 32);
}

inline sb::ui32 sb::countLeadingZeros(ui64 val)
{
    // __builtin_clz(0) is an undefined behavior
    return (ui32)(val ? __builtin_clzl(val) : 64);
}
