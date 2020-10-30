#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include "pool_allocator.h"

namespace sb {

// todo: move to a separate file
// todo: remove inheritance and move to member
template <typename TObject, typename TMemProvider>
class ObjectPoolAllocator : public PoolAllocator<TMemProvider>
{
    sbBaseClass(PoolAllocator<TMemProvider>);

public:
    ObjectPoolAllocator()
        : BaseClass()
    {
    }

    ObjectPoolAllocator(usize obj_count)
        : BaseClass(sizeof(TObject), obj_count, alignOf<TObject>())
    {
    }

    ObjectPoolAllocator(TMemProvider const & mem_provider, usize obj_count)
        : BaseClass(mem_provider, sizeof(TObject), obj_count, alignOf<TObject>())

    {
    }

    ObjectPoolAllocator & operator=(ObjectPoolAllocator const &) = delete;
    ObjectPoolAllocator & operator=(ObjectPoolAllocator &&) = delete;
    ObjectPoolAllocator(ObjectPoolAllocator const &) = delete;
    ObjectPoolAllocator(ObjectPoolAllocator &&) = delete;

    using BaseClass::deallocate;

    using BaseClass::owns;

    using BaseClass::deallocateAll;
    using BaseClass::allocate;

    using BaseClass::getAlignment;
    using BaseClass::getArena;
};

} // namespace sb
