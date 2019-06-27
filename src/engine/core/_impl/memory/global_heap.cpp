#include <core/memory/global_heap.h>

namespace sb {

void * GlobalHeap::allocate(usize const size)
{
    return m_parent.allocate(size);
}

void * GlobalHeap::allocate(usize const size, Alignment const alignment)
{
    return m_parent.allocate(size, alignment);
}

void GlobalHeap::deallocate(void * ptr)
{
    m_parent.deallocate(ptr);
}

b8 GlobalHeap::owns(void const * ptr) const
{
    return m_parent.owns(ptr);
}

usize GlobalHeap::getBlockSize(void * ptr) const
{
    return m_parent.getBlockSize(ptr);
}

void GlobalHeap::deallocateAll()
{
    m_parent.deallocateAll();
}

GlobalHeap * getGlobalHeap()
{
    static GlobalHeap s_global_heap;

    return &s_global_heap;
}

AllocatorView const & getGlobalHeapView()
{
    static AllocatorView s_global_heap_view{*getGlobalHeap()};

    return s_global_heap_view;
}

} // namespace sb
