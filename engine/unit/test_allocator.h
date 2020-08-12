#include <base/base.h>
#include <core/container/vector.h>

namespace sb {

class TestAllocator
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
    Vector<AllocDesc> m_allocs;
};

} // namespace sb
