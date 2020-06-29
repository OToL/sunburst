#include <core/os.h>
#include <core/error.h>
#include <core/conversion.h>

#include <libc++/cstdio>
#include <libc++/cunistd>

#include <direct.h>

namespace sb {

char * getWorkingDirectory(char * buff, usize buff_capacity)
{
    sbAssert(nullptr != buff);

    return _getcwd(buff, numericCast<int>(buff_capacity));
}

void outputDebugString(char const * msg)
{
    fputs(msg, stdout);
}

} // namespace sb
