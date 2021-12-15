#include <sb_core/os.h>

#include <sb_std/cstdio>
#include <sb_std/cunistd>

void sb::getWorkingDirectory(sbstd::span<char> buffer)
{
    sb_assert(!buffer.empty());

    getcwd(buffer.data(), integral_cast<int>(buffer.size_bytes()));
}

void sb::outputDebugString(char const * msg)
{
    fputs(msg, stdout);
}
