#pragma once

#include <sb_core/core.h>
#include <sb_core/string/string_utility.h>
#include <sb_core/error.h>

#include <sb_std/cstring>

namespace sb {

// TODO: Proper character traits
template <usize CAPACITY, typename TChar = char>
class FixString
{
    static_assert(CAPACITY != 0, "FixString cannot 0 as CAPACITY");

public:
    FixString()
    {
        clear();
    }

    FixString(TChar const * src)
    {
        if (nullptr == src)
        {
            m_data[0] = TChar{};
        }
        else
        {
            m_len = strCpyT(m_data, src);
        }
    }

    template <usize SRC_CAPACITY>
    FixString(FixString<SRC_CAPACITY, TChar> const & src)
    {
        m_len = strCpyT(m_data, src.c_str());
    }

    FixString(FixString<CAPACITY, TChar> const & src)
    {
        m_len = strCpyT(m_data, src.c_str());
    }

    ~FixString() { }

    usize capacity() const
    {
        return CAPACITY;
    }

    usize length() const
    {
        return m_len;
    }

    bool empty() const
    {
        return (m_data[0] == TChar{});
    }

    void clear()
    {
        m_len = 0;
        m_data[0] = TChar{};
    }

    TChar operator[](usize idx) const
    {
        // Accessing the null terminator is a valid operation
        sbAssert(idx <= m_len);

        return m_data[idx];
    }

    TChar back() const
    {
        sbWarn(0 != m_len)

            TChar backChar = TChar{};

        if (0 != m_len)
        {
            backChar = m_data[m_len - 1];
        }

        return backChar;
    }

    bool push_back(TChar c)
    {
        sbWarn(m_len != (CAPACITY - 1))

            if (m_len != (CAPACITY - 1))
        {
            m_data[m_len] = c;
            ++m_len;
            m_data[m_len] = TChar{};

            return true;
        }

        return false;
    }

    FixString & append(TChar const * str)
    {
        sbstd::strncat(m_data, str, CAPACITY - 1);
        m_data[CAPACITY - 1] = TChar{};

        // TODO: implement my own strncat which returns the numbers of copied character
        m_len = sbstd::strlen(m_data);

        return *this;
    }

    TChar const * c_str() const
    {
        return m_data;
    }

    TChar * data()
    {
        return m_data;
    }

    template <usize SRC_CAPACITY>
    FixString & operator=(FixString<SRC_CAPACITY, TChar> const & src)
    {
        m_len = strCpyT(m_data, src.c_str());

        return *this;
    }

    FixString & operator=(FixString const & src)
    {
        m_len = strCpyT(m_data, src.c_str());

        return *this;
    }

    FixString & operator=(TChar const * src)
    {
        m_len = strCpyT(m_data, src);

        return *this;
    }

    template <usize SRC_CAPACITY>
    FixString & operator+=(FixString<SRC_CAPACITY, TChar> const & src)
    {
        return append(src.c_str());
    }

    FixString & operator+=(TChar const * src)
    {
        return append(src);
    }

private:
    TChar m_data[CAPACITY];
    usize m_len;
};

template <usize CAPACITY, typename TChar = char>
using FString = FixString<CAPACITY, TChar>;

} // namespace sb
