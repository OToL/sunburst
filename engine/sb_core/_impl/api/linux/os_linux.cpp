#include <sb_core/os.h>

#include <sb_std/cstdio>
#include <sb_std/cunistd>

namespace sb {

char * getWorkingDirectory(char * buff, usize buff_capacity)
{
    return getcwd(buff, buff_capacity);
}

void outputDebugString(char const * msg)
{
    fputs(msg, stdout);
}

} // namespace sb
