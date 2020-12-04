#pragma once

#if !defined(_MSC_VER)
#    error "Unsupported compiler"
#endif

#define sb_noalias_internal __restrict

namespace sb {  
__forceinline void debugBreak()
{
    __debugbreak();
}
} // namespace sb

#define sbStaticUnreachableInternal(msg) static_assert(false, msg)
