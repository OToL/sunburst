#pragma once

#include <core/memory/memory.h>

#include <libc++/cstddef>
#include <libc++/vector>

namespace gtest::ext {

// TODO: Make it a proper composer or even part of core
class AllocatorStats
{
public:
    struct InitParams
    {
    };

    struct Stats
    {
        size_t m_alloc_count = 0;
        size_t m_allocated_byte = 0;
    };

    static constexpr size_t ALIGNMENT = sizeof(void *);

    AllocatorStats() = default;

    AllocatorStats(InitParams const &) {}

    ~AllocatorStats();

    constexpr size_t getAlignment() const
    {
        return ALIGNMENT;
    }

    void * allocate(size_t const size);

    void * allocate(size_t const size, sb::Alignment alignment);

    void deallocate(void * ptr);

    bool owns(void const * ptr) const;

    Stats getStats() const
    {
        return m_stats;
    }

private:

    struct AllocDesc
    {
        AllocDesc(void * mem_ptr, size_t size)
            : m_mem(mem_ptr)
            , m_size(size)
        {            
        }

        void * m_mem = nullptr; 
        size_t m_size = 0ULL;
    };

    Stats m_stats;
    wstd::detail::vector<AllocDesc> m_allocs;
};

} // namespace wgtest
