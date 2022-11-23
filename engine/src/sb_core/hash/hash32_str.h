#pragma once

#include <sb_core/core.h>

#include "hasher.h"

namespace sb {

struct Hash32Str
{
    using ValueType = u32;

    u32 value = 0U;

    static constexpr Hash32Str make(char const * str)
    {
        return {computeHash32(str)};
    }

    static constexpr Hash32Str make(char const * str, usize len)
    {
        return {computeHash32(str, len)};
    }

    constexpr b8 isValid() const
    {
        return (value != 0U);
    }
};

constexpr b8 operator==(Hash32Str lval, Hash32Str rval)
{
    return (lval.value == rval.value);
}

constexpr b8 operator!=(Hash32Str lval, Hash32Str rval)
{
    return (lval.value != rval.value);
}

constexpr Hash32Str operator"" _h32s(char const * str, usize len)
{
    return Hash32Str::make(str, len);
}

} // namespace sb
