#pragma once

#include <sb_core/_impl/config.h>

#if sb_ctf_equal(HASH_POLICY, CRC)
#    include "crc.h"
#else
#    include "fnv1a.h"
#endif

namespace sb {

inline constexpr u32 computeHash32(slw::string_view buffer)
{
#if sb_ctf_equal(HASH_POLICY, CRC)
    return computeCRC32(buffer);
#else // SB_CTV_HASH_POLICY_FNV1A
    return computeFNV1a32(buffer);
#endif
}

inline constexpr u32 computeHash32(slw::span<u8 const> buffer)
{
#if sb_ctf_equal(HASH_POLICY, CRC)
    return computeCRC32(buffer);
#else // SB_CTV_HASH_POLICY_FNV1A
    return computeFNV1a32(buffer);
#endif
}

inline constexpr u32 computeHash32(char const * const str)
{
#if sb_ctf_equal(HASH_POLICY, CRC)
    return computeCRC32(str);
#else // SB_CTV_HASH_POLICY_FNV1A
    return computeFNV1a32(str);
#endif
}

inline constexpr u64 computeHash64(slw::string_view buffer)
{
#if sb_ctf_equal(HASH_POLICY, CRC)
    return computeCRC64(buffer);
#else // SB_CTV_HASH_POLICY_FNV1A
    return computeFNV1a64(buffer);
#endif
}

inline constexpr u64 computeHash64(slw::span<u8 const> buffer)
{
#if sb_ctf_equal(HASH_POLICY, CRC)
    return computeCRC64(buffer);
#else // SB_CTV_HASH_POLICY_FNV1A
    return computeFNV1a64(buffer);
#endif
}

inline constexpr u64 computeHash64(char const * const str)
{
#if sb_ctf_equal(HASH_POLICY, CRC)
    return computeCRC64(str);
#else // SB_CTV_HASH_POLICY_FNV1A
    return computeFNV1a64(str);
#endif
}

inline constexpr u64 computeHash(slw::string_view buffer)
{
#if sb_ctf_equal(HASH_POLICY, CRC)
#    if sb_ctf_equal(HASH_SIZE, 32BIT)
    return computeCRC32(buffer);
#    else //  SB_CTV_HASH_SIZE_64BIT
    return computeCRC64(buffer);
#    endif
#else // SB_CTV_HASH_POLICY_FNV1A
#    if sb_ctf_equal(HASH_SIZE, 32BIT)
    return computeFNV1a32(buffer);
#    else // SB_CTV_HASH_SIZE_64BIT
    return computeFNV1a64(buffer);
#    endif
#endif
}

inline constexpr u64 computeHash(slw::span<u8 const> buffer)
{
#if sb_ctf_equal(HASH_POLICY, CRC)
#    if sb_ctf_equal(HASH_SIZE, 32BIT)
    return computeCRC32(buffer);
#    else //  SB_CTV_HASH_SIZE_64BIT
    return computeCRC64(buffer);
#    endif
#else // SB_CTV_HASH_POLICY_FNV1A
#    if sb_ctf_equal(HASH_SIZE, 32BIT)
    return computeFNV1a32(buffer);
#    else // SB_CTV_HASH_SIZE_64BIT
    return computeFNV1a64(buffer);
#    endif
#endif
}

inline constexpr u64 computeHash(char const * const str)
{
#if sb_ctf_equal(HASH_POLICY, CRC)
#    if sb_ctf_equal(HASH_SIZE, 32BIT)
    return computeCRC32(str);
#    else //  SB_CTV_HASH_SIZE_64BIT
    return computeCRC64(str);
#    endif
#else // SB_CTV_HASH_POLICY_FNV1A
#    if sb_ctf_equal(HASH_SIZE, 32BIT)
    return computeFNV1a32(str);
#    else // SB_CTV_HASH_SIZE_64BIT
    return computeFNV1a64(str);
#    endif
#endif
}

} // namespace sb
