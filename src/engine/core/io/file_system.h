#pragma once

#include <core/platform.h>
#include <core/hash/hash_str.h>
#include <core/container/span.h>
#include "base.h"

namespace sb {

struct FileSystem
{
    sbConstructProtect(FileSystem);

    using LayerName = HashStr;

    struct LayerDesc
    {
        char const * m_logical_path;
        char const * m_physical_path; // TODO: Possibility to specify something else than physical FS layer
        LayerName m_name;
    };

    struct InitParams
    {
        Span<LayerDesc> m_layers;
    };

    static usize const MAX_CONCURRENT_OPENED_FILES = 50U;

    static b8 initialize(InitParams const & init = {});

    static b8 terminate();

    static FileHdl openFileRead(char const * path, FileFormat fmt = FileFormat::Bin);

    static FileHdl openFileWrite(char const * path, FileWriteMode mode = FileWriteMode::APPEND, FileFormat fmt = FileFormat::Bin);

    static FileHdl createFile(char const * path, FileFormat fmt = FileFormat::Bin);

    static void closeFile(FileHdl hdl);

    static FileSize readFile(FileHdl hdl, Span<ui8> buffer, FileSize cnt = -1);

    static FileSize getFileLength(FileHdl hdl);

protected:
};

using FS = FileSystem;

} // namespace sb
