#pragma once

#if !defined(__clang__)
#    error "Unsupported compiler"
#endif

#define sb_noalias_internal

namespace sb {
inline void debugBreak()
{
    __asm__ volatile("int $0x03");
}
} // namespace sb

#define sb_static_unreachable_codeInternal(msg)                                                                               \
    []<bool flag = false>()                                                                                            \
    {                                                                                                                  \
        static_assert(flag, msg);                                                                                      \
    }                                                                                                                  \
    ()
