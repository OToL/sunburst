#pragma once

#include <sb_core/memory/memory_arena.h>
#include <sb_core/core.h>

namespace sb::memory_system {

void initialize();
b8 isInitialized();
void terminate();

MemoryArena malloc(usize size);
MemoryArena malloc(Alignment alignment, usize size);
usize getMallocBlockSize(void * memPtr);
void free(void * memPtr);

MemoryArena reserveVMem(usize size, char const * name);
bool releaseVMem(void * base_mem);
bool mapVMem(MemoryArena arena);
bool unmapVMem(MemoryArena arena);
MemoryArena reserveAndMapVMem(usize size, char const * name);

} // namespace sb::memory_system
