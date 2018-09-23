#pragma once

#include <core/platform.h>

namespace sb {

template <>
struct HashPolicyCRC<32>
{
public:
    using ValueType = CRC32Value;

    static ValueType compute(ui8 const * const buffer, usize const len)
    {
        return (computeCRC32(buffer, len));
    }

    static ValueType compute(char const * const str)
    {
        return (computeCRC32(str));
    }
};

template <>
struct HashPolicyCRC<64>
{
public:
    using ValueType = CRC64Value;

    static ValueType compute(ui8 const * const buffer, usize const len)
    {
        return (computeCRC64(buffer, len));
    }

    static ValueType compute(char const * const str)
    {
        return (computeCRC64(str));
    }
};

} // namespace sb
