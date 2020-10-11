#pragma once

#include <sb_core/core.h>

#include "hasher.h"

namespace sb {

struct Hash32Str
{
    using ValueType = u32;

    u32 value;
};

constexpr Hash32Str operator"" _h32s(char const * str, usize len)
{
    return {computeHash32({str, len})};
}

constexpr Hash32Str makeHash32Str(char const * str)
{
    return {computeHash32(str)};
}

constexpr Hash32Str makeHash32Str(char const * str, usize len)
{
    return {computeHash32({str, len})};
}

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
