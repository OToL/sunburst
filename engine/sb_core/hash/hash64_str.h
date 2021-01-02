#pragma once

#include <sb_core/core.h>

#include "hasher.h"

namespace sb {

struct Hash64Str
{
    using ValueType = u64;

    ValueType value;
};

constexpr Hash64Str operator"" _h64s(char const * str, usize len)
{
    return {computeHash64({str, len})};
}

constexpr Hash64Str makeHash64Str(char const * str)
{
    return {computeHash64(str)};
}

constexpr Hash64Str makeHash64Str(char const * str, usize len)
{
    return {computeHash64({str, len})};
}

constexpr b32 h64str_isValid(Hash64Str hash_val)
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
