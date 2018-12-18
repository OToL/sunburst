#pragma once

#include <core/platform.h>
#include <core/hash/hash_str.h>
#include <core/memory/unique_ptr.h>
#include "base.h"

#include <libc++/span>

namespace sb {

class IFileSystemLayer;

// TODO: abstract the filesystem path time i.e. char * vs wchar_t *
struct FileSystem
{
    sbConstructProtect(FileSystem);

    using LayerName = HashStr;
    using LayerPtr = UniquePtr<IFileSystemLayer>;

    struct LayerDesc
    {
        char const * m_logical_path;
        LayerPtr m_layer;
        LayerName m_name;
    };

    struct InitParams
    {
        wstd::span<LayerDesc> m_layers;
    };

    static usize const MAX_CONCURRENT_OPENED_FILES = 50U;

    static b8 initialize(InitParams & init);

    static b8 terminate();

    static FileHdl openFileRead(char const * path, FileFormat fmt = FileFormat::Bin);

    static FileHdl openFileWrite(char const * path, FileWriteMode mode = FileWriteMode::APPEND, FileFormat fmt = FileFormat::Bin);

    static FileHdl createFile(char const * path, FileFormat fmt = FileFormat::Bin);

    static void closeFile(FileHdl hdl);

    static FileSize readFile(FileHdl hdl, wstd::span<ui8> buffer, FileSize cnt = -1);

    static FileSize getFileLength(FileHdl hdl);

    static LayerPtr createLocalFileSystemLayer(char const * local_root_path);

    static char const * getLayerPhysicalPath(LayerName name);
};

using FS = FileSystem;

} // namespace sb
