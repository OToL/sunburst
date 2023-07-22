#include "utility.h"

#include <sb_core/extern/windows/windows.h>


sb::b8 setConsoleSize(sb::i32 cols, sb::i32 rows)
{
    HWND hWnd = nullptr;
    HANDLE hConOut = nullptr;
    CONSOLE_FONT_INFO fi = {};
    CONSOLE_SCREEN_BUFFER_INFO bi = {};

    int w = 0, h = 0, bw = 0, bh = 0;
    RECT rect = {0, 0, 0, 0};
    COORD coord = {0, 0};
    hWnd = GetConsoleWindow();

    if (hWnd)
    {
        hConOut = GetStdHandle(STD_OUTPUT_HANDLE);

        if (nullptr != hConOut)
        {
            if (GetCurrentConsoleFont(hConOut, FALSE, &fi))
            {
                if (GetClientRect(hWnd, &rect))
                {
                    w = rect.right - rect.left;
                    h = rect.bottom - rect.top;
                    if (GetWindowRect(hWnd, &rect))
                    {
                        bw = rect.right - rect.left - w;
                        bh = rect.bottom - rect.top - h;
                        if (GetConsoleScreenBufferInfo(hConOut, &bi))
                        {
                            coord.X = bi.dwSize.X;
                            coord.Y = bi.dwSize.Y;
                            if (coord.X < cols || coord.Y < rows)
                            {
                                if (coord.X < cols)
                                {
                                    coord.X = (short)cols;
                                }
                                if (coord.Y < rows)
                                {
                                    coord.Y = (short)rows;
                                }
                                if (!SetConsoleScreenBufferSize(hConOut, coord))
                                {
                                    return FALSE;
                                }
                            }
                            return SetWindowPos(hWnd, nullptr, rect.left, rect.top, cols * fi.dwFontSize.X + bw, rows * fi.dwFontSize.Y + bh,
                                                SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
                        }
                    }
                }
            }
        }
    }
    return FALSE;
}

void clearConsole(COORD pos, ConsoleAttrib attrib, HANDLE console, sb::i32 rows, sb::i32 cols)
{
    DWORD written = 0;
    unsigned _size = 0;

    _size = rows * cols;

    FillConsoleOutputCharacter(console, ' ', _size, pos, &written);
    FillConsoleOutputAttribute(console, attrib.value, _size, pos, &written);
    SetConsoleCursorPosition(console, pos);
}

COORD getConsoleCursorPosition(HANDLE console_hdl)
{
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    if (GetConsoleScreenBufferInfo(console_hdl, &cbsi))
    {
        return cbsi.dwCursorPosition;
    }
    else
    {
        // The function failed. Call GetLastError() for details.
        COORD invalid = { 0, 0 };
        return invalid;
    }
}

