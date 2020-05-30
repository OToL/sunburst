#pragma once

#include <core/compiler.h>

namespace sb {

constexpr inline FNV1a32::Value FNV1a32::compute(ui8 const * const buffer, usize const len)
{
    Value hval = OFFSET;

    ui8 const * bp = buffer;
    ui8 const * const be = bp + len;

    while (bp < be)
    {
        hval ^= (FNV1a32::Value)*bp++;
        hval *= PRIME;
        // hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);
    }

    return hval;
}

constexpr inline FNV1a32::Value FNV1a32::compute(char const * const str)
{
    Value hval = OFFSET;

    char const * s = str;

    while (*s)
    {
        hval ^= (Value)*s++;
        hval *= PRIME;
        // hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);
    }

    return hval;
}

constexpr inline FNV1a64::Value FNV1a64::compute(ui8 const * const buffer, usize const len)
{
    FNV1a64::Value hval = OFFSET;

    ui8 const * bp = buffer;
    ui8 const * be = bp + len;

    while (bp < be)
    {
        hval ^= (Value)*bp++;
        hval *= PRIME;
        // hval `+= (hval << 1) + (hval << 4) + (hval << 5) + (hval << 7) + (hval << 8) + (hval << 40);
    }

    return hval;
}

constexpr inline FNV1a64::Value FNV1a64::compute(char const * const str)
{
    FNV1a64::Value hval = OFFSET;

    char const * s = str;

    while (*s)
    {
        hval ^= (Value)*s++;
        hval *= PRIME;
        // hval += (hval << 1) + (hval << 4) + (hval << 5) + (hval << 7) + (hval << 8) + (hval << 40);
    }

    return hval;
}

template <>
struct HashPolicyFNV1a<32>
{
public:
    using ValueType = FNV1a32::Value;

    static constexpr ValueType compute(ui8 const * const buffer, usize const len)
    {
        return FNV1a32::compute(buffer, len);
    }

    static constexpr ValueType compute(char const * const str)
    {
        return FNV1a32::compute(str);
    }
};

template <>
struct HashPolicyFNV1a<64>
{
public:
    using ValueType = FNV1a64::Value;

    static constexpr ValueType compute(ui8 const * const buffer, usize const len)
    {
        return FNV1a64::compute(buffer, len);
    }

    static constexpr ValueType compute(char const * const str)
    {
        return FNV1a64::compute(str);
    }
};
} // namespace sb
