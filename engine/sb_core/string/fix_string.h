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
            _data[0] = TChar{};
        }
        else
        {
            _len = strCpyT(_data, src);
        }
    }

    template <usize SRC_CAPACITY>
    FixString(FixString<SRC_CAPACITY, TChar> const & src)
    {
        _len = strCpyT(_data, src.c_str());
    }

    FixString(FixString<CAPACITY, TChar> const & src)
    {
        _len = strCpyT(_data, src.c_str());
    }

    ~FixString() { }

    usize capacity() const
    {
        return CAPACITY;
    }

    usize length() const
    {
        return _len;
    }

    bool empty() const
    {
        return (_data[0] == TChar{});
    }

    void clear()
    {
        _len = 0;
        _data[0] = TChar{};
    }

    TChar operator[](usize idx) const
    {
        // Accessing the null terminator is a valid operation
        sbAssert(idx <= _len);

        return _data[idx];
    }

    TChar back() const
    {
        sbWarn(0 != _len)

            TChar backChar = TChar{};

        if (0 != _len)
        {
            backChar = _data[_len - 1];
        }

        return backChar;
    }

    bool push_back(TChar c)
    {
        sbWarn(_len != (CAPACITY - 1))

            if (_len != (CAPACITY - 1))
        {
            _data[_len] = c;
            ++_len;
            _data[_len] = TChar{};

            return true;
        }

        return false;
    }

    FixString & append(TChar const * str)
    {
        strCatT(_data, _len, CAPACITY, str);

        // TODO: implement my own strncat which returns the numbers of copied character
        _len = sbstd::strlen(_data);

        return *this;
    }

    TChar const * c_str() const
    {
        return _data;
    }

    TChar * data()
    {
        return _data;
    }

    template <usize SRC_CAPACITY>
    FixString & operator=(FixString<SRC_CAPACITY, TChar> const & src)
    {
        _len = strCpyT(_data, src.c_str());

        return *this;
    }

    FixString & operator=(FixString const & src)
    {
        _len = strCpyT(_data, src.c_str());

        return *this;
    }

    FixString & operator=(TChar const * src)
    {
        _len = strCpyT(_data, src);

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
    TChar _data[CAPACITY];
    usize _len;
};

template <usize CAPACITY, typename TChar = char>
using FString = FixString<CAPACITY, TChar>;

} // namespace sb
