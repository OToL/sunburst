#include <core/memory/allocator/incremental_allocator.h>
#include <core/math.h>
#include <core/error.h>

namespace sb {

IncrementalAllocator::IncrementalAllocator(InitParams const & init)
    : m_arena(init.m_arena)
    , m_top(static_cast<ui8 *>(init.m_arena.m_ptr))
{
}

IncrementalAllocator::IncrementalAllocator(MemoryArena const & arena)
    : m_arena(arena)
    , m_top(static_cast<ui8 *>(arena.m_ptr))
{
}

void * IncrementalAllocator::allocate(usize const size)
{
    void * mem_ptr = nullptr;

    if (nullptr != m_top)
    {
        ui8 * new_top = reinterpret_cast<ui8 *>(alignUp(reinterpret_cast<uiptr>(m_top), ALIGNMENT));

        if (m_arena.isInRange(new_top, size))
        {
            mem_ptr = new_top;
            m_top = new_top + size;
        }
    }

    return mem_ptr;
}

void * IncrementalAllocator::allocate(usize const size, usize const alignment)
{
    sbWarn(isPowerOf2(alignment));

    void * mem_ptr = nullptr;

    if (nullptr != m_top)
    {
        ui8 * new_top = reinterpret_cast<ui8 *>(alignUp(reinterpret_cast<uiptr>(m_top), alignment));

        if (m_arena.isInRange(new_top, size))
        {
            mem_ptr = new_top;
            m_top = new_top + size;
        }
    }

    return mem_ptr;
}

void IncrementalAllocator::deallocate([[maybe_unused]] void * ptr)
{
    sbWarn(m_arena.isInRange(ptr));
}

void IncrementalAllocator::deallocateAll()
{
    m_top = static_cast<ui8 *>(m_arena.m_ptr);
}

} // namespace sb
