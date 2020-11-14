#pragma once

#if defined(SB_PLATFORM_MACOS)
#    include "_impl/api/macos/compiler_macos.h"
#elif defined(SB_PLATFORM_WINDOWS)
#    if defined(SB_COMPILER_MSVC)
#        include "_impl/api/windows/compiler_msvc_win.h"
#    elif defined(SB_COMPILER_CLANG)
#        include "_impl/api/windows/compiler_clang_win.h"
#    else
#        error "Unsupported windows compiler"
#    endif
#elif defined(SB_PLATFORM_LINUX)
#    include "_impl/api/windows/compiler_linux.h"
#else
#    error "Unsupported platform"
#endif

#define __sb_restrict __sb_restrict_internal

namespace sb {

void debugBreak();

}

#define sbStaticUnreachable(msg) sbStaticUnreachableInternal(msg)
