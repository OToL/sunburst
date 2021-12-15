#include "malloc_rpmalloc.h"
#include <sb_core/_impl/extern/rpmalloc/rpmalloc.h>
#include <sb_core/memory/virtual_memory_backstore.h>
#include <sb_core/error.h>

namespace {

void * rpmallocMap(size_t size, size_t *)
{
    return sb::allocateVMem(size, "rpmalloc").data;
}

void rpmallocUnmmap(void * address, size_t size, size_t, size_t release)
{
    if (release)
    {
        // on most platforms we cannot relase memory in the middle of a previously reserved/mapped area
        sb::releaseVMem(address);
    }
    else
    {
        sb::unmapVMem({address, size});
    }
}

} // namespace

void sb::initializeRPmalloc()
{
    rpmalloc_config_t cfg = {.memory_map = &rpmallocMap,
                             .memory_unmap = &rpmallocUnmmap,
                             .page_size = VMEM_PAGE_SIZE,
                             .span_size = VMEM_PAGE_SIZE,
                             .span_map_count = 64,
                             .enable_huge_pages = 0};

    [[maybe_unused]] int const res = rpmalloc_initialize_config(&cfg);
    sb_assert(0 == res);
}

void sb::finalizeRPmalloc()
{
    rpmalloc_finalize();
}
