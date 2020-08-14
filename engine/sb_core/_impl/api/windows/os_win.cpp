#include <sb_core/os.h>
#include <sb_core/error.h>
#include <sb_core/conversion.h>

#include <sb_std/cstdio>
#include <sb_std/cunistd>

#include <direct.h>

namespace sb {

char * getWorkingDirectory(sbstd::span<char> buffer)
{
    sbAssert(!buffer.empty());

    return _getcwd(buffer.data(), numericConv<int>(buffer.size_bytes()));
}

void outputDebugString(char const * msg)
{
    fputs(msg, stdout);
}

} // namespace sb
