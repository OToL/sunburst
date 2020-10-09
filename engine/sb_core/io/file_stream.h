#pragma once

#include <sb_core/core.h>
#include "io.h"
#include "file_hdl.h"

#include <sb_std/span>

namespace sb {

// @todo: implement buffered input/output
class CFileStream
{
    CFileStream(CFileStream const &) = delete;
    CFileStream & operator=(CFileStream const &) = delete;

public:
    CFileStream() = default;

    explicit CFileStream(FileHdl hdl)
        : _hdl(hdl)
    {
    }

    CFileStream(CFileStream && src)
        : _hdl(src._hdl)
    {
        src._hdl = {};
    }

    ~CFileStream();

    CFileStream & operator=(CFileStream && src);

    b8 isValid() const
    {
        return sb::isValid(_hdl);
    }

    FileSize read(sbstd::span<u8> buffer, FileSize cnt = -1);

    FileSize write(sbstd::span<u8 const> buffer, FileSize cnt = -1);

    FileSize getLength() const;

    void reset(FileHdl hdl = {});

    FileHdl swap(FileHdl hdl);

private:
    FileHdl _hdl;
};

} // namespace sb
