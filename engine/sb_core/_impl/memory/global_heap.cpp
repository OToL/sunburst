#include <sb_core/memory/global_heap.h>

sb::MemoryArena sb::GlobalHeap::allocate(usize const size)
{
    return m_parent.allocate(size);
}

sb::MemoryArena sb::GlobalHeap::allocate(usize const size, Alignment const alignment)
{
    return m_parent.allocate(size, alignment);
}

void sb::GlobalHeap::deallocate(void * ptr)
{
    m_parent.deallocate(ptr);
}

sb::b8 sb::GlobalHeap::owns(void const * ptr) const
{
    return m_parent.owns(ptr);
}

sb::usize sb::GlobalHeap::getBlockSize(void * ptr) const
{
    return m_parent.getBlockSize(ptr);
}

sb::GlobalHeap & sb::getGlobalHeap()
{
    static GlobalHeap _global_heap;

    return _global_heap;
}

sb::AllocatorView const & sb::getGlobalHeapView()
{
    static AllocatorView _global_heap_view{getGlobalHeap()};

    return _global_heap_view;
}
