#include <sb_core/io/file.h>
#include <sb_core/io/file_system.h>
#include <sb_core/bit.h>
#include <sb_core/error.h>

namespace sb {

File & File::operator=(File && src)
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

File::~File()
{
    if (isValid(m_hdl))
    {
        FS::closeFile(m_hdl);
        m_hdl = {};
    }
}

FileSize File::read(sbstd::span<ui8> buffer, FileSize cnt)
{
    sbAssert(isValid(m_hdl));

    return FS::readFile(m_hdl, buffer, cnt);
}

FileSize File::getLength()
{
    sbAssert(isValid(m_hdl));

    return FS::getFileLength(m_hdl);
}

} // namespace sb
