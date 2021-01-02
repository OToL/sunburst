#pragma once

#include "io.h"
#include "file.h"
#include <sb_core/hash/hash_str.h>
#include <sb_core/string/fix_string.h>

#include <sb_core/core.h>

#include <sb_std/span>

namespace sb {

class IAllocator;

struct VirtualFileSystem
{
    using LayerName = HashStr;

    struct LayerInitDesc
    {
        LayerName name;
        FString<VFS_PATH_MAX_LEN + 1> vfs_path;
        FString<LOCAL_PATH_MAX_LEN + 1> local_path;
    };

    struct InitDesc
    {
        sbstd::span<LayerInitDesc> layers;
    };

    static b8 initialize(InitDesc const & init);
    static b8 terminate();

    static b8 fileExists(char const * path);

    static File openFile(char const * path, FileWriteMode mode = FileWriteMode::APPEND,
                         FileFormat fmt = FileFormat::BIN, bool create_if_not_exist = false);
    static File openFileWrite(char const * path, FileWriteMode mode = FileWriteMode::APPEND,
                              FileFormat fmt = FileFormat::BIN, bool create_if_not_exist = false);
    static File openFileRead(char const * path, FileFormat fmt = FileFormat::BIN);

    // Overwite file if exists
    static File createFile(char const * path, FileFormat fmt = FileFormat::BIN);
    static File createFileWrite(char const * path, FileFormat fmt = FileFormat::BIN);

    static void closeFile(File hdl);

    static FileSize readFile(File hdl, sbstd::span<u8> buffer, FileSize cnt = -1);
    static FileSize writeFile(File hdl, sbstd::span<u8 const> buffer, FileSize cnt = -1);

    static FileSize getFileLength(File hdl);
    static FileProps getFileProps(File hdl);

    static sbstd::span<u8> readFile(char const * path, IAllocator & alloc, FileFormat fmt = FileFormat::BIN);
};

using VFS = VirtualFileSystem;

} // namespace sb
