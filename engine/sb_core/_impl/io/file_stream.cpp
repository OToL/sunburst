#include <sb_core/io/file_stream.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/error/error.h>

sb::FileStream & sb::FileStream::operator=(FileStream && src)
{
    if (this != &src)
    {
        reset(src._hdl);
        src._hdl = {};
    }

    return *this;
}

sb::FileStream::~FileStream()
{
    reset();
}

sb::FileSize sb::FileStream::read(sbstd::span<u8> buffer, FileSize cnt)
{
    sbAssert(sb::isValid(_hdl));

    return VFS::readFile(_hdl, buffer, cnt);
}

sb::FileSize sb::FileStream::write(sbstd::span<u8 const> buffer, FileSize cnt)
{
    sbAssert(sb::isValid(_hdl));

    return VFS::writeFile(_hdl, buffer, cnt);
}

sb::FileSize sb::FileStream::getLength() const
{
    sbAssert(sb::isValid(_hdl));

    return VFS::getFileLength(_hdl);
}

void sb::FileStream::reset(FileHdl hdl)
{
    if (hdl != _hdl)
    {
        if (sb::isValid(_hdl))
        {
            VFS::closeFile(_hdl);
        }

        _hdl = hdl;
    }
}

sb::FileHdl sb::FileStream::swap(FileHdl hdl)
{
    FileHdl const prev_hdl = _hdl;
    _hdl = hdl;
    return prev_hdl;
}
