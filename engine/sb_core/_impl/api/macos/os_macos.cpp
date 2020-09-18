#include <sb_core/os.h>

#include <sb_std/cstdio>
#include <sb_std/cunistd>

char * sb::getWorkingDirectory(sbstd::span<char> buffer)
{
    sbAssert(!buffer.empty());

    return getcwd(buffer.data(), numericConv<int>(buffer.size_bytes()));
}

void sb::outputDebugString(char const * msg)
{
    fputs(msg, stdout);
}
