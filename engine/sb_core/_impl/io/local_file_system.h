#pragma once

#include <sb_core/core.h>
#include <sb_core/io/io.h>
#include "file_system.h"

namespace sb {

File openLocalFileRead(char const * path, FileFormat fmt);

File openLocalFileWrite(char const * path, FileWriteMode mode, FileFormat fmt);

File createLocalFile(char const * path, FileFormat fmt);

File createLocalFileWrite(char const * path, FileFormat fmt);

FileSize readLocalFile(File hdl, u8 * buffer, FileSize count);

FileSize writeLocalFile(File hdl, u8 const * buffer, FileSize count);

FileSize getLocalFileLength(File hdl);

void closeLocalFile(File hdl);

b8 localFileExists(char const * path);

b8 isLocalFileValid(File hdl);

FileSystem getLocalFileSystem();

} // namespace sb::internal
