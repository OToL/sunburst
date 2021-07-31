#pragma once

#include <sb_core/core.h>

#include "hasher.h"

namespace sb {

struct Hash64Str
{
    using ValueType = u64;

    ValueType value;
};

namespace hash64_str {

    constexpr Hash64Str make(char const * str)
    {
        return {computeHash64(str)};
    }

    constexpr Hash64Str make(char const * str, usize len)
    {
        return {computeHash64({str, len})};
    }

    constexpr b32 isValid(Hash64Str hash_val)
    {
        return (hash_val.value != 0);
    }

} // namespace hash64str

constexpr Hash64Str operator"" _h64s(char const * str, usize len)
{
    return {computeHash64({str, len})};
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
