#include <core/os.h>

#include <libc++/cstdio>
#include <libc++/cunistd>

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
