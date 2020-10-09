#include <sb_core/io/file_stream.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/error.h>

sb::CFileStream & sb::CFileStream::operator=(CFileStream && src)
{
    if (this != &src)
    {
        reset(src._hdl);
        src._hdl = {};
    }

    return *this;
}

sb::CFileStream::~CFileStream()
{
    reset();
}

sb::FileSize sb::CFileStream::read(sbstd::span<u8> buffer, FileSize cnt)
{
    sbAssert(sb::isValid(_hdl));

    return VFS::readFile(_hdl, buffer, cnt);
}

sb::FileSize sb::CFileStream::write(sbstd::span<u8 const> buffer, FileSize cnt)
{
    sbAssert(sb::isValid(_hdl));

    return VFS::writeFile(_hdl, buffer, cnt);
}

sb::FileSize sb::CFileStream::getLength() const
{
    sbAssert(sb::isValid(_hdl));

    return VFS::getFileLength(_hdl);
}

void sb::CFileStream::reset(FileHdl hdl)
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

sb::FileHdl sb::CFileStream::swap(FileHdl hdl)
{
    FileHdl const prev_hdl = _hdl;
    _hdl = hdl;
    return prev_hdl;
}
