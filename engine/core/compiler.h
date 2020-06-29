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

#define sb_restrict sb_restrict_internal

#define sbBaseClass(...) using BaseClass = __VA_ARGS__

#define SB_CTF_DISABLED 1
#define SB_CTF_ENABLED 2

#define sbCTFIsEnabled(feature) (1 == 2 / SB_CTF_##feature)
#define sbCTVIsEq(feature, value) ((1 / SB_CTV_##feature + 1) && (SB_CTV_##feature == value))

namespace sb {

void debugBreak();

}
