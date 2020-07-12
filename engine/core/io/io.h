#pragma once

#include <core/base.h>
#include <core/handle.h>

namespace sb {

enum class FileWriteMode : ui8
{
    TRUNC,
    APPEND
};

enum class FileFormat : ui8
{
    Bin,
    Text
};

struct FileHdlTag
{
};

using FileHdl = Handle<FileHdlTag, ui32>;
using FileSize = si64;
using FilePos = si64;

} // namespace sb
