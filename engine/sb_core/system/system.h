#pragma once

#include <sb_core/core.h>

#if defined(SB_PLATFORM_WINDOWS)
#    include <sb_core/_impl/system/system_win.h>
#elif defined(SB_PLATFORM_LINUX)
#    include <sb_core/_impl/system/system_linux.h>
#else
#    error "Unhandled platform"
#endif

#include <sb_std/span>

namespace sb {

inline constexpr char SYS_PATH_SEPARATOR = internal::SYS_PATH_SEPARATOR;
inline constexpr u32 SYS_PATH_MAX_LEN = internal::SYS_PATH_MAX_LEN;

void getWorkingDirectory(sbstd::span<char> buffer);
void outputDebugString(char const * msg);

} // namespace sb
