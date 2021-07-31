#pragma once

#if !defined(_MSC_VER)
#    error "Unsupported compiler"
#endif

#define sb_noalias_internal __restrict
#define sb_debug_break_internal __debugbreak()

#define sb_static_unreachable_code_internal(msg) static_assert(false, msg)
