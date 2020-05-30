#pragma once

#if !defined(__clang__)
#    error "Unsupported compiler"
#endif

#define sb_forceinline_impl __attribute__((always_inline))
#define sb_noinline_impl

#define sb_restrict_impl __restrict

namespace sb {
inline void debugBreak()
{
    __asm__ volatile("int $0x03");
}
} // namespace sb
