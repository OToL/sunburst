#pragma once

#if !defined(__clang__)
#    error "Unsupported compiler"
#endif

#define _sb_noalias_internal

namespace sb {
inline void debugBreak()
{
    __asm__ volatile("int $0x03");
}
} // namespace sb

#define sbStaticUnreachableInternal(msg)                                                                               \
    []<bool flag = false>()                                                                                            \
    {                                                                                                                  \
        static_assert(flag, msg);                                                                                      \
    }                                                                                                                  \
    ()