#pragma once

#include <sb_core/_impl/config.h>

#include <sb_slw/forward_decls>

#if sb_ctf_equal(HASH_POLICY, CRC)
#    include "crc.h"
#else
#    include "fnv1a.h"
#endif

namespace sb {

inline constexpr u32 computeHash32(char const * str, size_t len)
{
#if sb_ctf_equal(HASH_POLICY, CRC)
    return computeCRC32(str, len);
#else // SB_CTV_HASH_POLICY_FNV1A
    return computeFNV1a32(str, len);
#endif
}

inline u32 computeHash32(slw::string_view_fw const & buffer)
{
#if sb_ctf_equal(HASH_POLICY, CRC)
    return computeCRC32(buffer);
#else // SB_CTV_HASH_POLICY_FNV1A
    return computeFNV1a32(buffer);
#endif
}

inline u32 computeHash32(slw::span_fw<u8 const> const & buffer)
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

inline constexpr u64 computeHash64(char const * const str, size_t len)
{
#if sb_ctf_equal(HASH_POLICY, CRC)
    return computeCRC64(str, len);
#else // SB_CTV_HASH_POLICY_FNV1A
    return computeFNV1a64(str, len);
#endif
}

inline u64 computeHash64(slw::string_view_fw const & buffer)
{
#if sb_ctf_equal(HASH_POLICY, CRC)
    return computeCRC64(buffer);
#else // SB_CTV_HASH_POLICY_FNV1A
    return computeFNV1a64(buffer);
#endif
}

inline u64 computeHash64(slw::span_fw<u8 const> const & buffer)
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

inline u64 computeHash(slw::string_view_fw const &buffer)
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

inline u64 computeHash(slw::span_fw<u8 const> const &buffer)
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
