#pragma once

#include <sb_core/core.h>

namespace sb {

namespace file_hdl {

    struct FileHdl
    {
        using ValueType = u32;

        ValueType value = 0;
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
} // namespace file_hdl

using file_hdl::FileHdl;

} // namespace sb
