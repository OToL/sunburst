#pragma once

#include <sb_base/base.h>

#include <sb_core/_impl/hash/hashstr_policy.h>

namespace sb {

// use span in low level
// use string_view instead of char + len
template<typename ValueType = ui64>
struct HashStr
{
    using HashPolicy = internal::HashStrPolicy<ValueType>;
    using Value = typename ValueType;

    HashStr() = default;

    constexpr explicit HashStr(Value value)
        : value(value)
    {
    }

    constexpr explicit HashStr(char const * str)
        : value(HashPolicy::compute(str))
    {
    }

    constexpr HashStr(char const * str, usize len)
        : value(HashPolicy::compute((ui8 const *)str, len))
    {
    }

    constexpr b32 isValid() const
    {
        return (value != 0);
    }

    Value value = 0;
};

template<typename ValueType> 
constexpr b8 operator==(HashStr<ValueType> lval, HashStr<ValueType> rval)
{
    return (lval.value == rval.value);
}

template<typename ValueType> 
constexpr b8 operator!=(HashStr<ValueType> lval, HashStr<ValueType> rval)
{
    return (lval.value != rval.value);
}

using HashStr64 = HashStr<>;
using HashStr32 = HashStr<ui32>;

} // namespace sb
