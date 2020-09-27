#pragma once

#include <sb_core/io/io.h>
#include <sb_core/core.h>
#include "io.h"

namespace sb::internal {

LayerFileHdl platformOpenFileRead(char const * path, FileFormat fmt);

LayerFileHdl platformOpenFileWrite(char const * path, FileWriteMode mode, FileFormat fmt);

LayerFileHdl platformCreateFile(char const * path, FileFormat fmt);

FileSize platformReadFile(LayerFileHdl hdl, ui8 * buffer, FileSize count);

FileSize platformFileLength(LayerFileHdl hdl);

void platformCloseFile(LayerFileHdl hdl);

} // namespace sb::internal
