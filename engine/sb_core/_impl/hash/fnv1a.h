#pragma once

#include <sb_base/base.h>

namespace sb {

constexpr inline ui32 computeFNV1a32(ui8 const * const buffer, usize const len)
{
    if (0 == len)
    {
        return 0U;
    }

    ui32 hval = 0x811C9DC5U;

    ui8 const * bp = buffer;
    ui8 const * const be = bp + len;

    while (bp < be)
    {
        hval ^= (ui32)*bp++;
        hval *= 0x01000193U;
        // hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);
    }

    return hval;
}

constexpr inline ui32 computeFNV1a32(char const * const str)
{
    if (0 == *str)
    {
        return 0U;
    }

    ui32 hval = 0x811C9DC5U;

    char const * s = str;

    while (*s)
    {
        hval ^= (ui32)*s++;
        hval *= 0x01000193U;
        // hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);
    }

    return hval;
}

constexpr inline ui64 computeFNV1a64(ui8 const * const buffer, usize const len)
{
    if (0 == len)
    {
        return 0ULL;
    }

    ui64 hval = 0xCBF29CE484222325ULL;

    ui8 const * bp = buffer;
    ui8 const * be = bp + len;

    while (bp < be)
    {
        hval ^= (ui64)*bp++;
        hval *= 0x100000001b3ULL;
        // hval `+= (hval << 1) + (hval << 4) + (hval << 5) + (hval << 7) + (hval << 8) +
        // (hval << 40);
    }

    return hval;
}

constexpr inline ui64 computeFNV1a64(char const * const str)
{
    if (0 == *str)
    {
        return 0ULL;
    }

    ui64 hval = 0xCBF29CE484222325ULL;

    char const * s = str;

    while (*s)
    {
        hval ^= (ui64)*s++;
        hval *= 0x100000001b3ULL;
        // hval += (hval << 1) + (hval << 4) + (hval << 5) + (hval << 7) + (hval << 8) +
        // (hval << 40);
    }

    return hval;
}

} // namespace sb
