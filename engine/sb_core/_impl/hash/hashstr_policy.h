#pragma once

#include <sb_base/base.h>

#include <sb_core/_impl/config.h>

#if sbIsEqual(HASHSTR_POLICY, SB_HASHSTR_POLICY_CRC)

#   error "Not implemented"

#elif sbIsEqual(HASHSTR_POLICY, SB_HASHSTR_POLICY_FNV1A)

#    include <sb_core/hash/fnv1a.h>

namespace sb::internal {

template<typename ValueType>
struct HashStrPolicy
{
    static constexpr ValueType compute(ui8 const * const buffer, usize const len);
    static constexpr ValueType compute(char const * const str);
};

template<>
struct HashStrPolicy<ui64>
{
    static constexpr ui64 compute(ui8 const * const buffer, usize const len)
    {
        return computeFNV1a64(buffer, len);
    }

    static constexpr ui64 compute(char const * const str)
    {
        return computeFNV1a64(str);
    }
};

template<>
struct HashStrPolicy<ui32>
{
    static constexpr ui32 compute(ui8 const * const buffer, usize const len)
    {
        return computeFNV1a32(buffer, len);
    }

    static constexpr ui32 compute(char const * const str)
    {
        return computeFNV1a32(str);
    }
};

}

#endif

