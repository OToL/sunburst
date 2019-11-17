#pragma once

#if !defined(_MSC_VER)
#   error "Unsupported compiler"
#endif

#define _ENABLE_EXTENDED_ALIGNED_STORAGE

#define sb_forceinline_impl __forceinline
#define sb_noinline_impl

#define sb_restrict_impl __restrict

namespace sb {
    inline void debugBreak()
    {
        __debugbreak();
    }
}
