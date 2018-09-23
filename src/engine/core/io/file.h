#pragma once

#include <core/platform.h>
#include <core/container/span.h>
#include "base.h"

namespace sb {

class File
{
    sbCopyProtect(File);

    friend class FileSystemImpl;

    void reset()
    {
        m_hdl.reset();
    }

public:
    File() = default;

    File(FileHdl hdl)
        : m_hdl(hdl)
    {
    }

    File(File && src)
        : m_hdl(src.m_hdl)
    {
        src.m_hdl.reset();
    }

    ~File();

    File & operator=(File && src);

    FileHdl getHdl() const
    {
        return m_hdl;
    }

    b8 isNull() const
    {
        return m_hdl.isNull();
    }

    FileSize read(Span<ui8> buffer, FileSize cnt = -1);

    FileSize getLength();

private:
    FileHdl m_hdl;
};

} // namespace sb
