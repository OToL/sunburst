#pragma once

#include <sb_core/core.h>

namespace sb {

template <typename TId, typename TStorage = ui32>
struct Handle
{
    using Type = TId;
    using StorageType = TStorage;

    constexpr Handle() = default;

    constexpr explicit Handle(TStorage val)
        : value(val)
    {
    }

    constexpr void reset()
    {
        value = TStorage{};
    }

    constexpr b8 isValid() const
    {
        return TStorage{} != value;
    }

    TStorage value = TStorage{};
};

template <typename TId, typename TStorage>
constexpr b8 operator==(Handle<TId, TStorage> lval, Handle<TId, TStorage> rval)
{
    return lval.value == rval.value;
}

template <typename TId, typename TStorage>
constexpr b8 operator!=(Handle<TId, TStorage> lval, Handle<TId, TStorage> rval)
{
    return lval.value != rval.value;
}

} // namespace sb
