#pragma once

#include <core/platform.h>

#include <libc++/type_traits>

namespace sb {

// TODO: replace by std::span
template <typename TType>
class Span
{
public:
    using value_type = TType;
    using pointer = value_type *;
    using const_pointer = value_type const *;
    using reference = value_type &;
    using const_reference = value_type const &;
    using iterator = value_type *;
    using const_iterator = value_type const *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    Span(pointer data, usize size)
        : m_data(data)
        , m_size(size)
    {
//        sbAssert((nullptr != data) || (0ULL == size));
    }

    template <typename T>
    Span(T & values, typename wstd::enable_if_t<wstdx::is_contiguous_sequence_container<T>::value> * = nullptr)
        : m_data(values.data())
        , m_size(values.size())
    {
    }

    template <usize COUNT>
    Span(TType (&values)[COUNT])
        : m_data(&values[0])
        , m_size(COUNT)
    {
    }

    Span()
        : m_data(nullptr)
        , m_size(0)
    {
    }

    Span sub(size_type offset) const
    {
        // When equal to m_size, a valid empty span will be created
        //sbAssert(offset <= m_size);

        return {m_data + offset, m_size - offset};
    }

    bool empty() const
    {
        return (0 == m_size);
    }

    size_type size() const
    {
        return m_size;
    }

    pointer data() const
    {
        return m_data;
    }

    const_reference operator[](size_type idx) const
    {
        //sbAssert(idx < m_size);

        return m_data[idx];
    }

    reference operator[](size_type idx)
    {
        //sbAssert(idx < m_size);

        return m_data[idx];
    }

    const_iterator cbegin() const
    {
        return m_data;
    }

    const_iterator cend() const
    {
        return (m_data + m_size);
    }

    const_iterator begin() const
    {
        return m_data;
    }

    iterator begin()
    {
        return m_data;
    }

    iterator end()
    {
        return (m_data + m_size);
    }

    const_iterator end() const
    {
        return (m_data + m_size);
    }

private:
    pointer m_data;
    usize m_size;
};

template <typename TType>
typename Span<TType>::iterator begin(Span<TType> & view)
{
    return view.begin();
}

template <typename TType>
typename Span<TType>::iterator end(Span<TType> & view)
{
    return view.end();
}

template <typename TType>
typename Span<TType>::const_iterator begin(Span<TType> const & view)
{
    return view.begin();
}

template <typename TType>
typename Span<TType>::const_iterator end(Span<TType> const & view)
{
    return view.end();
}
} // namespace sb
