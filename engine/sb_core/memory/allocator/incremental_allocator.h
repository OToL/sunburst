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
    using MemProvider = TMemProvider;

    IncrementalAllocator() = default;

    IncrementalAllocator(MemProvider const & mem_provider, usize capacity, Alignment default_align = ALIGNMENT_DEFAULT)
        : BaseClass()
        , _mem_provider(mem_provider)
    {
        BaseClass::init(_mem_provider.allocate(capacity, default_align));
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
        _mem_provider.deallocate(_arena.m_ptr);
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
    MemProvider _mem_provider;
};

} // namespace sb
