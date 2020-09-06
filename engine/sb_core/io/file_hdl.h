#pragma once

#include <sb_core/core.h>

namespace sb {

struct FileHdl
{
    using ValueType = ui32;

    constexpr FileHdl() = default;

    constexpr explicit FileHdl(ValueType val)
        : value(val)
    {
    }

    constexpr void reset()
    {
        value = 0U;
    }

    constexpr b8 isValid() const
    {
        return 0U != value;
    }

    ValueType value = 0U;
};

constexpr b8 operator==(FileHdl lval, FileHdl rval)
{
    return lval.value == rval.value;
}

constexpr b8 operator!=(FileHdl lval, FileHdl rval)
{
    return lval.value != rval.value;
}

} // namespace sb
