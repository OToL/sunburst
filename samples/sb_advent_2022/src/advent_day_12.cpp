#include "advent.h"
#include "sb_core/containers/dynamic_array.h"
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
#include <sb_core/containers/ring_buffer.h>
#include <sb_core/extern/windows/windows.h>
#include <winnt.h>
#include <winsvc.h>
#undef max
#undef min

#include <sb_slw/string_view>

#include <ranges>
#include <charconv>
#include <algorithm>
#include <vector>
#include <xutility>
#include <functional>
#include <unordered_map>
#include <cmath>
#include <conio.h>
#include <io.h>

using namespace sb;

struct Vect2D
{
    i32 x = 0;
    i32 y = 0;
};

static Vect2D operator-(Vect2D lval, Vect2D rval)
{
    return {lval.x - rval.x, lval.y - rval.y};
}

static Vect2D operator+(Vect2D lval, Vect2D rval)
{
    return {lval.x + rval.x, lval.y + rval.y};
}

b8 operator==(Vect2D lval, Vect2D rval)
{
    return ((lval.x == rval.x) && (lval.y == rval.y));
}

double getLength(Vect2D vect)
{
    return std::sqrt(vect.x * vect.x + vect.y * vect.y);
}

template <>
struct std::hash<Vect2D>
{
    std::size_t operator()(Vect2D const & pos) const noexcept
    {
        return pos.y * 135 + pos.x; // or use boost::hash_combine
    }
};

constexpr char END_VAL = 'E';
constexpr char END_HEIGHT = 'z';
constexpr char START_VAL = 'S';
constexpr char START_HEIGHT = 'a';

static HANDLE g_console_hdl = nullptr;

i32 getHeghtAt(char height)
{
    char actual_value = height;
    if (height == START_VAL)
    {
        actual_value = START_HEIGHT;
    }
    else if (height == END_VAL)
    {
        actual_value = END_HEIGHT;
    }

    return actual_value - 'a';
}

usize getArrayIndexFromPos(Vect2D pos, usize col_count)
{
    return pos.y * col_count + pos.x;
}

Vect2D getPosFromArrayIndex(i32 index, i32 col_count)
{
    return Vect2D{index % col_count, index / col_count};
}

