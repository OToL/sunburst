#pragma once

#if defined(SB_PLATFORM_WINDOWS)
#    include <intrin.h>
#    include <sb_slw/bit>
#else
#    error "unsupported platform"
#endif
