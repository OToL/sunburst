#pragma once

#if !defined(_MSC_VER)
#    error "Unsupported compiler"
#endif

#include <sb_slw/bit>

#define sb_noalias_internal __restrict
#define sb_debug_break_internal __debugbreak()
#define sb_lzcnt_internal(val) slw::countl_zero(val)

#define sb_static_unreachable_code_internal(msg) static_assert(false, msg)
