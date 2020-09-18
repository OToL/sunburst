#pragma once

#include <sb_core/core.h>
#include <sb_core/io/file_system.h>

namespace sb::internal {

struct PlatformFileHdl
{
    void * m_value;
};

PlatformFileHdl platformOpenFileRead(char const * path, FileFormat fmt);

PlatformFileHdl platformOpenFileWrite(char const * path, FileWriteMode mode, FileFormat fmt);

PlatformFileHdl platformCreateFile(char const * path, FileFormat fmt);

FileSize platformReadFile(PlatformFileHdl hdl, ui8 * buffer, FileSize count);

FileSize platformFileLength(PlatformFileHdl hdl);

void platformCloseFile(PlatformFileHdl hdl);

} // namespace sb::internal
