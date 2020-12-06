#pragma once

#include <sb_core/core.h>
#include <sb_core/io/io.h>
#include "io.h"

namespace sb::internal {

LayerFile platformOpenFileRead(char const * path, FileFormat fmt);

LayerFile platformOpenFileReadWrite(char const * path, FileWriteMode mode, FileFormat fmt);

LayerFile platformOpenFileWrite(char const * path, FileWriteMode mode, FileFormat fmt);

LayerFile platformCreateFileReadWrite(char const * path, FileFormat fmt);

LayerFile platformCreateFileWrite(char const * path, FileFormat fmt);

FileSize platformReadFile(LayerFile hdl, u8 * buffer, FileSize count);

FileSize platformWriteFile(LayerFile hdl, u8 const * buffer, FileSize count);

FileSize platformFileLength(LayerFile hdl);

void platformCloseFile(LayerFile hdl);

b8 platformFileExists(char const * path);

} // namespace sb::internal
