#pragma once

#include "io.h"
#include "file_hdl.h"
#include <sb_core/hash/hash_str.h>
#include <sb_core/string/fix_string.h>

#include <sb_core/core.h>

#include <sb_std/span>

namespace sb {

class IAllocator;

namespace internal {
    class VirtualFileSystemImpl;
}

class VirtualFileSystem
{
public:

    using LayerName = HashStr;

    struct LayerDesc
    {
        LayerName name;
        FString<VFS_PATH_MAX_LEN + 1> vfs_path;
        FString<SYS_PATH_MAX_LEN + 1> local_path;
    };

    struct InitDesc
    {
        sbstd::span<LayerDesc> layers;
    };

    b8 initialize(InitDesc const & init);
    b8 terminate();

    b8 fileExists(char const * path);

    FileHdl openFile(char const * path, FileWriteMode mode = FileWriteMode::APPEND, FileFormat fmt = FileFormat::BIN,
                  bool create_if_not_exist = false);
    FileHdl openFileWrite(char const * path, FileWriteMode mode = FileWriteMode::APPEND, FileFormat fmt = FileFormat::BIN,
                       bool create_if_not_exist = false);
    FileHdl openFileRead(char const * path, FileFormat fmt = FileFormat::BIN);

    FileHdl createFile(char const * path, FileFormat fmt = FileFormat::BIN);
    FileHdl createFileWrite(char const * path, FileFormat fmt = FileFormat::BIN);

    void closeFile(FileHdl hdl);

    FileSize readFile(FileHdl hdl, sbstd::span<u8> buffer, FileSize cnt = -1);
    FileSize writeFile(FileHdl hdl, sbstd::span<u8 const> buffer, FileSize cnt = -1);

    FileSize getFileLength(FileHdl hdl);
    FileProps getFileProps(FileHdl hdl);

    sbstd::span<u8> readFile(char const * path, IAllocator & alloc, FileFormat fmt = FileFormat::BIN);

    static b8 createDefault(InitDesc const & init);
    static b8 destroyDefault();
    static VirtualFileSystem * getDefault();

private:
    internal::VirtualFileSystemImpl * _impl;
};

using VFS = VirtualFileSystem;

} // namespace sb
