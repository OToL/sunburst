#include "advent.h"
#include "sb_core/containers/dynamic_array.h"
#include "utility.h"

#include <limits>
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
#include <system_error>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <memory>
#include <stack>

using namespace sb;

struct Vec2D
{
    i32 x;
    i32 y;
};

template <>
struct std::hash<Vec2D>
{
    std::size_t operator()(Vec2D const & pos) const noexcept
    {
        return pos.y * 135 + pos.x; // or use boost::hash_combine
    }
};

[[maybe_unused]] static Vec2D operator-(Vec2D lval, Vec2D rval)
{
    return {lval.x - rval.x, lval.y - rval.y};
}

[[maybe_unused]] static Vec2D operator+(Vec2D lval, Vec2D rval)
{
    return {lval.x + rval.x, lval.y + rval.y};
}

[[maybe_unused]] static b8 operator==(Vec2D lval, Vec2D rval)
{
    return (lval.x == rval.x) && (lval.y == rval.y);
}

[[maybe_unused]] static Vec2D operator*(Vec2D vec, i32 val)
{
    return {vec.x * val, vec.y * val};
}

[[maybe_unused]] static Vec2D operator*(Vec2D lval, Vec2D rval)
{
    return {lval.x * rval.x, lval.y * rval.y};
}

usize getArrayIndexFromPos(Vec2D pos, usize col_count)
{
    return pos.y * col_count + pos.x;
}

Vec2D getPosFromArrayIndex(i32 index, i32 col_count)
{
    return Vec2D{index % col_count, index / col_count};
}

u32 getManhattanDistance(Vec2D v1, Vec2D v2)
{
    int const diff1 = std::abs(v2.x - v1.x);
    int const diff2 = std::abs(v2.y - v1.y);

    return diff1 + diff2;
}

struct ObjectPair
{
    Vec2D sensor_pos;
    Vec2D beacon_pos;
    i32 dist;
};

// Solution
void advent::runDay15()
{
    sb_log_info("Running advent day 15 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day15.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);

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

    // Sensor at x=9450, y=2172986: closest beacon is at x=-657934, y=1258930
    auto const sensor_x = strlen("Sensor at x=");
    auto const sensor_y = strlen(", y=");
    auto const beacon_x = strlen(": closest beacon is at x=");
    auto const beacon_y = strlen(", y=");

    sb::DynamicArray<ObjectPair> objs;
    objs.reserve(tokens.size());

    for (auto const & token : tokens)
    {
        ObjectPair obj = {};
        auto const token_end = token.data() + token.size();

        auto res = std::from_chars(token.data() + sensor_x, token_end, obj.sensor_pos.x);
        sb_assert(res.ec == std::errc());
        res = std::from_chars(res.ptr + sensor_y, token_end, obj.sensor_pos.y);
        sb_assert(res.ec == std::errc());
        res = std::from_chars(res.ptr + beacon_x, token_end, obj.beacon_pos.x);
        sb_assert(res.ec == std::errc());
        res = std::from_chars(res.ptr + beacon_y, token_end, obj.beacon_pos.y);
        sb_assert(res.ec == std::errc());

        obj.dist = getManhattanDistance(obj.beacon_pos, obj.sensor_pos);
        objs.push_back(obj);
    }

    Vec2D min_pos = {std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max()};
    Vec2D max_pos = {std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min()};

    Vec2D extend_dirs[] = {
        {-1, 0},
        {1, 0},
        {0, 1},
        {0, -1},
    };

    std::unordered_set<Vec2D> base_obj_map;
    for (auto const & obj_pair : objs)
    {
        sb_log_info("sensor {} {} ; beacon {} {} ; dist {}", obj_pair.sensor_pos.x, obj_pair.sensor_pos.y, obj_pair.beacon_pos.x,
                    obj_pair.beacon_pos.y, obj_pair.dist);

        base_obj_map.insert(obj_pair.sensor_pos);
        base_obj_map.insert(obj_pair.beacon_pos);

        for (auto const & extend : extend_dirs)
        {
            Vec2D sensor_ext = obj_pair.sensor_pos + extend * obj_pair.dist;

            min_pos = {std::min(min_pos.x, sensor_ext.x), std::min(min_pos.y, sensor_ext.y)};
            max_pos = {std::max(max_pos.x, sensor_ext.x), std::max(max_pos.y, sensor_ext.y)};
        }
    }

    sb_log_info("Bounds: min ({},{}) ; max ({},{})", min_pos.x, min_pos.y, max_pos.x, max_pos.y);

    [[maybe_unused]] Vec2D const grid_offset = min_pos;
    [[maybe_unused]] u32 const col_cnt = max_pos.x - min_pos.x + 1;
    [[maybe_unused]] u32 const row_cnt = max_pos.y - min_pos.y + 1;
    [[maybe_unused]] u32 const elem_cnt = col_cnt * row_cnt;
    //[[maybe_unused]] sb::DArray<char> buffer(elem_cnt, '.');

    // sb::DArray<Vec2D> target_ys;
    {
        std::unordered_set<Vec2D> pos_map;
        for (auto const & obj : objs)
        {
            auto const min_y = obj.sensor_pos.y - obj.dist;
            auto const max_y = obj.sensor_pos.y + obj.dist;
            if ((min_y <= 2000000) && (max_y >= 2000000))
            {
                Vec2D const target_pos = {obj.sensor_pos.x, 2000000};

                auto const orig_y_dist = abs(2000000 - obj.sensor_pos.y);
                auto const remaining_x_dist = obj.dist - orig_y_dist;

                for (i32 x_idx = target_pos.x - remaining_x_dist; x_idx <= target_pos.x + remaining_x_dist; ++x_idx)
                {
                    Vec2D const pos = {x_idx, 2000000};

                    if (!pos_map.contains(pos) && !base_obj_map.contains(pos))
                    {
                        pos_map.insert(pos);
                    }
                }
            }
        }
        sb_log_info("Impossible beacon pos count at y=2000000 : {}", pos_map.size());
    }

    Vec2D free_pos = {};
    b8 free_found = false;
    for (u32 x_idx = 0; /*!free_found &&*/ (x_idx <= 4000000); ++x_idx)
    {
        for (u32 y_idx = 0; /*!free_found && */(y_idx <= 4000000); /*++y_idx*/)
        {
            Vec2D const probe_pos = {(int)x_idx, (int)y_idx};

            // can be optimized by finding the one with the longest remaining distance because it may help jumping
            // further
            auto const obj_iter = std::find_if(begin(objs), end(objs), [probe_pos](auto const & obj) 
                    {
                        return getManhattanDistance(probe_pos, obj.sensor_pos) <= (u32)obj.dist; 
                    });

            if (obj_iter == end(objs))
            {
                sb_assert(!free_found);
                free_pos = probe_pos;
                free_found = true;
                ++y_idx;
            }
            else {
                auto probe_dist = getManhattanDistance(probe_pos, obj_iter->sensor_pos);
                auto remaining_dist = obj_iter->dist - probe_dist;
                if (0 == remaining_dist)
                {
                    ++y_idx;
                }
                else
                {
                    y_idx += remaining_dist;
                }
            }
        }
    }

    sb_log_info("Free spot {} {}", free_pos.x, free_pos.y);
    sb_log_info("Magnitude {}", ((u64)free_pos.x) * 4000000 + free_pos.y);

    sb_free(GHEAP, byte_buffer);
}
