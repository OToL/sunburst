#include <sb_core/_impl/memory/malloc_platform.h>
#include <sb_core/_impl/memory/malloc.h>
#include <sb_core/memory/memory.h>
#include <sb_core/core.h>
#include <sb_core/error/error.h>
#include <sb_core/bit.h>
#include <sb_core/conversion.h>

namespace sb::internal {

usize const SYS_MALLOC_DEFAULT_ALIGNMENT = sizeof(void *);

struct AllocHeader
{
    usize size;
    usize offset;
};

static inline usize sizeWithPadding(usize size, usize align)
{
    return (align <= SYS_MALLOC_DEFAULT_ALIGNMENT) ? (size + alignUp(sizeof(AllocHeader), align))
                                                   : (size + align + sizeof(AllocHeader));
}

static inline AllocHeader * dataToHeader(void * dataPtr)
{
    return static_cast<AllocHeader *>(dataPtr) - 1;
}

sb::MemoryArena platformMalloc(usize alignment, usize size)
{
    sbAssert((0 != alignment) && sb::isPowerOf2(alignment));

    usize const total_size = sizeWithPadding(size, alignment);
    // NOLINTNEXTLINE(cppcoreguidelines-no-malloc)
    u8 * const mem_ptr = reinterpret_cast<u8 *>(::malloc(total_size));

    if (nullptr == mem_ptr)
    {
        return {};
    }

    void * const aligned_mem_ptr = reinterpret_cast<void *>(alignUp(usize(mem_ptr + sizeof(AllocHeader)), alignment));

    AllocHeader * const header = dataToHeader(aligned_mem_ptr);
    header->size = size;
    header->offset = sb::numericConv<u16>(uptr(header) - uptr(mem_ptr));

    return {aligned_mem_ptr, size};
}

sb::MemoryArena platformMalloc(usize size)
{
    return ::sb::internal::malloc(ALIGNMENT_DEFAULT, size);
}

void platformFree(void * mem_ptr)
{
    if (nullptr != mem_ptr)
    {
        AllocHeader * const header = dataToHeader(mem_ptr);
        void * const raw_mem_ptr = reinterpret_cast<u8 *>(header) - header->offset;

        // NOLINTNEXTLINE(cppcoreguidelines-no-malloc)
        ::free(raw_mem_ptr);
    }
}

usize platformMallocUsableSize(void * mem_ptr)
{
    AllocHeader * const header = dataToHeader(mem_ptr);
    return header->size;
}

} // namespace sb::internal
