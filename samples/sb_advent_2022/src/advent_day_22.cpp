#include "advent.h"
#include "sb_core/cast.h"
#include "sb_core/core.h"
#include "sb_slw/forward_decls"
#include "utility.h"

#include <algorithm>
#include <charconv>
#include <cstdlib>
#include <iterator>
#include <limits>
#include <minwinbase.h>
#include <sb_core/log.h>
#include <sb_core/error.h>
#include <sb_core/string/utility.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/containers/dynamic_array.h>
#include <sb_core/hash/hash_str.h>

#include <sb_slw/string_view>
#include <sb_slw/span>
#include <system_error>
#include <unordered_map>

using namespace sb;

#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"

struct Vec2i
{
    i32 x;
    i32 y;

    Vec2i & operator+=(Vec2i val)
    {
        x += val.x;
        y += val.y;

        return *this;
    }

    Vec2i & operator*=(i32 val)
    {
        x *= val;
        y *= val;

        return *this;
    }
};

enum class DirCode
{
    RIGHT,
    DOWN,
    LEFT,
    UP
};

constexpr Vec2i DIRECTIONS[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

struct Command
{
    Vec2i dir = {};
    DirCode dir_mode = DirCode::RIGHT;
    u32 count = 0;
};

struct Tile
{
    Vec2i pos = {};
    Vec2i extents = {};
    DArray<slw::string_view> data;
};

i32 wrapVal(i32 val, i32 count)
{
    if (0 > val)
    {
        sb_assert(val == -1);
        return count - 1;
    }
    else if (val >= count)
    {
        sb_assert(val == count);
        return 0;
    }

    return val;
}

b8 isInWorldBounds(Vec2i pos, Tile const & tile)
{
    if ((pos.x >= tile.pos.x) && ((tile.pos.x + tile.extents.x) > pos.x) && (pos.y >= tile.pos.y) && ((tile.pos.y + tile.extents.y) > pos.y))
    {
        return true;
    }

    return false;
}

b8 isInLocalBounds(Vec2i pos, Vec2i extends)
{
    if (pos.x < 0 || pos.y < 0 || pos.x >= extends.x || pos.y >= extends.y)
    {
        return false;
    }

    return true;
}

Vec2i toLocalPos(Vec2i pos, Tile const & tile)
{
    sb_assert(pos.x >= tile.pos.x);
    sb_assert(pos.y >= tile.pos.y);
    return {pos.x - tile.pos.x, pos.y - tile.pos.y};
}

Vec2i toWorldPos(Vec2i pos, Tile const & tile)
{
    return {pos.x + tile.pos.x, pos.y + tile.pos.y};
}

constexpr u32 DIR_COUNT = std::size(DIRECTIONS);
constexpr char WALL = '#';

u32 rotateDirection(u32 currDir, bool clockwise)
{
    if (clockwise)
    {
        return (currDir + 1) % DIR_COUNT;
    }
    else if (currDir == 0)
    {
        return DIR_COUNT - 1;
    }
    else
    {
        return (currDir - 1);
    }
}

void advent::runDay22()
{
    sb_log_info("Running advent day 22 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day22.txt");
    // FileHdl file_hdl = vfs::openFileRead("/data/day22_ref.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));
    auto tokens_raw = splitString(file_string_buffer, '\n');
    slw::span<slw::string_view const> tokens_view(tokens_raw);
    slw::string_view commands_view = trim(tokens_view[0]);

    DArray<Command> commands;
    commands.reserve(commands_view.size() / 2);

    // 10R5L5R10L4R5L5
    u32 currDir = 0;
    while (!commands_view.empty())
    {
        u32 value = 0;
        auto const conv_res = std::from_chars(commands_view.data(), commands_view.data() + commands_view.size(), value);
        if (conv_res.ec != std::errc::invalid_argument)
        {
            commands_view = commands_view.substr(std::distance(commands_view.data(), conv_res.ptr));
            commands.push_back({DIRECTIONS[currDir], (DirCode)currDir, value});
        }
        else
        {
            auto const rotationCmd = commands_view[0];
            sb_assert((rotationCmd == 'L') || (rotationCmd == 'R'));
            currDir = rotateDirection(currDir, rotationCmd == 'R');

            commands_view = commands_view.substr(1);
        }
    }

    DArray<Tile> tiles;
    auto map_view = tokens_view.subspan(1);
    i32 tile_pos_y = 0;
    for (auto token : map_view)
    {
        if (trim(token).empty())
        {
            continue;
        }

        token = rtrim(token);

        auto const tile_begin = slw::find_if(begin(token), end(token), [](char val) { return val != ' '; });
        sb_assert(tile_begin != end(token));

        Vec2i const tile_offset = {(i32)std::distance(begin(token), tile_begin), tile_pos_y};
        auto const tile_data = trim(token);

        if (tiles.empty() || (tiles.back().pos.x != tile_offset.x) || (tiles.back().extents.x != (i32)tile_data.size()))
        {
            Tile new_tile;
            new_tile.pos = tile_offset;
            new_tile.extents = {integral_cast<i32>(tile_data.size()), 1};
            new_tile.data.push_back(tile_data);
            tiles.push_back(new_tile);
        }
        else
        {
            Tile & curr_tile = tiles.back();
            curr_tile.extents.y += 1;
            curr_tile.data.push_back(tile_data);
        }

        ++tile_pos_y;
    }

    u32 total_y = 0;
    for (auto & tile : tiles)
    {
        total_y += tile.extents.y;
    }

    Vec2i local_pos = {0, 0};
    u32 curr_tile_idx = 0;
    Tile const * curr_tile = nullptr;
    u32 cmd_idx = 0;

    for (auto const & cmd : commands)
    {
        curr_tile = &tiles[curr_tile_idx];
        [[maybe_unused]] Vec2i dbg_world_pos = toWorldPos(local_pos, *curr_tile);
        dbg_world_pos += {1, 1};

        u32 move_cnt = cmd.count;
        while (move_cnt)
        {
            Vec2i next_local_pos = local_pos;
            next_local_pos += cmd.dir;

            if (isInLocalBounds(next_local_pos, curr_tile->extents))
            {
                if (curr_tile->data[next_local_pos.y][next_local_pos.x] == WALL)
                {
                    break;
                }
                else
                {
                    local_pos = next_local_pos;
                }
            }
            else
            {
                if (cmd.dir_mode == DirCode::RIGHT || cmd.dir_mode == DirCode::LEFT)
                {
                    next_local_pos.x = wrapVal(next_local_pos.x, curr_tile->extents.x);
                    if (curr_tile->data[next_local_pos.y][next_local_pos.x] == WALL)
                    {
                        break;
                    }
                    else
                    {
                        local_pos = next_local_pos;
                    }
                }
                else
                {
                    Vec2i next_world_pos = toWorldPos(next_local_pos, *curr_tile);
                    next_world_pos.y = wrapVal(next_world_pos.y, (i32)total_y);

                    i32 next_tile_idx = wrapVal((i32)curr_tile_idx + cmd.dir.y, (i32)tiles.size());
                    Tile const * next_tile = &tiles[next_tile_idx];

                    if (isInWorldBounds(next_world_pos, *next_tile))
                    {
                        next_local_pos = toLocalPos(next_world_pos, *next_tile);
                        if (next_tile->data[next_local_pos.y][next_local_pos.x] == WALL)
                        {
                            break;
                        }
                        else
                        {
                            local_pos = next_local_pos;
                            curr_tile = next_tile;
                            curr_tile_idx = next_tile_idx;
                        }
                    }
                    else
                    {
                        const i32 tile_move = cmd.dir.y * -1;

                        i32 prev_tile_idx = (i32)curr_tile_idx;

                        next_world_pos = toWorldPos(local_pos, *curr_tile);
                        next_tile_idx = wrapVal(prev_tile_idx + tile_move, (i32)tiles.size());
                        next_tile = &tiles[next_tile_idx];

                        if (tile_move > 0)
                        {
                            next_world_pos.y = next_tile->pos.y + next_tile->extents.y - 1;
                            while (isInWorldBounds(next_world_pos, *next_tile))
                            {
                                prev_tile_idx = (i32)next_tile_idx;
                                next_tile_idx = wrapVal(prev_tile_idx + tile_move, (i32)tiles.size());
                                next_tile = &tiles[next_tile_idx];

                                next_world_pos.y = next_tile->pos.y + next_tile->extents.y - 1;
                            }

                            next_tile = &tiles[prev_tile_idx];
                            next_world_pos.y = next_tile->pos.y + next_tile->extents.y - 1;
                            next_local_pos = toLocalPos(next_world_pos, *next_tile);

                            if (next_tile->data[next_local_pos.y][next_local_pos.x] == WALL)
                            {
                                break;
                            }
                            else
                            {
                                local_pos = next_local_pos;
                                curr_tile = next_tile;
                                curr_tile_idx = prev_tile_idx;
                            }
                        }
                        else
                        {
                            next_world_pos.y = next_tile->pos.y;
                            while (isInWorldBounds(next_world_pos, *next_tile))
                            {
                                prev_tile_idx = (i32)next_tile_idx;
                                next_tile_idx = wrapVal(prev_tile_idx + tile_move, (i32)tiles.size());
                                next_tile = &tiles[next_tile_idx];

                                next_world_pos.y = next_tile->pos.y;
                            }

                            next_tile = &tiles[prev_tile_idx];
                            next_world_pos.y = next_tile->pos.y;
                            next_local_pos = toLocalPos(next_world_pos, *next_tile);

                            if (next_tile->data[next_local_pos.y][next_local_pos.x] == WALL)
                            {
                                break;
                            }
                            else
                            {
                                local_pos = next_local_pos;
                                curr_tile = next_tile;
                                curr_tile_idx = prev_tile_idx;
                            }
                        }
                    }
                }
            }

            --move_cnt;
        }
        cmd_idx++;
    }

    Vec2i final_world_pos = toWorldPos(local_pos, *curr_tile);
    final_world_pos += {1, 1};

    size_t res = 1000 * integral_cast<size_t>(final_world_pos.y) + 4 * integral_cast<size_t>(final_world_pos.x) + (u32)commands.back().dir_mode;
    sb_log_info("target value {}", res);

    sb_free(GHEAP, byte_buffer);
}
