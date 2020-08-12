#include <sb_core/os.h>
#include <sb_core/error.h>
#include <sb_core/conversion.h>

#include <sb_std/cstdio>
#include <sb_std/cunistd>

#include <direct.h>

namespace sb {

char * getWorkingDirectory(char * buff, usize buff_capacity)
{
    sbAssert(nullptr != buff);

    return _getcwd(buff, numericConv<int>(buff_capacity));
}

void outputDebugString(char const * msg)
{
    fputs(msg, stdout);
}

} // namespace sb
