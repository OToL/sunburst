#pragma once

#include <sb_core/core.h>
#include <sb_core/bit.h>
#include <sb_core/memory/allocator/allocator_view.h>

#include <sb_std/type_traits>
#include <sb_std/utility>
#include <sb_std/new>

namespace sb {

AllocatorView const & getGlobalHeapView();

}

namespace sb::internal {

void * malloc(usize size);

void * malloc(usize size, Alignment alignment);

usize mallocUsableSize(void * memPtr);

void free(void * memPtr);

template <typename TType>
struct MemoryOperatorHelper
{
    struct ArrayInfo
    {
        usize m_base_offset;
        usize m_obj_count;
    };

    MemoryOperatorHelper()
        : m_allocator(getGlobalHeapView())
    {
    }

    MemoryOperatorHelper(AllocatorView allocator)
        : m_allocator(allocator)
    {
    }

    template <typename... TArgs>
    TType * operator()(TArgs... args)
    {
        void * const mem_ptr = m_allocator.allocate(sizeof(TType), alignOf<TType>());
        return new (mem_ptr) TType(sbstd::forward<TArgs>(args)...);
    }

    template <typename... TArgs>
    TType * operator[](usize count)
    {
        TType * client_obj = nullptr;

        if (sbstd::is_trivial<TType>::value)
        {
            void * const mem_ptr = m_allocator.allocate(sizeof(TType) * count, alignOf<TType>());
            client_obj = reinterpret_cast<TType *>(mem_ptr);
        }
        else
        {
            usize const required_padding = (alignof(TType) <= m_allocator.getAlignment())
                                               ? sb::alignUp(sizeof(ArrayInfo), alignof(TType))
                                               : (alignof(TType) + sizeof(ArrayInfo));
            usize const total_size = required_padding + sizeof(TType) * count;

            ui8 * const mem_ptr = reinterpret_cast<ui8 *>(m_allocator.allocate(total_size));
            ui8 * const client_ptr =
                reinterpret_cast<ui8 *>(alignUp(reinterpret_cast<uptr>(mem_ptr + sizeof(ArrayInfo)), alignof(TType)));
            client_obj = reinterpret_cast<TType *>(client_ptr);

            ArrayInfo * const array_info = reinterpret_cast<ArrayInfo *>(client_ptr - sizeof(ArrayInfo));
            *array_info = {(usize)(client_ptr - mem_ptr), count};

            for (usize obj_idx = 0; obj_idx != count; ++obj_idx)
            {
                new (&client_obj[obj_idx]) TType();
            }
        }

        return client_obj;
    }

    static void Destroy(TType * ptr, AllocatorView allocator)
    {
        if (nullptr != ptr)
        {
            ptr->~TType();
            allocator.deallocate(ptr);
        }
    }

    static void Destroy(TType * ptr)
    {
        Destroy(ptr, getGlobalHeapView());
    }

    static void DestroyArray(TType * ptr, AllocatorView allocator)
    {
        if (nullptr != ptr)
        {
            if (sbstd::is_trivial<TType>::value)
            {
                allocator.deallocate(ptr);
            }
            else
            {
                ui8 * const objs_ptr = reinterpret_cast<ui8 *>(ptr);
                ArrayInfo * const array_info = reinterpret_cast<ArrayInfo *>(objs_ptr - sizeof(ArrayInfo));

                ui8 * const base_ptr = objs_ptr - array_info->m_base_offset;

                for (usize obj_idx = 0; obj_idx != array_info->m_obj_count; ++obj_idx)
                {
                    ptr[obj_idx].~TType();
                }

                allocator.deallocate(base_ptr);
            }
        }
    }

    static void DestroyArray(TType * ptr)
    {
        DestroyArray(ptr, getGlobalHeapView());
    }

    AllocatorView m_allocator;
};

template <typename TType>
struct MemoryOperatorHelperNew : protected MemoryOperatorHelper<TType>
{
    sbBaseClass(MemoryOperatorHelper<TType>);

    using BaseClass::MemoryOperatorHelper;
    using BaseClass::operator();
};

template <typename TType>
struct MemoryOperatorHelperDelete : protected MemoryOperatorHelper<TType>
{
    sbBaseClass(MemoryOperatorHelper<TType>);

    using BaseClass::Destroy;
    using BaseClass::MemoryOperatorHelper;
};

template <typename TType>
struct MemoryOperatorHelperNewArray : protected MemoryOperatorHelper<TType>
{
    sbBaseClass(MemoryOperatorHelper<TType>);

    using BaseClass::MemoryOperatorHelper;
    using BaseClass::operator[];
};

template <typename TType>
struct MemoryOperatorHelperDeleteArray : protected MemoryOperatorHelper<TType>
{
    sbBaseClass(MemoryOperatorHelper<TType>);

    using BaseClass::DestroyArray;
    using BaseClass::MemoryOperatorHelper;
};

} // namespace sb::internal

void * operator new(sb::usize byte_count);

void * operator new(sb::usize count, sbstd::nothrow_t const & tag) noexcept;

void * operator new(sb::usize byte_count, sbstd::align_val_t alignment);

void * operator new[](sb::usize byte_count);

void * operator new[](sb::usize count, sbstd::nothrow_t const & tag) noexcept;

void * operator new[](sb::usize byte_count, sbstd::align_val_t alignment);

void operator delete(void * ptr) noexcept;

void operator delete(void * ptr, sbstd::nothrow_t const & tag) noexcept;

void operator delete(void *, sbstd::align_val_t alignment) noexcept;

void operator delete[](void * ptr) noexcept;

void operator delete[](void * ptr, sbstd::nothrow_t const & tag) noexcept;

void operator delete[](void *, sbstd::align_val_t alignment) noexcept;
