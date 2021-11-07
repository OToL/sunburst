#pragma once

#if defined(SB_PLATFORM_WINDOWS)
#    include <sb_core/_impl/memory/virtual_memory_backstore_win.h>
#else
#    error "Unsupported platform"
#endif

#include "memory_arena.h"

#include <sb_core/core.h>

namespace sb {

constexpr inline usize VMEM_PAGE_SIZE = internal::VMEM_PAGE_SIZE;

MemoryArena reserveVMem(usize size, char const * name);

bool mapVMem(MemoryArena arena);
bool unmapVMem(MemoryArena arena);

// reserve + map
MemoryArena allocateVMem(usize size, char const * name);

// unmap + release
bool releaseVMem(void * base_mem);

} // namespace sb
