#include <sb_core/os.h>

#include <sb_std/cstdio>
#include <sb_std/cunistd>

namespace sb {

char * getWorkingDirectory(sbstd::span<char> buffer)
{
    sbAssert(!buffer.empty());
 
    return getcwd(buffer.data(), numericConv<int>(buffer.size_bytes()));
}

void outputDebugString(char const * msg)
{
    fputs(msg, stdout);
}

} // namespace sb
