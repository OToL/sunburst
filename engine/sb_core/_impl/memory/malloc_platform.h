#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/memory_arena.h>

namespace sb::internal {

MemoryArena platformMalloc(usize alignment, usize size);

MemoryArena platformMalloc(usize size);

void platformFree(void * mem_ptr);

usize platformMallocUsableSize(void * mem_ptr);

} // namespace sb::internal
