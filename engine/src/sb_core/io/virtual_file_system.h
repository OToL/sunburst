#pragma once

#include "io.h"
#include "file_hdl.h"
#include <sb_core/hash/hash_str.h>
#include <sb_core/string/fix_string.h>

#include <sb_core/core.h>

#include <sb_slw/span>

namespace sb {

class IAllocator;

struct virtual_file_system
{
public:
    using LayerName = HashStr;

    struct MountResult
    {
        b8 success;
        LayerName name;
    };

    static b8 initialize();
    static b8 terminate();

    static b8 fileExists(char const * path);

    static FileHdl openFileWrite(char const * path, FileWriteMode mode = FileWriteMode::APPEND,
                                 FileFormat fmt = FileFormat::BIN);
    static FileHdl openFileRead(char const * path, FileFormat fmt = FileFormat::BIN);
    static void closeFile(FileHdl hdl);

    static FileSize readFile(FileHdl hdl, slw::span<u8> buffer, FileSize cnt = -1);
    static FileSize writeFile(FileHdl hdl, slw::span<u8 const> buffer, FileSize cnt = -1);

    static FileSize getFileLength(FileHdl hdl);
    static FileProps getFileProps(FileHdl hdl);

    static slw::span<u8> readFile(char const * path, IAllocator & alloc, FileFormat fmt = FileFormat::BIN);

    static MountResult mountLocalFileSystem(char const * vfs_path, char const * local_path, LayerName name = {});
    static b8 umount(LayerName name);
};

using vfs = virtual_file_system;

} // namespace sb
