#pragma once

namespace sb::internal {

template <typename TType, typename TAllocator>
class SArrayBase
{
public:
    using value_type = TType;
    using reference = TType &;
    using const_reference = TType const &;
    using pointer = TType *;
    using const_pointer = TType const *;
    using iterator = TType *;
    using const_iterator = TType const *;
    using size_type = usize;
    using difference_type = sptrdiff;

protected:
    SArrayBase & operator=(SArrayBase const &) = delete;
    SArrayBase(SArrayBase const &) = delete;

    SArrayBase() = default;

    struct Impl : public TAllocator
    {
        sbBaseClass(TAllocator);

        Impl() = default;

        Impl(TAllocator const & alloc)
            : BaseClass(alloc)
        {
        }

        Impl(pointer begin, pointer storage_end, TAllocator const & alloc)
            : BaseClass(alloc)
            , m_begin(begin)
            , m_end(begin)
            , m_storage_end(storage_end)
        {
        }

        Impl(pointer begin, pointer storage_end)
            : BaseClass()
            , m_begin(begin)
            , m_end(begin)
            , m_storage_end(storage_end)
        {
        }

        void reset()
        {
            m_begin = nullptr;
            m_end = nullptr;
            m_storage_end = nullptr;
        }

        void init(pointer begin, pointer storage_end, TAllocator const & alloc)
        {
            m_begin = begin;
            m_end = begin;
            m_storage_end = storage_end;

            get_allocator() = alloc;
        }

        void init(pointer begin, pointer storage_end)
        {
            m_begin = begin;
            m_end = begin;
            m_storage_end = storage_end;
        }

        void setAllocator(TAllocator const & alloc)
        {
            *((BaseClass *)this) = alloc;
        }

        TAllocator & get_allocator()
        {
            return *this;
        }

        TAllocator const & get_allocator() const
        {
            return *this;
        }

        pointer m_begin = nullptr;
        pointer m_end = nullptr;
        pointer m_storage_end = nullptr;
    };
};
} // namespace sb::internal
