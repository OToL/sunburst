#pragma once

#include "platform.h"

namespace sb {

    template <typename TId, typename TStorage>
    struct TypedId
    {
        using IdType = TId;
        using StorageType = TStorage;

        TypedId() = default;

        explicit TypedId(TStorage val) :
            m_value(val)
        {
        }

        constexpr TStorage get() const
        {
            return m_value;
        }

        constexpr void reset()
        {
            m_value = 0;
        }

        constexpr b8 isNull() const
        {
            return 0 == m_value;
        }

        TStorage m_value = 0;
    };

    template <typename TId, typename TStorage>
    constexpr b8 operator == (TypedId<TId, TStorage> lval, TypedId<TId, TStorage> rval)
    {
        return lval.m_value == rval.m_value;
    }

    template <typename TId, typename TStorage>
    constexpr b8 operator != (TypedId<TId, TStorage> lval, TypedId<TId, TStorage> rval)
    {
        return lval.m_value != rval.m_value;
    }
}
