#pragma once

#include <sb_slw/bit>

#define sb_noalias_internal
#define sb_debug_break_internal __asm__ volatile("int $0x03")
#define sb_lzcnt_internal(val) slw::countl_zero(val)
#define sb_static_unreachable_code_internal(msg)                                                                                                     \
    []<bool flag = false>()                                                                                                                          \
    {                                                                                                                                                \
        static_assert(flag, msg);                                                                                                                    \
    }                                                                                                                                                \
    ()
