#include "memory_system.h"
#include <sb_core/core.h>

#pragma warning(push)
#pragma warning(disable : 4073)

#pragma init_seg(lib)

class MemoryHook
{
public:
    MemoryHook()
    {
        sb::memory_system::initialize();
    }

    ~MemoryHook()
    {
        sb::memory_system::terminate();
    }
};

static MemoryHook g_MemoryHook;

#pragma warning(pop)
