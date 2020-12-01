#pragma once

#if defined(SB_PLATFORM_WINDOWS)
#    include <sb_core/_impl/api/windows/virtual_memory_backstore_win.h>
#else
#    error "Unsupported platform"
#endif

#include "memory_arena.h"

#include <sb_core/core.h>

namespace sb {

constexpr inline usize MEMORY_PAGE_SIZE = internal::MEMORY_PAGE_SIZE;

MemoryArena allocateVirtualMemory(usize size, char const * client_name);
MemoryArena reserveVirtualMemory(usize size, char const * client_name);
MemoryArena mapVirtualMemory(void * base_mem, usize size);
void unmapVirtualMemory(void * base_mem, usize size);
void releaseVirtualMemory(void * base_mem, usize size);

} // namespace sb
