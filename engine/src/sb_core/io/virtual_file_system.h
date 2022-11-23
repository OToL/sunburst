#pragma once

#include "io.h"
#include "file_hdl.h"
#include <sb_core/hash/hash_str.h>
#include <sb_core/core.h>

#include <sb_slw/span>

namespace sb {

class IAllocator;

namespace virtual_file_system {

    using LayerName = HashStr;

    struct MountResult
    {
        b8 success = false;
        LayerName name;
    };

    b8 initialize();
    b8 isInitialized();
    b8 terminate();

    b8 fileExists(char const * path);

    FileHdl openFileWrite(char const * path, FileWriteMode mode = FileWriteMode::APPEND, FileFormat fmt = FileFormat::BIN);
    FileHdl openFileRead(char const * path, FileFormat fmt = FileFormat::BIN);
    void closeFile(FileHdl hdl);

    FileSize readFile(FileHdl hdl, slw::span<u8> buffer, FileSize cnt = -1);
    FileSize writeFile(FileHdl hdl, slw::span<u8 const> buffer, FileSize cnt = -1);

    FileSize getFileLength(FileHdl hdl);
    FileProps getFileProps(FileHdl hdl);

    slw::span<u8> readFile(char const * path, IAllocator & alloc, FileFormat fmt = FileFormat::BIN);

    MountResult mountLocalFileSystem(char const * vfs_path, char const * local_path, LayerName name = {});
    b8 umount(LayerName name);

} // namespace virtual_file_system

namespace vfs = virtual_file_system;

} // namespace sb
