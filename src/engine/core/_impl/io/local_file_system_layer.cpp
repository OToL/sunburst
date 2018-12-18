#include "local_file_system_layer.h"
#include "file_system_platform.h"
#include <core/io/path.h>

namespace sb {

    LocalFileSystemLayer::LocalFileSystemLayer(char const * physical_path)
    {
        m_physical_path = physical_path;

        if (m_physical_path.back() != *PPath::SEPARATOR)
        {
            m_physical_path.push_back(*PPath::SEPARATOR);
        }
    }

    FileSize LocalFileSystemLayer::readFile(FileId file_id, ui8 * buffer, FileSize cnt)
    {
        return platformReadFile({file_id.m_value}, buffer, cnt);
    }

    FileSize LocalFileSystemLayer::getFileLength(FileId file_id)
    {
        return platformFileLength({file_id.m_value});
    }

    IFileSystemLayer::FileId LocalFileSystemLayer::openFileRead(char const * path, FileFormat fmt)
    {
        StaticPhysicalPath file_path(m_physical_path);
        file_path += path;

        return {platformOpenFileRead(file_path.c_str(), fmt).m_value};
    }

    IFileSystemLayer::FileId LocalFileSystemLayer::openFileWrite(char const * path, FileWriteMode mode, FileFormat fmt)
    {
        StaticPhysicalPath file_path(m_physical_path);
        file_path += path;

        return {platformOpenFileWrite(file_path.c_str(), mode, fmt).m_value};
    }

    IFileSystemLayer::FileId LocalFileSystemLayer::createFile(char const * path, FileFormat fmt)
    {
        StaticPhysicalPath file_path(m_physical_path);
        file_path += path;

        return {platformCreateFile(file_path.c_str(), fmt).m_value};
    }

    void LocalFileSystemLayer::closeFile(FileId file_id)
    {
        platformCloseFile({file_id.m_value});
    }
}
