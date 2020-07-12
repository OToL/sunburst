#include <core/os.h>
#include <core/error.h>
#include <core/conversion.h>

#include <wstd/cstdio>
#include <wstd/cunistd>

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
