#pragma once

#if !defined(_MSC_VER)
#    error "Unsupported compiler"
#endif

#define _ENABLE_EXTENDED_ALIGNED_STORAGE

#define sb_restrict_detail __restrict

namespace sb {
__forceinline void debugBreak()
{
    __debugbreak();
}
} // namespace sb
