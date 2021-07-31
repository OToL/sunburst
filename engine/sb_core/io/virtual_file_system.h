#pragma once

#include "io.h"
#include "file.h"
#include <sb_core/hash/hash_str.h>
#include <sb_core/string/fix_string.h>

#include <sb_core/core.h>

#include <sb_std/span>

namespace sb {

class IAllocator;

namespace internal {
    class VirtualFileSystemImpl;
}

struct VirtualFileSystem
{
    using LayerName = HashStr;

    struct LayerDesc
    {
        LayerName name;
        FString<VFS_PATH_MAX_LEN + 1> vfs_path;
        FString<LOCAL_PATH_MAX_LEN + 1> local_path;
    };

    struct InitDesc
    {
        sbstd::span<LayerDesc> layers;
    };

    b8 initialize(InitDesc const & init);
    b8 terminate();

    b8 fileExists(char const * path);

    File openFile(char const * path, FileWriteMode mode = FileWriteMode::APPEND, FileFormat fmt = FileFormat::BIN,
                  bool create_if_not_exist = false);
    File openFileWrite(char const * path, FileWriteMode mode = FileWriteMode::APPEND, FileFormat fmt = FileFormat::BIN,
                       bool create_if_not_exist = false);
    File openFileRead(char const * path, FileFormat fmt = FileFormat::BIN);

    File createFile(char const * path, FileFormat fmt = FileFormat::BIN);
    File createFileWrite(char const * path, FileFormat fmt = FileFormat::BIN);

    void closeFile(File hdl);

    FileSize readFile(File hdl, sbstd::span<u8> buffer, FileSize cnt = -1);
    FileSize writeFile(File hdl, sbstd::span<u8 const> buffer, FileSize cnt = -1);

    FileSize getFileLength(File hdl);
    FileProps getFileProps(File hdl);

    sbstd::span<u8> readFile(char const * path, IAllocator & alloc, FileFormat fmt = FileFormat::BIN);

    static b8 createDefault(InitDesc const & init);
    static b8 destroyDefault();
    static VirtualFileSystem * getDefault();

private:
    internal::VirtualFileSystemImpl * _impl;
};

using VFS = VirtualFileSystem;

} // namespace sb
