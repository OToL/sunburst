#pragma once

#include <sb_core/core.h>

#if defined(SB_PLATFORM_WINDOWS)
#    include <sb_core/_impl/os_win.h>
#elif defined(SB_PLATFORM_LINUX)
#    include <sb_core/_impl/os_linux.h>
#else
#    error "Unhandled platform"
#endif

#include <sb_std/span>

namespace sb {

using internal::SYS_PATH_SEPARATOR;
using internal::SYS_PATH_MAX_LEN;

void getWorkingDirectory(sbstd::span<char> buffer);
void outputDebugString(char const * msg);

} // namespace sb
