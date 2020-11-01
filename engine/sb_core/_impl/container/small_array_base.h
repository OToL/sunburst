#pragma once

namespace sb::internal {

template <typename TType, typename TAllocator>
class SmallArrayBase
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
    SmallArrayBase & operator=(SmallArrayBase const &) = delete;
    SmallArrayBase(SmallArrayBase const &) = delete;

    SmallArrayBase() = default;

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
            , _begin(begin)
            , _end(begin)
            , _storage_end(storage_end)
        {
        }

        Impl(pointer begin, pointer storage_end)
            : BaseClass()
            , _begin(begin)
            , _end(begin)
            , _storage_end(storage_end)
        {
        }

        void reset()
        {
            _begin = nullptr;
            _end = nullptr;
            _storage_end = nullptr;
        }

        void init(pointer begin, pointer storage_end, TAllocator const & alloc)
        {
            _begin = begin;
            _end = begin;
            _storage_end = storage_end;

            get_allocator() = alloc;
        }

        void init(pointer begin, pointer storage_end)
        {
            _begin = begin;
            _end = begin;
            _storage_end = storage_end;
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

        pointer _begin = nullptr;
        pointer _end = nullptr;
        pointer _storage_end = nullptr;
    };
};
} // namespace sb::internal
