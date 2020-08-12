#pragma once

#include <sb_base/base.h>

namespace sb::detail {

struct AllocatorVTable
{
    using AllocateCB = void * (*)(void *, usize const);
    using AllocateAlignedCB = void * (*)(void *, usize const, Alignment const);
    using DeallocateCB = void (*)(void *, void *);
    using OwnsCB = b8 (*)(void *, void const *);
    using AlignmentCB = usize (*)(void *);

    AllocateCB m_allocate;
    AllocateAlignedCB m_aligned_allocate;
    DeallocateCB m_deallocate;
    OwnsCB m_owns;
    AlignmentCB m_alignment;
};

template <typename TAllocator>
AllocatorVTable const & getAllocatorVTable()
{
    static AllocatorVTable vtable = {
        [](void * alloc, usize const size) {
            return static_cast<TAllocator *>(alloc)->allocate(size);
        },
        [](void * alloc, usize const size, Alignment const alignment) {
            return static_cast<TAllocator *>(alloc)->allocate(size, alignment);
        },
        [](void * alloc, void * ptr) { static_cast<TAllocator *>(alloc)->deallocate(ptr); },
        [](void * alloc, void const * ptr) { return static_cast<TAllocator *>(alloc)->owns(ptr); },
        [](void * alloc) -> usize { return static_cast<TAllocator *>(alloc)->getAlignment(); }};

    return vtable;
}
} // namespace sb::detail
