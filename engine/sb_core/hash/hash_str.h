#pragma once

#include <sb_core/core.h>

#include <sb_core/_impl/config.h>

#if sbIsEqual(HASH_SIZE, SB_HASH_SIZE_32BIT)

#    include "hash32_str.h"

namespace sb {

using HashStr = Hash32Str;
using HashStrValue = u32;

constexpr HashStr operator"" _hs(char const * str, usize len)
{
    return {computeHash32({str, len})};
}

constexpr HashStr makeHashStr(char const * str)
{
    return {computeHash32(str)};
}

constexpr HashStr makeHashStr(char const * str, usize len)
{
    return {computeHash32({str, len})};
}

} // namespace sb

#elif sbIsEqual(HASH_SIZE, SB_HASH_SIZE_64BIT)

#    include "hash64_str.h"

namespace sb {

using HashStr = Hash64Str;
using HashStrValue = u64;

constexpr HashStr operator"" _hs(char const * str, usize len)
{
    return {computeHash64({str, len})};
}

constexpr HashStr makeHashStr(char const * str)
{
    return {computeHash64(str)};
}

constexpr HashStr makeHashStr(char const * str, usize len)
{
    return {computeHash64({str, len})};
}

constexpr b32 hashstr_isValid(HashStr hash_val)
{
    return (hash_val.value != 0);
}

} // namespace sb

#else

#    error "Unsupported hash size"

#endif
