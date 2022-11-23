#include <sb_core/system.h>

#include <sb_slw/cstdio>
#include <sb_slw/cunistd>
#include <sb_slw/span>

void sb::getWorkingDirectory(slw::span<char> & buffer)
{
    sb_assert(!buffer.empty());

    getcwd(buffer.data(), integral_cast<int>(buffer.size_bytes()));
}

void sb::outputDebugString(char const * msg)
{
    fputs(msg, stdout);
}
