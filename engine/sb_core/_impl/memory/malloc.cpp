#include "malloc_cfg.h"
#include "malloc.h"
#include <sb_core/cast.h>
#include <sb_core/memory/utility.h>

void sb::internal::mallocInitialize()
{
    sbMallocWrapInitialize();
}

void sb::internal::mallocTerminate()
{
    sbMallocWrapTerminate();
}

sb::MemoryArena sb::internal::malloc(usize size)
{
    void * const mem_ptr = sbMallocWrapAlloc(size);

    if (nullptr != mem_ptr)
    {
        return {mem_ptr, size};
    }

    notifyOOM(size, "aligned malloc OOM");
}

sb::MemoryArena sb::internal::malloc(Alignment alignment, usize size)
{
    void * const mem_ptr = sbMallocWrapAlignedAlloc(alignment, size);

    if (nullptr != mem_ptr)
    {
        return {mem_ptr, size};
    }

    notifyOOM(size, "aligned malloc OOM");
}

sb::usize sb::internal::mallocUsableSize(void * memPtr)
{
    return sbMallocWrapUsableSize(memPtr);
}

void sb::internal::free(void * memPtr)
{
    sbMallocWrapFree(memPtr);
}
