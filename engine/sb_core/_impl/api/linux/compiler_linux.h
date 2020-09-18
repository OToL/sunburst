#pragma once

#if !defined(__clang__)
#    error "Unsupported compiler"
#endif

#define __sb_restrict_internal

namespace sb {
inline void debugBreak()
{
    __asm__ volatile("int $0x03");
}
} // namespace sb
