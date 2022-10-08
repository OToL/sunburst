#include "malloc_cfg.h"
#include "memory_system.h"
#include <sb_core/memory/utility.h>

void sb::memory_system::initialize() 
{
    sb_initialize_malloc_backend();
}

void sb::memory_system::terminate() 
{
    sb_terminate_malloc_backend();
}

sb::MemoryArena sb::memory_system::malloc(usize size) 
{ 
    void * const mem_ptr = sb_malloc_backend_alloc(size);

    if (nullptr != mem_ptr)
    {
        return {mem_ptr, size};
    }

    notifyOOM(size, "aligned malloc OOM");
}

sb::MemoryArena sb::memory_system::malloc(Alignment alignment, usize size) 
{
    void * const mem_ptr = sb_malloc_backend_alloca(alignment, size);

    if (nullptr != mem_ptr)
    {
        return {mem_ptr, size};
    }

    notifyOOM(size, "aligned malloc OOM");
}

sb::usize sb::memory_system::getBlockSize(void * memPtr)
{
    return sb_malloc_backend_usable_size(memPtr);
}

void sb::memory_system::free(void * memPtr)
{
    sb_malloc_backend_free(memPtr);
}
