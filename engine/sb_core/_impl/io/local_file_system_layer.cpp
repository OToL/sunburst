#include "local_file_system_layer.h"
#include "file_system_platform.h"
#include <sb_core/io/path.h>

sb::LocalFileSystemLayer::LocalFileSystemLayer(char const * physical_path)
{
    m_physical_path = physical_path;

    if (m_physical_path.back() != PHYS_PATH_SEPARATOR)
    {
        m_physical_path.push_back(PHYS_PATH_SEPARATOR);
    }
}

sb::FileSize sb::LocalFileSystemLayer::readFile(FileId file_id, ui8 * buffer, FileSize cnt)
{
    return internal::platformReadFile({file_id.m_value}, buffer, cnt);
}

sb::FileSize sb::LocalFileSystemLayer::getFileLength(FileId file_id)
{
    return internal::platformFileLength({file_id.m_value});
}

sb::IFileSystemLayer::FileId sb::LocalFileSystemLayer::openFileRead(char const * path,
                                                                    FileFormat fmt)
{
    StaticPhysicalPath file_path(m_physical_path);
    file_path += path;

    return {internal::platformOpenFileRead(file_path.c_str(), fmt).m_value};
}

sb::IFileSystemLayer::FileId
    sb::LocalFileSystemLayer::openFileWrite(char const * path, FileWriteMode mode, FileFormat fmt)
{
    StaticPhysicalPath file_path(m_physical_path);
    file_path += path;

    return {internal::platformOpenFileWrite(file_path.c_str(), mode, fmt).m_value};
}

sb::IFileSystemLayer::FileId sb::LocalFileSystemLayer::createFile(char const * path, FileFormat fmt)
{
    StaticPhysicalPath file_path(m_physical_path);
    file_path += path;

    return {internal::platformCreateFile(file_path.c_str(), fmt).m_value};
}

void sb::LocalFileSystemLayer::closeFile(FileId file_id)
{
    internal::platformCloseFile({file_id.m_value});
}
