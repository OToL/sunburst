#include <sb_core/io/file_stream.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/error.h>

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
    sb_assert(file_hdl::isValid(_hdl));

    return vfs::readFile(_hdl, buffer, cnt);
}

sb::FileSize sb::FileStream::write(sbstd::span<u8 const> buffer, FileSize cnt)
{
    sb_assert(file_hdl::isValid(_hdl));

    return vfs::writeFile(_hdl, buffer, cnt);
}

sb::FileSize sb::FileStream::getLength() const
{
    sb_assert(file_hdl::isValid(_hdl));

    return vfs::getFileLength(_hdl);
}

void sb::FileStream::reset(FileHdl hdl)
{
    if (hdl != _hdl)
    {
        if (file_hdl::isValid(_hdl))
        {
            vfs::closeFile(_hdl);
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
