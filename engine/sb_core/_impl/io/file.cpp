#include <sb_core/io/file.h>
#include <sb_core/io/file_system.h>
#include <sb_core/bit.h>
#include <sb_core/error.h>

sb::File & sb::File::operator=(File && src)
{
    if (this != &src)
    {
        if (isValid(m_hdl))
        {
            FS::closeFile(m_hdl);
        }

        m_hdl = src.m_hdl;
    }

    return *this;
}

sb::File::~File()
{
    if (isValid(m_hdl))
    {
        FS::closeFile(m_hdl);
        m_hdl = {};
    }
}

sb::FileSize sb::File::read(sbstd::span<ui8> buffer, FileSize cnt)
{
    sbAssert(isValid(m_hdl));

    return FS::readFile(m_hdl, buffer, cnt);
}

sb::FileSize sb::File::getLength()
{
    sbAssert(isValid(m_hdl));

    return FS::getFileLength(m_hdl);
}
