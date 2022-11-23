#include <sb_core/memory/memory_system.h>
#include <sb_core/string/format.h>
#include <sb_core/cast.h>
#include <sb_core/error.h>
#include <sb_core/memory/memory.h>
#include <sb_core/extern/windows/windows.h>

sb::MemoryArena sb::memory_system::reserveVMem(usize size, [[maybe_unused]] char const * name)
{
    sb_assert(isAlignedTo(size, MEMORY_PAGE_SIZE));
    sb_assert((nullptr != name) && (0 != name[0]));

    void * const mem_ptr = VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_READWRITE);

    if (sb_expect(nullptr != mem_ptr, "Failed to reserve {} bytes for client '{}' with error '{}'", size, name,
                 GetLastError()))
    {
        return {mem_ptr, size};
    }

    return {};
}

bool sb::memory_system::mapVMem(MemoryArena arena)
{
    sb_assert(arena.isValid());
    sb_assert(isAlignedTo(arena.data, MEMORY_PAGE_SIZE));

    if (sb_expect(nullptr != VirtualAlloc(arena.data, arena.size, MEM_COMMIT, PAGE_READWRITE),
                 "Failed to reserve {} bytes with error '{}'", arena.size, GetLastError()))
    {
        return true;
    }

    return false;
}

bool sb::memory_system::unmapVMem(MemoryArena arena)
{
    sb_assert(arena.isValid());

    if (sb_expect(VirtualFree(arena.data, arena.size, MEM_DECOMMIT), "Failed to unmap {} bytes with error '{}'",
                 arena.size, GetLastError()))
    {
        return true;
    }

    return false;
}

bool sb::memory_system::releaseVMem(void * base_mem)
{
    sb_assert(nullptr != base_mem);

    if (sb_expect(VirtualFree(base_mem, 0, MEM_RELEASE), "Failed to release memory with error '{}'", GetLastError()))
    {
        return true;
    }

    return false;
}

sb::MemoryArena sb::memory_system::reserveAndMapVMem(usize size, char const * name)
{
    sb_assert(isAlignedTo(size, MEMORY_PAGE_SIZE));
    sb_assert((nullptr != name) && (0 != name[0]));

    void * const mem_ptr = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (sb_expect(nullptr != mem_ptr, "Failed to reserve {} bytes for client '{}' with error '{}'", size, name,
                 GetLastError()))
    {
        return {mem_ptr, size};
    }

    return {};
}
