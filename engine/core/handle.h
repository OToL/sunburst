#pragma once

#include "types.h"

namespace sb {

template <typename TId, typename TStorage>
struct Handle
{
    using Type = TId;
    using StorageType = TStorage;

    constexpr Handle() = default;

    constexpr explicit Handle(TStorage val)
        : m_value(val)
    {
    }

    constexpr void reset()
    {
        m_value = TStorage{};
    }

    constexpr StorageType get() const
    {
        return m_value;
    } 

    constexpr b8 isNull() const
    {
        return TStorage{} == m_value;
    }

    TStorage m_value = TStorage{};
};

template <typename TId, typename TStorage>
constexpr b8 operator==(Handle<TId, TStorage> lval, Handle<TId, TStorage> rval)
{
    return lval.m_value == rval.m_value;
}

template <typename TId, typename TStorage>
constexpr b8 operator!=(Handle<TId, TStorage> lval, Handle<TId, TStorage> rval)
{
    return lval.m_value != rval.m_value;
}

} // namespace sb
