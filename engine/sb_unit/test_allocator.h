#include <sb_core/core.h>
#include <sb_core/container/dynamic_array.h>
#include <sb_core/memory/allocator/allocator.h>

namespace sb {

class TestAllocator final : public IAllocator
{
public:
    struct Stats
    {
        size_t _alloc_count = 0U;
        size_t _allocated_byte = 0U;
    };

    TestAllocator() = default;

    ~TestAllocator();

    usize getAlignment() const;

    MemoryArena allocate(usize const size) override;

    MemoryArena allocate(usize const size, Alignment alignment) override;

    void deallocate(void * ptr) override;

    void deallocate(MemoryArena arena) override;

    bool owns(void const * ptr) const;

    Stats getStats() const
    {
        return _stats;
    }

private:
    struct AllocDesc
    {
        AllocDesc(void * mem_ptr, size_t size)
            : _mem(mem_ptr)
            , size(size)
        {
        }

        void * _mem = nullptr;
        size_t size = 0ULL;
    };

    Stats _stats;
    DArray<AllocDesc> _allocs;
};

} // namespace sb
