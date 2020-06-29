#include <core/memory/memory.h>
#include <core/types.h>
#include <core/error.h>
#include <core/bit.h>
#include <core/string/utility.h>
#include <core/conversion.h>

namespace sb::detail {

usize const SYS_MALLOC_DEFAULT_ALIGNMENT = sizeof(void *);

struct AllocHeader
{
    usize m_size;
    usize m_offset;
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
    ui8 * const mem_ptr = reinterpret_cast<ui8 *>(::malloc(total_size));

    if (nullptr == mem_ptr)
    {
        notifyOOM(size, "malloc OOM");
    }

    void * const aligned_mem_ptr =
        reinterpret_cast<void *>(alignUp(usize(mem_ptr + sizeof(AllocHeader)), alignment));

    AllocHeader * const header = dataToHeader(aligned_mem_ptr);
    header->m_size = size;
    header->m_offset = sb::numericCast<ui16>(uiptr(header) - uiptr(mem_ptr));

    return aligned_mem_ptr;
}

void * malloc(usize size)
{
    return ::sb::detail::malloc(size, ALIGN_DEFAULT);
}

void free(void * mem_ptr)
{
    if (nullptr != mem_ptr)
    {
        AllocHeader * const header = dataToHeader(mem_ptr);
        void * const raw_mem_ptr = reinterpret_cast<ui8 *>(header) - header->m_offset;

        ::free(raw_mem_ptr);
    }
}

usize mallocUsableSize(void * mem_ptr)
{
    AllocHeader * const header = dataToHeader(mem_ptr);
    return header->m_size;
}

} // namespace sb::detail
