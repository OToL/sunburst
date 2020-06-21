#pragma once

#if !defined(__clang__)
#    error "Unsupported compiler"
#endif

namespace sb {
inline void debugBreak()
{
    __asm__ volatile("int $0x03");
}
} // namespace sb
