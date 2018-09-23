#include <core/io/file.h>
#include <core/io/file_system.h>
#include <core/math.h>
#include <core/error.h>

namespace sb {

File & File::operator=(File && src)
{
    if (this != &src)
    {
        if (!m_hdl.isNull())
        {
            FS::closeFile(m_hdl);
        }

        m_hdl = src.m_hdl;
    }

    return *this;
}

File::~File()
{
    if (!m_hdl.isNull())
    {
        FS::closeFile(m_hdl);
        m_hdl.reset();
    }
}

FileSize File::read(Span<ui8> buffer, FileSize cnt)
{
    sbAssert(!m_hdl.isNull());

    return FS::readFile(m_hdl, buffer, cnt);
}

FileSize File::getLength()
{
    sbAssert(!m_hdl.isNull());

    return FS::getFileLength(m_hdl);
}

} // namespace sb
