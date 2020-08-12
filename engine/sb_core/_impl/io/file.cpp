#include <sb_core/io/file.h>
#include <sb_core/io/file_system.h>
#include <sb_core/bit.h>
#include <sb_core/error.h>

namespace sb {

File & File::operator=(File && src)
{
    if (this != &src)
    {
        if (m_hdl.isValid())
        {
            FS::closeFile(m_hdl);
        }

        m_hdl = src.m_hdl;
    }

    return *this;
}

File::~File()
{
    if (m_hdl.isValid())
    {
        FS::closeFile(m_hdl);
        m_hdl.reset();
    }
}

FileSize File::read(sbstd::span<ui8> buffer, FileSize cnt)
{
    sbAssert(m_hdl.isValid());

    return FS::readFile(m_hdl, buffer, cnt);
}

FileSize File::getLength()
{
    sbAssert(m_hdl.isValid());

    return FS::getFileLength(m_hdl);
}

} // namespace sb
