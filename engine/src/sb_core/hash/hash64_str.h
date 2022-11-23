#pragma once

#include <sb_core/core.h>

#include "hasher.h"

namespace sb {

struct Hash64Str
{
    using ValueType = u64;

    ValueType value = 0ULL;

    static constexpr Hash64Str make(char const * str)
    {
        return {computeHash64(str)};
    }

    static constexpr Hash64Str make(char const * str, usize len)
    {
        return {computeHash64(str, len)};
    }

    constexpr b32 isValid() const
    {
        return (0ULL != value);
    }
};

constexpr b8 operator==(Hash64Str lval, Hash64Str rval)
{
    return (lval.value == rval.value);
}

constexpr b8 operator!=(Hash64Str lval, Hash64Str rval)
{
    return (lval.value != rval.value);
}

constexpr Hash64Str operator"" _h64s(char const * str, usize len)
{
    return Hash64Str::make(str, len);
}

} // namespace sb