i32 getShortestPath(slw::span<slw::string_view> const & terrain, Vect2D start, Vect2D end, std::unordered_map<Vect2D, Vect2D> & came_from)
{
    struct Option
    {
        Vect2D position;
        double cost;
    };

    DArray<Option> open_set;
    open_set.push_back(Option{start, 0});

    i32 const col_cnt = (i32)terrain[0].size();
    i32 const row_cnt = (i32)terrain.size();
    i32 const terrain_size = col_cnt * row_cnt;

    DArray<double> g_score(terrain_size, slw::numeric_limits<double>::max());
    DArray<double> f_score(terrain_size, slw::numeric_limits<double>::max());
    g_score[(i32)getArrayIndexFromPos(start, col_cnt)] = 0;
    f_score[(i32)getArrayIndexFromPos(start, col_cnt)] = getLength(end - start);

    DArray<CHAR_INFO> terrain_disp(terrain_size);

    Vect2D neighbors[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    while (!open_set.empty())
    {
        [[maybe_unused]] Vect2D curr_pos = open_set.back().position;
        if (curr_pos == end)
        {
            auto pos_iter = curr_pos;
            i32 step_cnt = 0;
            while (pos_iter != start)
            {
                ++step_cnt;
                pos_iter = came_from[pos_iter];
            }

            return step_cnt;
        }

        open_set.resize(open_set.size() - 1);
        for (auto const & dir : neighbors)
        {
            auto curr_height = getHeghtAt(terrain[curr_pos.y][curr_pos.x]);
            Vect2D neighbor_pos = curr_pos + dir;
            if ((0 < neighbor_pos.x) && (0 < neighbor_pos.y) && (col_cnt > neighbor_pos.x) && (row_cnt > neighbor_pos.y))
            {
                auto neighbor_height = getHeghtAt(terrain[neighbor_pos.y][neighbor_pos.x]);
                if (neighbor_height <= (curr_height + 1))
                {
                    auto neighbor_idx = (i32)getArrayIndexFromPos(neighbor_pos, col_cnt);
                    double tentative_score = g_score[(i32)getArrayIndexFromPos(curr_pos, col_cnt)] + 1.0;
                    if (tentative_score < g_score[neighbor_idx])
                    {
                        double fscore = tentative_score + getLength(end - neighbor_pos);
                        came_from[neighbor_pos] = curr_pos;
                        g_score[neighbor_idx] = tentative_score;
                        f_score[neighbor_idx] = fscore;

                        auto iter = std::find_if(begin(open_set), std::end(open_set),
                                                 [&neighbor_pos](auto const & val) { return val.position == neighbor_pos; });

                        if (iter == std::end(open_set))
                        {
                            open_set.push_back(Option{neighbor_pos, fscore});
                        }
                        else
                        {
                            iter->cost = fscore;
                        }
                        std::sort(begin(open_set), std::end(open_set), [](auto const & lval, auto const & rval) { return lval.cost > rval.cost; });
                    }
                }
            }
        }
    }

    return std::numeric_limits<i32>::max();
}

// Solution
//    449
//    443
// @todo: study A*
// @todo: visualize A* (imgui or console)
// @todo: implement priority queue
// @todo: Use Dijkstra's algorithm instead of length
void advent::runDay12()
{
    // sb_log_info("Running advent ** day 12 ** trivia ...\n");

    g_console_hdl = GetStdHandle(STD_OUTPUT_HANDLE);
    sb_assert(nullptr != g_console_hdl);

    FileHdl file_hdl = vfs::openFileRead("/data/day12.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    vfs::closeFile(file_hdl);
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

    Vect2D start, end = {};

    i32 row_idx = 0;
    for (auto & row : tokens)
    {
        i32 col_idx = 0;
        for (auto & val : row)
        {
            if (val == START_VAL)
            {
                start = {col_idx, row_idx};
            }
            else if (val == END_VAL)
            {
                end = {col_idx, row_idx};
            }
            ++col_idx;
        }
        ++row_idx;
    }

    sb_log_info("start = row:{} column:{}", start.y, start.x);
    sb_log_info("end = row:{} column:{}", end.y, end.x);

    COORD cursor_coords = getConsoleCursorPosition(g_console_hdl);

    // Render the terrain
    int const col_cnt = (int)tokens[0].size();
    int const row_cnt = (int)tokens.size();
    COORD buffer_size = {(short)col_cnt, (short)row_cnt};
    COORD start_pos = {0, 0};

    auto const y_offset = static_cast<sb::i16>(cursor_coords.Y);
    COORD const offset_pos = {0, y_offset};

    SMALL_RECT dest = {0, y_offset, (short)col_cnt, static_cast<SHORT>((short)row_cnt + y_offset)};
    DArray<CHAR_INFO> terrain_buffer(col_cnt * row_cnt);

    setConsoleSize(col_cnt, row_cnt + y_offset);

    std::unordered_map<Vect2D, Vect2D> explored_cells;
    i32 step_cnt = getShortestPath(tokens, start, end, explored_cells);
    sb_log_info("Step count {}", step_cnt);

    // fill initial data
    for (int line_idx = 0; line_idx != row_cnt; ++line_idx)
    {
        for (int col_idx = 0; col_idx != col_cnt; ++col_idx)
        {
            int const char_idx = line_idx * col_cnt + col_idx;
            terrain_buffer[char_idx].Char.AsciiChar = tokens[line_idx][col_idx];
            terrain_buffer[char_idx].Attributes = 0x71;
        }
    }

    sb::DArray<Vect2D> path;

    auto pos_iter = explored_cells[end];
    while (pos_iter != start)
    {
        path.push_back(pos_iter);
        pos_iter = explored_cells[pos_iter];
    }
    path.push_back(start);
    std::ranges::reverse(path);

    for (auto const &path_iter : path)
    {
        auto idx = getArrayIndexFromPos(path_iter, col_cnt);
        terrain_buffer[idx].Attributes = ConsoleAttrib::TEXT_GREEN;

        Sleep(33);
        WriteConsoleOutput(g_console_hdl, (CHAR_INFO *)terrain_buffer.data(), buffer_size, start_pos, &dest);
    }

    getchar();
    clearConsole(offset_pos, ConsoleAttrib{}, g_console_hdl, row_cnt, col_cnt);

    DArray<Vect2D> all_0_height;
    row_idx = 0;
    for (auto const & row : tokens)
    {
        i32 col_idx = 0;
        for (auto const & val : row)
        {
            if (getHeghtAt(val) == 0)
            {
                all_0_height.push_back(Vect2D{col_idx, row_idx});
            }

            ++col_idx;
        }

        ++row_idx;
    }

    i32 shortest_path = std::numeric_limits<i32>::max();
    sb_log_info("start point count {}", all_0_height.size());
    for (auto const & start_point : all_0_height)
    {
        explored_cells.clear();
        auto const new_val = getShortestPath(tokens, start_point, end, explored_cells);
        if (new_val != std::numeric_limits<i32>::max())
        {
            sb_log_info("new val {}", new_val);
            shortest_path = std::min(shortest_path, new_val);
        }
    }
    sb_log_info("Shortest hike {}", shortest_path);

    CloseHandle(g_console_hdl);
    sb_free(GHEAP, byte_buffer);
}
