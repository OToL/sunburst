#pragma once

#include <sb_core/core.h>

namespace sb {

struct FileHdl
{
    using ValueType = u32;

    ValueType value;
};

constexpr b8 operator==(FileHdl lval, FileHdl rval)
{
    return lval.value == rval.value;
}

constexpr b8 operator!=(FileHdl lval, FileHdl rval)
{
    return lval.value != rval.value;
}

constexpr b8 isValid(FileHdl hdl)
{
    return hdl.value != 0;
}

} // namespace sb
