#pragma once

#include <sb_core/core.h>
#include <sb_core/containers/dynamic_array.h>
#include <sb_core/extern/windows/windows.h>

#include <sb_slw/string_view>
#include <sb_slw/algorithm>

constexpr inline char const * const WHITESPACE = " \n\r\t\f\v";

inline slw::string_view ltrim(const slw::string_view & s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == slw::string_view::npos) ? "" : s.substr(start);
}

inline slw::string_view rtrim(const slw::string_view & s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == slw::string_view::npos) ? "" : s.substr(0, end + 1);
}

inline slw::string_view trim(const slw::string_view & s)
{
    return rtrim(ltrim(s));
}

// @todo: one empty item is appending at the end
// @todo: windows file termination is not handled properly \\r\\n
inline sb::DArray<slw::string_view> splitString(slw::string_view value, char sep)
{
    //[[maybe_unused]] auto const ret_count = slw::count(begin(file_string_buffer), end(file_string_buffer), '\n');
    sb::DArray<slw::string_view> tokens;

    auto const ret_count = slw::count(begin(value), end(value), '\n');
    tokens.reserve(ret_count);

    while (true)
    {
        auto const break_idx = value.find(sep);

        if (slw::string_view::npos == break_idx)
        {
            tokens.push_back(value);
            break;
        }
        else
        {
            tokens.push_back(value.substr(0, break_idx));
            value = value.substr(break_idx + 1);
        }
    }

    return tokens;
}

inline sb::DArray<slw::string_view> splitString(slw::string_view value, slw::string_view sep)
{
    //[[maybe_unused]] auto const ret_count = slw::count(begin(file_string_buffer), end(file_string_buffer), '\n');
    sb::DArray<slw::string_view> tokens;

    // auto const ret_count = slw::count(begin(value), end(value), '\n');
    // tokens.reserve(ret_count);

    while (true)
    {
        auto const break_idx = value.find_first_of(sep);

        if (slw::string_view::npos == break_idx)
        {
            tokens.push_back(value);
            break;
        }
        else
        {
            tokens.push_back(value.substr(0, break_idx));
            value = value.substr(break_idx + sep.size());
        }
    }

    return tokens;
}

struct ConsoleAttrib
{
    using ValueType = sb::u16;

    enum : sb::u32
    {
        TEXT_BLUE = FOREGROUND_BLUE,
        TEXT_GREEN = FOREGROUND_GREEN,
        TEXT_RED = FOREGROUND_RED,
        TEXT_MORE_INTENSE = FOREGROUND_INTENSITY,
        BG_BLUE = BACKGROUND_BLUE,
        BG_GREEN = BACKGROUND_GREEN,
        BG_RED = BACKGROUND_RED,
        BG_MORE_INTENSE = BACKGROUND_INTENSITY,
    };

    ValueType value = 0;
};

sb::b8 setConsoleSize(sb::i32 cols, sb::i32 rows);

void clearConsole(COORD pos, ConsoleAttrib attrib, HANDLE console, sb::i32 rows, sb::i32 cols);

COORD getConsoleCursorPosition(HANDLE console_hdl);
