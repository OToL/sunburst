#include <sb_core/memory/memory_system.h>
#include <sb_core/core.h>
#include <sb_core/compiler.h>
#include <cstdio>

#if defined(SB_COMPILER_CLANG)

struct MemorySystemInit
{
    MemorySystemInit()
    {
        sb::memory_system::initialize();
    }
};

#pragma init_seg(lib)
static MemorySystemInit g_memory_system_init __attribute__((init_priority(101))) {};

#else
#    error "Unhandled compiler"
#endif
