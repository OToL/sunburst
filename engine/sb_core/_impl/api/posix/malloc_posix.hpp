#include <sb_core/memory/memory.h>
#include <sb_core/core.h>
#include <sb_core/error.h>
#include <sb_core/bit.h>
#include <sb_core/conversion.h>

namespace sb::internal {

usize const SYS_MALLOC_DEFAULT_ALIGNMENT = sizeof(void *);

struct AllocHeader
{
    usize size;
    usize offset;
};

static inline usize sizeWithPadding(usize size, Alignment align)
{
    return (align <= SYS_MALLOC_DEFAULT_ALIGNMENT) ? (size + alignUp(sizeof(AllocHeader), align))
                                                   : (size + align + sizeof(AllocHeader));
}

static inline AllocHeader * dataToHeader(void * dataPtr)
{
    return static_cast<AllocHeader *>(dataPtr) - 1;
}

void * malloc(usize size, Alignment alignment)
{
    sbAssert((0 != alignment) && sb::isPowerOf2(alignment));

    usize const total_size = sizeWithPadding(size, alignment);
    // NOLINTNEXTLINE(cppcoreguidelines-no-malloc)
    u8 * const mem_ptr = reinterpret_cast<u8 *>(::malloc(total_size));

    if (nullptr == mem_ptr)
    {
        notifyOOM(size, "malloc OOM");
    }

    void * const aligned_mem_ptr = reinterpret_cast<void *>(alignUp(usize(mem_ptr + sizeof(AllocHeader)), alignment));

    AllocHeader * const header = dataToHeader(aligned_mem_ptr);
    header->size = size;
    header->offset = sb::numericConv<u16>(uptr(header) - uptr(mem_ptr));

    return aligned_mem_ptr;
}

void * malloc(usize size)
{
    return ::sb::internal::malloc(size, ALIGNMENT_DEFAULT);
}

void free(void * mem_ptr)
{
    if (nullptr != mem_ptr)
    {
        AllocHeader * const header = dataToHeader(mem_ptr);
        void * const raw_mem_ptr = reinterpret_cast<u8 *>(header) - header->offset;

        // NOLINTNEXTLINE(cppcoreguidelines-no-malloc)
        ::free(raw_mem_ptr);
    }
}

usize mallocUsableSize(void * mem_ptr)
{
    AllocHeader * const header = dataToHeader(mem_ptr);
    return header->size;
}

} // namespace sb::internal
