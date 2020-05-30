#pragma once

#if defined(SB_PLATFORM_WINDOWS)
#   include "_impl/platform/os_win.h"
#elif defined(SB_PLATFORM_LINUX)
#   include "_impl/platform/os_linux.h"
#else
#   error "Unhandled platform"
#endif

#include "types.h"

namespace sb {

constexpr usize GLOBAL_HEAP_ALIGNMENT = 8;
constexpr char const * FILE_PATH_SEPARATOR = detail::FILE_PATH_SEPARATOR;

char * getWorkingDirectory(char * buff, usize buff_capacity);
void outputDebugString(char const * msg);

}
