#include <sb_core/core.h>
#include <sb_core/container/dynamic_array.h>
#include <sb_core/memory/allocator/allocator.h>

namespace sb {

class TestAllocator final : public IAllocator
{
public:
    struct Stats
    {
        size_t alloc_count = 0U;
        size_t allocated_byte = 0U;
    };

    TestAllocator() = default;
    ~TestAllocator() override;

    TestAllocator(TestAllocator const &) = delete;
    TestAllocator(TestAllocator &&) = delete;
    TestAllocator & operator=(TestAllocator const &) = delete;
    TestAllocator & operator=(TestAllocator &&) = delete;

    usize getAlignment() const;

    MemoryArena allocate(usize const size) override;

    MemoryArena allocate(usize const size, Alignment alignment) override;

    void deallocate(void * ptr) override;

    void deallocate(MemoryArena arena) override;

    bool owns(void const * ptr) const override;

    Stats getStats() const
    {
        return _stats;
    }

private:
    struct AllocDesc
    {
        AllocDesc(void * mem_ptr, size_t data_size)
            : mem(mem_ptr)
            , size(data_size)
        {
        }

        void * mem = nullptr;
        size_t size = 0ULL;
    };

    Stats _stats;
    DArray<AllocDesc> _allocs;
};

} // namespace sb
