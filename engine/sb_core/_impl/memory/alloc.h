#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>

namespace sb::internal {

void * malloc(usize size);

void * malloc(usize size, Alignment alignment);

usize mallocUsableSize(void * memPtr);

void free(void * memPtr);

} // namespace sb::internal
