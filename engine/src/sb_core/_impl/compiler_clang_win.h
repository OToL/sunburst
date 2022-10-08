#pragma once

#define sb_noalias_internal
#define sb_debug_break_internal __asm__ volatile("int $0x03")
#define sb_static_unreachable_code_internal(msg)                                                                               \
    []<bool flag = false>()                                                                                            \
    {                                                                                                                  \
        static_assert(flag, msg);                                                                                      \
    }                                                                                                                  \
    ()
