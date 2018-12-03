#include "physical_file_system_layer.h"
#include "file_system_platform.h"

namespace sb {

    PhysicalFileSystemLayer::PhysicalFileSystemLayer(char const * physical_path)
    {
        m_physical_path = physical_path;

        if (m_physical_path.back() != *PHYSICAL_PATH_SEPARATOR)
        {
            m_physical_path.push_back(*PHYSICAL_PATH_SEPARATOR);
        }
    }

    FileSize PhysicalFileSystemLayer::readFile(FileId file_id, ui8 * buffer, FileSize cnt)
    {
        return platformReadFile({file_id.m_value}, buffer, cnt);
    }

    FileSize PhysicalFileSystemLayer::getFileLength(FileId file_id)
    {
        return platformFileLength({file_id.m_value});
    }

    IFileSystemLayer::FileId PhysicalFileSystemLayer::openFileRead(char const * path, FileFormat fmt)
    {
        StaticPhysicalPath file_path(m_physical_path);
        file_path += path;

        return {platformOpenFileRead(file_path.c_str(), fmt).m_value};
    }

    IFileSystemLayer::FileId PhysicalFileSystemLayer::openFileWrite(char const * path, FileWriteMode mode, FileFormat fmt)
    {
        StaticPhysicalPath file_path(m_physical_path);
        file_path += path;

        return {platformOpenFileWrite(file_path.c_str(), mode, fmt).m_value};
    }

    IFileSystemLayer::FileId PhysicalFileSystemLayer::createFile(char const * path, FileFormat fmt)
    {
        StaticPhysicalPath file_path(m_physical_path);
        file_path += path;

        return {platformCreateFile(file_path.c_str(), fmt).m_value};
    }

    void PhysicalFileSystemLayer::closeFile(FileId file_id)
    {
        platformCloseFile({file_id.m_value});
    }
}
