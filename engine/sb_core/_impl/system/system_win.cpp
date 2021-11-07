#include <sb_core/system/system.h>
#include <sb_core/error/error.h>
#include <sb_core/cast.h>
#include <sb_core/system/windows/windows.h>

#include <sb_std/cstdio>

#include <direct.h>

void sb::getWorkingDirectory(sbstd::span<char> buffer)
{
    sb_assert(!buffer.empty());

    _getcwd(buffer.data(), integral_cast<int>(buffer.size_bytes()));
}

void sb::outputDebugString(char const * msg)
{
    OutputDebugString(msg);
}
