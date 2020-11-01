#pragma once

#include <sb_core/core.h>
#include <sb_core/io/io.h>
#include "io.h"

namespace sb::internal {

LayerFileHdl platformOpenFileRead(char const * path, FileFormat fmt);

LayerFileHdl platformOpenFileReadWrite(char const * path, FileWriteMode mode, FileFormat fmt);

LayerFileHdl platformOpenFileWrite(char const * path, FileWriteMode mode, FileFormat fmt);

LayerFileHdl platformCreateFileReadWrite(char const * path, FileFormat fmt);

LayerFileHdl platformCreateFileWrite(char const * path, FileFormat fmt);

FileSize platformReadFile(LayerFileHdl hdl, u8 * buffer, FileSize count);

FileSize platformWriteFile(LayerFileHdl hdl, u8 const * buffer, FileSize count);

FileSize platformFileLength(LayerFileHdl hdl);

void platformCloseFile(LayerFileHdl hdl);

b8 platformFileExists(char const * path);

} // namespace sb::internal
