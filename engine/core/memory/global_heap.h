#pragma once

#include <core/memory/allocator/allocator_view.h>
#include <core/memory/allocator/global_heap_allocator.h>
#include <core/memory/memory.h>

namespace sb {

class GlobalHeap
{
    GlobalHeap() { }
    ~GlobalHeap() { }

    friend GlobalHeap * getGlobalHeap();

public:
    void * allocate(usize const size);

    void * allocate(usize const size, Alignment const alignment);

    void deallocate(void * ptr);

    b8 owns(void const * ptr) const;

    constexpr usize getAlignment() const
    {
        return ALIGN_DEFAULT;
    }

    usize getBlockSize(void * ptr) const;

private:
    GlobalHeapAllocator m_parent;
};

AllocatorView const & getGlobalHeapView();

GlobalHeap * getGlobalHeap();

} // namespace sb
