#pragma once

#if !defined(__clang__)
#   error "Unsupported compiler"
#endif

#define sb_forceinline_impl __forceinline
#define sb_noinline_impl

#define sb_restrict_impl

#define sbDebugBreakImpl() __asm__ volatile("int $0x03");

