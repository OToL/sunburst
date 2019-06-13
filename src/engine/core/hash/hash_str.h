#pragma once

#include <core/platform.h>
#include <core/_impl/config/hash_cfg.h>

namespace sb {

struct HashStr
{
    using HashPolicy = detail::HashStrPolicy;
    using Value = typename HashPolicy::Value;

    HashStr() = default;

    constexpr explicit HashStr(Value value)
        : m_value(value)
    {
    }

    constexpr explicit HashStr(char const * str)
        : m_value(0)
    {
        if (str[0] != '\0')
        {
            m_value = HashPolicy::compute(str);
        }
    }

    constexpr HashStr(char const * str, usize len)
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

    Value m_value = 0;
};

constexpr b8 operator==(HashStr lval, HashStr rval)
{
    return (lval.m_value == rval.m_value);
}

constexpr b8 operator!=(HashStr lval, HashStr rval)
{
    return (lval.m_value != rval.m_value);
}

} // namespace sb
