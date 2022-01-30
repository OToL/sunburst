#pragma once

#include <sb_core/core.h>
#include <sb_core/utility.h>
#include <sb_core/string/utility.h>
#include <sb_core/error.h>

#include <sb_std/cstring>
#include <sb_std/string_view>

namespace sb {

// @todo: Proper character traits
template <usize CAPACITY, typename TChar = char>
class FixString
{
    static_assert(CAPACITY != 0, "FixString cannot 0 as CAPACITY");

    using StringView = sbstd::basic_string_view<TChar>;

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

    FixString(TChar const * src, usize len)
    {
        if ((nullptr == src) || (0 == len))
        {
            _data[0] = TChar{};
        }
        else
        {
            _len = strCpyT(&_data[0], minValue(CAPACITY, len + 1), src);
        }
    }

    FixString(StringView const & view)
        : FixString(view.data(), view.size())
    {
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

    template <usize SRC_CAPACITY>
    FixString(FixString<SRC_CAPACITY, TChar> && src)
    {
        _len = strCpyT(_data, src.c_str());
    }

    FixString(FixString && src)
    {
        _len = strCpyT(_data, src.c_str());
    }

    ~FixString() = default;

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
        sb_assert(idx <= _len);

        return _data[idx];
    }

    template <usize LCAPACITY>
    b8 operator==(FixString<LCAPACITY> const & lvalue)
    {
        if (_len != lvalue._len)
        {
            return false;
        }

        // @todo: use character size agnostic comparison
        return (0 == strcmp(_data, lvalue._data));
    }

    TChar back() const
    {
        sb_warn(0 != _len)

        auto backChar = TChar{};

        if (0 != _len)
        {
            backChar = _data[_len - 1];
        }

        return backChar;
    }

    bool push_back(TChar c)
    {
        sb_warn(_len != (CAPACITY - 1))

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
        strCatT(&_data[0], _len, CAPACITY, str);

        // @todo: implement my own strncat which returns the numbers of copied character
        _len = sbstd::strlen(&_data[0]);

        return *this;
    }

    TChar const * c_str() const
    {
        return &_data[0];
    }

    TChar * data()
    {
        return &_data[0];
    }

    template <usize SRC_CAPACITY>
    FixString & operator=(FixString<SRC_CAPACITY, TChar> const & src)
    {
        _len = strCpyT(_data, src.c_str());

        return *this;
    }

    // NOLINTNEXTLINE(bugprone-unhandled-self-assignment)
    FixString & operator=(FixString const & src)
    {
        if (this != &src)
        {
            _len = strCpyT(_data, src.c_str());
        }

        return *this;
    }

    template <usize SRC_CAPACITY>
    FixString & operator=(FixString<SRC_CAPACITY, TChar> && src)
    {
        _len = strCpyT(_data, src.c_str());

        return *this;
    }

    // NOLINTNEXTLINE(bugprone-unhandled-self-assignment)
    FixString & operator=(FixString && src)
    {
        if (this != &src)
        {
            _len = strCpyT(_data, src.c_str());
        }

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
    usize _len = 0ULL;
};

template <usize CAPACITY, typename TChar = char>
using FString = FixString<CAPACITY, TChar>;

} // namespace sb
