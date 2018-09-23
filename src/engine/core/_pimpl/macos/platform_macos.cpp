#include <core/platform.h>

#include <libc++/cstdio>
#include <libc++/cunistd>

namespace sb {

char const * getWorkingDirectory()
{
    return getcwd(nullptr, 0);
}

void outputDebugString(char const * msg) 
{
    fputs(msg, stdout);
}


} // namespace sb
