#pragma once

#include <core/platform.h>
#include <core/_impl/hash/hash_str_cfg.h>

namespace sb {

template <usize BIT_COUNT>
struct HashStrBase
{
    using HashPolicy = HashStrPolicy<BIT_COUNT>;
    using ValueType = typename HashPolicy::ValueType;

    HashStrBase() = default;

    constexpr explicit HashStrBase(ValueType value)
        : m_value(value)
    {
    }

    constexpr explicit HashStrBase(char const * str)
        : m_value(0)
    {
        if (str[0] != '\0')
        {
            m_value = HashPolicy::compute(str);
        }
    }

    constexpr HashStrBase(char const * str, usize len)
        : m_value(0)
    {
        if ((str[0] != '\0') && (0 != len))
        {
            m_value = HashPolicy::compute((ui8 const *)str, len);
        }
    }

    constexpr b32 isNull() const
    {
        return (m_value == 0);
    }

    ValueType m_value = 0;
};

using HashStr = HashStrBase<64>;

constexpr b8 operator==(HashStr lval, HashStr rval)
{
    return (lval.m_value == rval.m_value);
}

constexpr b8 operator!=(HashStr lval, HashStr rval)
{
    return (lval.m_value != rval.m_value);
}

} // namespace sb
