#include <sb_core/memory/virtual_memory_backstore.h>
#include <sb_core/error.h>
#include <sb_core/bit.h>

#include <sb_core/api/windows/windows.h>
#include <memoryapi.h>

sb::MemoryArena sb::reserveVirtualMemory(usize, char const *)
{
    // sbAssert(isAlignedTo(size, MEMORY_PAGE_SIZE));
    // sbAssert((nullptr != client_name) && (0 != client_name[0]));

    // void * const mem_ptr = VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);

    // if (sbExpect(nullptr != mem_ptr, "Failed to reserve {} bytes for client '{}' with error '{}'", size, client_name,
    // GetLastError()))
    // {
    //     return {mem_ptr, size};
    // }

    return {};
}

sb::MemoryArena sb::mapVirtualMemory(void *, usize)
{
    // sbAssert(nullptr != base_mem);

    // return {VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE), size};
    return {};
}

void sb::unmapVirtualMemory([[maybe_unused]] void * base_mem, [[maybe_unused]] usize size) { }

void sb::releaseVirtualMemory([[maybe_unused]] void * base_mem, [[maybe_unused]] usize size) { }
