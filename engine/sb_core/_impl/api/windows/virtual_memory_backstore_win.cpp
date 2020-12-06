#include <sb_core/memory/virtual_memory_backstore.h>
#include <sb_core/string/string_format.h>
#include <sb_core/conversion.h>
#include <sb_core/error/error.h>
#include <sb_core/bit.h>

#include <sb_core/api/windows/windows.h>

sb::MemoryArena sb::reserveVirtualMemory(usize size, [[maybe_unused]] char const * name)
{
    sbAssert(isAlignedTo(size, MEMORY_PAGE_SIZE));
    sbAssert((nullptr != name) && (0 != name[0]));

    void * const mem_ptr = VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_READWRITE);

    if (sbExpect(nullptr != mem_ptr, "Failed to reserve {} bytes for client '{}' with error '{}'", size, name,
                 GetLastError()))
    {
        return {mem_ptr, size};
    }

    return {};
}

bool sb::mapVirtualMemory(MemoryArena arena)
{
    sbAssert(memarena_isValid(arena));
    sbAssert(isAlignedTo(arena.data, MEMORY_PAGE_SIZE));

    if (sbExpect(nullptr != VirtualAlloc(arena.data, arena.size, MEM_COMMIT, PAGE_READWRITE),
                 "Failed to reserve {} bytes with error '{}'", arena.size, GetLastError()))
    {
        return true;
    }

    return false;
}

bool sb::unmapVirtualMemory(MemoryArena arena)
{
    sbAssert(memarena_isValid(arena));

    if (sbExpect(VirtualFree(arena.data, arena.size, MEM_DECOMMIT), "Failed to unmap {} bytes with error '{}'",
                 arena.size, GetLastError()))
    {
        return true;
    }

    return false;
}

bool sb::releaseVirtualMemory(void * base_mem)
{
    sbAssert(nullptr != base_mem);

    if (sbExpect(VirtualFree(base_mem, 0, MEM_RELEASE), "Failed to release memory with error '{}'", GetLastError()))
    {
        return true;
    }

    return false;
}

sb::MemoryArena sb::allocateVirtualMemory(usize size, char const * name)
{
    sbAssert(isAlignedTo(size, MEMORY_PAGE_SIZE));
    sbAssert((nullptr != name) && (0 != name[0]));

    void * const mem_ptr = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (sbExpect(nullptr != mem_ptr, "Failed to reserve {} bytes for client '{}' with error '{}'", size, name,
                 GetLastError()))
    {
        return {mem_ptr, size};
    }

    return {};
}
