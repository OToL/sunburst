#pragma once

#include <core/types.h>
#include <core/hash/hash_str.h>
#include <core/unique_ptr.h>
#include "io.h"

#include <libc++/span>

namespace sb {

class IFileSystemLayer;

struct FileSystem
{
    FileSystem() = delete;

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

    static b8 initialize(InitParams const & init);

    static b8 terminate();

    static FileHdl openFileRead(char const * path, FileFormat fmt = FileFormat::Bin);

    static FileHdl openFileWrite(char const * path, FileWriteMode mode = FileWriteMode::APPEND,
                                 FileFormat fmt = FileFormat::Bin);

    static FileHdl createFile(char const * path, FileFormat fmt = FileFormat::Bin);

    static void closeFile(FileHdl hdl);

    static FileSize readFile(FileHdl hdl, wstd::span<ui8> buffer, FileSize cnt = -1);

    static FileSize getFileLength(FileHdl hdl);

    static char const * getLayerPhysicalPath(LayerName name);

    static LayerPtr createLocalFileSystemLayer(char const * local_root_path);
};

using FS = FileSystem;

} // namespace sb