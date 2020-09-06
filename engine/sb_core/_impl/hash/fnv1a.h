#pragma once

#include <sb_core/core.h>

namespace sb {

inline constexpr ui32 computeFNV1a32(sbstd::span<ui8 const> buffer)
{
    auto const buffer_size = buffer.size();

    if (0 == buffer_size)
    {
        return 0U;
    }

    ui32 hval = 0x811C9DC5U;

    ui8 const * bp = buffer.data();
    ui8 const * const be = bp + buffer_size;

    while (bp < be)
    {
        hval ^= (ui32)*bp++;
        hval *= 0x01000193U;
        // hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);
    }

    return hval;
}

inline constexpr ui32 computeFNV1a32(sbstd::string_view buffer)
{
    auto const buffer_size = buffer.size();

    if (0 == buffer_size)
    {
        return 0U;
    }

    ui32 hval = 0x811C9DC5U;

    char const * bp = buffer.data();
    char const * const be = bp + buffer_size;

    while (bp < be)
    {
        hval ^= (ui32)*bp++;
        hval *= 0x01000193U;
        // hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);
    }

    return hval;
}

inline constexpr ui32 computeFNV1a32(char const * const str)
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

inline constexpr ui64 computeFNV1a64(sbstd::span<ui8 const> buffer)
{
    auto const len = buffer.size();

    if (0 == len)
    {
        return 0ULL;
    }

    ui64 hval = 0xCBF29CE484222325ULL;

    ui8 const * bp = buffer.data();
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

inline constexpr ui64 computeFNV1a64(sbstd::string_view buffer)
{
    auto const len = buffer.size();

    if (0 == len)
    {
        return 0ULL;
    }

    ui64 hval = 0xCBF29CE484222325ULL;

    char const * bp = buffer.data();
    char const * be = bp + len;

    while (bp < be)
    {
        hval ^= (ui64)*bp++;
        hval *= 0x100000001b3ULL;
        // hval `+= (hval << 1) + (hval << 4) + (hval << 5) + (hval << 7) + (hval << 8) +
        // (hval << 40);
    }

    return hval;
}

inline constexpr ui64 computeFNV1a64(char const * const str)
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
