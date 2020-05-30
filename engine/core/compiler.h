#pragma once

#if defined(SB_PLATFORM_MACOS)
#    include "_impl/platform/compiler_macos.h"
#elif defined(SB_PLATFORM_WINDOWS)
#    include "_impl/platform/compiler_win.h"
#elif defined(SB_PLATFORM_LINUX)
#    include "_impl/platform/compiler_linux.h"
#else
#    error "Unsupported compiler"
#endif

#define sb_forceinline sb_forceinline_impl
#define sb_noinline sb_noinline_impl

#define sb_restrict sb_restrict_impl

#define sbBaseClass(...) using BaseClass = __VA_ARGS__

#define sbCopyProtect(classname)                                                                   \
    classname & operator=(classname const &) = delete;                                             \
    classname(classname const &) = delete

#define SB_CTF_DISABLED 1
#define SB_CTF_ENABLED 2

#define sbCTFIsEnabled(feature) (1 == 2 / SB_CTF_##feature)
#define sbCTVIsEq(feature, value) ((1 / SB_CTV_##feature + 1) && (SB_CTV_##feature == value))

namespace sb {

void debugBreak();

}
