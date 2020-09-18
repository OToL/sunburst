#pragma once

#include <sb_core/core.h>

#if defined(SB_PLATFORM_WINDOWS)
#    include "_impl/api/windows/os_win.h"
#elif defined(SB_PLATFORM_LINUX)
#    include "_impl/api/linux/os_linux.h"
#else
#    error "Unhandled platform"
#endif

#include <sb_std/span>

namespace sb {

inline constexpr usize GLOBAL_HEAP_ALIGNMENT = 8;

char * getWorkingDirectory(sbstd::span<char> buffer);
void outputDebugString(char const * msg);

} // namespace sb
