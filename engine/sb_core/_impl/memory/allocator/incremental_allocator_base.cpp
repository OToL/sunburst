#include "incremental_allocator_base.h"
#include <sb_core/error.h>
#include <sb_core/bit.h>

void sb::IncrementalAllocatorBase::init(MemoryArena arena, Alignment default_align)
{
    _arena = arena;
    _top = reinterpret_cast<u8 *>(_arena.m_ptr);
    _default_alignment = default_align;
}

sb::IncrementalAllocatorBase::IncrementalAllocatorBase()
    : _arena()
    , _top(nullptr)
    , _default_alignment(ALIGNMENT_DEFAULT)
{
}

sb::MemoryArena sb::IncrementalAllocatorBase::allocate(usize const size)
{
    if (_arena.isEmpty() || (size == 0U))
    {
        return {};
    }

    u8 * new_top = reinterpret_cast<u8 *>(alignUp(reinterpret_cast<uptr>(_top), _default_alignment));

    if (_arena.isInRange(new_top, size))
    {
        void * mem_ptr = new_top;
        _top = new_top + size;

        return {mem_ptr, size};
    }

    return {};
}

sb::MemoryArena sb::IncrementalAllocatorBase::allocate(usize const size, Alignment const alignment)
{
    if (_arena.isEmpty() || (size == 0U))
    {
        return {};
    }

    u8 * new_top = reinterpret_cast<u8 *>(alignUp(reinterpret_cast<uptr>(_top), alignment));

    if (_arena.isInRange(new_top, size))
    {
        void * mem_ptr = new_top;
        _top = new_top + size;

        return {mem_ptr, size};
    }

    return {};
}

void sb::IncrementalAllocatorBase::deallocate(void * ptr)
{
    sbWarn(_arena.isInRange(ptr));
}

sb::b8 sb::IncrementalAllocatorBase::owns(void const * ptr) const
{
    return _arena.isInRange(ptr);
}

void sb::IncrementalAllocatorBase::deallocateAll()
{
    _top = static_cast<u8 *>(_arena.m_ptr);
}
