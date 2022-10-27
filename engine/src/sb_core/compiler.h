#pragma once

#if defined(SB_PLATFORM_MACOS)
#    include "_impl/compiler_macos.h"
#elif defined(SB_PLATFORM_WINDOWS)
#    if defined(SB_COMPILER_MSVC)
#        include "_impl/compiler_msvc_win.h"
#    elif defined(SB_COMPILER_CLANG)
#        include "_impl/compiler_clang_win.h"
#    else
#        error "Unsupported windows compiler"
#    endif
#elif defined(SB_PLATFORM_LINUX)
#    include "_impl/compiler_linux.h"
#else
#    error "Unsupported platform"
#endif

#define sb_unreacheable_code() sb_debug_break()
#define sb_static_unreachable_code(msg) sb_static_unreachable_code_internal(msg)

// common intrisics
#define sb_noalias sb_noalias_internal
#define sb_nobreak()
#define sb_debug_break() sb_debug_break_internal
#define sb_lzcnt(val) sb_lzcnt_internal(val)
