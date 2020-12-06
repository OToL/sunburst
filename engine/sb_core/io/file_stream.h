#pragma once

#include <sb_core/core.h>
#include "io.h"
#include "file.h"

#include <sb_std/span>

namespace sb {

// @todo: implement buffered input/output
class FileStream
{
public:
    FileStream() = default;

    FileStream(FileStream const &) = delete;
    FileStream & operator=(FileStream const &) = delete;

    explicit FileStream(File hdl)
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
        return sb::file_isValid(_hdl);
    }

    FileSize read(sbstd::span<u8> buffer, FileSize cnt = -1);

    FileSize write(sbstd::span<u8 const> buffer, FileSize cnt = -1);

    FileSize getLength() const;

    void reset(File hdl = {});

    File swap(File hdl);

private:
    File _hdl;
};

} // namespace sb
