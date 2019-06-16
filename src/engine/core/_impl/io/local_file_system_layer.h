#pragma once

#include <core/platform.h>
#include <core/string/static_string.h>

#include <core/io/file_system_layer.h>
#include <core/io/io.h>
#include <core/io/path.h>

namespace sb {

using StaticPhysicalPath = StaticString<PPath::MAX_LEN + 1>;

class LocalFileSystemLayer : public IFileSystemLayer
{
public:
    ~LocalFileSystemLayer() override {}

    LocalFileSystemLayer(char const * physical_path);

    FileSize readFile(FileId file_id, ui8 * buffer, FileSize cnt) override;

    FileSize getFileLength(FileId file_id) override;

    FileId openFileRead(char const * path, FileFormat fmt) override;

    FileId openFileWrite(char const * path, FileWriteMode mode, FileFormat fmt) override;

    FileId createFile(char const * path, FileFormat fmt) override;

    void closeFile(FileId file_id) override;

    char const * getPhysicalPath() const override
    {
        return m_physical_path.c_str();
    }

private:
    StaticPhysicalPath m_physical_path;
};

}
