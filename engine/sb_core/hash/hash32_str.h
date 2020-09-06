#pragma once

#include <sb_core/core.h>

#include "hasher.h"

namespace sb {

struct Hash32StrLiteral
{
    ui32 value;
    char const * str;
};

constexpr Hash32StrLiteral operator"" _h32s(char const * str, usize len)
{
    return {computeHash32({str, len}), str};
}

struct Hash32Str
{
    Hash32Str() = default;

    constexpr explicit Hash32Str(ui32 value)
        : value(value)
    {
    }

    constexpr explicit Hash32Str(char const * str)
        : value(computeHash32(str))
    {
        // TODO: register hash/string
    }

    constexpr explicit Hash32Str(Hash32StrLiteral val)
        : value(val.value)
    {
        // TODO: register hash/string
    }

    constexpr Hash32Str(char const * str, usize len)
        : value(computeHash32({str, len}))
    {
        // TODO: register hash/string
    }

    constexpr b32 isValid() const
    {
        return (value != 0);
    }

    ui32 value = 0;
};

constexpr b8 operator==(Hash32Str lval, Hash32Str rval)
{
    return (lval.value == rval.value);
}

constexpr b8 operator!=(Hash32Str lval, Hash32Str rval)
{
    return (lval.value != rval.value);
}

} // namespace sb
