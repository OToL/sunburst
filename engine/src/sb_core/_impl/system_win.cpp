#include <sb_core/system.h>
#include <sb_core/error.h>
#include <sb_core/cast.h>
#include <sb_core/extern/windows/windows.h>

#include <sb_slw/cstdio>
#include <sb_slw/span>

#include <direct.h>

void sb::getWorkingDirectory(slw::span<char> & buffer)
{
    sb_assert(!buffer.empty());

    _getcwd(buffer.data(), integral_cast<int>(buffer.size_bytes()));
}

void sb::outputDebugString(char const * msg)
{
    // Changed it to 'puts' because it was not visible from the terminal
    //OutputDebugString(msg);
    puts(msg);
}
