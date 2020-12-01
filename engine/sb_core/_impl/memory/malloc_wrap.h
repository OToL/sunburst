#pragma once

#include <sb_core/_impl/config.h>

#if SB_MALLOC_BACKEND == SB_MALLOC_RPMALLOC
#    include <sb_core/_impl/api/rpmalloc/rpmalloc.h>
#    define sbMallocWrapInitialize() rpmalloc_initialize()
#    define sbMallocWrapTerminate() rpmalloc_finalize()
#    define sbMallocWrapAlloc(size) rpmalloc(size)
#    define sbMallocWrapAlignedAlloc(alignment, size) rpaligned_alloc(alignment, size)
#    define sbMallocWrapFree(ptr) rpfree(ptr)
#    define sbMallocWrapUsableSize(ptr) rpmalloc_usable_size(ptr)
#elif SB_MALLOC_BACKEND == SB_MALLOC_PLATFORM
#    include "malloc_platform.h"
#    define sbMallocWrapInitialize()
#    define sbMallocWrapTerminate()
#    define sbMallocWrapAlloc(size) sb::internal::platformMalloc(size)
#    define sbMallocWrapAlignedAlloc(alignment, size) sb::internal::platformMalloc(alignment, size)
#    define sbMallocWrapFree(ptr) sb::internal::platformFree(ptr)
#    define sbMallocWrapUsableSize(ptr) sb::internal::platformMallocUsableSize(ptr)
#else
#    error "Unsupported malloc backend"
#endif
