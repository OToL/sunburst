#pragma once

#include "io.h"
#include "file_hdl.h"
#include <sb_core/hash/hash_str.h>
#include <sb_core/string/static_string.h>

#include <sb_core/core.h>

#include <sb_std/span>

namespace sb {

class IFileSystemLayer;

struct VirtualFileSystem
{
    using LayerName = HashStr;

    struct LayerInitDesc
    {
        LayerName name;
        StaticString<VFS_PATH_MAX_LEN + 1> vfs_path;
        StaticString<LOCAL_PATH_MAX_LEN + 1> local_path;
    };

    struct InitDesc
    {
        sbstd::span<LayerInitDesc> layers;
    };

    static b8 initialize(InitDesc const & init);

    static b8 terminate();

    static FileHdl openFileRead(char const * path, FileFormat fmt = FileFormat::Bin);

    static FileHdl openFileWrite(char const * path, FileWriteMode mode = FileWriteMode::APPEND,
                                 FileFormat fmt = FileFormat::Bin);

    static FileHdl createFile(char const * path, FileFormat fmt = FileFormat::Bin);

    static void closeFile(FileHdl hdl);

    static FileSize readFile(FileHdl hdl, sbstd::span<ui8> buffer, FileSize cnt = -1);

    static FileSize getFileLength(FileHdl hdl);

    static char const * getLayerPhysicalPath(LayerName name);
};

using VFS = VirtualFileSystem;

} // namespace sb
