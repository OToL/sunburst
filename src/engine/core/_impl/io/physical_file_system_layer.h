#pragma once

#include <core/platform.h>
#include <core/string/static_string.h>

#include <core/io/file_system_layer.h>
#include <core/io/base.h>

namespace sb {

using StaticPhysicalPath = StaticString<PHYSICAL_PATH_MAX_LEN + 1>;

class PhysicalFileSystemLayer : public IFileSystemLayer
{
public:
    ~PhysicalFileSystemLayer() override {}

    PhysicalFileSystemLayer(char const * physical_path);

    FileSize readFile(FileId file_id, ui8 * buffer, FileSize cnt) override;

    FileSize getFileLength(FileId file_id) override;

    FileId openFileRead(char const * path, FileFormat fmt) override;

    FileId openFileWrite(char const * path, FileWriteMode mode, FileFormat fmt) override;

    FileId createFile(char const * path, FileFormat fmt) override;

    void closeFile(FileId file_id) override;

private:
    StaticPhysicalPath m_physical_path;
};

}
