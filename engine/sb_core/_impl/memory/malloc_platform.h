#include <sb_core/core.h>
#include <sb_core/memory/memory.h>

namespace sb::internal {

void * platformMalloc(usize alignment, usize size);

void * platformMalloc(usize size);

void platformFree(void * mem_ptr);

usize platformMallocUsableSize(void * mem_ptr);

} // namespace sb::internal
