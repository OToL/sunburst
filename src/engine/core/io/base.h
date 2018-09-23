#pragma once

#include <core/platform.h>
#include <core/typed_id.h>

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

struct FileHdlTypedId {};

using FileHdl = TypedId<FileHdlTypedId, ui32>;
using FileSize = si64;
using FilePos = si64;

}
