#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include "pool_allocator.h"

namespace sb {

template <typename TObject, typename TMemProvider>
class ObjectPoolAllocator final : public IAllocator
{
    sb_base(PoolAllocator<TMemProvider>);

public:
    ObjectPoolAllocator() = default;

    ObjectPoolAllocator(usize obj_count)
        : _allocator(sizeof(TObject), obj_count, alignof(TObject))
    {
    }

    ObjectPoolAllocator(TMemProvider const & mem_provider, usize obj_count)
        : _allocator(mem_provider, sizeof(TObject), obj_count, alignof(TObject))

    {
    }

    ~ObjectPoolAllocator() override = default;

    ObjectPoolAllocator & operator=(ObjectPoolAllocator const &) = delete;
    ObjectPoolAllocator & operator=(ObjectPoolAllocator &&) = delete;
    ObjectPoolAllocator(ObjectPoolAllocator const &) = delete;
    ObjectPoolAllocator(ObjectPoolAllocator &&) = delete;

    MemoryArena allocate(usize const size) override
    {
        return _allocator.allocate(size);
    }

    MemoryArena allocate(Alignment const alignment, usize const size) override
    {
        return _allocator.allocate(alignment, size);
    }

    void deallocate(void * ptr) override
    {
        _allocator.deallocate(ptr);
    }

    void deallocate(MemoryArena arena) override
    {
        _allocator.deallocate(arena);
    }

    b8 owns(void const * ptr) const override
    {
        return _allocator.owns(ptr);
    }

    void deallocateAll()
    {
        _allocator.deallocateAll();
    }

    MemoryArena allocate()
    {
        return _allocator.allocate();
    }

    Alignment getAlignment() const
    {
        return _allocator.getAlignment();
    }

    MemoryArena getArena() const
    {
        return _allocator.getArena();
    }

private:
    PoolAllocator<TMemProvider> _allocator;
};

} // namespace sb
