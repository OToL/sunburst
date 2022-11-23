#pragma once

#define sb_noalias_internal
#define sb_debug_break_internal __debugbreak()
#define sb_lzcnt_internal(val) slw::countl_zero(val)
#define sb_static_unreachable_code_internal(msg)                                                                                                     \
    []<bool flag = false>()                                                                                                                          \
    {                                                                                                                                                \
        static_assert(flag, msg);                                                                                                                    \
    }                                                                                                                                                \
    ()
