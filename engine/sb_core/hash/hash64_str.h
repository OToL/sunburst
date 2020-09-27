#pragma once

#include <sb_core/core.h>

#include "hasher.h"

namespace sb {

struct Hash64StrLiteral
{
    ui64 value;
    char const * str;
};

constexpr Hash64StrLiteral operator"" _h64s(char const * str, usize len)
{
    return {computeHash64({str, len}), str};
}

struct Hash64Str
{
    Hash64Str() = default;

    constexpr explicit Hash64Str(ui64 value)
        : value(value)
    {
    }

    constexpr explicit Hash64Str(char const * str)
        : value(computeHash64(str))
    {
        // TODO: register hash/string
    }

    constexpr Hash64Str(Hash64StrLiteral val)
        : value(val.value)
    {
        // TODO: register hash/string
    }

    constexpr Hash64Str(char const * str, usize len)
        : value(computeHash64({str, len}))
    {
        // TODO: register hash/string
    }

    ui64 value;
};

constexpr b32 isValid(Hash64Str hash_val)
{
    return (hash_val.value != 0);
}

constexpr b8 operator==(Hash64Str lval, Hash64Str rval)
{
    return (lval.value == rval.value);
}

constexpr b8 operator!=(Hash64Str lval, Hash64Str rval)
{
    return (lval.value != rval.value);
}

} // namespace sb
