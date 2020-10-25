#pragma once

#if defined(SB_PLATFORM_MACOS)
#    include "_impl/api/macos/compiler_macos.h"
#elif defined(SB_PLATFORM_WINDOWS)
#    include "_impl/api/windows/compiler_win.h"
#elif defined(SB_PLATFORM_LINUX)
#    include "_impl/api/windows/compiler_linux.h"
#else
#    error "Unsupported platform"
#endif

#define __sb_restrict __sb_restrict_internal

#ifdef __cpp_exceptions
#    error "Exceptions must be disabled"
#endif

namespace sb {

void debugBreak();

}
