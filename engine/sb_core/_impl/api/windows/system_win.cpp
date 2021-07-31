#include <sb_core/system.h>
#include <sb_core/error/error.h>
#include <sb_core/conversion.h>

#include <sb_std/cstdio>

#include <direct.h>
#include <sb_core/api/windows/windows.h>

char * sb::getWorkingDirectory(sbstd::span<char> buffer)
{
    sb_assert(!buffer.empty());

    return _getcwd(buffer.data(), truncValue<int>(buffer.size_bytes()));
}

void sb::outputDebugString(char const * msg)
{
    OutputDebugString(msg);
}
