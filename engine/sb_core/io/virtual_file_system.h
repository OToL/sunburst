#pragma once

#include "io.h"
#include "file_hdl.h"
#include <sb_core/hash/hash_str.h>
#include <sb_core/string/fix_string.h>

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
        FixString<VFS_PATH_MAX_LEN + 1> vfs_path;
        FixString<LOCAL_PATH_MAX_LEN + 1> local_path;
    };

    struct InitDesc
    {
        sbstd::span<LayerInitDesc> layers;
    };

    static b8 initialize(InitDesc const & init);
    static b8 terminate();

    static b8 fileExists(char const * path);

    static FileHdl openFile(char const * path, FileWriteMode mode = FileWriteMode::APPEND,
                            FileFormat fmt = FileFormat::BIN, bool create_if_not_exist = false);
    static FileHdl openFileWrite(char const * path, FileWriteMode mode = FileWriteMode::APPEND,
                                 FileFormat fmt = FileFormat::BIN, bool create_if_not_exist = false);
    static FileHdl openFileRead(char const * path, FileFormat fmt = FileFormat::BIN);

    // Overwite file if exists
    static FileHdl createFile(char const * path, FileFormat fmt = FileFormat::BIN);
    static FileHdl createFileWrite(char const * path, FileFormat fmt = FileFormat::BIN);

    static void closeFile(FileHdl hdl);

    static FileSize readFile(FileHdl hdl, sbstd::span<u8> buffer, FileSize cnt = -1);
    static FileSize writeFile(FileHdl hdl, sbstd::span<u8 const> buffer, FileSize cnt = -1);

    static FileSize getFileLength(FileHdl hdl);
    static FileProps getFileProps(FileHdl hdl);
};

using VFS = VirtualFileSystem;

} // namespace sb
