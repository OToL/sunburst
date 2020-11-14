#pragma once

#include <sb_core/core.h>
#include <sb_core/memory/memory.h>
#include <sb_core/_impl/memory/allocator/incremental_allocator_base.h>

namespace sb {

template <typename TMemProvider>
class IncrementalAllocator final : public IncrementalAllocatorBase
{
    sbBaseClass(IncrementalAllocatorBase);

public:
    IncrementalAllocator() = default;

    IncrementalAllocator(TMemProvider const & mem_provider, usize capacity, Alignment default_align = ALIGNMENT_DEFAULT)
        : BaseClass()
        , _mem_provider(mem_provider)
    {
        BaseClass::init(_mem_provider.allocate(capacity, default_align), default_align);
    }

    IncrementalAllocator(usize capacity, Alignment default_align = ALIGNMENT_DEFAULT)
        : BaseClass()
        , _mem_provider()
    {
        BaseClass::init(_mem_provider.allocate(capacity, default_align));
    }

    IncrementalAllocator & operator=(IncrementalAllocator const &) = delete;
    IncrementalAllocator & operator=(IncrementalAllocator const &&) = delete;
    IncrementalAllocator(IncrementalAllocator const &) = delete;
    IncrementalAllocator(IncrementalAllocator &&) = delete;

    ~IncrementalAllocator() override
    {
        MemoryArena mem_arena = BaseClass::getArena();
        if (!isEmpty(mem_arena))
        {
            _mem_provider.deallocate(mem_arena.data);
        }
    }

    using BaseClass::allocate;
    using BaseClass::deallocate;
    using BaseClass::owns;

    using BaseClass::deallocateAll;
    using BaseClass::getAlignment;

    TMemProvider const & getProvider() const
    {
        return _mem_provider;
    }

private:
    TMemProvider _mem_provider;
};

} // namespace sb
