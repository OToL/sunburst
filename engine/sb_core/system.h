#pragma once

#include <sb_core/core.h>

#if defined(SB_PLATFORM_WINDOWS)
#    include "_impl/api/windows/system_win.h"
#elif defined(SB_PLATFORM_LINUX)
#    include "_impl/api/linux/system_linux.h"
#else
#    error "Unhandled platform"
#endif

#include <sb_std/span>

namespace sb {

inline constexpr char SYS_FILE_PATH_SEPARATOR = internal::SYS_FILE_PATH_SEPARATOR;
inline constexpr u32 SYS_FILE_PATH_MAX_LEN = internal::SYS_FILE_PATH_MAX_LEN;

char * getWorkingDirectory(sbstd::span<char> buffer);
void outputDebugString(char const * msg);

} // namespace sb
