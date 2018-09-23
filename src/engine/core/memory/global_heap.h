#pragma once

#include "allocator/allocator_view.h"
#include "allocator/global_heap_allocator.h"

namespace sb {

class GlobalHeap
{
    GlobalHeap() {}
    ~GlobalHeap() {}

    friend GlobalHeap * getGlobalHeap();

public:
    static constexpr usize ALIGNMENT = GlobalHeapAllocator::ALIGNMENT;

    void * allocate(usize const size);

    void * allocate(usize const size, usize const alignment);

    void deallocate(void * ptr);

    void deallocateAll();

    constexpr usize getAlignment() const
    {
        return ALIGNMENT;
    }

    b8 owns(void const * ptr) const;

    usize getBlockSize(void * ptr) const;

private:
    GlobalHeapAllocator m_parent;
};

AllocatorView & getGlobalHeapView();

GlobalHeap * getGlobalHeap();

} // namespace sb
