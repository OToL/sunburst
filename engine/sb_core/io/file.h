#pragma once

#include <sb_base/base.h>
#include "io.h"

#include <sb_std/span>

namespace sb {

class File
{
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

    File & operator=(File const &) = delete;
    File(File const &) = delete;

    ~File();

    File & operator=(File && src);

    FileHdl getHdl() const
    {
        return m_hdl;
    }

    b8 isNull() const
    {
        return !m_hdl.isValid();
    }

    FileSize read(sbstd::span<ui8> buffer, FileSize cnt = -1);

    FileSize getLength();

private:
    FileHdl m_hdl;
};

} // namespace sb