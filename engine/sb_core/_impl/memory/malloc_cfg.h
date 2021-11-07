#pragma once

#include <sb_core/_impl/config.h>

#if sb_ctf_equal(MALLOC_BACKEND, RPMALLOC)
#    include <sb_core/_impl/extern/rpmalloc/rpmalloc.h>
#    include <sb_core/_impl/extern/rpmalloc/rpmalloc_wrap.h>
#    define sbMallocWrapInitialize() initializeRPmalloc()
#    define sbMallocWrapTerminate() finalizeRPmalloc()
#    define sbMallocWrapAlloc(size) rpmalloc(size)
#    define sbMallocWrapAlignedAlloc(alignment, size) rpaligned_alloc(alignment, size)
#    define sbMallocWrapFree(ptr) rpfree(ptr)
#    define sbMallocWrapUsableSize(ptr) rpmalloc_usable_size(ptr)
#elif sb_ctf_equal(MALLOC_BACKEND, PLATFORM)
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
