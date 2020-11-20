#pragma once

#include <sb_core/core.h>
#include "io.h"
#include "file_hdl.h"

#include <sb_std/span>

namespace sb {

// @todo: implement buffered input/output
class FileStream
{
public:
    FileStream() = default;

    FileStream(FileStream const &) = delete;
    FileStream & operator=(FileStream const &) = delete;

    explicit FileStream(FileHdl hdl)
        : _hdl(hdl)
    {
    }

    FileStream(FileStream && src)
        : _hdl(src._hdl)
    {
        src._hdl = {};
    }

    ~FileStream();

    FileStream & operator=(FileStream && src);

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
