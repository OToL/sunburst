#pragma once

#include "io.h"
#include "file_hdl.h"
#include <sb_core/hash/hash_str.h>
#include <sb_core/string/static_string.h>

#include <sb_core/core.h>

#include <sb_std/span>

namespace sb {

class IFileSystemLayer;

// @todo: Review the different ways to open/create files vs fopen options
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

    static b8 fileExists(char const * path);
    // enumerate
    // delete file

    static FileHdl openFile(char const * path, FileWriteMode mode = FileWriteMode::APPEND,
                            FileFormat fmt = FileFormat::Bin, bool create_if_not_exist = false);
    static FileHdl openFileWrite(char const * path, FileWriteMode mode = FileWriteMode::APPEND,
                                 FileFormat fmt = FileFormat::Bin, bool create_if_not_exist = false);
    static FileHdl openFileRead(char const * path, FileFormat fmt = FileFormat::Bin);

    // Overwite file if exists
    static FileHdl createFile(char const * path, FileFormat fmt = FileFormat::Bin);
    static FileHdl createFileWrite(char const * path, FileFormat fmt = FileFormat::Bin);

    static void closeFile(FileHdl hdl);

    static FileSize readFile(FileHdl hdl, sbstd::span<ui8> buffer, FileSize cnt = -1);
    // write file

    static FileSize getFileLength(FileHdl hdl);
};

using VFS = VirtualFileSystem;

} // namespace sb
