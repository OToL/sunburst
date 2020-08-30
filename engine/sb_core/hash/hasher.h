#pragma once

#include <sb_core/_impl/config.h>

#if sbIsEqual(HASH_POLICY, SB_HASH_POLICY_CRC)
#    include "crc.h"
#else
#    include "fnv1a.h"
#endif

namespace sb {

#if sbIsEqual(HASH_POLICY, SB_HASH_POLICY_CRC)

inline constexpr ui32 computeHash32(sbstd::string_view buffer)
{
    return computeCRC32(buffer);
}

inline constexpr ui32 computeHash32(sbstd::span<ui8 const> buffer)
{
    return computeCRC32(buffer);
}

inline constexpr ui32 computeHash32(char const * const str)
{
    return computeCRC32(str);
}

inline constexpr ui64 computeHash64(sbstd::string_view buffer)
{
    return computeCRC64(buffer);
}

inline constexpr ui64 computeHash64(sbstd::span<ui8 const> buffer)
{
    return computeCRC64(buffer);
}

inline constexpr ui64 computeHash64(char const * const str)
{
    return computeCRC64(str);
}

#elif sbIsEqual(HASH_POLICY, SB_HASH_POLICY_FNV1A)

inline constexpr ui32 computeHash32(sbstd::string_view buffer)
{
    return computeFNV1a32(buffer);
}

inline constexpr ui32 computeHash32(sbstd::span<ui8 const> buffer)
{
    return computeFNV1a32(buffer);
}

inline constexpr ui32 computeHash32(char const * const str)
{
    return computeFNV1a32(str);
}

inline constexpr ui64 computeHash64(sbstd::string_view buffer)
{
    return computeFNV1a64(buffer);
}

inline constexpr ui64 computeHash64(sbstd::span<ui8 const> buffer)
{
    return computeFNV1a64(buffer);
}

inline constexpr ui64 computeHash64(char const * const str)
{
    return computeFNV1a64(str);
}

#else

#    error "Unsupported hash policy"

#endif

#if sbIsEqual(HASH_SIZE, SB_HASH_SIZE_32BIT)

inline constexpr ui32 computeHash(sbstd::string_view buffer)
{
    return computeHash32(buffer);
}

inline constexpr ui32 computeHash(sbstd::span<ui8 const> buffer)
{
    return computeHash32(buffer);
}

inline constexpr ui32 computeHash(char const * const str)
{
    return computeHash32(str);
}

#elif sbIsEqual(HASH_SIZE, SB_HASH_SIZE_64BIT)

inline constexpr ui64 computeHash(sbstd::string_view buffer)
{
    return computeHash64(buffer);
}

inline constexpr ui64 computeHash(sbstd::span<ui8 const> buffer)
{
    return computeHash64(buffer);
}

inline constexpr ui64 computeHash(char const * const str)
{
    return computeHash64(str);
}

#else
#    error "Unsupported hash size"
#endif

} // namespace sb
