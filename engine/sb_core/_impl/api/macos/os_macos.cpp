#include <sb_core/system.h>

#include <sb_std/cstdio>
#include <sb_std/cunistd>

char * sb::getWorkingDirectory(sbstd::span<char> buffer)
{
    sb_assert(!buffer.empty());

    return getcwd(buffer.data(), truncValue<int>(buffer.size_bytes()));
}

void sb::outputDebugString(char const * msg)
{
    fputs(msg, stdout);
}
