#pragma once

#include <sb_core/core.h>
#include <sb_core/_impl/config.h>

#if sb_ctf_equal(HASH_SIZE, 32BIT)

#    include "hash32_str.h"

namespace sb {

using HashStr = Hash32Str;
using HashStrValue = typename HashStr::ValueType;

constexpr HashStr operator"" _hs(char const * str, usize len)
{
    return {computeHash32({str, len})};
}

} // namespace sb

#elif sb_ctf_equal(HASH_SIZE, 64BIT)

#    include "hash64_str.h"

namespace sb {

using HashStr = Hash64Str;
using HashStrValue = typename HashStr::ValueType;

consteval HashStr operator"" _hs(char const * str, usize len)
{
    return {computeHash64(str, len)};
}

} // namespace sb

#else

#    error "Unsupported hash size"

#endif