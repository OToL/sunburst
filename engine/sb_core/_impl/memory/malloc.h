#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/memory_arena.h>

namespace sb::internal {

void mallocInitialize();

void mallocTerminate();

MemoryArena malloc(usize size);

MemoryArena malloc(Alignment alignment, usize size);

usize mallocUsableSize(void * memPtr);

void free(void * memPtr);
} // namespace sb::internal
