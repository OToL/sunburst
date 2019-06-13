#pragma once

#include <core/platform.h>
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

struct FileHdlHandle {};

using FileHdl = Handle<FileHdlHandle, ui32>;
using FileSize = si64;
using FilePos = si64;

}
