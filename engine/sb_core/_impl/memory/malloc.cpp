#include "malloc_wrap.h"
#include "malloc.h"
#include <sb_core/enum.h>

void sb::internal::mallocInitialize()
{
    sbMallocWrapInitialize();
}

void sb::internal::mallocTerminate()
{
    sbMallocWrapTerminate();
}

void * sb::internal::malloc(usize size)
{
    return sbMallocWrapAlloc(size);
}

void * sb::internal::malloc(Alignment alignment, usize size)
{
    return sbMallocWrapAlignedAlloc(getEnumValue(alignment), size);
}

sb::usize sb::internal::mallocUsableSize(void * memPtr)
{
    return sbMallocWrapUsableSize(memPtr);
}

void sb::internal::free(void * memPtr)
{
    sbMallocWrapFree(memPtr);
}
