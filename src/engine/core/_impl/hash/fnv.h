#pragma once

#include <core/compiler.h>

namespace sb {

constexpr inline FNV32Value computeFNV1a32(ui8 const * const buffer, usize const len)
{
    FNV32Value hval = FNV32_OFFSET;

    ui8 const * bp = buffer;
    ui8 const * const be = bp + len;

    while (bp < be)
    {
        hval ^= (FNV32Value)*bp++;
        hval *= FNV32_PRIME;
        // hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);
    }

    return hval;
}

constexpr inline FNV32Value computeFNV1a32(char const * const str)
{
    FNV32Value hval = FNV32_OFFSET;

    char const * s = str;

    while (*s)
    {
        hval ^= (FNV32Value)*s++;
        hval *= FNV32_PRIME;

        // hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);
    }

    return hval;
}

constexpr inline FNV64Value computeFNV1a64(ui8 const * const buffer, usize const len)
{
    FNV64Value hval = FNV64_OFFSET;

    ui8 const * bp = buffer;
    ui8 const * be = bp + len;

    while (bp < be)
    {
        hval ^= (FNV64Value)*bp++;
        hval *= FNV64_PRIME;
        // hval += (hval << 1) + (hval << 4) + (hval << 5) + (hval << 7) + (hval << 8) + (hval << 40);
    }

    return hval;
}

constexpr inline FNV64Value computeFNV1a64(char const * const str)
{
    FNV64Value hval = FNV64_OFFSET;

    char const * s = str;

    while (*s)
    {
        hval ^= (FNV64Value)*s++;
        hval *= FNV64_PRIME;
        // hval += (hval << 1) + (hval << 4) + (hval << 5) + (hval << 7) + (hval << 8) + (hval << 40);
    }

    return hval;
}

template <>
struct HashPolicyFNV1a<32>
{
public:
    using ValueType = FNV32Value;

    static constexpr ValueType compute(ui8 const * const buffer, usize const len)
    {
        return computeFNV1a32(buffer, len);
    }

    static constexpr ValueType compute(char const * const str)
    {
        return computeFNV1a32(str);
    }
};

template <>
struct HashPolicyFNV1a<64>
{
public:
    using ValueType = FNV64Value;

    static constexpr ValueType compute(ui8 const * const buffer, usize const len)
    {
        return computeFNV1a64(buffer, len);
    }

    static constexpr ValueType compute(char const * const str)
    {
        return computeFNV1a64(str);
    }
};
} // namespace sb
