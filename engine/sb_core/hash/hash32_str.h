#pragma once

#include <sb_core/core.h>

#include "hasher.h"

namespace sb {

struct Hash32StrLiteral
{
    u32 value;
    char const * str;
};

constexpr Hash32StrLiteral operator"" _h32s(char const * str, usize len)
{
    return {computeHash32({str, len}), str};
}

struct Hash32Str
{
    Hash32Str() = default;

    constexpr explicit Hash32Str(u32 value)
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

    u32 value;
};

constexpr b8 isValid(Hash32Str hash_val)
{
    return (hash_val.value != 0);
}

constexpr b8 operator==(Hash32Str lval, Hash32Str rval)
{
    return (lval.value == rval.value);
}

constexpr b8 operator!=(Hash32Str lval, Hash32Str rval)
{
    return (lval.value != rval.value);
}

} // namespace sb
