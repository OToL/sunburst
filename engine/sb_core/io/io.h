#pragma once

#include <sb_core/core.h>

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

using FileSize = si64;
using FilePos = si64;

} // namespace sb
