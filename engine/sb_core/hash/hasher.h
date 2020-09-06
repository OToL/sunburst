#pragma once

#include <sb_core/_impl/config.h>

#if sbIsEqual(HASH_POLICY, SB_HASH_POLICY_CRC)
#    include "crc.h"
#else
#    include "fnv1a.h"
#endif

namespace sb {

inline constexpr ui32 computeHash32(sbstd::string_view buffer)
{
#if sbIsEqual(HASH_POLICY, SB_HASH_POLICY_CRC)
    return computeCRC32(buffer);
#else // SB_HASH_POLICY_FNV1A
    return computeFNV1a32(buffer);
#endif
}

inline constexpr ui32 computeHash32(sbstd::span<ui8 const> buffer)
{
#if sbIsEqual(HASH_POLICY, SB_HASH_POLICY_CRC)
    return computeCRC32(buffer);
#else // SB_HASH_POLICY_FNV1A
    return computeFNV1a32(buffer);
#endif
}

inline constexpr ui32 computeHash32(char const * const str)
{
#if sbIsEqual(HASH_POLICY, SB_HASH_POLICY_CRC)
    return computeCRC32(str);
#else // SB_HASH_POLICY_FNV1A
    return computeFNV1a32(str);
#endif
}

inline constexpr ui64 computeHash64(sbstd::string_view buffer)
{
#if sbIsEqual(HASH_POLICY, SB_HASH_POLICY_CRC)
    return computeCRC64(buffer);
#else // SB_HASH_POLICY_FNV1A
    return computeFNV1a64(buffer);
#endif
}

inline constexpr ui64 computeHash64(sbstd::span<ui8 const> buffer)
{
#if sbIsEqual(HASH_POLICY, SB_HASH_POLICY_CRC)
    return computeCRC64(buffer);
#else // SB_HASH_POLICY_FNV1A
    return computeFNV1a64(buffer);
#endif
}

inline constexpr ui64 computeHash64(char const * const str)
{
#if sbIsEqual(HASH_POLICY, SB_HASH_POLICY_CRC)
    return computeCRC64(str);
#else // SB_HASH_POLICY_FNV1A
    return computeFNV1a64(str);
#endif
}

inline constexpr ui64 computeHash(sbstd::string_view buffer)
{
#if sbIsEqual(HASH_POLICY, SB_HASH_POLICY_CRC)
#   if sbIsEqual(HASH_SIZE, SB_HASH_SIZE_32BIT)
    return computeCRC32(buffer);
#   else //  SB_HASH_SIZE_64BIT
    return computeCRC64(buffer);
#   endif
#else // SB_HASH_POLICY_FNV1A
#   if sbIsEqual(HASH_SIZE, SB_HASH_SIZE_32BIT)
    return computeFNV1a32(buffer);
#   else // SB_HASH_SIZE_64BIT
    return computeFNV1a64(buffer);
#   endif
#endif
}

inline constexpr ui64 computeHash(sbstd::span<ui8 const> buffer)
{
#if sbIsEqual(HASH_POLICY, SB_HASH_POLICY_CRC)
#   if sbIsEqual(HASH_SIZE, SB_HASH_SIZE_32BIT)
    return computeCRC32(buffer);
#   else //  SB_HASH_SIZE_64BIT
    return computeCRC64(buffer);
#   endif
#else // SB_HASH_POLICY_FNV1A
#   if sbIsEqual(HASH_SIZE, SB_HASH_SIZE_32BIT)
    return computeFNV1a32(buffer);
#   else // SB_HASH_SIZE_64BIT
    return computeFNV1a64(buffer);
#   endif
#endif
}

inline constexpr ui64 computeHash(char const * const str)
{
#if sbIsEqual(HASH_POLICY, SB_HASH_POLICY_CRC)
#   if sbIsEqual(HASH_SIZE, SB_HASH_SIZE_32BIT)
    return computeCRC32(str);
#   else //  SB_HASH_SIZE_64BIT
    return computeCRC64(str);
#   endif
#else // SB_HASH_POLICY_FNV1A
#   if sbIsEqual(HASH_SIZE, SB_HASH_SIZE_32BIT)
    return computeFNV1a32(str);
#   else // SB_HASH_SIZE_64BIT
    return computeFNV1a64(str);
#   endif
#endif
}

} // namespace sb
