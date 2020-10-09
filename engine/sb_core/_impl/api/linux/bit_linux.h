#pragma once

inline sb::u32 sb::countLeadingZeros(u32 val)
{
    // __builtin_clz(0) is an undefined behavior
    return (u32)(val ? __builtin_clz(val) : 32);
}

inline sb::u32 sb::countLeadingZeros(u64 val)
{
    // __builtin_clz(0) is an undefined behavior
    return (u32)(val ? __builtin_clzl(val) : 64);
}
