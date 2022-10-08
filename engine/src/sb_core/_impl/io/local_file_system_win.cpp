#include <sb_core/extern/windows/windows.h>

// @todo: use Win32 API (CreateFile, etc.)
#include "local_file_system_posix.hpp"

sb::b8 sb::localFileExists(char const * path)
{
    DWORD dwAttrib = GetFileAttributesA(path);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
