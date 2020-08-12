#pragma once

#if defined(SB_PLATFORM_MACOS)
#    include "_impl/api/macos/compiler_macos.h"
#elif defined(SB_PLATFORM_WINDOWS)
#    include "_impl/api/windows/compiler_win.h"
#elif defined(SB_PLATFORM_LINUX)
#    include "_impl/api/windows/compiler_linux.h"
#else
#    error "Unsupported compiler"
#endif

#define __restrict __restrict_internal

namespace sb {

void debugBreak();

}
