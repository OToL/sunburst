#include "malloc.h"

#pragma warning(push)
#pragma warning(disable : 4073)

#pragma init_seg(lib)

class MemoryHook
{
public:
    MemoryHook()
    {
        sb::internal::mallocInitialize();
    }

    MemoryHook(MemoryHook const &) = default;
    MemoryHook(MemoryHook &&) = default;
    MemoryHook & operator=(MemoryHook const &) = default;
    MemoryHook & operator=(MemoryHook &&) = default;

    ~MemoryHook()
    {
        sb::internal::mallocTerminate();
    }
};

static MemoryHook g_MemoryHook;

#pragma warning(pop)
