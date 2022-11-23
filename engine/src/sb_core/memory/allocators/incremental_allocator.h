#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include <sb_core/_impl/memory/allocators/incremental_allocator_base.h>

namespace sb {

template <typename TMemProvider>
class IncrementalAllocator final : public IncrementalAllocatorBase
{
    sb_base(IncrementalAllocatorBase);

public:
    IncrementalAllocator() = default;

    IncrementalAllocator(TMemProvider const & mem_provider, usize capacity, Alignment default_align = DEFAULT_MEMORY_ALIGNMENT)
        : Base()
        , _mem_provider(mem_provider)
    {
        Base::init(_mem_provider.allocate(default_align, capacity), default_align);
    }

    IncrementalAllocator(usize capacity, Alignment default_align = DEFAULT_MEMORY_ALIGNMENT)
        : Base()
        , _mem_provider()
    {
        Base::init(_mem_provider.allocate(capacity, default_align));
    }

    IncrementalAllocator & operator=(IncrementalAllocator const &) = delete;
    IncrementalAllocator & operator=(IncrementalAllocator const &&) = delete;
    IncrementalAllocator(IncrementalAllocator const &) = delete;
    IncrementalAllocator(IncrementalAllocator &&) = delete;

    ~IncrementalAllocator() override
    {
        MemoryArena mem_arena = Base::getArena();
        if (!mem_arena.isEmpty())
        {
            _mem_provider.deallocate(mem_arena.data);
        }
    }

    using Base::allocate;
    using Base::deallocate;
    using Base::owns;

    using Base::deallocateAll;
    using Base::getAlignment;

    TMemProvider const & getProvider() const
    {
        return _mem_provider;
    }

private:
    TMemProvider _mem_provider;
};

} // namespace sb
