#pragma once

#include <core/types.h>
#include "io.h"

#include <wstd/span>

namespace sb {

class IFileSystemLayer
{
public:
    struct FileId
    {
        void * m_value;
    };

    virtual ~IFileSystemLayer() { }

    virtual FileSize readFile(FileId hdl, ui8 * buffer, FileSize cnt) = 0;

    virtual FileSize getFileLength(FileId hdl) = 0;

    virtual FileId openFileRead(char const * path, FileFormat fmt) = 0;

    virtual FileId openFileWrite(char const * path, FileWriteMode mode, FileFormat fmt) = 0;

    virtual FileId createFile(char const * path, FileFormat fmt) = 0;

    virtual void closeFile(FileId) = 0;

    virtual char const * getPhysicalPath() const = 0;
};

} // namespace sb
