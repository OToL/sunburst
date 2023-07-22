#include "advent.h"
#include "utility.h"

#include <sb_core/log.h>
#include <sb_core/error.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/io/file_hdl.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/containers/dynamic_fix_array.h>
#include <sb_core/string/string.h>

#include <sb_slw/string_view>

#include <ranges>
#include <charconv>
#include <algorithm>
#include <vector>
#include <xutility>
#include <functional>

using namespace sb;

struct Vect2D
{
    i64 x = 0;
    i64 y = 0;
};

Vect2D getScreenPos(i64 clock_idx)
{
    i64 const row_idx = (clock_idx - 1) / 40;
    i64 const column_idx = (clock_idx - 1) % 40;

    return {row_idx, column_idx};
};

// Solution
//    14820
//    ###..####.####.#..#.####.####.#..#..##..
//    #..#....#.#....#.#..#....#....#..#.#..#.
//    #..#...#..###..##...###..###..####.#..#.
//    ###...#...#....#.#..#....#....#..#.####.
//    #.#..#....#....#.#..#....#....#..#.#..#.
//    #..#.####.####.#..#.####.#....#..#.#..#.
// @todo: reduce code duplication in clock idx management
void advent::runDay10()
{
    sb_log_info("Running advent day 10 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day10.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));
    auto tokens_raw = splitString(file_string_buffer, '\n');

    for (auto & token : tokens_raw)
    {
        token = trim(token);
    }

    slw::span<slw::string_view> tokens = tokens_raw;
    if (tokens.back().empty())
    {
        tokens = tokens.subspan(0, tokens.size() - 1);
    }

    i64 signal_strength = 0;
    [[maybe_unused]] i64 clock_idx = 1;
    i64 reg_x = 1;
    i64 constexpr SCREEN_WIDTH = 40;
    i64 constexpr SCREEN_HEIGHT = 6;

    char screen[6][SCREEN_WIDTH + 1];

    for (i64 idx = 0; idx != SCREEN_HEIGHT; ++idx)
    {
        screen[idx][SCREEN_WIDTH] = 0;
    }

    for (auto const & token : tokens)
    {
        if (token.starts_with("noop"))
        {
            if ((clock_idx == 20) || (0 == ((20 + clock_idx) % 40)))
            {
                signal_strength += reg_x * clock_idx;
            }
            Vect2D screen_pos = getScreenPos(clock_idx);

            if ((screen_pos.y >= (reg_x - 1)) && (screen_pos.y <= (reg_x + 1)))
            {
                screen[screen_pos.x][screen_pos.y] = '#';
            }
            else
            {
                screen[screen_pos.x][screen_pos.y] = '.';
            }

            ++clock_idx;
        }
        else if (token.starts_with("addx"))
        {
            i64 value = 0;
            std::from_chars(token.data() + 5, token.data() + token.size(), value);

            for (i32 idx = 0; idx != 2; ++idx)
            {
                if ((clock_idx == 20) || (0 == ((20 + clock_idx) % 40)))
                {
                    signal_strength += reg_x * clock_idx;
                }
                Vect2D screen_pos = getScreenPos(clock_idx);
                if ((screen_pos.y >= (reg_x - 1)) && (screen_pos.y <= (reg_x + 1)))
                {
                    screen[screen_pos.x][screen_pos.y] = '#';
                }
                else
                {
                    screen[screen_pos.x][screen_pos.y] = '.';
                }
                ++clock_idx;
            }

            reg_x += value;
        }
    }

    // print screen
    for (i64 idx = 0; idx != SCREEN_HEIGHT; ++idx)
    {
        printf("%s\n", screen[idx]);
    }

    for (i64 idx = 0; idx != SCREEN_HEIGHT; ++idx)
    {
        screen[idx][SCREEN_WIDTH] = 0;
    }

    sb_log_info("Signal strength {}", signal_strength);

    sb_free(GHEAP, byte_buffer);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);
}
