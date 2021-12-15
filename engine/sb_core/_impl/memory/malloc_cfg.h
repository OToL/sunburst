#pragma once

#include <sb_core/_impl/config.h>

#if sb_ctf_equal(MALLOC_BACKEND, RPMALLOC)
#    include <sb_core/_impl/extern/rpmalloc/rpmalloc.h>
#    include "malloc_rpmalloc.h"
#    define sb_initialize_malloc_backend() initializeRPmalloc()
#    define sb_terminate_malloc_backend() finalizeRPmalloc()
#    define sb_malloc_backend_alloc(size) rpmalloc(size)
#    define sb_malloc_backend_alloca(alignment, size) rpaligned_alloc(alignment, size)
#    define sb_malloc_backend_free(ptr) rpfree(ptr)
#    define sb_malloc_backend_usable_size(ptr) rpmalloc_usable_size(ptr)
#elif sb_ctf_equal(MALLOC_BACKEND, PLATFORM)
#    include "malloc_platform.h"
#    define sb_initialize_malloc_backend()
#    define sb_terminate_malloc_backend()
#    define sb_malloc_backend_alloc(size) sb::platformMalloc(size)
#    define sb_malloc_backend_alloca(alignment, size) sb::platformMalloc(alignment, size)
#    define sb_malloc_backend_free(ptr) sb:platformFree(ptr)
#    define sb_malloc_backend_usable_size(ptr) sb::platformMallocUsableSize(ptr)
#else
#    error "Unsupported malloc backend"
#endif
