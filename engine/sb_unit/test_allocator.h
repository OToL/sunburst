#include <sb_core/core.h>
#include <sb_core/container/dynamic_array.h>
#include <sb_core/memory/allocator/allocator.h>

namespace sb {

class TestAllocator final : public IAllocator
{
public:
    struct Stats
    {
        size_t m_alloc_count = 0U;
        size_t m_allocated_byte = 0U;
    };

    TestAllocator() = default;

    ~TestAllocator();

    usize getAlignment() const;

    MemoryArena allocate(usize const size) override;

    MemoryArena allocate(usize const size, Alignment alignment) override;

    void deallocate(void * ptr);

    void deallocate(MemoryArena arena);

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
    DArray<AllocDesc> m_allocs;
};

} // namespace sb
