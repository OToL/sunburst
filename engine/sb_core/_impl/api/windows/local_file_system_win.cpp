#include <winapi/windows.h>

// @todo: use Win32 API (CreateFile, etc.)
#include <sb_core/_impl/api/posix/local_file_system_posix.hpp>

sb::b8 sb::internal::platformFileExists(char const * path)
{
    DWORD dwAttrib = GetFileAttributesA(path);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
