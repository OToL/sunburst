#pragma once

#include <sb_core/core.h>

namespace sb {

struct File
{
    using ValueType = u32;

    ValueType value;
};

constexpr b8 operator==(File lval, File rval)
{
    return lval.value == rval.value;
}

constexpr b8 operator!=(File lval, File rval)
{
    return lval.value != rval.value;
}

constexpr b8 file_isValid(File hdl)
{
    return hdl.value != 0;
}

} // namespace sb
