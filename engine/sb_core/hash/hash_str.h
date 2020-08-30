#pragma once

#include <sb_base/base.h>

#include <sb_core/_impl/config.h>

#if sbIsEqual(HASH_SIZE, SB_HASH_SIZE_32BIT)

#    include "hash32_str.h"

namespace sb {

using HashStr = Hash32Str;
using HashStrValue = ui32;

constexpr Hash32StrLiteral operator"" _hs(char const * str, usize len)
{
    return {computeHash32({str, len}), str};
}

} // namespace sb

#elif sbIsEqual(HASH_SIZE, SB_HASH_SIZE_64BIT)

#    include "hash64_str.h"

namespace sb {

using HashStr = Hash64Str;
using HashStrValue = ui64;

constexpr Hash64StrLiteral operator"" _hs(char const * str, usize len)
{
    return {computeHash64({str, len}), str};
}

} // namespace sb

#else

#    error "Unsupported hash size"

#endif
