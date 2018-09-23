#pragma once

#include <cstddef>

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

    constexpr size_t getAlignment() const
    {
        return ALIGNMENT;
    }

    void * allocate(size_t const size);

    void * allocate(size_t const size, size_t alignment);

    void deallocate(void * ptr);

    void deallocateAll();

    bool owns(void const * ptr) const;

    Stats getStats() const
    {
        return m_stats;
    }

private:
    Stats m_stats;
};

} // namespace wgtest
