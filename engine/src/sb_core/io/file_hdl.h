#pragma once

#include <sb_core/core.h>

namespace sb {

struct FileHdl
{
    using ValueType = u32;

    ValueType value = 0;

    constexpr b8 isValid() const
    {
        return (0 != value);
    }
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
