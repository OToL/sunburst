#include "advent.h"
#include "utility.h"

#include <sb_core/log.h>
#include <sb_core/error.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/io/file_hdl.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/string/fix_string.h>

#include <sb_slw/string_view>

#include <charconv>
#include <algorithm>
#include <variant>
#include <memory>
#include <stack>

using namespace sb;

struct Vec2
{
    int x;
    int y;
};

Vec2 operator-(Vec2 lval, Vec2 rval)
{
    return {lval.x - rval.x, lval.y - rval.y};
}

Vec2 operator+(Vec2 lval, Vec2 rval)
{
    return {lval.x + rval.x, lval.y + rval.y};
}

b8 operator==(Vec2 lval, Vec2 rval)
{
    return (lval.x == rval.x && lval.y == rval.y);
}

// Solution
//    692
//    31705
void advent::runDay14()
{
    sb_log_info("Running advent day 14 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day14.txt");
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

    DArray<DArray<Vec2>> paths;

    Vec2 board_extents = {};
    Vec2 board_mins = {
        .x = std::numeric_limits<int>::max(),
        .y = std::numeric_limits<int>::max(),
    };

    DArray<slw::string_view> path;
    sb_log_info("path item count {}", path.size());
    for (auto const & path_str : tokens)
    {
        path = splitString(path_str, "->");
        DArray<Vec2> actual_path;

        for (auto const & raw_item : path)
        {
            auto const item = trim(raw_item);

            Vec2 path_point = {};
            auto [ptr, ec] = std::from_chars(item.data(), item.data() + item.size(), path_point.x);
            sb_assert(ec == std::errc());

            ++ptr;
            auto [_, ec2] = std::from_chars(ptr, ptr + (item.size() - std::distance(item.data(), ptr)), path_point.y);
            sb_assert(ec2 == std::errc());

            board_extents.x = std::max(board_extents.x, path_point.x);
            board_extents.y = std::max(board_extents.y, path_point.y);

            board_mins.x = std::min(board_mins.x, path_point.x);
            board_mins.y = std::min(board_mins.y, path_point.y);

            actual_path.push_back(path_point);
        }

        paths.emplace_back(std::move(actual_path));
    }

    // x=514 y=180
    sb_log_info("board extents: {} {}", board_extents.x, board_extents.y);

    // 0 = air
    constexpr int LINE_CNT = 181 + 5;
    constexpr int COL_CNT = 515 + 200;
    char board[LINE_CNT][COL_CNT] = {};

    for (int x = 0; x != COL_CNT; ++x)
    {
        board[board_extents.y + 2][x] = '#';
    }

    for (auto const & path_val : slw::span(paths) /*.subspan(0, 2)*/)
    {
        Vec2 prev_point = path_val[0];
        for (auto const & point : slw::span(path_val).subspan(1))
        {
            sb_assert(point.x == prev_point.x || point.y == prev_point.y);
            Vec2 const dist = point - prev_point;
            Vec2 const dir = {.x = (dist.x == 0) ? 0 : dist.x / std::abs(dist.x), .y = (dist.y == 0) ? 0 : dist.y / std::abs(dist.y)};

            while (!(prev_point == point))
            {
                board[prev_point.y][prev_point.x] = '#';
                prev_point = prev_point + dir;
            }

            board[point.y][point.x] = '#';

            prev_point = point;
        }
    }

    Vec2 const START_SAND = {.x = 500, .y = 0};
    Vec2 const MOVES[] = {{0, 1}, {-1, 1}, {1, 1}};
    int sand_idx = 0;

    bool do_test = true;
    if (do_test)
    {
        b8 break_sanding = false;
        b8 break_infinity = false;
        for (; !break_sanding && !break_infinity; ++sand_idx)
        {
            auto sand_pos = START_SAND;

            while (!break_sanding && !break_infinity)
            {
                auto old_pos = sand_pos;
                for (auto const & move : MOVES)
                {
                    auto new_pos = sand_pos + move;

                    if (new_pos.y >= LINE_CNT)
                    {
                        break_infinity = true;
                        break;
                    }

                    if (board[new_pos.y][new_pos.x] == 0)
                    {
                        sand_pos = new_pos;
                        break;
                    }
                }

                if (old_pos == sand_pos)
                {
                    board[sand_pos.y][sand_pos.x] = 'o';

                    break_sanding = (sand_pos == START_SAND);
                    break;
                }
            }
        }
    }

    FString<COL_CNT + 1, char> line_str;
    for (int line_idx = 0; line_idx != LINE_CNT; ++line_idx)
    {
        for (int col_idx = 100; col_idx != COL_CNT; ++col_idx)
        {
            auto const board_val = board[line_idx][col_idx];
            line_str.push_back(board_val == 0 ? '.' : board_val);
        }

        printf("%s\n", line_str.c_str());
        line_str.clear();
    }

    sb_log_info("Sand count {}", sand_idx - 1);
    // sb_log_info("Seperators prod {}", seps_dist);

    sb_free(GHEAP, byte_buffer);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);
}
