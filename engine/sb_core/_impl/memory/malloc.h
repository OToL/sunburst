#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>

namespace sb::internal {
// @todo: return arena
void * malloc(usize size);

// @todo: return arena
void * malloc(usize size, Alignment alignment);

usize mallocUsableSize(void * memPtr);

// @todo: add free with arena
void free(void * memPtr);
} // namespace sb::internal
